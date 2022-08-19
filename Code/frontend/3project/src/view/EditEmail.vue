<template>
  <div>
    <div class="head">
      <div class="container between">
        <div style="width: 210px" class="between">
          <span style="margin-left: 30px; cursor: pointer; font-size: 18px" @click="ChangeMain()">XXX物联网</span>
          <div style="color: #a7a7a7;margin: 0px 10px" >|</div>
        </div>

        <div class=" between" style="width: 100%; margin-left: 29.5px">
          <el-menu
            :default-active="activeIndex2"
            class="el-menu-demo"
            style="border-bottom: 0px;"
            mode="horizontal"
            background-color="#111"
            text-color="#ccc"
            active-text-color="#2d8cf0">
            <el-menu-item index="1" @click="GoCenter()"><i class="el-icon-user"></i>个人中心</el-menu-item>
          </el-menu>
        </div>
      </div>
    </div>

    <div class="mainCenter container" >
      <h2 style="margin:0 auto; color: #333333;padding-bottom: 30px">修改邮箱</h2>
      <el-form :model="DataForm" ref="DataForm" label-width="100px" class="demo-dynamic" :rules="rules">
        <el-form-item prop="email" label="新邮箱" >
          <el-input v-model="DataForm.email"></el-input>
        </el-form-item>
        <el-form-item prop="number">
            <el-input style="width: 250px;margin-right: 30px" v-model="DataForm.number"></el-input>
            <el-button  v-if="count===0" style="width: 115px" @click="ChangeCount()">获取验证码</el-button>
            <el-button type="info" v-if="count!==0" style="width: 115px" disabled>{{this.count}}</el-button>
        </el-form-item>
        <el-form-item>
          <el-button type="primary" @click="submitForm('DataForm')">提交</el-button>
          <el-button @click="resetForm('DataForm')">重置</el-button>
        </el-form-item>
      </el-form>
    </div>
  </div>
</template>

<script>
  import {getCookie} from "../js/cookie";

  export default {
    name: "EditEmail",
    data(){
      return{
        username:"",
        activeIndex: '1',
        activeIndex2: '1',
        value:'',
        DataForm: {
          email: '',
          number:'',
        },
        rules: {
          email: [
              {required: true, message: '请输入邮箱地址', trigger: 'blur'},
              {type: 'email', message: '请输入正确的邮箱地址', trigger: ['blur', 'change']}
            ],
          number:[
            {required: true, message: '请输入验证码', trigger: 'blur'},
          ]
        },
        count:0,
      }
    },
    mounted(){
      let uname=getCookie('username');
      this.username=uname;
      if(uname ==="")
      {
        this.$router.push("/Main");
      };
    },
    methods:{
      ChangeMain(){
        this.$router.push('/Home');
      },
      submitForm(formName) {
        this.$refs[formName].validate((valid) => {
          if (valid) {
            let data={email:this.DataForm.email,identity:this.DataForm.number,name:this.username}
            this.$axios({
              url:this.URL.BASE_URL+'/user/changemail',
              method:'POST',
              data:JSON.stringify(data),
              contentType:"application/json",
              headers:{
                'Content-Type':'application/json'
              }
            }).then((res)=> {

              console.log(res.data);
              if(res.data==='success'){
                alert(res.data);
                this.$router.push('/Center');
              }
              else {
                alert(res.data);
              }
              setTimeout(function(){

              }.bind(this),1000)
            });
          } else {
            console.log('error submit!!');
            return false;
          }
        });
      },
      resetForm(formName) {
        this.$refs[formName].resetFields();
      },
      GoCenter(){
        this.$router.push('/Center');
      },
      ChangeCount(){
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

        let data={email:this.DataForm.email}
        this.$axios({
          url:this.URL.BASE_URL+'/user/identity',
          method:'POST',
          data:JSON.stringify(data),
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {

          console.log(res.data);

          setTimeout(function(){

          }.bind(this),1000)
        });

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
    margin-bottom: 160px;
  }
  .mainCenter{
    width: 500px;
    height: 590px;
    /*background-color: #5accaf;*/
    /*background: url("../assets/未标题-1.jpg");*/
  }

</style>
