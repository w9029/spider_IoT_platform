<template>
  <div class="page_main top">
    <LineComponent :title="title" :content="content" class="line_marbottom"></LineComponent>
    <!--<div class="wrap">{{this.id}}</div>-->
    <div class="left container line_marbottom">
      <span>当</span>
      <el-cascader
        class="mar_letf"
        v-model="value1"
        :options="options"
        :props="{ expandTrigger: 'hover' }"
        @change="handleChange"
        placeholder="请选择设备的数据类型" clearable style="width: 250px" size="mini">
      </el-cascader>

      <el-select class="mar_letf" v-model="value2" placeholder="控制"  size="mini" style="width: 100px"
                 @change="handleChange2" clearable>
        <el-option
          v-for="item in option2"
          :key="item.value"
          :label="item.label"
          :value="item.value"  size="mini">
        </el-option>
      </el-select>

      <el-input  class="mar_letf" v-model="number" style="width: 70px"  size="mini" clearable></el-input>
      <span>&nbsp时，&nbsp&nbsp&nbsp操作：</span>

      <el-select class="mar_letf"  v-model="value3" placeholder=""  size="mini" style="width: 100px"
                 @change="handleChange2" clearable>
        <el-option
          v-for="item in option3"
          :key="item.value"
          :label="item.label"
          :value="item.value"  size="mini">
        </el-option>
      </el-select>

      <div class="left" v-if="value3==='amateur'">

        <el-select class="mar_letf" v-model="value4" placeholder="继电器"  size="mini" style="width: 120px"
                   @change="handleChange4" clearable>
          <el-option
            v-for="item in option4"
            :key="item.value"
            :label="item.label"
            :value="item.value"  size="mini">
          </el-option>
        </el-select>

        <el-select class="mar_letf" v-model="value5" placeholder="开/关"  size="mini" style="width: 120px"
                   @change="handleChange5" clearable>
          <el-option
            v-for="item in option5"
            :key="item.value"
            :label="item.label"
            :value="item.value"  size="mini">
          </el-option>
        </el-select>

        <el-select  class="mar_letf" v-model="value6" placeholder="几个"  size="mini" style="width: 120px"
                   @change="handleChange6" clearable>
          <el-option
            v-for="item in option6"
            :key="item.value"
            :label="item.label"
            :value="item.value"  size="mini">
          </el-option>
        </el-select>

      </div>

      <div class="left" v-if="value3==='profession'">
        <el-input class="mar_letf" v-model="code" style="width: 300px" placeholder="请输入操作指令" size="mini" clearable></el-input>
      </div>

      <el-button class="mar_letf" size="mini" type="primary" @click="submit()">提交</el-button>
    </div>

    <el-table
      :data="ruleData" style="width: 75%;min-height: 600px" max-height="600" >
      <el-table-column  prop="belongName" label="设备" width="200"></el-table-column>
      <el-table-column prop="ruleMessage" label="规则" width="480"></el-table-column>
      <el-table-column  label="操作" width="150">
        <template slot-scope="scope">
          <el-button @click.native.prevent="deleteRow(scope, ruleData)"
            type="text" size="small">移除</el-button>
        </template>
      </el-table-column>
      <el-table-column  label="使用" width="150">
        <template slot-scope="scope">
          <el-switch
            v-model="ruleData[scope.$index].ruleState" @change="changeState(scope.$index, ruleData)">
          </el-switch>
        </template>

      </el-table-column>
    </el-table>
  </div>
</template>

