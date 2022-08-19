<template>
  <!--添加传感器的弹框-->
  <div>
    <div class='popContainer'></div>
    <div class="addsensor animated slideInRight top">
      <!--标题-->
      <div style="width: 100%; height: 80px; border-bottom: 1px solid #cccccc" class="center" >
        <div class="addtitle center between">
          <div class="addtitle-left left">
            <span>继电器详情</span>
          </div>
          <div class="addtitle-right right">
            <el-button type="text" icon="el-icon-close" v-on:click="RelDetailShow()"></el-button>
          </div>
        </div>
      </div>
      <!--主体-->
      <div style="width: 100%; height:100%;" class="top">
        <div style="width: 60%; height:80px;" class="between">
          <label>继电器名称:</label>
          <div style="width: 50%;height: 100%" class="left">{{RelDetail.relayName}}</div>
        </div>
        <div style="width: 60%; height:80px" class=" between">
          <label>继电器站号：</label>
          <div style="width: 50%;height: 100%" class="left">{{RelDetail.relayAddr}}</div>
        </div>

        <div style="width: 100%; " >
          <el-table :data="RelDetail.relayMessage" style="width: 100%; min-height: 500px">
            <el-table-column prop="machinePosition" label="继电器引脚号" width="180"></el-table-column>
            <el-table-column prop="machineState" label="状态" width="180"></el-table-column>
            <el-table-column prop="machineName" label="名称"></el-table-column>
          </el-table>
        </div >


      </div>

    </div>
  </div>
  <!--添加传感器的弹框-end-->

</template>

<script>
  export default {
    name: "RelayDetail",
    props:{
      RelDetail:{
        type:Object,
      },
    },
    data(){
      return{

      }
    },
    methods:{
      RelDetailShow(){
        this.$emit('RelDetailShow');
      },

    },
  }
</script>

<style scoped>
  @import "../../../../../css/add.css";
  @import "../../../../../css/page.css";
  @import "../../../../../css/state.css";

  label{
    color: #666666;
  }

  div.popContainer{
    position: fixed;
    width: 100%;
    height: 100%;
    top: 0;
    left: 0;
    background: rgba(0,0,0,0.3);
    z-index: 80;
    overflow: hidden;
  }
  .addsensor{
    position: fixed;
    height: 100%;
    width: 450px;
    top:0px;
    right: 0px;
    overflow: hidden;
    z-index: 999;
    background: rgb(240, 240, 240);
  }

</style>
