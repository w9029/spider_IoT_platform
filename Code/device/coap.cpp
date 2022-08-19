extern "C"
{
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>

#include "config.h"
#include "resource.h"
#include "coap.h"
#include "debug.h"
#include "encode.h"

}
#include "BASE64.h"
#include <string>
#include "mqtt.h"
using namespace std;

#define COAP_RESOURCE_CHECK_TIME 2

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

/* temporary storage for dynamic resource representations */
static int quit = 0;

/* changeable clock base (see handle_put_time()) */
static time_t my_clock_base = 0;

struct coap_resource_t *time_resource = NULL;

#ifndef WITHOUT_ASYNC
/* This variable is used to mimic long-running tasks that require
 * asynchronous responses. */
static coap_async_state_t *async = NULL;
#endif /* WITHOUT_ASYNC */

/* SIGINT handler: set quit to 1 for graceful termination */
void handle_sigint(int signum) {
    quit = 1;
}


void hnd_post_hello(coap_context_t *ctx, struct coap_resource_t *resource,
        coap_address_t *peer, coap_pdu_t *request, str *token, coap_pdu_t *response){
    unsigned char* data;
    size_t data_len;
    char resStr[2048]={0};
    string resString;
    if(coap_get_data(request,&data_len,&data))
    {
        char temp[2048];
        int ret = BASE64_Decode(data,data_len,temp);
        if (ret <= 0)
        {
            resString = "payload_error";
        }
        else
        {
            //got return str
            resString = choose_handle(temp);
        }

        printf("Coap Got Message :%s\n",temp);
    }

    //base64 encode
    int ret = BASE64_Encode(resString.c_str(),strlen(resString.c_str()),resStr);

    unsigned char buf[3];

    response->hdr->code = COAP_RESPONSE_CODE(205);

    coap_add_option(response, COAP_OPTION_CONTENT_TYPE,
            coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);

    coap_add_option(response, COAP_OPTION_MAXAGE, 
            coap_encode_var_bytes(buf, 0x2ffff) ,buf);

    coap_add_data(response, strlen(resStr), (unsigned char *)resStr);
}


void check_async(coap_context_t  *ctx, coap_tick_t now) {
    coap_pdu_t *response;
    coap_async_state_t *tmp;

    size_t size = sizeof(coap_hdr_t) + 13;

    if (!async || now < async->created + (unsigned long)async->appdata) 
        return;

    response = coap_pdu_init(async->flags & COAP_ASYNC_CONFIRM 
            ? COAP_MESSAGE_CON
            : COAP_MESSAGE_NON,
            COAP_RESPONSE_CODE(205), 0, size);
    if (!response) {
        debug("check_async: insufficient memory, we'll try later\n");
        async->appdata = 
            (void *)((unsigned long)async->appdata + 15 * COAP_TICKS_PER_SECOND);
        return;
    }

    response->hdr->id = coap_new_message_id(ctx);

    if (async->tokenlen)
        coap_add_token(response, async->tokenlen, async->token);

    coap_add_data(response, 4, (unsigned char *)"done");

    if (coap_send(ctx, &async->peer, response) == COAP_INVALID_TID) {
        debug("check_async: cannot send response for message %d\n", 
                response->hdr->id);
    }
    coap_delete_pdu(response);
    coap_remove_async(ctx, async->id, &tmp);
    coap_free_async(async);
    async = NULL;
}


void init_resources(coap_context_t *ctx) {
    coap_resource_t *r;

    r = coap_resource_init((unsigned char *)"control", 7, 0);
    coap_register_handler(r, COAP_REQUEST_POST, hnd_post_hello);
    coap_add_attr(r, (unsigned char *)"world", 5, (unsigned char *)"0", 1, 0);
    coap_add_resource(ctx, r);
}

void usage( const char *program, const char *version) {
    const char *p;

    p = strrchr( program, '/' );
    if ( p )
        program = ++p;

    fprintf( stderr, "%s v%s -- a small CoAP implementation\n"
            "(c) 2010,2011 Olaf Bergmann <bergmann@tzi.org>\n\n"
            "usage: %s [-A address] [-p port]\n\n"
            "\t-A address\tinterface address to bind to\n"
            "\t-p port\t\tlisten on specified port\n"
            "\t-v num\t\tverbosity level (default: 3)\n",
            program, version, program );
}

