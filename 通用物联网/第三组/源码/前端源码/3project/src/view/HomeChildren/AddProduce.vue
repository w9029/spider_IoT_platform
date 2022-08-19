<template>
  <div class="container top">

    <div class="add between mar">

      <!--产品数量显示栏-->
      <div class="top width">
        <span class="number">产业数量（个）</span>
        <h2 style="margin-top: 10px;">{{this.count}}</h2>
      </div>

      <!--添加产品弹出框-->
      <div>
        <el-button  type="primary" icon="el-icon-box" @click="hadIndutry = true" style="margin-right: 40px" >添加已有产业</el-button>
        <el-button  type="primary" icon="el-icon-box" @click="dialogVisible = true" style="margin-right: 40px" >添加产业</el-button>
      </div>

      <el-dialog
        title="添加产业"
        :visible.sync="dialogVisible"
        width="30%">
        <el-form :model="ruleForm" :rules="rules" ref="ruleForm" label-width="100px" class="demo-ruleForm">
          <el-form-item label="产业名称" prop="name">
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


      <el-dialog
        title="添加已有产品"
        :visible.sync="hadIndutry"
        width="30%">
        <el-form :model="haveForm" :rules="rules" ref="haveForm" label-width="100px" class="demo-ruleForm">
          <el-form-item label="产品id号" prop="id">
            <el-input v-model="haveForm.id"></el-input>
          </el-form-item>
        </el-form>
        <span slot="footer" class="dialog-footer">
            <el-button @click="resetForm3('haveForm')">取 消</el-button>
            <el-button type="primary" @click="submitForm3('haveForm')">确 定</el-button>
         </span>
      </el-dialog>

      <!--修改产品弹出框-->
      <el-dialog
        title="修改产业"
        :visible.sync="dialogEdit"
        width="30%">
        <el-form :model="editForm" :rules="rules" ref="editForm" label-width="100px" class="demo-ruleForm">
          <el-form-item label="产业名称" prop="name">
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

    </div>

  <!--动态已有产品-->
    <div   class="add2 mar2 between" v-for="(item,index) in AddProduceData" :child="item" >
      <Function :item="item" :index="index"  @remove="remove" @editor="editor"></Function>
    </div>

  </div>
</template>

