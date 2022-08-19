<template >
    <div>
<!--导航栏-->
      <div class="head head_height">
        <div class="between head_height">
          <div class="head_height between">
            <h1 style="margin-left: 30px">Spider IoT</h1>
          </div>
          <ul class="right">
            <li v-on:click="start()" style="margin-right: 30px">登录</li>
            <!--<li v-on:click="reg()">注册</li>-->
          </ul>
        </div>
      </div>
<!--登录弹框-->
      <transition name="el-zoom-in-center">

        <div class="login container top" v-show="ShowLogin">

          <h2 class="title">Sign In</h2>

          <el-form :model="ruleForm" status-icon :rules="rules" ref="ruleForm" class="center column_between">

            <el-form-item prop="username">
            <div  class="between mar" >
              <label>E-mail</label>
              <el-input class="width" placeholder="E-mail" v-model="ruleForm.username" clearable></el-input>
            </div>
            </el-form-item>

            <el-form-item  prop="password">
              <div class=" between mar">
                <label>Password</label>
                <el-input class="width" placeholder="Password" v-model="ruleForm.password" show-password></el-input>
              </div>
            </el-form-item>

            <div class="right mar">
              <span style="color: #3c3c3c; font-size: 18px" v-on:click="foget()">forget password?</span>
            </div>

            <el-form-item class="between mar">
              <el-button class="button" v-on:click="enter()">Enter</el-button>
              <el-button  class="button" v-on:click="resetForm('ruleForm')">重置</el-button>
            </el-form-item>

          </el-form>

        </div>
      </transition>
<!--注册弹窗-->
      <RegisterComponent v-show="ShowReg"></RegisterComponent>
<!--忘记密码弹窗-->
      <transition name="el-zoom-in-center">

      <div class="forget container top" v-show="ShowFoget">

        <h2 class="title">Forget</h2>

        <el-form :model="forgetForm" status-icon :rules="rules" ref="ruleForm2" class="center2 column_between">

          <el-form-item prop="username">
            <div  class="between mar" >
              <label>E-mail</label>
              <el-input class="width" placeholder="E-mail" v-model="forgetForm.username" clearable></el-input>
            </div>
          </el-form-item>

          <el-form-item>
            <div  class="between mar" >
              <label>验证码</label>
              <el-input class="width" placeholder="E-mail" v-model="forgetForm.code" clearable></el-input>
            </div>
          </el-form-item>


          <el-form-item  prop="pass1">
            <div class=" between mar">
              <label>Password1</label>
              <el-input class="width" placeholder="Password" v-model="forgetForm.pass1" show-password></el-input>
            </div>
          </el-form-item>

          <el-form-item  prop="pass2">
            <div class=" between mar">
              <label>Password2</label>
              <el-input class="width" placeholder="Password" v-model="forgetForm.pass2" show-password></el-input>
            </div>
          </el-form-item>


          <div class="between mar">
            <el-button class="button">Enter</el-button>
            <el-button class="button" v-on:click="resetForm('ruleForm2')">重置</el-button>
          </div>

        </el-form>

      </div>
    </transition>
    </div>
</template>

