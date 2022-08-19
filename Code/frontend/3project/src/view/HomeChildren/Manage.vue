<template>
<div class="left" style="width: 100%;">
  <!--侧边栏-->
  <div style="width: 13.9%;height:100%;background-color: #3a8ee6" id="ManageLeftPart">
    <div class="a" :class="searchBarFixed == true ? 'isFixed' :''">
      <el-menu
        default-active="/Home/Manage"
        background-color="#303035"
        text-color="#fff"
        active-text-color="#2d8cf0"
        :default-active="$route.path"
        :router="true">

        <el-menu-item index="/Home/Manage" class="left"  >
          <i class="el-icon-box"></i>
          <span>&nbsp&nbsp 产业概况</span>
        </el-menu-item>

        <el-menu-item index="/Home/Manage/Page2" class="left">
          <i class="el-icon-menu"></i>
          <span slot="title">&nbsp&nbsp  采集单元管理</span>
        </el-menu-item>


        <el-menu-item index="/Home/Manage/Page3" class="left">
          <i class="el-icon-document"></i>
          <span slot="title">&nbsp&nbsp  历史数据</span>
        </el-menu-item>

        <el-menu-item index="/Home/Manage/Page4" class="left">
          <i class="el-icon-key"></i>
          <span slot="title">&nbsp&nbsp  实时监测</span>
        </el-menu-item>

        <el-menu-item index="/Home/Manage/Page5" class="left">
          <i class="el-icon-setting" ></i>
          <span slot="title">&nbsp&nbsp  规则定义</span>
        </el-menu-item>

        <el-menu-item index="/Home/Manage/Page6" class="left">
          <i class="el-icon-help" ></i>
          <span slot="title">&nbsp&nbsp  远程调试</span>
        </el-menu-item>

        <el-menu-item index="/Home/Manage/Page8" class="left">
          <i class="el-icon-mouse" ></i>
          <span slot="title">&nbsp&nbsp  webSSH调试</span>
        </el-menu-item>

        <el-menu-item index="/Home/Manage/Page7" class="left">
          <i class="el-icon-notebook-2" ></i>
          <span slot="title">&nbsp&nbsp  操作日志</span>
        </el-menu-item>



      </el-menu>
    </div>
  </div>
  <!--右侧主体-->

  <div  style="width: 86.2%; height:100%;background-color: pink">
    <router-view style="width: 86.2%;"></router-view>
  </div>



</div>
</template>

<!--style="margin-left: 210px;"-->

<script>
  import PersonComponent from '../../components/PersonComponent'

  export default {
        name: "Manage",
       components:{
         PersonComponent,
       },
        mounted(){
          this.$router.push("/Home/Manage");
          window.addEventListener('scroll', this.handleScroll);
        },

      data(){
          return{
            searchBarFixed:false,
          }
        },
      methods:{
        handleScroll () {
          var scrollTop = window.pageYOffset || document.documentElement.scrollTop || document.body.scrollTop
          // console.log(scrollTop);
          let offsetTop = document.querySelector('#ManageLeftPart').offsetTop
          scrollTop > offsetTop ? this.searchBarFixed = true : this.searchBarFixed = false
        },
        destroyed () {
          window.removeEventListener('scroll', this.handleScroll)
        },
      }
    }
</script>

<style scoped>
  @import "../../css/state.css";
  .a{
    height: 100%;
    width: 13.9%;
    position: absolute;
    background-color: #303035;
  }
  *{
    border: 0px;
  }
  .isFixed{
    position:fixed;
    top:0;
    z-index:999;
  }


</style>