<script>
  import Function from './AddChild/Function'
  import {getTime} from "../../js/time";
  import {getCookie} from "../../js/cookie";

  export default {
    name: "AddProduce",
    components:{
      Function,
    },
    data(){
      return{
        username:'',
        dialogEdit:false,
        dialogVisible: false,
        hadIndutry:false,
        haveForm:{
          id:'',
        },
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
            { min: 3, max: 10, message: '长度在 3 到 10 个字符', trigger: 'blur' }
          ],
          id:[
            { required: true, message: '请输入产品id号', trigger: 'blur' },
          ],
        },
        count:'',
        AddProduceData:[],

      }
    },
    mounted(){
      let uname=getCookie('username');
      this.username=uname;
// //获取已有产业
      console.log(this.username);
      this.$axios({
        url:this.URL.BASE_URL+'/userindustry/find',
        method:'POST',
        params:{
          name:this.username,
        },
        contentType:"application/json",
        headers:{
          'Content-Type':'application/json'
        }
      }).then((res)=> {

        console.log("获取已有产业")
        console.log(res);

        this.AddProduceData=res.data;
        this.count=res.data.length;

        setTimeout(function(){

        }.bind(this),1000)
      });
//获取已有产业-end
//
// //模拟测试数据-start
//       this.AddProduceData=[
//         {
//           industryId:'001',
//           industryName:'水产业',
//           industryRemark:'这是水产业',
//           industryUnitNum:10,
//           industryTime:'2019-6-12'
//         },
//         {
//           industryId:'002',
//           industryName:'农产业',
//           industryRemark:'这是农产业',
//           industryUnitNum:30,
//           industryTime:'2019-7-12'
//         },
//         {
//           industryId:'003',
//           industryName:'畜牧业',
//           industryRemark:'这是畜牧业',
//           industryUnitNum:20,
//           industryTime:'2019-8-12'
//         },
//       ];
//
//       this.count=this.AddProduceData.length;
// //模拟测试数据-end


      /**
       *传用户名给服务端
       * 在此处与后端请求产业列表
       *
       * **/
    },
    methods:{

      submitForm(formName) {
        this.$refs[formName].validate((valid) => {
          if (valid) {
            this.dialogVisible = false;
//添加产业实际请求-start
            this.$axios({
              url:this.URL.BASE_URL+'/industry/add/industry',
              method:'POST',
              params:{
                username:this.username,
                industryName:this.ruleForm.name,
                industryRemark:this.ruleForm.remark,
                industryTime:getTime(),
              },
              contentType:"application/json",
              headers:{
                'Content-Type':'application/json'
              }
            }).then((res)=> {
              console.log("添加产业获取并ID");
              console.log(res.data);

              this.AddProduceData.push({
                industryId:res.data,
                industryRemark:this.ruleForm.remark,
                industryUnitNum:0,
                industryName:this.ruleForm.name,
                industryTime:getTime(),
              });

              this.count++;
              this.$refs[formName].resetFields();

              setTimeout(function(){

              }.bind(this),1000)
            });
//实际请求-end


// //模拟数据-start
//             /***
//              * 用户名
//              * 将添加的数据发送给服务端( this.ruleForm.name
//              *   this.gettime
//              *   this.ruleForm.remark（传3个数据）
//              * )并接受一个id
//              * */
//             this.AddProduceData.push({
//               industryId:"004",
//               industryRemark:this.ruleForm.remark,
//               industryUnitNum:0,
//               industryName:this.ruleForm.name,
//               industryTime:getTime(),
//             });
//
//             this.count++;
//             this.$refs[formName].resetFields();
// //模拟数据-end
          }
        });
      },
      submitForm2(formName){
        this.$refs[formName].validate((valid) => {
          if (valid) {
            this.dialogEdit = false;
            console.log("修改产业");
            console.log(this.AddProduceData[this.editForm.index]);


//修改产业实际请求-start
            this.$axios({
              url:this.URL.BASE_URL+'/industry/update/industry',
              method:'POST',
              params:{
                industryId:this.AddProduceData[this.editForm.index].industryId,
                industryName:this.editForm.name,
                industryRemark:this.editForm.remark,
              },
              contentType:"application/json",
              headers:{
                'Content-Type':'application/json'
              }
            }).then((res)=> {
              console.log("修改产业信息");
              console.log(res.data);

              this.AddProduceData[this.editForm.index].industryName=this.editForm.name;
              this.AddProduceData[this.editForm.index].industryRemark=this.editForm.remark;
              this.$refs[formName].resetFields();

              setTimeout(function(){

              }.bind(this),1000)
            });
//实际请求-end
            /***
             * 产业ID
             * 将修改的的数据发送给服务端( this.ruleForm.name
             *   this.ruleForm.remark
             * )
             * */
// //模拟
//             this.AddProduceData[this.editForm.index].industryName=this.editForm.name;
//             this.AddProduceData[this.editForm.index].industryRemark=this.editForm.remark;
//             this.$refs[formName].resetFields();
          }
        });
      },
      resetForm(formName) {
        this.$refs[formName].resetFields();
        this.dialogVisible = false;
      },
      resetForm2(formName) {
        this.dialogEdit = false;
        this.$refs[formName].resetFields();
      },

      resetForm3(formName){
        this.$refs[formName].resetFields();
        this.hadIndutry = false;
      },
      submitForm3(formName){
        this.$refs[formName].validate((valid) => {
          if (valid) {
            this.hadIndutry = false;
            console.log(this.haveForm);

            this.$axios({
              url:this.URL.BASE_URL+'/userindustry/add/userindustry',
              method:'POST',
              params:{
                industryId:this.haveForm.id,
                name:this.username,
              },
              contentType:"application/json",
              headers:{
                'Content-Type':'application/json'
              }
            }).then((res)=> {
            console.log(res.data);
            if(res.data!=='fail'){
              this.AddProduceData.push({
                industryId:res.data.industryId,
                industryRemark:res.data.industryRemark,
                industryUnitNum:res.data.industryUnitNum,
                industryName:res.data.industryName,
                industryTime:res.data.industryTime,
              });

              this.count++;
              this.$refs[formName].resetFields();
            }
            else{
              alert(res.data);
            }

              setTimeout(function(){

              }.bind(this),1000)
            });

          }
        });
      },

      remove(n){
        console.log("删除产业的ID号"+this.AddProduceData[n].industryId);
        console.log(this.username)
//删除产业实际请求-start
        this.$axios({
          url:this.URL.BASE_URL+'/industry/delete/industry',
          method:'POST',
          params:{
            name:this.username,
            id:this.AddProduceData[n].industryId,
          },
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {
          console.log("删除产业");
          console.log(res.data);
          if(res.data==='success'){
            this.AddProduceData.splice(n,1);
            this.count--;
          }
          else{
            alert("不是创建者,没有权限");
          }
          setTimeout(function(){

          }.bind(this),1000)
        });
//删除产业实际请求-end
        /***
         * 发送删除的产业的ID号
         * **/
 // //模拟数据
 //        this.AddProduceData.splice(n,1);
 //        this.count--;
      },
      editor(n){
        this.dialogEdit=true;
        console.log("点击编辑"+n);
        this.$nextTick(()=>{
          this.editForm.name=this.AddProduceData[n].industryName;
          this.editForm.remark=this.AddProduceData[n].industryRemark;
          this.editForm.index=n;
        });
      }
    }
  }
</script>

<style scoped>
  @import "../../css/state.css";
  .add{
    height: 100px;
    width: 100%;
    border: 1px solid #eee;
  }
  .add2{
    height: 100px;
    width: 100%;
  }
  .mar{
    margin-top:40px;
  }

  .mar2{
    margin-top: 25px;
  }
  .width{
    width: 200px;
  }

  .number{
    font-size: 12px;
    color: #666666;
  }
  h2{
    color: #333333;
  }



</style>
