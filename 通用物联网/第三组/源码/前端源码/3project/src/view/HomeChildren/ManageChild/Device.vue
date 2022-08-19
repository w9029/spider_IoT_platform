<template>
<div class="page_main top ">
<!--头-->
    <div class="left line_marbottom" style="width: 100%;">
      <el-breadcrumb separator="/" class="center left device_part1"  style="margin-left: 20px">
        <el-breadcrumb-item :to="{ path: '/Home/Manage/Page2' }">采集单元管理</el-breadcrumb-item>
        <el-breadcrumb-item >{{this.name}}</el-breadcrumb-item>
        <el-breadcrumb-item>设备列表</el-breadcrumb-item>
      </el-breadcrumb>
  </div>
  <!--显示个数、添加 start-->
  <div class="container between line_marbottom" style="background-color: white;height: 100px;padding: 30px 0px; margin-bottom: 30px">
    <div class="top">
      <span class="number" style="margin-left: 20px">设备数量（个）</span>
      <h2 style="margin-top: 10px;" class="world_color">{{this.DeviceCount}}</h2>
    </div>
    <el-button  type="primary" icon="el-icon-help" @click="DeviceAddVisible=true" style="margin-right: 40px" >添加设备</el-button>

    <!--添加弹框-->
    <el-dialog
      title="添加设备"
      :visible.sync="DeviceAddVisible"
      width="30%">
      <el-form :model="ruleForm" :rules="rules" ref="ruleForm" label-width="110px" class="demo-ruleForm">
        <el-form-item label="设备名称" prop="name">
          <el-input v-model="ruleForm.name"></el-input>
        </el-form-item>
        <el-form-item label="设备EUI" prop="id">
          <el-input v-model="ruleForm.id" placeholder="输入一次不能更改" ></el-input>
        </el-form-item>
        <el-form-item label="备注" prop="remark">
          <el-input v-model="ruleForm.remark"></el-input>
        </el-form-item>
        <el-form-item label="发送速率" prop="rate">
          <el-input v-model.number="ruleForm.rate"  autocomplete="off"></el-input>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
            <el-button @click="resetForm('ruleForm')">取 消</el-button>
            <el-button type="primary" @click="submitForm('ruleForm')">确 定</el-button>
         </span>
    </el-dialog>

    <!--修改弹框-->
    <el-dialog
      title="修改设备"
      :visible.sync="DeviceEditVisible"
      width="30%">
      <el-form :model="editForm" :rules="rules" ref="editForm" label-width="110px" class="demo-ruleForm">
        <el-form-item label="设备名称" prop="name">
          <el-input v-model="editForm.name"></el-input>
        </el-form-item>
        <el-form-item label="设备EUI" prop="id">
          <el-input v-model="editForm.id" disabled></el-input>
        </el-form-item>
        <el-form-item label="备注" prop="remark">
          <el-input v-model="editForm.remark"></el-input>
        </el-form-item>
        <el-form-item label="发送速率" prop="rate">
          <el-input v-model.number="editForm.rate"  autocomplete="off"></el-input>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
            <el-button @click="resetForm2('editForm')">取 消</el-button>
            <el-button type="primary" @click="submitForm2('editForm')">确 定</el-button>
         </span>
    </el-dialog>

  </div>

  <!--显示个数、添加 end-->
  <!--Part2-->
  <div class="container">
    <el-table :data="DeviceData" style="width: 100%;min-height: 600px">
      <el-table-column type="expand">  <!--通过设置 type="expand" 和 Scoped slot 可以开启展开行功能，-->
        <template slot-scope="props">     <!--el-table-column 的模板会被渲染成为展开行的内容，-->
          <!--展开行可访问的属性与使用自定义列模板时的 Scoped slot 相同。-->
 <!--下拉框里面的内容-->
          <el-form label-position="left" inline class="demo-table-expand">
            <el-form-item label="设备名  :" ><span >{{ props.row.deviceName }}</span></el-form-item>
            <el-form-item label="设备状态  :" ><span >{{ props.row.linkState }}</span></el-form-item>
            <el-form-item label="设备EUI  :"><span >{{ props.row.deviceId }}</span></el-form-item>
            <el-form-item label="发送速率  :"><span >{{ props.row.sendRate }}</span></el-form-item>
            <el-form-item label="备注  :"><span >{{ props.row.deviceRemark }}</span></el-form-item>
            <el-form-item label="所属产业  :"><span >{{ props.row.deviceBelong }}</span></el-form-item>
            <el-form-item label="接入的传感器  :">
              <!--<span>{{props.$index}}</span>-->
              <!--<span>{{DeviceData[props.$index].diviceChildren}}</span>-->
              <!--<div v-for="n in DeviceData[props.$index].diviceChildren">-->
                <!--{{n}}-->
              <!--</div>-->
