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
#include <stdbool.h>
#include <pthread.h>

#include "config.h"
#include "resource.h"
#include "coap.h"

#define COAP_RESOURCE_CHECK_TIME 2

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif


#define B64_EOLN            0xF0    // 换行/n    
#define B64_CR              0xF1    // 回车/r    
#define B64_EOF             0xF2    // 连字符-    
#define B64_WS              0xE0    // 跳格或者空格（/t、space）    
#define B64_ERROR           0xFF    // 错误字符    
#define B64_NOT_BASE64(a)   (((a)|0x13) == 0xF3)    

static const char DATA_ASCII2BIN[128] = {    
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0xF0,0xFF,0xFF,0xF1,0xFF,0xFF,    
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,    
    0xE0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3E,0xFF,0xF2,0xFF,0x3F,    
    0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,    
    0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,    
    0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,    
    0xFF,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,    
    0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0x33,0xFF,0xFF,0xFF,0xFF,0xFF    
};    

/*IP address*/
char IP[50]={0};

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
void
handle_sigint(int signum) {
  quit = 1;
}

bool isVaildIp(const char *ip)
{
    int dots = 0; /*字符.的个数*/
    int setions = 0; /*ip每一部分总和（0-255）*/ 

    if (NULL == ip || *ip == '.') { /*排除输入参数为NULL, 或者一个字符为'.'的字符串*/ 
        return false;
    }   

    while (*ip) {

        if (*ip == '.') {
            dots ++; 
            if (setions >= 0 && setions <= 255) { /*检查ip是否合法*/
                setions = 0;
                ip++;
                continue;
            }   
            return false;
        }   
        else if (*ip >= '0' && *ip <= '9') { /*判断是不是数字*/
            setions = setions * 10 + (*ip - '0'); /*求每一段总和*/
        } else 
            return false;
        ip++;   
    }   
	/*判断IP最后一段是否合法*/ 
    if (setions >= 0 && setions <= 255) {
        if (dots == 3) {
            return true;
        }   
    }   

    return false;
}


int BASE64_Decode( const unsigned char* inputBuffer, int inputCount, char* outputBuffer )    
{    
    int i, j;    
    char b[4];    
    char ch;    

    if( (inputBuffer == NULL) || (inputCount < 0) )    
    {    
        return -1;  // 参数错误    
    }    

    // 去除头部空白字符    
    while( inputCount > 0 )    
    {    
        ch = *inputBuffer;    
        if( (ch < 0) || (ch >= 0x80) ) //base64码表只有0-64，怎么到80了，多了回车换行？   
        {    
            return -2;  // 数据错误，不在ASCII字符编码范围内    
        }    
        else    
        {    
            if( DATA_ASCII2BIN[ch] == B64_WS )    
            {    
                inputBuffer++;    
                inputCount--;    
            }    
            else    
            {    
                break;    
            }    
        }    
    }    

    // 去除尾部的空白字符、回车换行字符、连字符    
    while( inputCount >= 4 )    
    {    
        ch = inputBuffer[inputCount - 1];    
        if( (ch < 0) || (ch >= 0x80) )    
        {    
            return -2;  // 数据错误，不在ASCII字符编码范围内    
        }    
        else    
        {    
            if( B64_NOT_BASE64(DATA_ASCII2BIN[ch]) )    
            {    
                inputCount--;    
            }    
            else    
            {    
                break;    
            }    
        }    
    }    

    // 字符串长度必须为4的倍数    
    if( (inputCount % 4) != 0 )    
    {    
        return -2;  // 数据错误    
    }    

    if( outputBuffer != NULL )    
    {    
        for( i = 0; i < inputCount; i += 4 )    
        {    
            for( j = 0; j < 4; j++ )    
            {    
                ch = *inputBuffer++;    
                if( (ch < 0) || (ch >= 0x80) )    
                {    
                    return -2;  // 数据错误，不在ASCII字符编码范围内    
                }    
                else    
                {    
                    if( ch == '=' ) // 发现BASE64编码中的填充字符    
                    {    
                        break;    
                    }    
                    else    
                    {    
                        b[j] = DATA_ASCII2BIN[ch];    
                        if( b[j] & 0x80 )    
                        {    
                            return -2;  // 数据错误，无效的Base64编码字符    
                        }    
                    }                       
                }    
            } // End for j    

            if( j == 4 )    
            {    
                *outputBuffer++ = (b[0] << 2) | (b[1] >> 4);    
                *outputBuffer++ = (b[1] << 4) | (b[2] >> 2 );    
                *outputBuffer++ = (b[2] << 6) | b[3];    
            }    
            else if( j == 3 )    
            {   // 有1个填充字节    
                *outputBuffer++ = (b[0] << 2) | (b[1] >> 4);    
                *outputBuffer++ = (b[1] << 4) | (b[2] >> 2 );    

                return (i >> 2) * 3 + 2;    
            }    
            else if( j == 2 )    
            {   // 有2个填充字节    
                *outputBuffer++ = (b[0] << 2) | (b[1] >> 4);    

                return (i >> 2) * 3 + 1;    
            }    
            else    
            {    
                return -2;  // 数据错误，无效的Base64编码字符    
            }               
        }   // End for i    
    }    
    return (inputCount >> 2) * 3;    
}

