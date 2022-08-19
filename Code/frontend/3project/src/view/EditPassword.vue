 <template>
     <div>
       <div class="head">
         <div class="container between">
           <div style="width: 210px" class="between">
             <span style="margin-left: 30px; cursor: pointer; font-size: 18px" @click="ChangeMain()">XXX物联网</span>
             <div style="color: #a7a7a7;margin: 0px 10px" >|</div>
           </div>

           <!--<p >{{alertText}}</p>-->
           <!--<button v-on:click="on()">click</button>-->
           <div class=" between" style="width: 100%; margin-left: 29.5px">
             <el-menu
               :default-active="activeIndex2"
               class="el-menu-demo"
               style="border-bottom: 0px;"
               mode="horizontal"
               background-color="#111"
               text-color="#ccc"
               active-text-color="#2d8cf0">
               <el-menu-item index="1" @click="GoCenter()"><i class="el-icon-user"></i>个人中心</el-menu-item>
             </el-menu>
           </div>
         </div>
       </div>

       <div class="mainCenter container" style="">
         <h2 style="margin:0 auto; color: #333333;padding-bottom: 30px">修改密码</h2>
         <el-form :model="ruleForm" status-icon :rules="rules2" ref="ruleForm" label-width="100px" class="demo-ruleForm">
           <el-form-item label="原密码"   prop="oldPwd" >
             <el-input type="password" v-model="ruleForm.oldPwd" autocomplete="off" show-password clearable></el-input>
           </el-form-item>
           <el-form-item label="新密码" prop="pass">
             <el-input type="password" v-model="ruleForm.pass" autocomplete="off" show-password clearable></el-input>
           </el-form-item>
           <el-form-item label="确认密码" prop="checkPass">
             <el-input type="password" v-model="ruleForm.checkPass" autocomplete="off" show-password clearable></el-input>
           </el-form-item>

           <el-form-item>
             <el-button type="primary" @click="submitForm('ruleForm')">提交</el-button>
             <el-button @click="resetForm('ruleForm')">重置</el-button>
           </el-form-item>
         </el-form>
       </div>

     </div>
   </template>

   <script>
     import {getCookie} from "../js/cookie";

     export default {
       name: "EditPassword",
       data(){

         var checkPwd = (rule, value, callback) => {
           if (!value) {
             return callback(new Error('密码不能为空'));
           }
           let data={name:this.username,password:value}
             this.$axios({
               url:this.URL.BASE_URL+'/user/ensure/password',
               method:'POST',
               data:JSON.stringify(data),
               contentType:"application/json",
               headers:{
                 'Content-Type':'application/json'
               }
             }).then((res)=> {

               var index;

               if(res.data!=="success")
               {
                 callback(new Error('密码错误'));
               }
               else {
                 callback();
               }

               setTimeout(function(){

               }.bind(this),1000)
             })

             // if ( value !== 'gyq' ) {
             //   callback(new Error('密码错误'));
             // } else if ( value=== 1 ) {
             //   callback();
             //   temp =1;
             // }
             // temp=1;
         };

         var validatePass = (rule, value, callback) => {
           if (value === '') {
             callback(new Error('请输入密码'));
           } else {
             if (this.ruleForm.checkPass !== '') {
               this.$refs.ruleForm.validateField('checkPass');
             }
             callback();
           }
         };

         var validatePass2 = (rule, value, callback) => {
           if (value === '') {
             callback(new Error('请再次输入密码'));
           } else if (value !== this.ruleForm.pass) {
             callback(new Error('两次输入密码不一致!'));
           } else {
             callback();

           }
         };

         return{
           username:"",
           activeIndex: '1',
           activeIndex2: '1',
           ruleForm: {
             oldPwd: '',
             pass: '',
             checkPass: '',
           },
           rules2: {
             oldPwd: [
               { validator: checkPwd, trigger: 'blur' }
             ],
             pass: [
               { validator: validatePass, trigger: 'blur' }
             ],
             checkPass: [
               { validator: validatePass2, trigger: 'blur' }
             ],

           },
         }
       },

       mounted(){
         let uname=getCookie('username');
         if(uname ==="")
         {
           this.$router.push("/Main");
         };
         this.username=uname;
       },

       methods:{
         ChangeMain(){
           this.$router.push('/Home');
         },
         resetForm(formName) {
           this.$refs[formName].resetFields();
         },

         submitForm(formName) {
           /****
            * 提交修改
            *
            */
           this.$refs[formName].validate((valid) => {
             if (valid) {
               let data={name:this.username,password:this.ruleForm.checkPass}
               this.$axios({
                 url:this.URL.BASE_URL+'/user/changepassword',
                 method:'POST',
                 data:JSON.stringify(data),
                 contentType:"application/json",
                 headers:{
                   'Content-Type':'application/json'
                 }
               }).then((res)=> {

                 if(res.data==="success")
                 {
                   alert(res.data);
                   this.$router.push('/Center');
                 }
                 else {
                   alert(res.data);
                 }

                 setTimeout(function(){

                 }.bind(this),1000)
               })


               console.log(this.ruleForm);
             } else {
               console.log('error submit!!');
               return false;
             }
           });
         },
         GoCenter(){
           this.$router.push('/Center');
         },
       }
     }
   </script>


   <style scoped>
     @import "../css/page.css";
     @import "../css/state.css";
     .head{
       background-color: #111;
       color: white;
       height: 60px;
       margin-bottom: 60px;
     }
     .mainCenter{
       width: 500px;
       height: 490px;
       padding-top: 100px;
       /*background-color: rgba(218, 218, 218, 0.37);*/
       /*background: url("../assets/未标题-1.jpg");*/
     }

   </style>
