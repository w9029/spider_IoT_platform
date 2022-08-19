 <template>
   <!--添加传感器的弹框-->
   <div>
     <div class='popContainer'></div>
     <div class="addsensor animated slideInRight top">
       <!--标题-->
       <div style="width: 100%; height: 80px; border-bottom: 1px solid #cccccc" class="center" >
         <div class="addtitle center between">
           <div class="addtitle-left left">
             <span>添加继电器</span>
           </div>
           <div class="addtitle-right right">
             <el-button type="text" icon="el-icon-close" v-on:click="RelayShow()"></el-button>
           </div>
         </div>
       </div>
       <!--主体-->
       <div class="addmain top">
         <el-form :model="AddRelayForm" :rules="rules" ref="AddRelayForm" label-width="" class="top" style="width: 100%">

           <el-row class="addmain top">
               <el-form-item label="继电器名称" prop="name" :key="" class="left">
                 <el-input style="width: 200px" size="mini" v-model="AddRelayForm.name"></el-input>
               </el-form-item>
             <el-form-item label="继电器站号" prop="number" :key="" class="left">
               <el-input style="width: 200px" size="mini" v-model="AddRelayForm.number"></el-input>
             </el-form-item>
           </el-row>

           <el-row style="height: 80px;width: 100%;border-top: 1px solid #cccccc;margin-top: 10px" class="center">
             <el-form-item style="margin-top: 12px">
               <el-button @click="RelayNot()">取消</el-button>
               <el-button @click="RelayOk()" style="margin-left: 80px">确认添加</el-button>
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
     name: "RelayWindow",
     props:{
       AddRelayForm:{
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
       RelayShow(){
         this.$emit('RelayShow');
       },

       RelayNot(){
         this.$emit('RelayNot');
         // this.$refs[formName].resetFields();
       },
       RelayOk(){
         this.$emit('RelayOk');
       },
     },
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

 </style>
