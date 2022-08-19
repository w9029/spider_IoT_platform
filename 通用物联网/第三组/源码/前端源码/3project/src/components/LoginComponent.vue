<template>
 <div>
   <!--登录弹框-->
   <transition name="el-zoom-in-center" class="">
       <div class="login container top shine_red" >
         <div  class="right" style="width: 95%">
           <el-button type="text" icon="el-icon-close" v-on:click="loginShow()"></el-button>
         </div>
         <h2 class="title" style="color: white;margin-top: -20px" >Sign In</h2>

         <el-form :model="LoginData" :rules="rules" ref="LoginData" label-width="0px" class="demo-ruleForm">

           <el-form-item prop="username">
             <el-input  class="width text" style="color: white"
                       placeholder="Username" v-model="LoginData.username" clearable></el-input>
           </el-form-item>

           <el-form-item prop="password">
             <el-input class="width text" style="margin-top:20px;color: white"
                       placeholder="Password" v-model="LoginData.password" show-password clearable></el-input>
           </el-form-item>

         </el-form>

         <div class="width between" style="margin-top:20px;">
           <span  style="color: rgb(225,225,225);border-bottom: 1px solid rgb(225,225,225) ;
               font-size: 18px;font-family: 方正姚体" @click="register()">没有账号？立马注册</span>
           <span  style="color: #bfbfbf; font-size: 18px;font-family: 方正姚体" v-on:click="forget()">忘记密码?</span>
         </div>

         <el-button class="button" style="margin-top: 40px" v-on:click="enter('LoginData')">Enter</el-button>


       </div>
   </transition>

 </div>

</template>

<script>

    export default {
        name: "LoginComponent",
      data(){
        return {
          rules:{
            username: [
              { required: true, message: '请输入用户名', trigger: 'blur' },
            ],
            password: [
              { required: true, message: '请选择密码', trigger: 'blur' }
            ],
          },
        }
      },
      props:{
        LoginData:{
          type:Object,
        }
      },
      methods:{
        loginShow(){
          this.$emit('loginShow');
        },
        forget(){
          this.$emit('forget');
        },
        register(){
          this.$emit('register');
        },

        enter(formName){
          this.$refs[formName].validate((valid) => {
            if (valid) {
              this.$emit('enter');
            }
          });

        },
      },
    }
</script>



<style scoped>
  @import "../css/state.css";

  .login{
    height: 420px;
    width: 600px;
    border:1px solid rgba(255,255,255,0.5);
    color: #050505;
    position: absolute;
    top: 25%;
    left: 33%;
    font-size: 20px;
  }

  .button{
    width: 300px;
    font-size: 18px;
  }

  .button:hover{
    color: #050505;
    background-color: #8d8d8d;
  }

  span{
    cursor: pointer;
  }

  .title{
    font-size: 50px;
    margin-bottom: 30px;
  }

  .width{
    width: 300px;
  }
</style>

<style>
  .text  .el-input__inner{
    color: white;
    background-color: rgba(255, 255, 255, 0);
    border: none;
    outline: none;
    border-bottom: #ffffff 1px solid;
    box-shadow: 0px 0px 0px 0px;
    maxlength:"4";
    height: 20px;
    padding: 20px 0px;
    font-size: 18px;
    border-radius:0;
  }
</style>
