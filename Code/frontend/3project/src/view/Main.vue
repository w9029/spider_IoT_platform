<template>
<div>
  <video autoplay muted loop>
    <source src="../assets/bg1.mp4" type="video/mp4">
  </video>
  <HeadComponent @start="start"></HeadComponent>
  <div class="wrap top center">

    <!--<span style="font-size: 250px;position: absolute;z-index: 99;top: 30%;left: 20%;font-family: 'Adobe Caslon Pro'">Spider IoT</span>-->

    <div v-if="clickButton===true" class="center top" style="margin-top: 25%">
      <img src="../assets/xia.png" class="animated infinite  bounce" alt="" style="width: 50px;height: 50px;">
      <div class="animated  bounce center " style="margin-top: 30px" v-on:click="start()">
        <p class="PStart" >START</p>
      </div>
    </div>

    <!--style="width: 100px;height: 100px;background-color: #5daf34"-->

      <LoginComponent v-if="ShowLoginMain===true" :LoginData="LoginData" @loginShow="loginShow" @forget="forget"
                      @register="register"  @enter="enter"></LoginComponent>



      <RegisterComponent v-if="ShowRegMain" :RegData="RegData" @loginShow="loginShow" @start="start"
                         @getCode="getCode" @RegOk="RegOk"> </RegisterComponent>


      <ForgetComponent v-if="ShowForMain" :ForData="ForData" @loginShow="loginShow" @start="start"
                       @getCode2="getCode2" @ForOk="ForOk"></ForgetComponent>


  </div>

  <!--<FootComponent></FootComponent>-->
</div>

</template>

<script>
import HeadComponent  from '../components/HeadComponent'
import FootComponent from  '../components/FootComponent'
import LoginComponent from  '../components/LoginComponent'
import RegisterComponent from  '../components/RegisterComponent'
import ForgetComponent from  '../components/ForgetComponent'

import {setCookie,getCookie} from "../js/cookie";
    export default {
        name: "Main",
      components:{
        HeadComponent,
        FootComponent,
        LoginComponent,
        RegisterComponent,
        ForgetComponent,
      },
      mounted(){
        if(getCookie('username')){

          this.$router.push('/Home')

        }
      },
      data(){
          return{
            clickButton:true,
            ShowLoginMain:false,
            ShowRegMain:false,
            ShowForMain:false,
            LoginData:{
              username:'',
              password:'',
            },
            RegData:{
              username:'',
              password1:'',
              password2:'',
              email:'',
              code:'',
            },
            ForData:{
              username:'',
              email:'',
              code:'',
              password1:'',
              password2:'',
            },
            NewLoginData:{
              username:'',
              password:'',
            },
            NewRegData:{
              username:'',
              password1:'',
              password2:'',
              email:'',
              code:'',
            },
            NewForData:{
              username:'',
              email:'',
              code:'',
              password1:'',
              password2:'',
            },
          }
      },

      methods: {
        start(){
          this.ShowLoginMain=true;
          this.clickButton=false;
          this.ShowForMain=false;
          this.ShowRegMain=false;
          this.LoginData= Object.assign({},this.NewLoginData);

        },
        loginShow(){
          this.clickButton=true;
          this.ShowLoginMain=false;
          this.ShowForMain=false;
          this.ShowRegMain=false;
        },
        enter(){

            let data = {name:this.LoginData.username,password:this.LoginData.password};

            /*接口请求*/
            console.log(data);

            // setCookie('username',this.LoginData.username,1000*60)
            // this.$router.push('/Home');

            this.$axios({
              url:this.URL.BASE_URL+'/user/login',
              method:'POST',
              data:JSON.stringify(data),
              contentType:"application/json",
              headers:{
                'Content-Type':'application/json'
              }
            }).then((res)=> {
              //新加的
              console.log(res);

              // window.localStorage["token"] = JSON.stringify(res.data.data.token);
              // console.log(JSON.stringify(res.data.data.token));

              console.log(res);
              if(res.data==='success'){
                this.ShowLoginMain=false;
                setCookie('username',this.LoginData.username,1000*60)
                this.$router.push('/Home')

              }
              else{
                alert("用户名或密码错误");
              }

            })
        },
        forget(){
          this.ShowForMain=true;
          this.clickButton=false;
          this.ShowRegMain=false;
          this.ShowLoginMain=false;
        },
        register(){
          this.ShowRegMain=true;
          this.clickButton=false;
          this.ShowLoginMain=false;
          this.ShowForMain=false;
          this.RegData=Object.assign({},this.NewRegData);
        },

        getCode(){

          let data={
            email:this.RegData.email
          };
          this.$axios({
            url:this.URL.BASE_URL+'/user/identity',
            method:'POST',
            data:JSON.stringify(data),
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {
            console.log(res);
            if(res.data!=='success'){
              alert(res.data);
            }

          })
        },

        RegOk(){
          let data={
            name:this.RegData.username,
            email:this.RegData.email,
            password:this.RegData.password2,
            identity:this.RegData.code,
          };
          console.log(data);
          this.$axios({
            url:this.URL.BASE_URL+'/user/regist',
            method:'POST',
            data:JSON.stringify(data),
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res);
            if(res.data==='name'){
              alert("用户已存在");
            }
            else if(res.data==='fail'){
              alert("验证码错误")
            }
            else{
              this.ShowRegMain=false;
              this.ShowLoginMain=true;
              this.LoginData= Object.assign({},this.NewLoginData);
            }

          })
        },
        getCode2(){
          let data={
            email:this.ForData.email
          };
          this.$axios({
            url:this.URL.BASE_URL+'/user/identity',
            method:'POST',
            data:JSON.stringify(data),
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res);
            if(res.data!=='success'){
              alert(res.data);
            }

          })
        },
        ForOk(){


          let data={
            email:this.ForData.email,
            name:this.ForData.username,
            password:this.ForData.password2,
            identity:this.ForData.code,
          };
          console.log(data);
          this.$axios({
            url:this.URL.BASE_URL+'/user/forgetpassword',
            method:'POST',
            data:JSON.stringify(data),
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res);
            if(res.data!=='success'){
              alert(res.data);
            }
            else{
              alert("修改成功");
              this.ShowForMain=false;
              this.ShowLoginMain=true;
              this.ForData= Object.assign({},this.NewForData);
              console.log(this.ForData);
            }

          })

        },
      },

    }

</script>

<style scoped>
  @import "../css/state.css";
  @import "../css/flow.css";
.wrap{
  height: 80%;
  background: rgba(0,0,0,0);
}

video {
  position: fixed;
  left: 0;
  top: 0;
  min-width: 100%;
  min-height: 100%;
  width: auto;
  height: auto;
  z-index: -100;
}
  .active{
    animation-delay:2s;
    -webkit-animation-delay:2s; /*等待俩秒　开始动画*/
    width: 90px;
    height: 90px;
    position: relative;
    /*background-color: rgba(100, 100, 100, 0.49);*/
  }
  .PStart{
    font-size: 50px;
    font-weight: 800;
  }
  p{
    cursor: pointer;
  }

</style>
