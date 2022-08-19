<template>
  <div class="page_main top">
    <LineComponent :title="title" :content="content" class="line_marbottom"></LineComponent>
    <div class="Page6_wrap">
      <div class="P6_main">
        <div class="P6_left top">
          <div style="width: 80%;align-items: center">
            <div class="between" style="width: 100%;margin-top: 20px">

              <el-select  style="width: 40%" v-model="agreement" placeholder="请选择协议" size="mini" @change="agreementChange">
                <el-option v-for="item in optionsAgreement" :key="item.value" :label="item.label" :value="item.value">
                </el-option>
              </el-select>

              <el-cascader style="width: 55%" size="mini" v-if="agreementDevice" v-model="LookDeviceId" :options="options" :props="{ expandTrigger: 'hover' }"
                           @change="handleChange" clearable placeholder="请选择你要调试的设备">
              </el-cascader>

              <el-input style="width: 55%" v-if="agreementIp" size="mini" v-model="LookDeviceIp" placeholder="请输入你要调试的设备iP"></el-input>
            </div>

            <el-input type="textarea" :rows="17" placeholder="请输入内容"
              v-model="textarea" style="margin: 30px 0px" >
            </el-input>
          </div>
          <div style="width: 80%" class="left">
            <el-button type="primary" v-on:click="send()">执行请求</el-button>
          </div>
        </div>

        <div class="P6_right top">
          <div style="width: 100%; margin:20px;" class="left">
            <h3 style="color: #333333;margin-left: 20px;margin-bottom: 10px">返回内容</h3>
          </div>

          <div v-if="ShowResult" class="result" style="background-color: black">
            <div style="margin: 10px;color: white;font-family: 方正姚体"class="left">
              {{this.result}}
            </div>
          </div>

          <div v-if="ShowLoad" class="center result">
            <video autoplay muted loop style="width: 450px;">
              <source src="../../../assets/load.mp4" type="video/mp4">
            </video>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
  import LineComponent from '../../../components/LineComponent'
  export default {
    name: "Page6",
    components:{
      LineComponent
    },
    data(){
      return{
        ShowResult:false,
        ShowLoad:false,
        agreementDevice:false,
        agreementIp:false,
        agreement:'',
        LookDeviceIp:'',
        id:'',
        title:'远程调试',
        content:'这是远程调试的内容',
        textarea:'',
        result:'',
        options: [],
        LookDeviceId: '',
        optionsAgreement:[
          {
            value: 'coap',
            label: 'coap协议'
          },
          {
            value: 'mqtt',
            label: 'mqtt协议'
          }
        ],
      }
    },
    mounted(){
      this.id=this.choose.produce;
      console.log(this.id);
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
    },
    methods:{
      fullscreenLoading(){

      },
      handleChange(value) {
        console.log(value[1]);
        this.LookDeviceId = value[1];
      },
      send(){
        // this.result='';
        this.ShowResult=false;
        this.ShowLoad=true;
        console.log(this.LookDeviceId);
        console.log(this.agreement);
        console.log(this.LookDeviceIp);

        if(this.agreement==='mqtt'){
          this.$axios({
            url:this.URL.BASE_URL+'/user/debug',
            method:'POST',
            params:{
              deviceId:this.LookDeviceId,
              content:this.textarea,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);
            if(res.data.length!==0){
              this.ShowLoad=false;
              this.ShowResult=true;
              this.result="[result]:"+res.data;
            }

            setTimeout(function(){

            }.bind(this),1000)
          });

        }
        else if(this.agreement==='coap'){
          this.ShowLoad=true;
          this.$axios({
            url:this.URL.BASE_URL+'/user/coap/debug',
            method:'POST',
            params:{
              ip:this.LookDeviceIp,
              content:this.textarea,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);
            if(res.data.length!==0){
              this.ShowLoad=false;
              this.ShowResult=true;
              this.result="[result]:"+res.data;
            }

            setTimeout(function(){

            }.bind(this),1000)
          });
        }


        /***
         * 将内容发送给后端
         * 接收后端的值
         */
        // this.result=this.textarea;
      },

      agreementChange(value){
        console.log(this.agreement);
        if(this.agreement==='mqtt'){
          this.agreementDevice=true;
          this.agreementIp=false;
        }
        else{
          this.agreementDevice=false;
          this.agreementIp=true;
        }
      }
    },
  }
</script>

<style scoped>
  @import "../../../css/state.css";
  @import "../../../css/page.css";
  .Page6_wrap{
    width: 100%;
    height: 800px;
    display: flex;
    justify-content: center;
  }
  .P6_main{
    width: 80%;
    height: 600px;
    /*background-color: #3a8ee6;*/
    display: flex;
    justify-content: space-between;
  }
  .P6_left{
    width: 500px;
    height: 550px;
    background-color: white;
  }
  .P6_right{
    width: 500px;
    height: 550px;
    background-color: white;
    display: flex;
  }
  .result{
    width: 450px;
    height: 450px;
    /*background-color: #000000;*/
    background-color: #000000;
  }
</style>
