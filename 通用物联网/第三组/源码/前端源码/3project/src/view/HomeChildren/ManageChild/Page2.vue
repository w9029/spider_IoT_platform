<template>
  <div class="page_main top">
    <LineComponent :title="title" :content="content" class="line_marbottom"></LineComponent>
    <!--显示个数、添加-->
    <div class="container add between line_marbottom ">
        <div class="top width center" style="height: 100px" >
          <span class="number">采集单元数量（个）</span>
          <h2 style="margin-top: 10px;" class="world_color">{{this.UnitCount}}</h2>
        </div>
      <!--添加采集点-->
      <el-button  type="primary" icon="el-icon-menu" @click="UnitAddVisible=true" style="margin-right: 40px" >添加采集点</el-button>
      <!--添加弹框-->
      <el-dialog
        title="添加采集单元"
        :visible.sync="UnitAddVisible"
        width="30%">
        <el-form :model="ruleForm" :rules="rules" ref="ruleForm" label-width="110px" class="demo-ruleForm">
          <el-form-item label="采集单元名称" prop="name">
            <el-input v-model="ruleForm.name"></el-input>
          </el-form-item>
          <el-form-item label="备注" prop="remark">
            <el-input v-model="ruleForm.remark"></el-input>
          </el-form-item>
        </el-form>
        <span slot="footer" class="dialog-footer">
            <el-button @click="resetForm('ruleForm')">取 消</el-button>
            <el-button type="primary" @click="submitForm('ruleForm')">确 定</el-button>
         </span>
      </el-dialog>

      <!--修改弹出框-->
      <el-dialog
        title="修改采集单元"
        :visible.sync="UnitEditVisible"
        width="30%">
        <el-form :model="editForm" :rules="rules" ref="editForm" label-width="110px" class="demo-ruleForm">
          <el-form-item label="采集单元名称" prop="name">
            <el-input v-model="editForm.name"></el-input>
          </el-form-item>
          <el-form-item label="备注" prop="remark">
            <el-input v-model="editForm.remark"></el-input>
          </el-form-item>
        </el-form>
        <span slot="footer" class="dialog-footer">
            <el-button @click="resetForm2('editForm')">取 消</el-button>
            <el-button type="primary" @click="submitForm2('editForm')">确 定</el-button>
         </span>
      </el-dialog>

      <el-dialog title="提示" :visible.sync="tips" width="30%" center>
        <span>采集单元添加成功</span>
        <span>是否立即添加设备</span>
        <span slot="footer" class="dialog-footer">
    <el-button @click="tips=false">稍后添加</el-button>
    <el-button type="primary" @click="addDevice()">立即添加</el-button>
  </span>
      </el-dialog>
    </div>

    <!--part2 列表-->
    <div class="container">
      <el-table
        :data="UnitData"
        max-height="600"
        style="width:1200px;height: 600px">
        <el-table-column fixed  prop="unitName" label="名称" width="150"  ></el-table-column>
        <el-table-column prop="unitId" label="ID" width="150"></el-table-column>
        <el-table-column prop="unitDeviceNum" label="设备数" width="150"></el-table-column>
        <el-table-column prop="unitRemark" label="备注" width="620"></el-table-column>
        <el-table-column fixed="right" label="操作" width="130">
          <template slot-scope="scope">
            <el-button @click="UnitRead(scope.row)" type="text" size="small">查看</el-button>
            <el-button @click="UnitEdit(scope.row,scope.$index)" type="text" size="small">编辑</el-button>
            <el-button @click="UnitRemove(scope.row,scope.$index)" type="text" size="small">删除</el-button>
          </template>
        </el-table-column>
      </el-table>
    </div>
  </div>
</template>