<script>
    import {setCookie,getCookie} from "../js/cookie";
    import RegisterComponent from "./RegisterComponent"
    export default {
        name: "HeadComponent",
      components:{
        RegisterComponent,
      },
      data(){
        var checkUsername = (rule, value, callback) => {
          if (value === '') {
            return callback(new Error('请输入用户名'));
          }
          else {
            callback();
          }

        };
        var validatePass = (rule, value, callback) => {
          if (value === '') {
            callback(new Error('请输入密码'));
          } else {
            callback();
          }
        };
        var checkpass1 = (rule, value, callback) => {
          if (value === '') {
            callback(new Error('请输入密码'));
          } else {
            if (this.forgetForm.pass2 !== '') {
              this.$refs.forgetForm.validateField('checkpass2');
            }
            callback();
          }
        };
        var checkpass2 = (rule, value, callback) => {
          if (value === '') {
            callback(new Error('请再次输入密码'));
          } else if (value !== this.forgetForm.pass1) {
            callback(new Error('两次输入密码不一致!'));
          } else {
            callback();
          }
        };
        return {
          ruleForm: {
            username: '',
            password: '',
          },
          forgetForm:{
            username:'',
            code:'',
            pass1:'',
            pass2:'',
          },
          rules: {
            username: [
              { validator: checkUsername, trigger: 'blur' }
            ],
            password: [
              { validator: validatePass, trigger: 'blur' }
            ],
            pass1:[
              { validator: checkpass1, trigger: 'blur' }
            ],
            pass2:[
              { validator: checkpass2, trigger: 'blur' }
            ],
          },

          ShowLogin:false,
          ShowReg:false,
          ShowFoget:false,

          reusername:'',
          repassword1:'',
          repassword2:'',
        }
      },
      mounted:function () {
        if(getCookie('username')){

          this.$router.push('/Home')

        }
      },
      methods:{
        start(){
          this.$emit('start');
        },


          login(){
            this.ShowLogin=true;
            this.ShowReg=false;
            this.ShowFoget=false;
          },
          reg(){
            this.ShowReg=true;
            this.ShowLogin=false;
            this.ShowFoget=false;
          },
        foget(){
          this.ShowFoget=true;
          this.ShowReg=false;
          this.ShowLogin=false;

        },
        enter(){
          if(this.ruleForm.username=="" || this.ruleForm.password=="")
          {
            alert("用户名或密码不能为空");
          }
          else {
            let data = {name:this.ruleForm.username,password:this.ruleForm.password};

            /*接口请求*/
             console.log(data);

            setCookie('username',this.ruleForm.username,1000*60)
            this.$router.push('/Home');

             // this.$axios({
             //   url:'http://192.168.100.94:8050/user/login',
             //   method:'POST',
             //   data:JSON.stringify(data),
             //   contentType:"application/json",
             //   headers:{
             //     'Content-Type':'application/json'
             //   }
             // }).then((res)=> {
             //   console.log("登录");
             //   console.log(res);
             //   this.ShowLogin=false;
             //   this.ShowReg=false;
             //   setCookie('username',this.ruleForm.username,1000*60);
             //
             //   setTimeout(function(){
             //
             //     this.$router.push('/Home');
             //
             //   }.bind(this),1000)
             // })
   //登录交互-end
          }
        },

        resetForm(formName) {
          this.$refs[formName].resetFields();
        },
    }
    }
</script>

<style scoped>
@import "../css/state.css";

  .head_height{
    height: 70px;
  }
  .head{
    width: 100%;
    border-bottom:2px solid white ;
    /*background-color: rgba(0,0,0,0.1);*/
    background-color: rgba(255,255,255,0.1);
  }

  li{
    margin-left:45px;
    font-size: 18px;
    cursor:pointer;
  }

  li:hover{
    color: #2078de;
  }

  .login{
    height: 400px;
    width: 600px;
    background-color: rgba(255,255,255,0.5);
    color: #050505;
    position: absolute;
    top: 200px;
    left: 31%;
    font-size: 20px;
  }

.forget{
  height: 500px;
  width: 600px;
  background-color: rgba(255,255,255,0.5);
  color: #050505;
  position: absolute;
  top: 150px;
  left: 31%;
  font-size: 20px;
}

.title{
  font-size: 50px;
  margin-bottom: 30px;
}
  .center{
      width: 400px;
      height: 300px;
    /*background-color: pink;*/
  }

.center2{
  width: 400px;
  height: 400px;
  /*background-color: pink;*/
}
label{
  font-size: 20px;
}
.width{
    width: 300px;
  }

.mar{
  margin: 10px 0px;
}

  .button{
    width: 150px;
    font-size: 18px;
  }

  .button:hover{
    color: #050505;
    background-color: #8d8d8d;
  }

  span{
    cursor: pointer;
  }

</style>
