<template>
    <div class="wrap">
      <PersonComponent :alertText="name"></PersonComponent>
      <router-view/>
      <!--<FootComponent></FootComponent>-->
    </div>
</template>

<script>
  import FootComponent from  '../components/FootComponent'
  import {getCookie,setCookie,delCookie} from "../js/cookie";
  import PersonComponent from '../components/PersonComponent'

  export default {
        name: "Home",
      components:{
          FootComponent,
        PersonComponent,
      },
    mounted(){
      //    初始路径

          //判断是否登录过，没有登录跳转到登录页面
          let uname=getCookie('username');
          this.name=uname;
          console.log("heha"+this.name);
          if(this.name =="")
          {
            this.$router.push("/Main")
          }
      // this.choose.setProduce(this.item.industryId);
      //     if(localStorage.getItem("produceState")!==''){
      //       this.$router.push("/Home/Manage");
      //     }
    },
      data(){
          return{
            name:""
          }
      },
    methods:{
      on(){

      }
    },
    }
</script>

<style scoped>
.wrap{
  background-color: #fff;
  color: #050505;

}

</style>
