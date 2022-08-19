<template>
  <div class="page_main top" style="width: 100%">
    <LineComponent :title="title" :content="content" ></LineComponent>
    <fullscreen :fullscreen.sync="fullscreen" style="background-color: #f5f6f7; width: 100%;">
      <div class="top" style="width: 100%">
        <div style="width: 90%;margin: 40px" class="between ">
          <div class="left">
            <el-cascader
              v-model="LookDeviceId"
              :options="options"
              :props="{ expandTrigger: 'hover' }"
              @change="handleChange" clearable
              placeholder="请选择你要查看的设备"></el-cascader>
            <el-button  type="primary"  @click="LookSubmit" style="margin-left: 40px">确认</el-button>
          </div>
            <i class="el-icon-full-screen" style="margin-right: 30px" @click="toggle"></i>
        </div>

        <div class="P4_main top" >

          <div style="width: 95%;margin: 5px;" class="top">
            <div style="width: 100%;display: flex;flex-wrap: wrap;justify-content: flex-start;">
              <div   v-for="(item,index) in relayMessage" :child="item" style="border-bottom: 1px dashed #cccccc;width: 100%; padding: 10px 0px" class="left" >
                <h3 class="el-icon-paperclip" style="color: #666666;font-family: 隶书">{{item.name}}</h3>
                <div  v-for="(mess,index) in item.valueList" :child="mess" class="left" style="margin-left: 15px">
                  <span style="font-family: 幼圆">{{mess.position}}({{mess.name}})</span>
                  <el-switch  v-model="mess.message" @change="MessageChange(mess.message,index,item.addr)">
                  </el-switch>
                </div>
              </div>

            </div>
          </div>

          <div style="width: 95%;height: 620px;overflow: auto;margin-top: 20px"  class="space"  >

            <el-table :data="this.tableData"  row-class-name="row"
                      cell-class-name="column" :highlight-current-row="true" fit >
              <el-table-column v-for="(item, index) in tableLabel" :key="index "
                               :prop="item.prop" :width="item.width" :label="item.label"  v-infinite-scroll="addMessage" >
              </el-table-column>
            </el-table>
          </div>



        </div>
      </div>
    </fullscreen>

  </div>
</template>

