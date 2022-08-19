<template>
  <div class="page_main top">
    <LineComponent :title="title" :content="content"></LineComponent>
    <div style="background-color: #f5f6f7; width: 100%" >

      <div style="width: 90%;margin: 40px;min-height: 650px" class="top">

        <div  style="width: 90%;" class="between ">
          <div class="left line_marbottom">
            <el-date-picker
              v-model="Time"
              type="datetimerange"
              start-placeholder="开始日期"
              end-placeholder="结束日期"
              value-format=yyyy-MM-dd-HH:mm:ss
              :picker-options="pickerOptions0"
              @change="changeTime">
            </el-date-picker>
            <el-cascader
              style="margin-left: 40px"
              v-model="LookDeviceId"
              :options="options"
              :props="{ expandTrigger: 'hover' }"
              @change="handleChange" clearable
              placeholder="请选择你要查看的设备"></el-cascader>
            <el-button  type="primary"  @click="LookSubmit" style="margin-left: 40px">确认</el-button>
          </div>
        </div>

        <div style="width: 90%">
          <el-table :data="tableData.slice((currentPage-1)*pagesize,currentPage*pagesize)" style="width: 100%" :default-sort = "{prop: 'date', order: 'descending'}">
            <el-table-column prop="time" label="时间" sortable width="180"></el-table-column>
            <el-table-column prop="deviceId" label="设备号" sortable width="180"></el-table-column>
            <el-table-column prop="message" label="内容"width="550"></el-table-column>
            <el-table-column prop="result" label="操作结果"></el-table-column>
          </el-table>
        </div>
        <div style="text-align: center;margin-top: 30px;">
          <el-pagination
            background
            layout="prev, pager, next"
            :total="total"
            @current-change="current_change">
          </el-pagination>
        </div>

      </div>

    </div>

  </div>
</template>

<script>
  import LineComponent from '../../../components/LineComponent'
  export default {
    name: "Page7",
    components: {
      LineComponent,
    },
    data() {
      return {
        pickerOptions0: {
          disabledDate(time) {
            return time.getTime() > Date.now();//如果没有后面的-8.64e7就是不可以选择今天的
          }
        },
        pagesize:10,
        currentPage:1,
        total: 0,
        id: '',
        title: '操作日志',
        content: '这是操作日志的内容',
        options: [],
        Time: [],
        Data: [],
        LookDeviceId:'',
        tableData:[],
      }
    },
    mounted() {
      this.id = this.choose.produce;
      console.log(this.id);

      this.$axios({
        url:this.URL.BASE_URL+'/industry/find/value',
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
          this.options=res.data;
        }

        setTimeout(function(){

        }.bind(this),1000)
      });


      // this.options = [
      //   {
      //     label: '采集单元一',
      //     children: [{
      //       value: '1201',
      //       label: '开发板1',
      //     },
      //       {
      //         value: '1202',
      //         label: '开发板2',
      //       }]
      //   }, {
      //     label: '采集单元二',
      //     children: [{
      //       value: '201',
      //       label: '开发板1',
      //     }, {
      //       value: '2202',
      //       label: '开发板2',
      //     }, {
      //       value: '2203',
      //       label: '开发板3',
      //     }]
      //   }, {
      //     label: '采集单元三',
      //     children: [{
      //       value: '3201',
      //       label: '开发板1'
      //     }, {
      //       value: '3202',
      //       label: '开发板2'
      //     }, {
      //       value: '3203',
      //       label: '开发板3'
      //     }]
      //   }];

    },

    methods: {
      handleChange(value) {
        console.log(value[1]);
        this.LookDeviceId = value[1];
      },
      LookSubmit() {
        console.log(this.LookDeviceId);
        console.log(this.Time);

        this.$axios({
          url:this.URL.BASE_URL+'/device/log',
          method:'POST',
          params:{
            industryId:this.choose.produce,
            start:this.Time[0],
            end:this.Time[1],
            deviceId:this.LookDeviceId,
          },
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json',
            'Access-Control-Allow-Origin':'*'
          }
        }).then((res)=> {

          console.log(res.data);
          if(res.data.length!==0){
            this.tableData=res.data;
            this.total=this.tableData.length;
          }


          setTimeout(function(){

          }.bind(this),1000)
        });

      },
      changeTime() {
        console.log(this.Time);
      },

      current_change:function(currentPage){
        this.currentPage = currentPage;
      },
    }
  }
</script>

<style scoped>
  @import "../../../css/state.css";
  @import "../../../css/page.css";
</style>



