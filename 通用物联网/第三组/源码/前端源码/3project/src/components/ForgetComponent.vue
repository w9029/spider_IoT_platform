<template>
    <div>
      <!--忘记密码弹窗-->
      <transition name="el-zoom-in-top">
        <div class="forget container top shine_red">

          <div  class="right" style="width: 95%">
            <el-button type="text" icon="el-icon-close" v-on:click="loginShow()"></el-button>
          </div>
          <h2 style="color: white;margin-top: -25px;margin-bottom: 5px;" >Forget Password</h2>
          <el-form :model="ForData" :rules="rules" ref="ForData" label-width="0px" class="demo-ruleForm">

            <el-form-item prop="username">
              <el-input  class="width text" style="color: white"
                         placeholder="请输入用户名" v-model="ForData.username" clearable></el-input>
            </el-form-item>

            <el-form-item prop="password1">
              <el-input class="width text" style="margin-top:0px;color: white"
                        placeholder="请设置密码" v-model="ForData.password1" show-password clearable></el-input>
            </el-form-item>

            <el-form-item prop="password2">
              <el-input class="width text" style="margin-top:0px;color: white"
                        placeholder="确认密码" v-model="ForData.password2" show-password clearable></el-input>
            </el-form-item>

            <el-form-item prop="email">
              <el-input class="width text" style="margin-top:0px;color: white"
                        placeholder="请输入邮箱" v-model="ForData.email"  clearable></el-input>
            </el-form-item>

            <el-form-item prop="code" class="width left">
              <el-input class="text" style="color: white;width: 50%;margin-left: -13px"
                        placeholder="验证码" v-model="ForData.code" clearable></el-input>
              <el-button   v-if="count===0" style="margin-left: 55px;width: 112px" @click="getCode2()">获取验证码</el-button>
              <el-button type="info" v-if="count!==0" style="margin-left: 55px;width: 112px" disabled>{{this.count}}</el-button>
              <!--<el-button style="margin-left: 55px;width: 112px" @click="getCode2()">获取验证码</el-button>-->
            </el-form-item>

          </el-form>

          <div class="width left" style="margin-top:10px;">
           <span  style="color: rgb(225,225,225);
               font-size: 18px;font-family: 方正姚体;width: 50%" @click="start()">想起密码了？</span>
            <el-button   style="margin-left:57px;width: 112px" v-on:click="ForOk('ForData')">确认</el-button>
          </div>



        </div>
      </transition>
    </div>
</template>

<script>
    export default {
        name: "ForgetComponent",
      props:{
        ForData:{
          type:Object,
        }
      },
      data(){
        var checkUsername = (rule, value, callback) => {
          var regExp = /\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/;
          if (!regExp.test(value)) {
            return callback(new Error('邮箱名不合法'));
          }
          else {
            callback();
          }

        };
        var checkpass1 = (rule, value, callback) => {
          if (value === '') {
            callback(new Error('请输入密码'));
          } else {
            if (this.ForData.password2 !== '') {
              this.$refs.ForData.validateField('checkpass2');
            }
            callback();
          }
        };
        var checkpass2 = (rule, value, callback) => {
          if (value === '') {
            callback(new Error('请再次输入密码'));
          } else if (value !== this.ForData.password1) {
            callback(new Error('两次输入密码不一致!'));
          } else {
            callback();
          }
        };
        return {
          count:0,
          rules: {
            username:[
              { required: true, message: '请输入用户名', trigger: 'blur' },
            ],
            email: [
              { required: true, message: '请输入邮箱', trigger: 'blur' },
              { validator: checkUsername, trigger: 'blur' }
            ],
            password1:[
              { validator: checkpass1, trigger: 'blur' }
            ],
            password2:[
              { validator: checkpass2, trigger: 'blur' }
            ],
            code:[
              { required: true, message: '请输入验证码', trigger: 'blur' },
            ]
          },
        }
      },
      methods:{
        loginShow(){
          this.$emit('loginShow');
        },
        getCode2(){

          const TIME_COUNT = 60;
          if (!this.timer) {
            this.count = TIME_COUNT;
            this.show = false;
            this.timer = setInterval(() => {
              if (this.count > 0 && this.count <= TIME_COUNT) {
                this.count--;
              } else {
                this.show = true;
                clearInterval(this.timer);
                this.timer = null;
              }
            }, 1000)
          }
          this.$emit('getCode2');
        },
        start(){
          this.$emit('start');
        },
        ForOk(formName){
          this.$refs[formName].validate((valid) => {
            if (valid) {
              this.$emit('ForOk');
            }
          });

        },
      }
    }
</script>

<style scoped>
  @import "../css/state.css";
  .forget{
    height: 450px;
    width: 600px;
    border:1px solid rgba(255,255,255,0.5);
    color: #050505;
    position: absolute;
    top: 25%;
    left: 33%;
    font-size: 20px;
  }

  .center{
    width: 400px;
    height: 300px;
    /*background-color: pink;*/
  }

  .title{
    font-size: 50px;
    margin-bottom: 30px;
  }

  .width{
    width:350px;
  }

  .mar{
    margin: 10px 0px;
  }

  .button{
    width: 100px;
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