<script>
  import LineComponent from '../../../components/LineComponent'
    export default {
        name: "Page4",
      components:{
        LineComponent
      },
      destroyed: function() {
        //页面销毁时关闭长连接
        this.websocketclose();
      },
      data(){
        return{
          id:'',
          title:'实时监测',
          content:'这是实时监测的内容',
          options:[],
          LookDeviceId:'',
          fullscreen: false,
          LookData: [],
          websock: null,
          Data:[],
          tableData: [],
          // 子组件的表头数据
          tableLabel: [],
          flag:0,
          relayMessage:[],
          temp:1,
        }
      },
      mounted(){

        this.$axios({
          url:this.URL.BASE_URL+'/industry/find/value',
          method:'POST',
          params:{
            industryId:this.choose.produce,
          },
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {

          console.log(res.data);
          if(res.data.length!==0){
            this.options=res.data;
          }

          setTimeout(function(){

          }.bind(this),1000)
        });
          /**
           * 接收选择器的内容
           *
           * **/
        //   this.options= [
        //     {
        //   label: '采集单元一',
        //   children: [{
        //     value: '1201',
        //     label: '开发板1',
        //   },
        //     {
        //       value: '1202',
        //       label: '开发板2',
        //     }]
        // }, {
        //   label: '采集单元二',
        //   children: [{
        //     value: '201',
        //     label: '开发板1',
        //   }, {
        //     value: '2202',
        //     label: '开发板2',
        //   }, {
        //     value: '2203',
        //     label: '开发板3',
        //   }]
        // }, {
        //   label: '采集单元三',
        //   children: [{
        //     value: '3201',
        //     label: '开发板1'
        //   }, {
        //     value: '3202',
        //     label: '开发板2'
        //   }, {
        //     value: '3203',
        //     label: '开发板3'
        //   }]
        // }];
        this.id=this.$route.params.LookDeviceid;
        if(typeof(this.id) !== 'undefined'){
          this.LookDeviceId=this.id;
          console.log(this.LookDeviceId);
          // this.LookDeviceId='6af6188e14aa';
          this.initWebSocket();
        }


      },
      methods:{
        addMessage(redata){
          if(typeof(redata) === 'undefined'){
            return '';
          }
          const SensorMessage=redata;
          console.log(redata);
          //表头
          if(this.flag===0){
            var i;
            for( i=0; i< SensorMessage.length; i++){
              let temp={label:'',prop:''};
              temp.label=SensorMessage[i].name;
              temp.prop=i.toString();
              // console.log(temp.prop)
              this.tableLabel.push(temp);
            }
            this.flag=1;
          }
          //表身数据段
          var obj = {};
          for(let i=0; i<SensorMessage.length; i++){
            // console.log("prop:"+this.tableLabel[i].prop);
            // console.log(SensorMessage[i].message);

            var key = this.tableLabel[i].prop;
            var value = SensorMessage[i].message;
            obj[key] = value;
            // console.info(obj);
          }
          // this.tableData.push(obj);

          if(this.tableData.length<8){
            this.tableData.unshift(obj);
          }
          else{
            this.tableData.unshift(obj);
            this.tableData.pop();
          }
        },
        initWebSocket(){ //初始化weosocket

          const wsuri = "ws://"+this.URL2.WEB_URL+"/webSocket";//ws地址
          this.websock = new WebSocket(wsuri);
          this.websock.onopen = this.websocketonopen;

          this.websock.onerror = this.websocketonerror;

          this.websock.onmessage = this.websocketonmessage;
          this.websock.onclose = this.websocketclose;
        },

        websocketonopen() {
          console.log("WebSocket连接成功");
          // this.websocketsend();
          this.websock.send(this.LookDeviceId);
        },
        websocketonerror(e) { //错误
          console.log("WebSocket连接发生错误");
        },
        websocketonmessage(e){ //数据接收
          // console.log(e);
          const redata = JSON.parse(e.data);
          //注意：长连接我们是后台直接1秒推送一条数据，
          //但是点击某个列表时，会发送给后台一个标识，后台根据此标识返回相对应的数据，
          //这个时候数据就只能从一个出口出，所以让后台加了一个键，例如键为1时，是每隔1秒推送的数据，为2时是发送标识后再推送的数据，以作区分
          console.log(redata.relay);
          var that =this;
          that.addMessage(redata.sensor);

            this.relayMessage=redata.relay;
            this.temp++;


          // this.relayMessage=[
          //   {
          //     name:'继电器1',
          //     valueList:[
          //       {
          //         name:'强电设备1',
          //         message:true,
          //       },
          //       {
          //         name:'强电设备2',
          //         message:false,
          //       }
          //     ],
          //   },
          //   {
          //     name:'继电器1',
          //     valueList:[
          //       {
          //         name:'强电设备1',
          //         message:true,
          //       },
          //       {
          //         name:'强电设备2',
          //         message:true,
          //       }
          //     ],
          //   },
          // ];
          // console.log( this.addMessage(redata.sensor));

        },

        // websocketsend(){//数据发送
        //   this.websock.send('6af6188e14aa');
        // },

        websocketclose(e){ //关闭
          this.websock.close();
          console.log("connection closed");
        },

        handleChange(value) {
          console.log(value);
          this.LookDeviceId=value[1];
        },
        LookSubmit(){
          console.log(this.LookDeviceId);
          this.initWebSocket();
        },
        toggle () {
          this.fullscreen = !this.fullscreen
        },
        MessageChange(value,index,addr){
          // console.log(value);
          // console.log(index);
          // console.log(addr);
          // console.log(this.LookDeviceId);

          this.$axios({
            url:this.URL.BASE_URL+'/relay/update/machineState',
            method:'POST',
            params:{
              industryId:this.choose.produce,
              deviceId:this.LookDeviceId,
              relayAddr:addr,
              position:index,
              state:value,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);

            setTimeout(function(){

            }.bind(this),1000)
          });


          /******
           *
           * 把状态给后端
           * *******/
        }
      },
    }
</script>

<style scoped>
  @import "../../../css/state.css";
  @import "../../../css/page.css";
  .klad{
    white-space:pre-wrap;
  }
  .P4_main{
    display: flex;
    flex-wrap: wrap;
    margin: 0px 30px;
    width: 95%;
    min-height: 600px;
    justify-content: flex-start;
  }

</style>

<style>
  .space .el-table .cell{
    white-space: pre-wrap
  }
</style>
