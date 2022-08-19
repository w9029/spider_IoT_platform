<template>
  <div class="page_main top">
    <LineComponent :title="title" :content="content"></LineComponent>
    <div style="background-color: #f5f6f7; width: 100%" >

      <div style="width: 90%;margin: 40px;min-height: 650px" class="top">
        <div style="width: 100%;margin-bottom: 20px" class="left" >
          <el-input style="width: 300px" clearable placeholder="输入设备IP地址" v-model="websshIP"></el-input>
          <el-button type="primary" style="margin-left: 40px" @click="submit()">确定</el-button>
        </div>
        <div style="width: 90%;height:650px;overflow: hidden">
          <iframe :src="src" width="100%" height="100%" frameborder="0">
            您的浏览器不支持iframe，请升级
          </iframe>
        </div>
      </div>

    </div>

  </div>
</template>

<script>
  import LineComponent from '../../../components/LineComponent'
  export default {
    name: "Page8",
    components: {
      LineComponent,
    },
    data() {
      return {
        id: '',
        title: 'webSSH调试',
        content: '这是Page8的内容',
        websshIP:'',
        src:''
      }
    },
    mounted() {

    },

    methods: {
      submit(){
        console.log(this.websshIP);
        this.$axios({
          url:this.URL.BASE_URL+'/user/webssh',
          method:'POST',
          params:{
            ip:this.websshIP,
          },
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {

          console.log(res.data);
          this.src='http://'+res.data;

          setTimeout(function(){

          }.bind(this),1000)
        });

      },
    }
  }
</script>

<style scoped>
  @import "../../../css/state.css";
  @import "../../../css/page.css";
</style>



