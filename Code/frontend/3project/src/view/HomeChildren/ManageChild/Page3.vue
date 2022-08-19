<template>


  <div class="page_main top">
    <LineComponent :title="title" :content="content"></LineComponent>
    <fullscreen  :fullscreen.sync="fullscreen" style="background-color: #f5f6f7; width: 100%" >

      <div v-show="LOD"
           style="width: 100%;  height:800px; overflow: hidden"
           v-loading="loading"
           element-loading-text="拼命加载中"
           element-loading-spinner="el-icon-loading"
           element-loading-background="rgba(255, 255, 255, 0.8)">
      </div>

      <div style="width: 90%;margin: 40px;min-height: 650px;a" class="top">
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
          <i class="el-icon-full-screen" @click="toggle" style="margin-bottom: 20px"></i>
        </div>

        <div v-if="EchartShow" style="width: 100%;display: flex;flex-wrap: wrap;">
            <div style="background-color: #f5f6f7;width: 580px;" class=" top center"  v-for="(item,index) in Data" :key="index" >

              <div style="width:580px; height: 400px;" :id='getId(index)'>

              </div>
            </div>
          </div>
        </div>

    </fullscreen>

  </div>
</template>

<script>
  import LineComponent from '../../../components/LineComponent'
  export default {
    name: "Page3",
    components: {
      LineComponent,
    },
    data() {
      return {
        LOD:false,
        loading: false,
        pickerOptions0: {
          disabledDate(time) {
            return time.getTime() > Date.now();//如果没有后面的-8.64e7就是不可以选择今天的
          }
        },
        id: '',
        title: '历史数据',
        content: '这是历史数据的内容',
        options: [],
        LookDeviceId: '',
        Time: [],
        fullscreen: false,
        Data: [],
        EchartShow: false,
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

      getId(index) {
        return "echarts_" + index;
      },
      handleChange(value) {
        console.log(value[1]);
        this.LookDeviceId = value[1];
      },
      LookSubmit() {
        console.log(this.LookDeviceId);
        console.log(this.Time);
        this.LOD=false;
        this.loading=false;
        this.LOD=true;
        this.loading=true;
        this.$axios({
          // url:'http://192.168.100.136:8082/history',
          url:this.URL.BASE_URL+'/user/find/history',
          method:'POST',
          params:{
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
            this.Data=res.data;
            this.LOD=false;
            this.loading=false;
            this.EchartShow = true;
            this.$nextTick(
              () => {
                this.drawLine();
              }
            );
          }

          setTimeout(function(){

          }.bind(this),1000)
        });

        //
        // this.Data = [
        //   {
        //     deviceId:"90824123",
        //     type:"temperature",
        //     min:20,
        //     max:40,
        //     data:[
        //       {
        //         time:"2019-07-6 09:11:08",
        //         value:"15.2"
        //       },
        //       {
        //         time:"2019-07-6 09:11:09",
        //         value:"25.2"
        //       },
        //       {
        //         time:"2019-07-6 09:11:10",
        //         value:"35.2"
        //       },
        //       {
        //         time:"2019-07-14 09:11:08",
        //         value:"45.2"
        //       },
        //     ],
        //   },
        //   {
        //     deviceId:"90824123",
        //     type:"hum",
        //     min:20,
        //     max:40,
        //     data:[
        //       {
        //         time:"2019-07-6 09:11:08",
        //         value:"15.2"
        //       },
        //       {
        //         time:"2019-07-6 09:11:09",
        //         value:"25.2"
        //       },
        //       {
        //         time:"2019-07-6 09:11:10",
        //         value:"35.2"
        //       },
        //       {
        //         time:"2019-07-14 09:11:08",
        //         value:"45.2"
        //       },
        //     ],
        //   }
        // ];
        //
        // // this.EchartShow=false;
        // this.EchartShow = true;
        // this.$nextTick(
        //   () => {
        //     this.drawLine();
        //   }
        // );
      },
      changeTime() {
        console.log(this.Time);
      },
      toggle() {
        this.fullscreen = !this.fullscreen
      },
      drawLine() {
        for (var i = 0; i < this.Data.length; i++) {

            let myChart = this.$echarts.init(document.getElementById('echarts_' + i));
            // alert("hi"+myChart);
            // 绘制图表

            var Xdata = [];
            var Sdata = [];

            let temp = this.Data[i].data;

            for (var j = 0; j < temp.length; j++) {

              Xdata[j] = temp[j].time;
              Sdata[j] = temp[j].value;
            }

            console.log(Xdata);
            if(this.Data[i].min===0 && this.Data[i].max===0){
              myChart.setOption({
                title: {
                  text: this.Data[i].type,
                },
                tooltip: {
                  trigger: 'axis'
                },
                xAxis: {
                  data: Xdata,
                },
                yAxis: {
                  splitLine: {
                    show: false
                  }
                },
                toolbox: {
                  // left: 'center',
                  orient: 'horizontal',//方向
                  left: '360',//距左
                  top: '2',//距上

                  feature: {
                    dataZoom: {
                      yAxisIndex: 'none'
                    },
                    restore: {},
                    saveAsImage: {}
                  }
                },
                dataZoom: [{
                  startValue: '2014-06-01'
                }, {
                  type: 'inside'
                }],
                series: {
                  name: this.Data[i].type,
                  type: 'line',
                  data: Sdata,
                }
              });
            }
            else if(this.Data[i].min!==0 && this.Data[i].max===0){
              myChart.setOption({
                title: {
                  text: this.Data[i].type,
                },
                tooltip: {
                  trigger: 'axis'
                },
                xAxis: {
                  data: Xdata,
                },
                yAxis: {
                  splitLine: {
                    show: false
                  }
                },
                toolbox: {
                  // left: 'center',
                  orient: 'horizontal',//方向
                  left: '360',//距左
                  top: '2',//距上

                  feature: {
                    dataZoom: {
                      yAxisIndex: 'none'
                    },
                    restore: {},
                    saveAsImage: {}
                  }
                },
                dataZoom: [{
                  startValue: '2014-06-01'
                }, {
                  type: 'inside'
                }],
                visualMap: {
                  top: 10,
                  right: 10,
                  pieces: [
                    {
                      gt: 0,
                      lte: this.Data[i].min,
                      color: '#ffde33'
                    },
                    {
                      gt:this.Data[i].min,
                      color: '#096'
                    },
                  ],
                  outOfRange: {
                    color: '#999'
                  }
                },
                series: {
                  name: this.Data[i].type,
                  type: 'line',
                  data: Sdata,
                  markLine: {
                    silent: true,
                    data: [{
                      yAxis: this.Data[i].min
                    }]
                  }
                }
              });
            }
            else if(this.Data[i].min===0 && this.Data[i].max!==0){
              myChart.setOption({
                title: {
                  text: this.Data[i].type,
                },
                tooltip: {
                  trigger: 'axis'
                },
                xAxis: {
                  data: Xdata,
                },
                yAxis: {
                  splitLine: {
                    show: false
                  }
                },
                toolbox: {
                  // left: 'center',
                  orient: 'horizontal',//方向
                  left: '360',//距左
                  top: '2',//距上

                  feature: {
                    dataZoom: {
                      yAxisIndex: 'none'
                    },
                    restore: {},
                    saveAsImage: {}
                  }
                },
                dataZoom: [{
                  startValue: '2014-06-01'
                }, {
                  type: 'inside'
                }],
                visualMap: {
                  top: 10,
                  right: 10,
                  pieces: [
                    {
                      gt:0,
                      lte:this.Data[i].max,
                      color: '#096'
                    },
                    {
                      gt: this.Data[i].max,
                      color: '#cc0033'
                    },
                  ],
                  outOfRange: {
                    color: '#999'
                  }
                },
                series: {
                  name: this.Data[i].type,
                  type: 'line',
                  data: Sdata,
                  markLine: {
                    silent: true,
                    data: [
                      {
                      yAxis: this.Data[i].max
                    }]
                  }
                }
              });
            }
            else{
              myChart.setOption({
                title: {
                  text: this.Data[i].type,
                },
                tooltip: {
                  trigger: 'axis'
                },
                xAxis: {
                  data: Xdata,
                },
                yAxis: {
                  splitLine: {
                    show: false
                  }
                },
                toolbox: {
                  // left: 'center',
                  orient: 'horizontal',//方向
                  left: '360',//距左
                  top: '2',//距上

                  feature: {
                    dataZoom: {
                      yAxisIndex: 'none'
                    },
                    restore: {},
                    saveAsImage: {}
                  }
                },
                dataZoom: [{
                  startValue: '2014-06-01'
                }, {
                  type: 'inside'
                }],
                visualMap: {
                  top: 10,
                  right: 10,
                  pieces: [
                    {
                      gt: 0,
                      lte: this.Data[i].min,
                      color: '#ffde33'
                    },
                    {
                      gt:this.Data[i].min,
                      lte:this.Data[i].max,
                      color: '#096'
                    },
                    {
                      gt: this.Data[i].max,
                      color: '#cc0033'
                    },
                  ],
                  outOfRange: {
                    color: '#999'
                  }
                },
                series: {
                  name: this.Data[i].type,
                  type: 'line',
                  data: Sdata,
                  markLine: {
                    silent: true,
                    data: [{
                      yAxis: this.Data[i].min
                    }, {
                      yAxis: this.Data[i].max
                    }]
                  }
                }
              });
            }

          }

          // 基于准备好的dom，初始化echarts实例
      },
    }
  }
</script>

<style scoped>
  @import "../../../css/state.css";
  @import "../../../css/page.css";
  @import "../../../css/load.css";

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


