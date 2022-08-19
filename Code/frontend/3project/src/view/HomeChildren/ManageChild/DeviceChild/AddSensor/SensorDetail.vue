 <template>
   <!--添加传感器的弹框-->
   <div>
     <div class='popContainer'></div>
     <div class="addsensor animated slideInRight top">
       <!--标题-->
       <div style="width: 100%; height: 80px; border-bottom: 1px solid #cccccc" class="center" >
         <div class="addtitle center between">
           <div class="addtitle-left left">
             <span>传感器详情</span>
           </div>
           <div class="addtitle-right right">
             <el-button type="text" icon="el-icon-close" v-on:click="SenDetailShow()"></el-button>
           </div>
         </div>
       </div>
       <!--主体-->
       <div style="width: 100%; height:100%;" class="top">
         <div style="width: 70%; height:60px;" class="between">
           <label>传感器名称:</label>
           <div style="width: 60%;height: 100%" class="left">{{this.SenDetail.sensorName}}</div>
         </div>
         <div style="width: 70%; height:60px" class=" between">
           <label>传感器站号：</label>
           <div style="width: 60%;height: 100%" class="left">{{this.SenDetail.sensorAddr}}</div>
         </div>
         <div style="width: 70%; height:60px" class=" between">
           <label>485指令：</label>
           <div style="width: 60%;height: 100%" class="left">{{this.SenDetail.code}}</div>
         </div>
         <div style="width: 70%; height:60px" class=" between">
           <label>指令返回长度：</label>
           <div style="width: 60%;height: 100%" class="left">{{this.SenDetail.returnLength}}</div>
         </div>

         <div style="width: 100%; " >
           <el-table :data="SenDetail.typeList" style="width: 100%;hmin-height: 400px">
             <el-table-column prop="typeName" label="typeName" width="180"></el-table-column>
             <el-table-column prop="min" label="min" width="180"></el-table-column>
             <el-table-column prop="max" label="max"></el-table-column>
           </el-table>
         </div >


       </div>

     </div>
   </div>
   <!--添加传感器的弹框-end-->

 </template>

 <script>
   export default {
     name: "SensorDetail",
     props:{
       SenDetail:{
         type:Object,
       },
     },
     data(){
       return{

       }
     },
     methods:{
       SenDetailShow(){
         this.$emit('SenDetailShow');
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