<!--接入的传感器-->
              <div   class="top" v-for="(item,index) in DeviceData[props.$index].sensorChildren" :child="item" >
                <SensorChildren :sensor="item" :deviceNum="props.$index"
                                :index="index" :state="DeviceData[props.$index].linkState"
                                @sensordetail="sensordetail" @sensorremove="sensorremove"
                                @Edit="Edit"></SensorChildren>
              </div>
              <div style="width: 40px;height: 40px;">
                <el-button  type="text"
                            icon="el-icon-circle-plus-outline" size="medium "
                            :disabled="DeviceData[props.$index].linkState==='已入网'?false:true"
                            v-on:click="addSensorChild(props.$index)"></el-button>
              </div>
            </el-form-item>


<!--接入的继电器-->
            <el-form-item label="接入的继电器  :">
              <div   class="top" v-for="(item,index) in DeviceData[props.$index].relayChildren" :child="item" >
                <RelayChildren  :relay="item" :deviceNum="props.$index" :index="index"
                                :state="DeviceData[props.$index].linkState"
                                @relaydetail="relaydetail" @relayremove="relayremove"
                                @ReEdit="ReEdit" @ReMachine="ReMachine"></RelayChildren>
              </div>
              <div style="width: 40px;height: 40px;">
                <el-button  type="text"
                            icon="el-icon-circle-plus-outline" size="medium "
                            :disabled="DeviceData[props.$index].linkState==='已入网'?false:true"
                            v-on:click="addRelayChild(props.$index)"></el-button>
              </div>

            </el-form-item>
          </el-form>
        </template>
      </el-table-column>

      <el-table-column label="设备名" prop="deviceName"></el-table-column>
      <el-table-column label="设备EUI" prop="deviceId" ></el-table-column>
      <el-table-column label="发送速率" prop="sendRate" ></el-table-column>
      <el-table-column label="状态" prop="linkState" ></el-table-column>
      <el-table-column label="操作">
        <template slot-scope="scope">
          <el-button  type="text"  @click="DeviceLook(scope.row)" :disabled="DeviceData[scope.$index].linkState==='已入网'?false:true">监测</el-button>
          <el-button @click="DeviceEdit(scope.row,scope.$index)" type="text">编辑</el-button>
          <el-button @click="DeviceRemove(scope.row,scope.$index)" type="text">删除</el-button>
        </template>
      </el-table-column>

    </el-table>
  </div>

  <!--添加传感器的弹框-->
  <SensorWindow v-if="AddSensor===true" :AddSensorForm="AddSensorForm" :rules="rules"
                @SensorShow='SensorShow' @submitDomain="submitDomain"
                @SensorNot="SensorNot" @SendorOk="SendorOk"></SensorWindow>
  <!--添加传感器的弹框-end-->

  <!--添加继电器的弹框-->
  <RelayWindow v-if="AddRelay===true" :AddRelayForm="AddRelayForm" :rules="rules"
               @RelayShow="RelayShow"
                @RelayNot="RelayNot" @RelayOk="RelayOk"></RelayWindow>
  <!--添加继电器的弹框-end-->

  <!--添加继电器的详情-->
  <RelayDetail v-if="ShowRelDetail===true"  :RelDetail="RelDetail"
               @RelDetailShow="RelDetailShow"></RelayDetail>
  <!--添加继电器的详情-end-->

  <!--添加传感器的详情-->
  <SensorDetail v-if="ShowSenDetail===true"  :SenDetail="SenDetail"
               @SenDetailShow="SenDetailShow"></SensorDetail>
  <!--添加继电器的详情-end-->

  <!--编辑传感器-->
  <Edit v-if="ShowSenEdit===true" :EditSenForm="EditSenForm" :rules="rules"
        @SenEditShow="SenEditShow" @SenEditOk="SenEditOk"></Edit>
  <!--end-->

<!--编辑继电器-->
  <EditRelay v-if="ShowReEdit===true" :EditReForm="EditReForm" :rules="rules"
        @ReEditShow="ReEditShow" @ReEditOk="ReEditOk"></EditRelay>
  <!--end-->

