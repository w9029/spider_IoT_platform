<template>
  <!--添加传感器的弹框-->
  <div>
    <div class='popContainer'></div>
    <div class="addsensor animated slideInRight top">
      <!--标题-->
      <div style="width: 100%; height: 80px; border-bottom: 1px solid #cccccc" class="center" >
        <div class="addtitle center between">
          <div class="addtitle-left left">
            <span>传感器编辑/阈值设置</span>
          </div>
          <div class="addtitle-right right">
            <el-button type="text" icon="el-icon-close" v-on:click="SenEditShow()"></el-button>
          </div>
        </div>
      </div>
      <!--主体-->
      <div class="addmain top">
        <el-form :model="EditSenForm" :rules="rules" ref="EditSenForm" label-width="" class="top" style="width: 100%">

          <el-row class="addmain top">
            <el-form-item style="width: 300px"  label="传感器名称"  :key="" class="right">
              <el-input style="width: 200px" size="mini" v-model="EditSenForm.sensorName"></el-input>
            </el-form-item>
            <el-form-item style="width: 300px;overflow :auto;overflow-x:hidden;"  v-for="item in EditSenForm.typeList" :label=item.typeName class="right" :key="">
              <div  style="width: 200px" class="left">
                <el-input style="width: 80px;" size="mini" v-model="item.min" placeholder="min"></el-input>
                <el-input style="width: 80px" size="mini" v-model="item.max" placeholder="max"></el-input>
              </div>
            </el-form-item>
          </el-row>

          <el-row style="height: 80px;width: 100%;border-top: 1px solid #cccccc;margin-top: 10px" class="center">
            <el-form-item style="margin-top: 12px">
              <el-button @click="SenEditShow()">取消</el-button>
              <el-button @click="SenEditOk()" style="margin-left: 80px">确认修改</el-button>
            </el-form-item>
          </el-row>
        </el-form>
      </div>

    </div>
  </div>
  <!--添加传感器的弹框-end-->

</template>

<script>
  export default {
    name: "Edit",
    props:{
      EditSenForm:{
        type:Object,
      },
      rules:{
        type:Object,
      }
    },
    data(){
      return{

      }
    },
    methods:{
      SenEditShow(){
        this.$emit('SenEditShow');
      },

      SenEditOk(){
        this.$emit('SenEditOk');
      }
    },
  }
</script>

<style scoped>
  @import "../../../../css/add.css";
  @import "../../../../css/page.css";
  @import "../../../../css/state.css";
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