<script>
  import LineComponent from '../../../components/LineComponent'
    export default {
        name: "Page2",
      components:{
        LineComponent,
      },
      data(){
        return{
          Add_unitId:'',
          Add_unitName:'',
          tips:false,
          UnitAddVisible:false,
          UnitEditVisible:false,
          ruleForm:{
            name:'',
            remark:'',
          },
          editForm:{
            name:'',
            remark:'',
            index:'',
          },
          rules: {
            name: [
              { required: true, message: '请输入产品名称', trigger: 'blur' },
              { min: 3, max: 5, message: '长度在 3 到 5 个字符', trigger: 'blur' }
            ]
          },
          id:'',
          title:'采集单元管理',
          content:'这是采集单元管理的内容',
          UnitData:[],
          UnitCount:'',
        }
      },
      mounted(){
        this.id=this.choose.produce;
        console.log("Page2产业的ID号:"+this.id);

 //获取已有的采集单元-start
        this.$axios({
          url:this.URL.BASE_URL+'/industry/find/unit',
          method:'POST',
          params:{
            id:this.id,
          },
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {

          console.log("获取已有采集单元")
          console.log(res.data);
          this.UnitData=res.data;
          this.UnitCount=this.UnitData.length;
          setTimeout(function(){

          }.bind(this),1000)
        });
//获取已有的采集单元-end

        /**
         *
         * 通过产业ID获取数据
         * **/
// //获取已有的采集单元模拟数据-start
//         this.UnitData=[
//           {
//           unitName: '西瓜田',
//           unitId:'101',
//           unitDeviceNum:'9',
//           unitRemark: '备注备注事发地李开复备注事发地李开复备注事发地李开复备注事发地李开复备注事发地李开复事发地李开复',
//           },
//           {
//             unitName: '草莓田',
//             unitId:'102',
//             unitDeviceNum:'9',
//             unitRemark: '备注事发地李开复',
//           },
//           {
//             unitName: '桃子园',
//             unitId:'103',
//             unitDeviceNum:'9',
//             unitRemark: '啊发发的说法',
//           },
//         ];
//         console.log("采集单元数："+this.UnitData.length);
//         this.UnitCount=this.UnitData.length;
// //获取已有的采集单元模拟数据-end
      },
      methods:{
   //添加采集单元
        submitForm(formName) {
          this.$refs[formName].validate((valid) => {
            if (valid) {
              this.UnitAddVisible = false;
              this.Add_unitName=this.ruleForm.name;
//添加采集单元-start
              this.$axios({
                url:this.URL.BASE_URL+'/industry/add/unit',
                method:'POST',
                params:{
                  industryId:this.id,
                  UnitName:this.ruleForm.name,
                  UnitRemark:this.ruleForm.remark,
                },
                contentType:"application/json",
                headers:{
                  'Content-Type':'application/json'
                }
              }).then((res)=> {

                console.log("添加采集单元ID")
                console.log(res.data);
                this.Add_unitId=res.data;
                this.UnitData.push({
                unitName: this.ruleForm.name,
                unitId:res.data,
                unitDeviceNum:0,
                unitRemark: this.ruleForm.remark,
              });
                this.tips=true;
              this.UnitCount++;
              this.$refs[formName].resetFields();
                setTimeout(function(){

                }.bind(this),1000)
              });
//添加采集单元-end
               /***
               *
               *产业ID
               * 发送添加的数据
               * 并接收采集单元的ID号
               * */
// //添加采集单元模拟数据-start
//               this.UnitData.push({
//                 unitName: this.ruleForm.name,
//                 unitId:'104',
//                 unitDeviceNum:0,
//                 unitRemark: this.ruleForm.remark,
//               });
//
//               this.UnitCount++;
//               this.$refs[formName].resetFields();
// //添加采集单元模拟数据-end
            }
          });
        },
        resetForm(formName) {
          this.$refs[formName].resetFields();
          this.UnitAddVisible = false;
        },
        submitForm2(formName){
          this.$refs[formName].validate((valid) => {
            if (valid) {
              this.UnitEditVisible = false;
              console.log("修改采集单元");
              // console.log(this.AddProduceData[this.editForm.index]);

//修改采集单元-start
              this.$axios({
                url:this.URL.BASE_URL+'/industry/update/unit',
                method:'POST',
                params:{
                  industryId:this.id,
                  unitId:this.UnitData[this.editForm.index].unitId,
                  newName:this.editForm.name,
                  unitRemark:this.editForm.remark,
                },
                contentType:"application/json",
                headers:{
                  'Content-Type':'application/json'
                }
              }).then((res)=> {

                console.log("修改采集单元ID")
                console.log(res.data);

                this.UnitData[this.editForm.index].unitName=this.editForm.name;
                this.UnitData[this.editForm.index].unitRemark=this.editForm.remark;
                this.$refs[formName].resetFields();
                setTimeout(function(){

                }.bind(this),1000)
              });
//修改采集单元-end


              /***
               * 产业ID+采集单元iD
               * 将修改的的数据发送给服务端( this.ruleForm.name
               *   this.ruleForm.remark
               * )
               * */
              // this.UnitData[this.editForm.index].unitName=this.editForm.name;
              // this.UnitData[this.editForm.index].unitRemark=this.editForm.remark;
              // this.$refs[formName].resetFields();
//修改模拟数据
            }
          });
        },
        resetForm2(formName) {
          this.UnitEditVisible = false;
          this.$refs[formName].resetFields();
        },
        UnitRead(row) {
          console.log("Page2:"+row.unitId);
          this.$router.push({
            name: 'Device',
            params: {
              id: row.unitId,
              name:row.unitName,
            }
          })
          // this.$router.push("/Home/Manage/Device");

        },

        UnitEdit(row,index){
          this.UnitEditVisible=true;
          console.log("点击编辑"+index);
          this.$nextTick(()=>{
            this.editForm.name=row.unitName;
            this.editForm.remark=row.unitRemark;
            this.editForm.index=index;
          });
        },
        UnitRemove(row,index){

          console.log("删除产业的ID号"+row.unitId);

//删除采集单元-start
          this.$axios({
            url:this.URL.BASE_URL+'/industry/delete/unit',
            method:'POST',
            params:{
              industryId:this.id,
              unitId:row.unitId,
            },
            contentType:"application/json",
            headers:{
              'Content-Type':'application/json'
            }
          }).then((res)=> {

            console.log("删除采集单元ID")
            console.log(res.data);
            if(res.data==='success'){
              this.UnitData.splice(index,1);
              this.UnitCount--;
            }
            else{
              alert(res.data);
            }
            setTimeout(function(){

            }.bind(this),1000)
          });
//删除采集单元-end

          /***
           *
           *
           * 发送删除的产业的ID号+采集单元号
           *
           *
           * **/
          // this.UnitData.splice(index,1);
          // this.UnitCount--;
//模拟数据
        },
        addDevice(){
          this.tips=false;
          console.log(this.ruleForm.name);
          this.$router.push({
            name: 'Device',
            params: {
              id:this.Add_unitId,
              name:this.Add_unitName,
            }
          })

        }
      }
    }
</script>

<style scoped>
  @import "../../../css/state.css";
  @import "../../../css/page.css";
  .add{
    background-color: white;
    width: 100%;
    height: 100px;
    border: 1px solid #eee;
  }

  .width{
    width: 200px;
  }

  .number{
    font-size: 12px;
    color: #666;
  }
  p,span,h2{
    cursor: default;
  }
</style>