<div   v-if="Machine===true" >
  <div class='popContainer'></div>
  <div class="addsensor animated slideInRight top">
    <div style="width: 100%; height: 80px;" class="center" >
      <div class="addtitle center between">
        <div class="addtitle-left left">
          <span>强电设备管理</span>
        </div>
        <div class="addtitle-right right">
          <el-button type="text" icon="el-icon-close" v-on:click="MachineShow()"></el-button>
        </div>
      </div>
    </div>

    <el-table :data="tableData" border style="width: 100%;" @cell-dblclick="cellClick">
      <el-table-column label="引脚号" width="230">
        <template slot-scope="scope">
          <i class="el-icon-connection"></i>
          <span style="margin-left: 10px">{{ scope.row.machinePosition }}</span>
        </template>
      </el-table-column>
      <el-table-column label="名称" width="220" >
        <template slot-scope="scope">
          <el-input v-model="scope.row.machineName" v-if="scope.row.seen"

                    @blur="loseFcous(scope.$index, scope.row)" > </el-input>

          <span style="margin-left: 10px" v-else>{{ scope.row.machineName }}</span>
        </template>
      </el-table-column>

    </el-table>

    <el-row style="height: 70px;width: 100%;" class="center">
      <el-row style="margin-top: 12px">
        <el-button @click="MachineShow()">取消</el-button>
        <el-button @click="MachineOk()" style="margin-left: 80px">确认添加</el-button>
      </el-row>
    </el-row>
  </div>
</div>

</div>
</template>

