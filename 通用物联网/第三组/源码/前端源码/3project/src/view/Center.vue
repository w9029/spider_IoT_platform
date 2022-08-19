<template>
  <div>
    <div class="head">
      <div class=" between">
        <div style="width: 210px" class="between">
          <span style="margin-left: 30px; cursor: pointer; font-size: 18px" @click="ChangeMain()">Spider IoT</span>
          <div style="color: #a7a7a7;margin: 0px 10px" >|</div>
        </div>

        <!--<p >{{alertText}}</p>-->
        <!--<button v-on:click="on()">click</button>-->
        <div class=" between" style="width: 100%; margin-left: 29.5px">
          <el-menu
            :default-active="activeIndex2"
            class="el-menu-demo"
            style="border-bottom: 0px;"
            mode="horizontal"
            background-color="#111"
            text-color="#ccc"
            active-text-color="#2d8cf0">
            <el-menu-item index="1"><i class="el-icon-user"></i>个人中心</el-menu-item>
          </el-menu>
        </div>
      </div>
    </div>

    <div class="mainCenter container" style="margin-top: 20px">

      <div style="height:200px; border-bottom: 1px solid #cccccc; background-color: rgba(163,166,173,0.12)" class="left shadow">
        <div style="height: 150px;width: 150px;padding-left: 20px;">
          <el-avatar :size="150" src="https://cube.elemecdn.com/0/88/03b0d39583f48206768a7534e55bcpng.png"></el-avatar>
        </div>
        <el-col  style="width: 300px;height: 50px;margin-left: 60px;color: #666666">
          <el-row class="left" ><span>用户名：</span><span>{{this.Data.name}}</span></el-row>
          <br>
          <el-row class="left"><span>邮箱号：</span><span>{{this.Data.email}}</span></el-row>
        </el-col>
      </div>

      <div style="width:1100px;margin-top: 100px;margin-left: 50px;color: #666666">

        <el-row  style="height: 60px;border-top: 1px dashed #cccccc; border-bottom: 1px dashed #cccccc"  class="between">
          <el-col class="left" style="width:80%">
            <div style="width: 25%" class="left">
              <span style="margin-left: 30px;">登陆密码</span>
            </div>
            <span style="margin-top: 10px">*******</span>
          </el-col >
          <el-button type="text"  style="width:20%" icon="el-icon-edit" @click="password">编辑</el-button>
        </el-row>

        <el-row  style="height: 60px;border-top: 1px dashed #cccccc; border-bottom: 1px dashed #cccccc"  class="between">
          <el-col class="left" style="width:80%">
            <div style="width: 25%" class="left">
              <span style="margin-left: 30px;">邮箱</span>
            </div>
            <span style="margin-top: 10px">{{this.Data.email}}</span>
          </el-col >
          <el-button type="text"  style="width:20%" icon="el-icon-edit" @click="emailChange">编辑</el-button>
        </el-row>

        <el-row  style="height: 60px;border-top: 1px dashed #cccccc; border-bottom: 1px dashed #cccccc"  class="between">
          <el-col class="left" style="width:88%">
              <span style="margin-left: 30px;">开启邮件预警</span>
          </el-col >
          <!--<el-button type="text"  style="width:20%" icon="el-icon-edit">编辑</el-button>-->
          <el-switch  style="width:12%;"  v-model="value" @change="stateChange()"></el-switch>
        </el-row>

      </div>
    </div>
  </div>
</template>

<script>
  import {getCookie} from "../js/cookie";

  export default {
    name: "Center",
    data(){
      return{
        username:"",
        activeIndex: '1',
        activeIndex2: '1',
        value:false,
        Data:{},
      }
    },
    mounted(){
      let uname=getCookie('username');
      this.username=uname;
      if(uname ==="")
      {
        this.$router.push("/Main");
      }
      let data={name:uname}
      this.$axios({
        url:this.URL.BASE_URL+'/user/find',
        method:'POST',
        data:JSON.stringify(data),
        contentType:"application/json",
        headers:{
          'Content-Type':'application/json'
        }
      }).then((res)=> {

        console.log(res.data);
        if(res.data.length!==0){
          this.Data=res.data[0];
          this.value=this.Data.email_ctl;
        }

        setTimeout(function(){

        }.bind(this),1000)
      });


      /****
       *
       * 接收信息
       * **/
      // this.Data={
      //   email:'123@qq.com',
      //   state:true,
      // };
      // this.value=this.Data.state;
    },
    methods:{
      ChangeMain(){
        this.$router.push('/Home');
      },
      password(){
        this.$router.push("/EditPassword");
      },
      emailChange(){
        this.$router.push("/EditEmail");
      },
      stateChange(){
        console.log(this.value);

        this.$axios({
          url:this.URL.BASE_URL+'/user/changemailctl',
          method:'POST',
          params:{
            name:this.username,
            email_ctl:this.value,
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


        /****
         * 发送状态
         * */
      },
    }
  }
</script>


<style scoped>
  @import "../css/page.css";
  @import "../css/state.css";
  .head{
    background-color: #111;
    color: white;
    height: 60px;
    margin-bottom: 60px;
  }
.mainCenter{
  width: 1200px;
  height: 590px;
  /*background-color: #5accaf;*/
  /*background: url("../assets/未标题-1.jpg");*/
}

</style>
