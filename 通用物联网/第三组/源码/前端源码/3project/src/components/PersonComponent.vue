<template>
<div class="head  between">
  <div style="width: 210px" class="between">
    <span style="margin-left: 15%; cursor: pointer; font-size: 18px" @click="ChangeMain()">Spider IoT</span>
    <div style="color: #a7a7a7;margin-left:7%" >|</div>
  </div>

  <!--<p >{{alertText}}</p>-->
  <!--<button v-on:click="on()">click</button>-->
  <div class=" between" style="width: 100%; margin-left: 33px">

    <el-menu
      :default-active="activeIndex2"
      class="el-menu-demo"
      style="border-bottom: 0px;"
      mode="horizontal"
      @select="handleSelect"
      background-color="#111"
      text-color="#ccc"
      active-text-color="#2d8cf0">
      <el-menu-item index="1"><i class="el-icon-bangzhu"></i>产业中心</el-menu-item>
      <!--<el-menu-item index="2"><i class="el-icon-s-tools"></i>设备管理</el-menu-item>-->
    </el-menu>

    <el-dropdown style="margin-right: 70px">
      <div class="top" >
        <el-avatar src="https://cube.elemecdn.com/0/88/03b0d39583f48206768a7534e55bcpng.png"></el-avatar>
      </div>
      <el-dropdown-menu slot="dropdown">
        <el-dropdown-item disabled>{{this.username}}</el-dropdown-item>
        <div @click="personalCenter()">
          <el-dropdown-item  divided  >个人中心</el-dropdown-item>
        </div>
        <div v-on:click="quit()">
          <el-dropdown-item divided>退出登录</el-dropdown-item>
        </div>
      </el-dropdown-menu>
    </el-dropdown>

  </div>
</div>
</template>

<script>
  import {delCookie} from "../js/cookie";

  export default {
        name: "PersonComponent",
      props: ['alertText'], //接收传的值
      data(){
          return{
            username:"",
            activeIndex: '1',
            activeIndex2: '1'
        }
      },
      watch:{
        alertText: function(newVal,oldVal){
          this.username = newVal;  //newVal即是chartData
        }
      },
      methods:{
        handleSelect(key, keyPath) {
          console.log(key, keyPath);
        },
        quit(){
              // alert("ok");
            /*删除cookie*/
           // localStorage.removeItem("produceState");
            delCookie('username');
            this.$router.push('/Main');
        },
        ChangeMain(){
          // localStorage.removeItem("produceState");
          this.$router.push('/Home');
        },
        personalCenter(){
          this.$router.push('/Center');
        },
      }
    }
</script>


<style scoped>
@import "../css/state.css";
.head{
  background-color: #111;
  color: white;
}


</style>