<script>
    import SensorChildren from './DeviceChild/SensorChildren'
    import RelayChildren from './DeviceChild/RelayChildren'
    import SensorWindow from './DeviceChild/AddSensor/SensorWindow'
    import RelayWindow from './DeviceChild/AddRelay/RelayWindow'
    import  RelayDetail from './DeviceChild/AddRelay/RelayDetail'
    import  SensorDetail from './DeviceChild/AddSensor/SensorDetail'
    import  EditRelay from './DeviceChild/EditRelay'
    import  Edit from './DeviceChild/Edit'
    export default {
        name: "Device",
      components:{
        SensorChildren,
        RelayChildren,
        SensorWindow,
        RelayWindow,
        RelayDetail,
        SensorDetail,
        Edit,
        EditRelay,
      },
      mounted(){

        this.id=this.$route.params.id;
        this.name=this.$route.params.name;
        console.log("所属产业:"+this.choose.produce+"|||当前采集单元的ID:"+this.id+"当前采集单元的名字："+this.name);
 //进入页面获取信息-start
        this.$axios({
          url:this.URL.BASE_URL+'/device/findall',
          method:'POST',
          params:{
            industryId:this.choose.produce,
            unitId:this.id,
            // industryId:'22',
            // unitId:'4',
          },
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {

          console.log(res.data);
          this.DeviceData=res.data;
          this.DeviceCount=this.DeviceData.length;

          setTimeout(function(){

          }.bind(this),1000)
        });
//进入页面获取信息-end

 //进入页面获取信息模拟数据-start
 //        this.DeviceData= [
 //          {
 //            deviceName: '设备1',
 //            deviceId: '1201',
 //            sendRate: 20,
 //            linkState:'已入网',
 //            deviceRemark: '设备1的备注',
 //            deviceBelong: '水产/西瓜田',
 //            sensorChildren: [
 //              {
 //                name:'传感器1',
 //                addr:'301',
 //              }
 //            ],
 //            relayChildren:[
 //              {
 //                name: '继电器111',
 //                addr: '301',
 //              }
 //            ],
 //
 //         },
 //          {
 //            deviceName: '设备2',
 //            deviceId: '1202',
 //            sendRate: 20,
 //            linkState:'已入网',
 //            deviceRemark: '设备2的备注',
 //            deviceBelong: '水产/西瓜田',
 //            sensorChildren: [
 //              {
 //                name:'传感器2',
 //                addr:'301',
 //              }
 //            ],
 //            relayChildren:[
 //              {
 //                name: '继电器111',
 //                addr: '301',
 //              }
 //            ],
 //          },
 //          {
 //            deviceName: '设备3',
 //            deviceId: '1203',
 //            sendRate: 20,
 //            linkState:'未入网',
 //            deviceRemark: '设备3的备注',
 //            sensorChildren: [
 //              {
 //                name:'传感器3',
 //                addr:'301',
 //              }
 //            ],
 //            relayChildren:[
 //              {
 //                name: '继电器111',
 //                addr: '301',
 //              }
 //            ],
 //          },
 //
 //        ];
 //        this.DeviceCount=this.DeviceData.length;
//进入页面获取信息模拟数据-end
      },
      data(){

         // 检查EUI
        var checkEUI = (rule, value, callback) => {
          if (value === '') {
            callback(new Error('请输入设备EUI'));
          } else{

            this.$axios({
              url:this.URL.BASE_URL+'/device/linkstate',
              method:'POST',
              params:{
                deviceId:value,
              },
              contentType:"application/json",
              headers:{
                'Content-Type':'application/json'
              }
            }).then((res)=> {

              console.log(res.data);
              var index=0;
              if(res.data==='已入网')
              {
                index=1;
              }
              if(index!=1){
                callback(new Error('EUI错误或设备未连接'));
              }else {
                callback();
              }

              setTimeout(function(){

              }.bind(this),1000)
            });


            /**
             * 向后端请求EUI的状态
             *
             * */
//模拟数据
          //   var index=0;
          //   if(value==='gyq')
          //   {
          //     index=1;
          //   }
          //   if(index!=1){
          //     callback(new Error('EUI错误或设备未连接'));
          //   }else {
          //     callback();
          //   }

//模拟数据
          }

        };

        var checkType = (rule, value, callback) =>{
          if(value<11 || value >245){

            callback(new Error('站号范围10-255之间'));
          }
          else{
            callback();
          }
        };

        return{
          DeviceAddVisible:false,
          DeviceEditVisible:false,
          AddSensor:false,  //传感器弹窗显示/隐藏
          AddRelay:false,
          ruleForm:{
            name:'',
            id:'',
            remark:'',
            rate:'',
            state:'',
          },
          editForm:{
            name:'',
            remark:'',
            id:'',
            rate:'',
            index:'',
          },
          rules: {
            name: [
              { required: true, message: '请输入名称', trigger: 'blur' },
              { min: 3, max: 10, message: '长度在 3 到 10 个字符', trigger: 'blur' }
            ],
            id:[
              { required: true,validator: checkEUI, trigger: 'blur' }
            ],
            rate:[
              { required: true, message: '请输入设备速率', trigger: 'blur' },
            ],
            type: [
              { required: true, message: '请选择类型', trigger: 'change' },
            ],
            number: [
              { required: true, message: '请输入站号', trigger: 'blur' },
              // {  required: true, validator: checkType, trigger: 'blur' }
            ],
            code: [
              { required: true, message: '请输入485指令', trigger: 'blur' },
            ],
            lenght: [
              { required: true, message: '请输入指令返回长度', trigger: 'blur' },
            ],
          },
          AddSensorForm:{
            name:'',
            type:'',
            number:'',
            code:'',
            lenght:'',
            domains: [{
              typeName: '',
              byteStart: '',
              byteCount: '',
              dataDefine: '',
              dicimal: '',
              negative:'',
            }],
            textarea:'测试结果',
          },
          AddRelayForm:{
            name:'',
            number:'',
          },
          EditSenForm:{},
          EditReForm:{
            name:'',
            diviceId:'',
            Addr:'',
            DeviceIndex:'',
            RelayIndex:'',
          },
          id:'',
          name:'',
          DeviceData:[],
          DeviceCount:'',
          SensorIndex:'',
          RelayIndex:'',
          RelDetail:[],
          SenDetail:[],
          ShowRelDetail:false,
          ShowSenDetail:false,
          ShowSenEdit:false,
          ShowReEdit:false,
          Machine:false,
          ReMachineData:{},
          tableData: [],
          SE_deviceIndex:'',
          SE_sensorIndex:'',
        }
      },
      methods:{
        submitForm(formName) {
          this.$refs[formName].validate((valid) => {
            if (valid) {
              this.DeviceAddVisible = false;
//添加设备-start
              this.$axios({
                url:this.URL.BASE_URL+'/device/add/device',
                method:'POST',
                params:{
                  industryId:this.choose.produce,
                  unitId:this.id,
                  // industryId:'22',
                  // unitId:'4',
                  deviceName: this.ruleForm.name,
                  deviceId:this.ruleForm.id,
                  sendRate:this.ruleForm.rate,
                  deviceRemark: this.ruleForm.remark,
                },
                contentType:"application/json",
                headers:{
                  'Content-Type':'application/json'
                }
              }).then((res)=> {

                console.log(res.data);

                if(res.data==='1'){
                  alert("产业已经被添加")
                }

                this.DeviceData.push({
                  deviceName: this.ruleForm.name,
                  deviceId:this.ruleForm.id,
                  sendRate:this.ruleForm.rate,
                  deviceRemark: this.ruleForm.remark,
                  linkState:'已入网',
                  relayChildren:[],
                  sensorChildren:[],
                  deviceBelong: res.data,
                });

                this.DeviceCount++;
                this.$refs[formName].resetFields();

                setTimeout(function(){

                }.bind(this),1000)
              });

              /**
               * 发送产业ID和采集单元ID
               * this.ruleForm.name
               * this.ruleForm.id
               * this.ruleForm.rate
               * this.ruleForm.remark
               * 得到一个deviceBelong
               * */
//添加设备模拟数据-start
//               this.DeviceData.push({
//                 deviceName: this.ruleForm.name,
//                 deviceId:this.ruleForm.id,
//                 sendRate:this.ruleForm.rate,
//                 deviceRemark: this.ruleForm.remark,
//                 linkState:'已入网',
//                 diviceChildren: [],
//                 deviceBelong: '水产/西瓜田',
//               });
//
//               this.DeviceCount++;
//               this.$refs[formName].resetFields();
//添加设备模拟数据-end
            }
          });
        },
        resetForm(formName) {
          this.$refs[formName].resetFields();
          this.DeviceAddVisible = false;
        },
        submitForm2(formName){
          this.$refs[formName].validate((valid) => {
            if (valid) {
              this.DeviceEditVisible = false;
              console.log("修改设备");
//修改设备信息-start
              this.$axios({
                url:this.URL.BASE_URL+'/device/update/device',
                method:'POST',
                params:{
                  industryId:this.choose.produce,
                  deviceId:this.editForm.id,
                  // industryId:'22',
                  // deviceId:'6af6188e14aa',
                  newName:this.editForm.name,
                  newRemark:this.editForm.remark,
                  newSendrate:this.editForm.rate,
                },
                contentType:"application/json",
                headers:{
                  'Content-Type':'application/json'
                }
              }).then((res)=> {

                console.log(res.data);

                this.DeviceData[this.editForm.index].deviceName=this.editForm.name;
                this.DeviceData[this.editForm.index].deviceRemark=this.editForm.remark;
                this.DeviceData[this.editForm.index].sendRate=this.editForm.rate;
                this.$refs[formName].resetFields();

                setTimeout(function(){

                }.bind(this),1000)
              });
//修改设备信息-end

              /***
               * 产业ID+采集单元iD+设备ID
               * 将修改的的数据发送给服务端( this.editForm.name
               *   this.editForm.remark
               *   this.editForm.rate
               * )
               * */
              // this.DeviceData[this.editForm.index].deviceName=this.editForm.name;
              // this.DeviceData[this.editForm.index].deviceRemark=this.editForm.remark;
              // this.DeviceData[this.editForm.index].sendRate=this.editForm.rate;
              // this.$refs[formName].resetFields();

            }
          });
        },
        resetForm2(formName) {
          this.DeviceEditVisible = false;
          this.$refs[formName].resetFields();
        },
        DeviceRemove(row,index){

          console.log("删除设备的ID号:"+row.deviceId);

          this.$axios({
            url:this.URL.BASE_URL+'/device/delete/device',
            method:'POST',
            params:{
              industryId:this.choose.produce,
              deviceId:row.deviceId,
              // industryId:'22',
              // deviceId:'3',
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);
            this.DeviceData.splice(index,1);
            this.DeviceCount--;

            setTimeout(function(){

            }.bind(this),1000)
          });

          /***
           *
           *
           * 发送删除的产业的ID号+采集单ID+设备ID
           *
           *
           * **/
          // this.DeviceData.splice(index,1);
          // this.DeviceCount--;
        },
        DeviceEdit(row,index){
          this.DeviceEditVisible=true;
          console.log("点击编辑"+index);
          this.$nextTick(()=>{
            this.editForm.name=row.deviceName;
            this.editForm.id=row.deviceId;
            this.editForm.remark=row.deviceRemark;
            this.editForm.rate=row.sendRate;
            this.editForm.index=index;
          });
        },
        DeviceLook(row){
          console.log('要监测的设备id'+row.deviceId);
          this.$router.push({
            name: 'Page4',
            params: {
              LookDeviceid: row.deviceId,
            }
          })
        },

        SensorNot(){     //关闭  添加传感器的弹窗
          this.AddSensor=false;
        },
        SensorShow(){    //打开  添加传感器的弹窗
          this.AddSensor=false;
        },
        addSensorChild(index){     //触发添加传感器的按钮
          const ReAddSensorForm={
            name:'',
            type:'',
            number:'',
            code:'',
            lenght:'',
            domains: [{
              typeName: '',
              byteStart: '',
              byteCount: '',
              dataDefine: '',
              dicimal: '',
              negative:'',
            }],
            textarea:'测试结果',
          };
          // console.log("在第几个设备上 添加传感器"+index);
          // console.log(this.DeviceData[index]);
          this.AddSensorForm = Object.assign({},ReAddSensorForm);
          this.AddSensor=true;
          this.SensorIndex=index;
        },
        sensorremove(m,n){
          console.log(this.DeviceData[m].sensorChildren[n].addr);
          console.log(this.DeviceData[m].deviceId)
          this.$axios({
            url:this.URL.BASE_URL+'/sensor/delete/sensor',
            method:'POST',
            params:{
              deviceId:this.DeviceData[m].deviceId,
              sensorAddr:this.DeviceData[m].sensorChildren[n].addr,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);

            if(res.data==="success"){
              this.DeviceData[m].sensorChildren.splice(n,1);
            }
            else{
              alert(res.data);
            }

            setTimeout(function(){

            }.bind(this),1000)
          });

          /****
           *
           * 删除传感器
           * */
          // this.DeviceData[m].sensorChildren.splice(n,1);


        },
        submitDomain(){
          let data={
            industryId:this.choose.produce,
            // industryId:'22',
            deviceId: this.DeviceData[this.SensorIndex].deviceId,
            sensorName:this.AddSensorForm.name,
            addr:this.AddSensorForm.number,
            code485:this.AddSensorForm.code,
            returnLength:this.AddSensorForm.lenght,
            types:this.AddSensorForm.domains,
          };
          console.log(data);
          this.$axios({
            url:this.URL.BASE_URL+'/sensor/link',
            method:'POST',
            data:JSON.stringify(data),
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);

            this.AddSensorForm.textarea=res.data;

            setTimeout(function(){

            }.bind(this),1000)
          });


          /***
           *
           * 提交测试
           * */
          // console.log(this.AddSensorForm);
          // this.AddSensorForm.textarea="hahahah";
        },
        SendorOk(){
          this.AddSensor=false;
          // console.log(this.AddSensorForm);
          // console.log(this.AddSensorForm.domains);
          console.log(this.DeviceData[this.SensorIndex].deviceId);
          if(this.AddSensorForm.type==='0'){
            let data={
              industryId:this.choose.produce,
              // industryId:'22',
              deviceId: this.DeviceData[this.SensorIndex].deviceId,
              sensorName:this.AddSensorForm.name,
              addr:this.AddSensorForm.number,
              code485:this.AddSensorForm.code,
              returnLength:this.AddSensorForm.lenght,
              types:this.AddSensorForm.domains,
            };
            console.log(data);
            this.$axios({
              url:this.URL.BASE_URL+'/sensor/add/sensor',
              method:'POST',
              data:JSON.stringify(data),
              contentType:"application/json",
              headers:{
                'Content-Type':'application/json'
              }
            }).then((res)=> {
              console.log(res.data);
              console.log(this.DeviceData);
              console.log(this.SensorIndex);
              if(res.data==='success'){

                  this.DeviceData[this.SensorIndex].sensorChildren.push({
                    name:this.AddSensorForm.name,
                    addr:this.AddSensorForm.number,
                  });
              }else{
                alert(res.data);
              }


              setTimeout(function(){

              }.bind(this),1000)
            });
          }
          else {
            let data={
              industryId:this.choose.produce,
              // industryId:'22',
              deviceId: this.DeviceData[this.SensorIndex].deviceId,
              sensorName:this.AddSensorForm.name,
              addr:this.AddSensorForm.type,
            };
            console.log(data);
            this.$axios({
              url:this.URL.BASE_URL+'/sensor/add/default',
              method:'POST',
              data:JSON.stringify(data),
              contentType:"application/json",
              headers:{
                'Content-Type':'application/json'
              }
            }).then((res)=> {

              console.log(res.data);
              if(res.data==='success')
              {
                this.AddSensor=false;
                this.DeviceData[this.SensorIndex].sensorChildren.push({
                  name:this.AddSensorForm.name,
                  addr:this.AddSensorForm.type
                });
              }
              else{
                alert(res.data);
              }

              setTimeout(function(){

              }.bind(this),1000)
            });

          }
          /***
           *
           *
           * 确认添加
           *
           * */
          // this.AddSensor=false;
          // this.DeviceData[this.SensorIndex].sensorChildren.push({
          //   name:this.AddSensorForm.name,
          // });

        },
        sensordetail(m,n){
          this.ShowSenDetail=true;
          console.log(this.DeviceData[m].sensorChildren[n].addr);

          this.$axios({
            url:this.URL.BASE_URL+'/sensor/detail',
            method:'POST',
            params:{
              deviceId:this.DeviceData[m].deviceId,
              sensorAddr:this.DeviceData[m].sensorChildren[n].addr,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data[0]);
            if(res.data.length!==0){
              this.SenDetail=res.data[0];
            }
            setTimeout(function(){

            }.bind(this),1000)
          });

          /*******
           * 通过传感器的addr请求详情数据
           *
           * ***/
          // this.SenDetail={
          //   sensorAddr:'01',
          //   code:"eqr",
          //   sensorName:"三合一",
          //   returnLength:"13",
          //   deviceId:"902483319",
          //   typeList:[
          //     {
          //       typeName:"temptrue",
          //       min:12,
          //       max:45,
          //     },
          //     {
          //       typeName:"hum",
          //       min:12,
          //       max:45,
          //     },
          //   ],
          // }
        },

        RelayShow(){
          this.AddRelay=false;
        },
        RelayNot(){
          this.AddRelay=false;
        },
        relayremove(m,n){

          this.$axios({
            url:this.URL.BASE_URL+'/relay/delete/relay',
            method:'POST',
            params:{
              industryId:this.choose.produce,
              // industryId:'22',
              relayAddr:this.DeviceData[m].relayChildren[n].addr,
              deviceId:this.DeviceData[m].deviceId,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);

            if(res.data==="success"){
              this.DeviceData[m].relayChildren.splice(n,1);
            }
            else{
              alert(res.data);
            }

            setTimeout(function(){

            }.bind(this),1000)
          });

          /***
           *
           * 删除继电器
           * **/
          // this.DeviceData[m].relayChildren.splice(n,1);
        },
        addRelayChild(index){  //触发添加继电器的按钮
          const ReAddRelayForm={
            name:'',
            number:'',
          };
          this.AddRelayForm = Object.assign({},ReAddRelayForm);
          console.log("在第几个设备上 添加继电器"+index);
          console.log(this.DeviceData[index]);
          this.AddRelay=true;
          this.RelayIndex=index;
        },
        RelayOk(){
          this.AddRelay=false;

          let data={
            industryId:this.choose.produce,
            // industryId:'22',
            deviceId:this.DeviceData[this.RelayIndex].deviceId,
            relayName:this.AddRelayForm.name,
            relayAddr:this.AddRelayForm.number,
          };
          this.$axios({
            url:this.URL.BASE_URL+'/relay/add/relay',
            method:'POST',
            data:JSON.stringify(data),
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);
            console.log(this.DeviceData[this.RelayIndex]);

            if(res.data==="success"){
              this.DeviceData[this.RelayIndex].relayChildren.push({
                name:this.AddRelayForm.name,
                addr:this.AddRelayForm.number,
              });
            }
            else{
              alert(res.data);
            }

            setTimeout(function(){

            }.bind(this),1000)
          });


          /***
           *
           * 产业ID+设备ID（this.RelayIndex）
           *  this.AddRelayForm.name,
           *  this.AddRelayForm.number,
           *
           * **/
          // this.DeviceData[this.RelayIndex].relayChildren.push({
          //   name:this.AddRelayForm.name,
          // });
        },
        relaydetail(m,n){
          this.ShowRelDetail=true;
          console.log(this.DeviceData[m]);
          console.log(this.DeviceData[m].relayChildren[n].addr);

          this.$axios({
            url:this.URL.BASE_URL+'/relay/message',
            method:'POST',
            params:{
              deviceId:this.DeviceData[m].deviceId,
              relayAddr:this.DeviceData[m].relayChildren[n].addr,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data[0]);
            if(res.data.length!==0){
              this.RelDetail=res.data[0];
            }
            else{
              alert(res.data);
            }

            setTimeout(function(){

            }.bind(this),1000)
          });

          /****
           *
           * 请求继电器的信息
           * **/
          // this.RelDetail={
          //   relayName:'继电器',
          //   relayAddr:'33',
          //   relayMessage:[
          //     {
          //       postion:"1",
          //       name:'强1',
          //       state:'on',
          //     },
          //     {
          //       postion:"2",
          //       name:'强2',
          //       state:'off',
          //     },
          //   ]
          // };

        },

        RelDetailShow(){
          this.ShowRelDetail=false;
        },
        SenDetailShow(){
          this.ShowSenDetail=false;
        },

        Edit(m,n){
          console.log(this.DeviceData[m].sensorChildren[n].addr);
          this.SE_deviceIndex=m;
          this.SE_sensorIndex=n;
          /***
           * 编辑传感器
           * */

          this.$axios({
            url:this.URL.BASE_URL+'/sensor/returnbefore',
            method:'POST',
            params:{
              deviceId:this.DeviceData[m].deviceId,
              sensorAddr:this.DeviceData[m].sensorChildren[n].addr,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);
            if(res.data.length!==0){
               this.ShowSenEdit=true;
               this.EditSenForm=res.data[0];
            }
            else {
              alert(res.data);
            }

            setTimeout(function(){

            }.bind(this),1000)
          });


          // this.ShowSenEdit=true;
          // this.EditSenForm={
          //   deviceId:'1243',
          //   sensorAddr:'01',
          //   sensorName:'三合一',
          //   typeList:[
          //     {
          //       typeName:'temperature',
          //       min:'23',
          //       max:'90',
          //     },
          //     {
          //       typeName:'hum',
          //       min:'23',
          //       max:'90',
          //     }
          //   ],
          // };

        },
        SenEditShow(){
          this.ShowSenEdit=false;
        },
        SenEditOk(){

          let data=this.EditSenForm.typeList;
          for(let i=0; i< data.length;i++){
            data[i].deviceId=this.EditSenForm.deviceId;
            data[i].sensorAddr=this.EditSenForm.sensorAddr;
            data[i].name=this.EditSenForm.sensorName;
          }
          console.log(data);
          this.$axios({
            url:this.URL.BASE_URL+'/sensor/update/sensor',
            method:'POST',
            data:JSON.stringify(data),
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);
            if(res.data==="success"){
              this.ShowSenEdit=false;
              this.DeviceData[this.SE_deviceIndex].sensorChildren[this.SE_sensorIndex].name=this.EditSenForm.sensorName;
            }
            else{
              alert(res.data);
            }

            setTimeout(function(){

            }.bind(this),1000)
          });


          /***
           * 修改传感器名字
           * **/
          // this.ShowSenEdit=false;
          // console.log(this.EditSenForm.name);
          // console.log(this.EditSenForm);
        },

        ReEdit(m,n){
          console.log(this.DeviceData[m].relayChildren[n].addr);
          this.ShowReEdit=true;
          this.EditReForm.name=this.DeviceData[m].relayChildren[n].name;
          this.EditReForm.diviceId=this.DeviceData[m].deviceId;
          this.EditReForm.Addr=this.DeviceData[m].relayChildren[n].addr;
          this.EditReForm.DeviceIndex=m;
          this.EditReForm.RelayIndex=n;
        },
        ReEditShow(){
          this.ShowReEdit=false;
        },
        ReEditOk(){
          this.ShowReEdit=false;
          console.log(this.EditReForm);


          this.$axios({
            url:this.URL.BASE_URL+'/relay/update/relay',
            method:'POST',
            params:{
              industryId:this.choose.produce,
              // industryId:'22',
              deviceId:this.EditReForm.diviceId,
              relayAddr:this.EditReForm.Addr,
              newName:this.EditReForm.name,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);

            if(res.data==='success'){
              this.DeviceData[this.EditReForm.DeviceIndex].relayChildren[this.EditReForm.RelayIndex].name=this.EditReForm.name;
            }
            else{
              alert(res.data);
            }

            setTimeout(function(){

            }.bind(this),1000)
          });

          /***
           *
           * 修改继电器的名称
           *
           * **/
        },

        ReMachine(m,n){
          console.log(this.DeviceData[m].relayChildren[n].addr);

          this.$axios({
            url:this.URL.BASE_URL+'/relay/returnbefore',
            method:'POST',
            params:{
              deviceId:this.DeviceData[m].deviceId,
              relayAddr:this.DeviceData[m].relayChildren[n].addr,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);
            if(res.data.length!==0){
              this.ReMachineData=res.data[0];
              this.tableData=this.ReMachineData.machineList;

              this.Machine=true;
              console.log(this.tableData);
            }

            setTimeout(function(){

            }.bind(this),1000)
          });

          /*******
           * 请求强电设备的信息
           *
           * *******/
          // this.ReMachineData={
          //   relayNmae:'三合一',
          //   relayAddr:'245',
          //   deviceId:'2890249',
          //   machineList:[
          //     {
          //       seen:false,
          //       machinePosition:'1',
          //       machineName:'test_1',
          //       machineState:'2',
          //     }
          //   ],
          // };
          // this.tableData=this.ReMachineData.machineList;
          //
          // this.Machine=true;
          // console.log(this.tableData);
        },
        MachineShow(){
          this.Machine=false;
        },
        MachineOk(){
          let data=this.ReMachineData.machineList;
          for( let i=0; i<data.length;i++){
            data[i].relayAddr= this.ReMachineData.relayAddr;
            data[i].deviceId=this.ReMachineData.deviceId;
            data[i].industryId=this.choose.produce;
          }
          console.log(data);
          this.$axios({
            url:this.URL.BASE_URL+'/relay/machine',
            method:'POST',
            data:JSON.stringify(data),
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log(res.data);
            if(res.data==="success"){
              this.Machine=false;
            }
            else{
              alert(res.data);
            }

            setTimeout(function(){

            }.bind(this),1000)
          });
          /******
           *
           * 确认修改
           * ***********/
          // this.Machine=false;
          // console.log(this.ReMachineData);
        },
        loseFcous(index, row) {
          row.seen=false;
        },
        cellClick(row, column) {
          row.seen=true;
        }
      }
    }
</script>

<style scoped>
  @import "../../../css/add.css";
@import "../../../css/page.css";
  @import "../../../css/state.css";

  .device_part1{
    width: 400px;
    height: 50px;
    /*background-color: #5daf34;*/
  }
.number{
  font-size: 12px;
  color: #666;
}

.demo-table-expand {
  font-size: 0;
}
.demo-table-expand label {
  width: 90px;
  color: #99a9bf;
}
.demo-table-expand .el-form-item {
  margin-right: 0;
  margin-bottom: 0;
  width: 50%;
}

  span{
    cursor: default;
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
