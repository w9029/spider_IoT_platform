<template>
  <!--添加传感器的弹框-->
  <div>
    <div class='popContainer'></div>
    <div class="addsensor animated slideInRight top">
      <!--标题-->
      <div style="width: 100%; height: 80px; border-bottom: 1px solid #cccccc" class="center" >
        <div class="addtitle center between">
          <div class="addtitle-left left">
            <span>添加传感器</span>
          </div>
          <div class="addtitle-right right">
            <el-button type="text" icon="el-icon-close" v-on:click="SensorShow()"></el-button>
          </div>
        </div>
      </div>
      <!--主体-->
      <div class="addmain top">
        <el-form :model="AddSensorForm" :rules="rules" ref="AddSensorForm" label-width="" class="top">

          <el-row class="addmain top">
            <el-row class="addmain-part1" style="min-height: 100px">

              <el-form-item label="传感器名称" prop="name" :key="">
                <el-input style="width: 200px" size="mini" v-model="AddSensorForm.name"></el-input>
              </el-form-item>

              <el-form-item label="传感器类型" prop="type" style="margin-top: -10px">
                <el-radio-group v-model="AddSensorForm.type"
                                style="display: flex; flex-wrap: wrap;margin-top: 10px" @change="changevalue">
                  <el-radio  label="1"  class="addmainpart_mar">温湿氨气三合一(1)</el-radio>
                  <el-radio  label="2" class="addmainpart_mar">氨气(2)</el-radio>
                  <el-radio  label="3" class="addmainpart_mar">风速(3)</el-radio>
                  <el-radio label="4" class="addmainpart_mar">GPS(4)</el-radio>
                  <el-radio label="5" class="addmainpart_mar">溶氧仪(5)</el-radio>
                  <el-radio label="6" class="addmainpart_mar">光照(6)</el-radio>
                  <el-radio label="7" class="addmainpart_mar">土壤(7)</el-radio>
                  <el-radio label="0" class="addmainpart_mar">其他</el-radio>
                </el-radio-group>
              </el-form-item>
            </el-row>

            <el-row  v-show="Part2" class="addmain-part2" >
              <el-row class="left" style="">
                <el-col style="width: 200px">
                  <el-form-item label="站号" prop="number">
                    <el-input style="width: 50px;" size="mini" v-model="AddSensorForm.number"></el-input>
                  </el-form-item>
                </el-col>
                <el-col>
                  <el-form-item label="485指令" prop="code">
                    <el-input style="width: 170px; " size="mini" v-model="AddSensorForm.code"></el-input>
                  </el-form-item>
                </el-col>
              </el-row>

              <el-row class="left" style="margin-top: -10px">
                <el-col style="width: 300px">
                  <el-form-item label="指令返回长度" prop="lenght" >
                    <el-input style="width: 120px; margin-left: -40px" size="mini" v-model="AddSensorForm.lenght"></el-input>
                  </el-form-item>
                </el-col>
              </el-row>
              <el-row style="width: 400px; " class="myselfinput" >
                <el-form-item
                  v-for="(domain, index) in AddSensorForm.domains"
                  :label="'数据' + index"
                  style="display: flex;justify-content: flex-start;width: 400px;margin-bottom: 10px" :key="index"
                >

                  <el-popover
                    ref="popover"
                    placement="top"
                    title="数据的类型名称"
                    style="font-family: 方正姚体"
                    trigger="focus"
                    :content=domain.typeName>
                  </el-popover>
                  <el-input id="input" style="width: 40px;" v-model="domain.typeName" size="mini" v-popover:popover placeholder="  type"></el-input>

                  <el-popover
                    ref="popStart"
                    placement="top"
                    title="数据的起始位置"
                    style="font-family: 方正姚体"
                    trigger="focus"
                    :content=domain.byteStart>
                  </el-popover>
                  <el-input style="width: 30px" v-model="domain.byteStart"  size="mini"  v-popover:popStart  placeholder="start"></el-input>

                  <el-popover
                    ref="popCount"
                    placement="top"
                    title="数据的字节数量"
                    style="font-family: 方正姚体"
                    trigger="focus"
                    :content=domain.byteCount>
                  </el-popover>
                  <el-input style="width: 30px" v-model="domain.byteCount"  size="mini"  v-popover:popCount  placeholder="count"></el-input>


                  <el-popover
                    ref="popFloat"
                    placement="top"
                    title="数据类型"
                    style="font-family: 方正姚体"
                    trigger="focus">
                  </el-popover>
                  <el-select style="width: 60px" size="mini"  v-popover:popFloat v-model="domain.dataDefine" placeholder="请选择" @change="NumTypeChange">
                    <el-option
                      v-for="item in options"
                      :key="item.value"
                      :label="item.label"
                      :value="item.value">
                    </el-option>
                  </el-select>

                  <el-popover
                    ref="popNum"
                    placement="top"
                    title="可否为负值"
                    style="font-family: 方正姚体"
                    trigger="focus">
                  </el-popover>
                  <el-select  :disabled="ZhengFu" style="width: 60px" size="mini"  v-popover:popNum v-model="domain.negative" placeholder="请选择">
                    <el-option
                      v-for="item in options2"
                      :key="item.value"
                      :label="item.label"
                      :value="item.value">
                    </el-option>
                  </el-select>

                  <!--<el-input style="width: 50px" v-model="domain.dataDefine"  size="mini" placeholder=""></el-input>-->

                  <el-popover
                    ref="popPoint"
                    placement="top"
                    title="数据的小数点位数"
                    style="font-family: 方正姚体"
                    trigger="click"
                    :content=domain.dicimal>
                  </el-popover>
                  <el-input :disabled="Count" style="width: 30px" v-model="domain.dicimal"  v-popover:popPoint  size="mini" placeholder="位数"></el-input>


                  <el-button @click.prevent="removeDomain(domain)" size="mini">删除</el-button>
                </el-form-item>
                <el-form-item style="display: flex;justify-content: flex-start">
                  <el-button @click="addDomain()" size="mini">新增数据类型</el-button>
                  <el-button @click="submitDomain('AddSensorForm')" size="mini">提交测试</el-button>
                </el-form-item>
                <el-input type="textarea" disabled v-model="AddSensorForm.textarea"></el-input>
              </el-row>
            </el-row>
          </el-row>

          <el-row style="height: 80px;width: 100%;border-top: 1px solid #cccccc;margin-top: 10px" class="center">
            <el-form-item style="margin-top: 12px">
              <el-button @click="SensorNot('AddSensorForm')">取消</el-button>
              <el-button @click="SendorOk('AddSensorForm')" style="margin-left: 80px" :disabled="TestValue">确认添加</el-button>
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
        name: "SensorWindow",
      props:{
        AddSensorForm:{
          type:Object,
        },
        rules:{
          type:Object,
        }
      },
      data(){
        return{
          TestValue:true,
          Data:false,
          Part2:false,
          Count:false,
          ZhengFu:false,
          options2:[
            {
              value:'0',
              label:'false'
            },
            {
              value:'1',
              label:'true'
            }
          ],
          options:[
            {
              value: 'float',
              label: 'float'
            },
            {
              value: 'int',
              label: 'int'
            },
            {
              value: 'double',
              label: 'double'
            },
            {
              value: 'string',
              label: 'string'
            }
          ],
        }
      },
      methods:{
        NumTypeChange(value){
          if(value==='string'){
            this.Count=true;
            this.ZhengFu=true;
          }
          else{
            this.Count=false;
            this.ZhengFu=false;
          }
        },
        SensorShow(){
          this.$emit('SensorShow');
        },
        changevalue(){
          console.log(this.AddSensorForm.type);
          if(this.AddSensorForm.type=='0'){
            this.rules.number[0].required=true;
            this.rules.code[0].required=true;
            this.rules.lenght[0].required=true;
            this.Part2=true;
            this.TestValue=true;
          }
          else{
            this.TestValue=false;
            this.rules.number[0].required=false;
            this.rules.code[0].required=false;
            this.rules.lenght[0].required=false;
            this.Part2=false;
          }
        },
        removeDomain(item) {
          // console.log(item);
          var index = this.AddSensorForm.domains.indexOf(item)
          if (index !== -1) {
            this.AddSensorForm.domains.splice(index, 1)
          }
        },
        addDomain() {

          this.AddSensorForm.domains.push({
            key: Date.now()
          });
        },
        submitDomain(formName){
          this.TestValue=false;
          this.$refs[formName].validate((valid) => {
            if (valid) {
              this.$emit('submitDomain');
            }
          });

        },
        SensorNot(formName){
          this.Part2=false;
          this.$emit('SensorNot');
          this.$refs[formName].resetFields();
        },
        SendorOk(formName){
          this.$refs[formName].validate((valid) => {
            if (valid) {
              this.Part2=false;
              this.$emit('SendorOk');
            }
          });
        },
      }
    }
</script>


<style scoped>
  @import "../../../../../css/add.css";
  @import "../../../../../css/page.css";
  @import "../../../../../css/state.css";
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

  /*#input{*/
    /*padding:0px!important;*/
    /*height: 100px;*/
  /*}*/

</style>

<style>
  .myselfinput .el-input__inner{
    padding:0px!important;
  }
</style>