void *create_shell()
{
	char sys_call[50]="gotty ssh root@";
	strcat(sys_call,IP);
	printf("sys_call--:     %s\n",sys_call);
	system(sys_call);
	while(1);
}

void 
hnd_post_shell(coap_context_t *ctx, struct coap_resource_t *resource,
        coap_address_t *peer, coap_pdu_t *request, str *token, coap_pdu_t *response){
	unsigned char* data;
	size_t data_len;
	if(coap_get_data(request,&data_len,&data))
	{
		printf("Received from Client :%s\n",data);
	}
	unsigned char buf[3];
	response->hdr->code = COAP_RESPONSE_CODE(205);
	coap_add_option(response, COAP_OPTION_CONTENT_TYPE,coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
	coap_add_option(response, COAP_OPTION_MAXAGE, coap_encode_var_bytes(buf, 0x2ffff) ,buf);


	char str[100]={0};
	if( BASE64_Decode(data,(int)data_len,str) < 0)
	{
		coap_add_data(response, strlen("code error"), (unsigned char *)"code error");
		return;
	}
	int i,j;
	/*
	for(i=0;str[i];i++)
	{
		if(str[i] >= '0' && str[i] <= '9')
			break;
	}
	*/
	printf("%s\n",str);
	for(i=0,j=0;str[i];i++)
	{
		if( (str[i] >= '0' && str[i] <= '9') || str[i] == '.')
			IP[j++] = str[i];
	}
	IP[j] = 0;
	printf("%s\n",IP);
	//check recevice data IP
	if(isVaildIp(IP)&&strcmp(IP,"192.168.100.235")==0)
	{
		printf("target IP:%s\n",IP);
		pthread_t pid;

		if( pthread_create(&pid,NULL,create_shell,NULL) != 0 )
		{
			coap_add_data(response, strlen("dGhyZWFkIGZhaWxlZA=="), (unsigned char *)"dGhyZWFkIGZhaWxlZA==");
		}
		else
		{
			coap_add_data(response, strlen("MTkyLjE2OC4xMDAuMTIyOjk5OTk="), (unsigned char *)"MTkyLjE2OC4xMDAuMTIyOjk5OTk=");
		}
	}
	else
	{
		coap_add_data(response, strlen("aXAgZXJyb3I="), (unsigned char *)"aXAgZXJyb3I=");
	}

}


void 
check_async(coap_context_t  *ctx, coap_tick_t now) {
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


void
init_resources(coap_context_t *ctx) {
  coap_resource_t *r;
	
  r = coap_resource_init((unsigned char *)"shell", 5, 0);
  coap_register_handler(r, COAP_REQUEST_POST, hnd_post_shell);
  coap_add_attr(r, (unsigned char *)"world", 5, (unsigned char *)"0", 1, 0);
  coap_add_resource(ctx, r);
	
}

void
usage( const char *program, const char *version) {
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

coap_context_t *
get_context(const char *node, const char *port) {
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

int
main(int argc, char **argv) {
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
  }

  coap_set_log_level(log_level);

  ctx = get_context(addr_str, port_str);
  if (!ctx)
    return -1;

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

  return 0;
}