<script>
  import LineComponent from '../../../components/LineComponent'
  export default {
    name: "Page5",
    components:{
      LineComponent,
    },
    data(){
      return{
        id:'',
        title:'规则定义',
        content:'这是规则定义的内容',
        value1:'',
        value2:'',
        value3:'amateur',
        value4:'',
        value5:'',
        value6:'',
        options: [],
        option2:[
          {
            value:'moreThan',
            label:'大于'
          },
          {
            value:'lessThan',
            label:'小于'
          },
          {
            value:'notLessThan',
            label:'大于等于'
          },{
            value:'notMoreThan',
            label:'小于等于'
          }
        ],
        number:'',
        option3:[
          {
            value:'amateur',
            label:'小白'
          },
          {
            value:'profession',
            label:'大牛'
          },
        ],
        option4:[],
        option5:[
          {
            value:'FF',
            label:'开'
          },
          {
            value:'00',
            label:'关'
          },
        ],
        option6:[
          {
            value: '-1',
            label: '全部'
          },
          {
            value:'0',
            label:'引脚0'
          },
          {
            value:'1',
            label:'引脚1'
          },
          {
            value:'2',
            label:'引脚2'
          },
          {
            value:'3',
            label:'引脚3'
          },
          {
            value:'4',
            label:'引脚4'
          },
          {
            value:'5',
            label:'引脚5'
          },
          {
            value:'6',
            label:'引脚6'
          },
          {
            value:'7',
            label:'引脚7'
          },
        ],
        code:'',
        ruleData:[],
      }
    },
    mounted(){
      this.id=this.choose.produce;
      console.log(this.id);

      this.$axios({
        url:this.URL.BASE_URL+'/industry/find/rule',
        method:'POST',
        params:{
          industryId:this.choose.produce,
        },
        contentType:"application/json",
        headers:{
          'Content-Type':'application/json'
        }
      }).then((res)=> {

        console.log(res.data);
        if(res.data.length!==0){
          this.options=res.data[0].options;
          this.ruleData=res.data[0].ruleData;
        }

        setTimeout(function(){

        }.bind(this),1000)
      });


      // this.options=[
      //   {
      //     value: '1111',
      //     label: '采集单元',
      //     children: [
      //       {
      //         value: '222',
      //         label: '设备',
      //         children: [
      //           {
      //             value: '3333',
      //             label: '传感器',
      //             children:[
      //               {
      //                 value:'typename',
      //                 label:'温度',
      //               }
      //             ],
      //           }],
      //       }],
      //   }
      // ];
      //
      // this.ruleData=[
      //   {
      //     deviceId:'124124',
      //     sensorAddr:'01',
      //     belongName:'设备名/三合一',
      //     ruleMessage:'当temperature>40时，继电器（引脚1）：开',
      //     ruleState:true,
      //     ruleId:'123'
      //   },
      //   {
      //     deviceId:'124124',
      //     sensorAddr:'01',
      //     belongName:'设备名/三合一',
      //     ruleMessage:'当temperature>40时，继电器（引脚1）：开',
      //     ruleState:false,
      //     ruleId:'123',
      //   },
      // ];

    },
    methods: {
      handleChange(value){
        console.log(value);
        console.log(value[1]);

        this.$axios({
          url:this.URL.BASE_URL+'/relay/find/relay',
          method:'POST',
          params:{
            deviceId:value[1],
          },
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {

          console.log(res.data);
          this.option4=res.data;

          setTimeout(function(){

          }.bind(this),1000)
        });

        /***
         *
         * ****/
        // this.option4=[
        //   {
        //     value:'124',
        //     label:'继电器一'
        //   },
        // ];
      },
      handleChange2(value){
        console.log(value);
      },
      handleChange3(value){
        console.log(value);
      },
      handleChange4(value){
        console.log(value);
      },
      handleChange5(value){
        console.log(value);
      },
      handleChange6(value){
        console.log(value);
      },
      submit(){
        let data={
          type:this.value3,
          industryId:this.choose.produce,
          deviceId:this.value1[1],
          addr:this.value1[2],
          typeName:this.value1[3],
          logic:this.value2,
          value:this.number,
          toaddr:this.value4,
          content:this.value6,
          action:this.value5,
          code:this.code,
        };
        console.log(data);

        this.$axios({
          url:this.URL.BASE_URL+'/device/add/rule',
          method:'POST',
          data:JSON.stringify(data),
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {

          console.log(res.data);
          if(res.data!=='fail'){
            this.ruleData.push(res.data[0]);
            this.value3='amateur';
            this.value1='';
            this.value2='';
            this.value4='';
            this.value5='';
            this.value6='';
            this.number='';
            this.code='';
          }

          setTimeout(function(){

          }.bind(this),1000)
        });


        // this.ruleData.push({
        //   deviceId:'124124',
        //   sensorAddr:'01',
        //   belongName:'设备名/三合一',
        //   ruleMessage:'当temperature>40时，继电器（引脚1）：开',
        //   ruleState:false
        // });
        // this.value3='amateur';
        // this.value1='';
        // this.value2='';
        // this.value4='';
        // this.value5='';
        // this.value6='';
        // this.number='';
        // this.code='';
        /*******
         *
         *
         *
         * *****/
      },
      deleteRow(index, rows) {

        console.log(rows);
        console.log(index.$index);
        this.$axios({
          url:this.URL.BASE_URL+'/device/delete/rule',
          method:'POST',
          params:{
            industryId:this.choose.produce,
            deviceId:index.row.deviceId,
            id:index.row.ruleId,
          },
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {

          console.log(res.data);
          if(res.data==='success'){
            this.ruleData.splice(index.$index, 1);
          }

          setTimeout(function(){

          }.bind(this),1000)
        });

        /********
         *
         *
         * ****/

        // rows.splice(index, 1);
        // console.log(index.row);

      },
      changeState(index,rows){
        console.log(rows[index]);

        this.$axios({
          url:this.URL.BASE_URL+'/device/operate/rule',
          method:'POST',
          params:{
            industryId:this.choose.produce,
            deviceId:rows[index].deviceId,
            id:rows[index].ruleId,
            operation:rows[index].ruleState,
          },
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {

          console.log(res.data);

          setTimeout(function(){

          }.bind(this),1000)
        });

      },
    }
  }
</script>

<style scoped>
  @import "../../../css/state.css";
  @import "../../../css/page.css";

  .mar_letf{
    margin-left: 10px;
  }
</style>

