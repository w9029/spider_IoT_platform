<template>
  <div class="between" style="width: 300px">
    <div>
      <span>{{relay.name}}({{relay.addr}})&nbsp&nbsp&nbsp&nbsp&nbsp</span>
    </div>
    <div>
      <el-button type="text" size="small" v-on:click="all()">详情</el-button>
      <el-button type="text" size="small" v-on:click="ReEdit()">编辑</el-button>
      <el-button type="text" size="small" v-on:click="ReMachine()">管理强电设备</el-button>
      <el-button type="text" size="small" :disabled='this.Isdisabled' v-on:click="del()">删除</el-button>
    </div>
  </div>
</template>

<script>
  export default {
    name: "RelayChildren",
    mounted(){
      // console.log("继电器的名字"+this.relayName);
      // console.log("第几个继电器"+this.index);
      // console.log("继电器所在的设备"+this.deviceNum);
      if(this.state==='已入网'){
        this.Isdisabled=false;
      }
      else{
        this.Isdisabled=true;
      }
    },
    props:{
      relay:{
        type:Object,
      },
      index:{
        type:Number,
      },
      state:{
        type:String,
      },
      deviceNum:{
        type:Number,
      },
    },
    data(){
      return{
        Isdisabled:'',
      }
    },
    methods:{
      del(){
        this.$emit('relayremove',this.deviceNum,this.index);
      },
      all(){
        this.$emit('relaydetail',this.deviceNum,this.index);
      },
      ReEdit(){
        this.$emit('ReEdit',this.deviceNum,this.index);
      },
      ReMachine(){
        this.$emit('ReMachine',this.deviceNum,this.index);
      }
    },
  }
</script>

<style scoped>
  @import "../../../../css/state.css";
  span{
    cursor: default;
  }
</style>
