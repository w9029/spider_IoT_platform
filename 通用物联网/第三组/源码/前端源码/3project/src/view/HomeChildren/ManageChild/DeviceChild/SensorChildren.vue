<template>
<div class="between" style="width: 300px" >
  <div>
    <span>{{sensor.name}}({{sensor.addr}})&nbsp&nbsp&nbsp&nbsp&nbsp</span>
  </div>
  <div>
    <el-button type="text" size="small" v-on:click="all()">详情</el-button>
    <el-button type="text" size="small" v-on:click="sensorEdit()">编辑/阈值设置</el-button>
    <el-button type="text" size="small" :disabled='this.Isdisabled' v-on:click="del()">删除</el-button>
  </div>
</div>
</template>

<script>
    export default {
        name: "SensorChildren",
      mounted(){
        // console.log("传感器的名字"+this.sensorName);
        // console.log("第几个传感器"+this.index);
        // console.log("传感器所在的设备"+this.deviceNum);
        if(this.state==='已入网'){
          this.Isdisabled=false;
        }
        else{
          this.Isdisabled=true;
        }
      },
      props:{
        sensor:{
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
            this.$emit('sensorremove',this.deviceNum,this.index);
          },
          all(){
            this.$emit('sensordetail',this.deviceNum,this.index);
          },
        sensorEdit(){
          this.$emit('Edit',this.deviceNum,this.index);
        },
      },
    }
</script>

<style scoped>
@import "../../../../css/state.css";
span{
  cursor: default;
}
</style>