coap_context_t* get_context(const char *node, const char *port) {
    coap_context_t *ctx = NULL;  
    int s;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Coap uses UDP */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST;

    s = getaddrinfo(node, port, &hints, &result);
    if ( s != 0 ) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return NULL;
    } 

    /* iterate through results until success */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        coap_address_t addr;

        if (rp->ai_addrlen <= sizeof(addr.addr)) {
            coap_address_init(&addr);
            addr.size = rp->ai_addrlen;
            memcpy(&addr.addr, rp->ai_addr, rp->ai_addrlen);

            ctx = coap_new_context(&addr);
            if (ctx) {
                /* TODO: output address:port for successful binding */
                goto finish;
            }
        }
    }

    fprintf(stderr, "no context available for interface '%s'\n", node);

finish:
    freeaddrinfo(result);
    return ctx;
}


void* runCoap(void *a) 
{
    coap_context_t  *ctx;
    fd_set readfds;
    struct timeval tv, *timeout;
    int result;
    coap_tick_t now;
    coap_queue_t *nextpdu;
    char addr_str[NI_MAXHOST] = "::";
    char port_str[NI_MAXSERV] = "5683";
    int opt;
    coap_log_t log_level = LOG_WARNING;
    /*
    while ((opt = getopt(argc, argv, "A:p:v:")) != -1) {
        switch (opt) {
            case 'A' :
                strncpy(addr_str, optarg, NI_MAXHOST-1);
                addr_str[NI_MAXHOST - 1] = '\0';
                break;
            case 'p' :
                strncpy(port_str, optarg, NI_MAXSERV-1);
                port_str[NI_MAXSERV - 1] = '\0';
                break;
            case 'v' :
                log_level = strtol(optarg, NULL, 10);
                break;
            default:
                usage( argv[0], PACKAGE_VERSION );
                exit( 1 );
        }
    }*/

    coap_set_log_level(log_level);

    ctx = get_context(addr_str, port_str);
    if (!ctx)
        return NULL;

    init_resources(ctx);

    signal(SIGINT, handle_sigint);

    while ( !quit ) {
        FD_ZERO(&readfds);
        FD_SET( ctx->sockfd, &readfds );

        nextpdu = coap_peek_next( ctx );

        coap_ticks(&now);
        while (nextpdu && nextpdu->t <= now - ctx->sendqueue_basetime) {
            coap_retransmit( ctx, coap_pop_next( ctx ) );
            nextpdu = coap_peek_next( ctx );
        }

        if ( nextpdu && nextpdu->t <= COAP_RESOURCE_CHECK_TIME ) {
            /* set timeout if there is a pdu to send before our automatic timeout occurs */
            tv.tv_usec = ((nextpdu->t) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
            tv.tv_sec = (nextpdu->t) / COAP_TICKS_PER_SECOND;
            timeout = &tv;
        } else {
            tv.tv_usec = 0;
            tv.tv_sec = COAP_RESOURCE_CHECK_TIME;
            timeout = &tv;
        }
        result = select( FD_SETSIZE, &readfds, 0, 0, timeout );

        if ( result < 0 ) {		/* error */
            if (errno != EINTR)
                perror("select");
        } else if ( result > 0 ) {	/* read from socket */
            if ( FD_ISSET( ctx->sockfd, &readfds ) ) {
                coap_read( ctx );	/* read received data */
                coap_dispatch( ctx );	/* and dispatch PDUs from receivequeue */
            }
        } else {			/* timeout */
            if (time_resource) {
                time_resource->dirty = 1;
            }
        }

#ifndef WITHOUT_ASYNC
        /* check if we have to send asynchronous responses */
        check_async(ctx, now);
#endif /* WITHOUT_ASYNC */

#ifndef WITHOUT_OBSERVE
        /* check if we have to send observe notifications */
        coap_check_notify(ctx);
#endif /* WITHOUT_OBSERVE */
    }

    coap_free_context( ctx );

    return NULL;
}
