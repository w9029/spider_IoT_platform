<template>
  <div class="page_main top">
    <!--part1-->
      <LineComponent :title="title" :content="content" class="line_marbottom"></LineComponent>
    <!--part2-->
      <div class="container page1_one line_marbottom between">
<!--产业名称-->
        <div class="page1_bar shadow center top">
          <el-popover
            placement="top-start"
            :title=Data.industryName
            width="200"
            trigger="hover"
            :content=Data.industryRemark>
            <span class="word" style="font-size: 40px; cursor: pointer" slot="reference">{{this.Data.industryName}}</span>
          </el-popover>

          <!--<span class="word" style="font-size: 40px;">{{this.Data.industryName}}</span>-->
          <div  style="margin-top: 15px;margin-right: 120px">
            <span class="page1_edit" style="cursor: pointer" v-on:click="edit()">编辑</span>
          </div>
        </div>
<!--产业ID-->
        <div class="page1_bar shadow center top">
          <div class="between" style="margin-top: 20px;">
            <img style="width: 40px;height: 40px" src="../../../assets/page1/page1_1.png" alt="">
            <span class="word" style="font-size: 30px;margin-left: 15px;">产业ID</span>
          </div>
          <span style="margin-top: 15px;font-size: 35px;color: #666;font-weight: 600">{{this.id}}</span>
        </div>
<!--采集单元数-->
        <div class="page1_bar shadow left">
          <div class="page1_left center top">
            <img  class="page1_img" src="../../../assets/page1/page1_2.png" alt="">
            <span style="color: #666666;">采集单元数</span>
          </div>
          <div class="page1_right center">
            <span class="page1_numSet">{{this.Data.industryUnitNum}}</span>
          </div>
        </div>
<!--接入设备数-->
        <div class="page1_bar shadow left">
          <div class="page1_left center top">
            <img  class="page1_img" src="../../../assets/page1/page1_3.png" alt="">
            <span style="color: #666666;">接入设备数</span>
          </div>
          <div class="page1_right center">
            <span class="page1_numSet">{{this.Data.industryDeviceNum}}</span>
          </div>
        </div>

      </div>

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
<!--part3-->
      <div class=" container page1_two  line_marbottom" id="allmap">
      </div>
    <!--<p>id={{this.id}}</p>-->
  </div>
</template>

<script>
    import LineComponent from '../../../components/LineComponent'

    export default {
        name: "Page1",
      components:{
        LineComponent
      },
      mounted(){
        this.id=this.choose.produce;
        console.log("产业的ID号:"+this.id);

//获取某个的信息产业-start
        this.$axios({
          url:this.URL.BASE_URL+'/industry/findall',
          method:'POST',
          params:{
            id:this.id,
          },
          contentType:"application/json",
          headers:{
            'Content-Type':'application/json'
          }
        }).then((res)=> {

          console.log("获取某个的信息产业")
          console.log(res);
          console.log(res.data[0]);
          // console.log(res.data[0].industryGps[0].longitude);

          this.Data=res.data[0];
          console.log(this.Data.industryName);
          this.map();

          setTimeout(function(){

          }.bind(this),1000)
        });

//获取产业的信息-end

        /**
         *
         * 通过产业id号向服务器请求数据
         * */
// //模拟数据-start
//         this.Data= {
//             industryId:'001',
//             industryName:'水产业',
//             industryRemark:'这是水产业',
//             industryUnitNum:10,
//             industryDeviceNum:21,
//             industryTime:'2019-6-12',
//             industryGps:[{
//               longitude:'118.906145',  //经度
//               latitude:'31.910135',  //温度
//               unitName:'水产',  //采集单元名称
//               unitId:'101',  //采集单元ID
//               deviceId:'201',  //设备ID号
//               deviceName:'设备一',//设备名
//               deviceRemark:'这是一个设备',  //设备的备注
//               sendRate:'12%',  //设备速率
//             },
//               {
//                 longitude:'118.9026937',  //经度
//                 latitude:'31.91891',  //纬度
//                 unitName:'水产',  //采集单元名称
//                 unitId:'102',  //采集单元ID
//                 deviceId:'202',  //设备ID号
//                 deviceName:'设备二',//设备名
//                 deviceRemark:'这是第二个设备',  //设备的备注
//                 sendRate:'12%',  //设备速率
//               }],
//           };
//         this.map();
// //模拟数据-end
      },
      data(){
          return{
            id:'',
            title:'产业概况',
            content:'这是产品概况的内容',
            Data:[],
            dialogEdit:false,
            editForm:{
              name:'',
              remark:'',
            },
            rules: {
              name: [
                { required: true, message: '请输入产品名称', trigger: 'blur' },
                { min: 3, max: 5, message: '长度在 3 到 5 个字符', trigger: 'blur' }
              ]
            },
          }
      },
      methods:{

          //构建地图
          map(){
            var map = new BMap.Map("allmap", {

              mapType: BMAP_NORMAL_MAP,
              enableMapClick:false,
            }); // 创建Map实例
            map.centerAndZoom(new BMap.Point(118.906145, 31.910135), 13); // 初始化地图,设置中心点坐标和地图级别

            map.setCurrentCity("南京"); // 设置地图显示的城市 此项是必须设置的
            map.enableScrollWheelZoom(true); //开启鼠标滚轮缩放

            map.addControl(new BMap.MapTypeControl());  //添加控制（地图，卫星，三维）

            var points=this.Data.industryGps;
            if(points.length!==''){
              console.log("GPS个数"+points.length);
              for (var i = 0, pointsLen = points.length; i < pointsLen; i++) {
                var point = new BMap.Point(points[i].longitude, points[i].latitude); //将标注点转化成地图上的点
                console.log("地图上的点");
                console.log(point);
                var marker = new BMap.Marker(point); //将点转化成标注点

                map.addOverlay(marker); //将标注点添加到地图上
                //添加监听事件
                (function() {
                  var thePoint = points[i];
                  // console.log(thePoint.deviceName);
                  marker.addEventListener("click",
                    function() {
                      var sContent =
                        '<ul style="margin:0 0 5px 0;padding:0.2em 0">' +
                        '<li style="line-height: 26px;font-size: 15px;">' +
                        '<span style="width: 100px;display: inline-block;">采集单元：</span>' + thePoint.unitName + '</li>' +
                        '<li style="line-height: 26px;font-size: 15px;">' +
                        '<span style="width: 100px;display: inline-block;">设备ID：</span>' + thePoint.deviceId + '</li>' +
                        '<li style="line-height: 26px;font-size: 15px;">' +
                        '<span style="width: 100px;display: inline-block;">设备名：</span>' + thePoint.deviceName + '</li>' +
                        '<li style="line-height: 26px;font-size: 15px;">' +
                        '<span style="width: 100px;display: inline-block;">备注：</span>' + thePoint.deviceRemark + '</li>' +
                        '<li style="line-height: 26px;font-size: 15px;">' +
                        '<span style="width: 100px;display: inline-block;">速率：</span>' + thePoint.sendRate + '</li>' +
                         '</ul>';
                      var infoWindow = new BMap.InfoWindow(sContent); //创建信息窗口对象
                      this.openInfoWindow(infoWindow); //图片加载完后重绘infoWindow
                    });
                })();
            }

            }
          },

      //  编辑产业
        edit(){
          this.dialogEdit=true;
          this.$nextTick(()=>{
            this.editForm.name=this.Data.industryName;
            this.editForm.remark=this.Data.industryRemark;
          });
        },
        change(id){

        },
        submitForm2(formName){
          this.$refs[formName].validate((valid) => {
            if (valid) {
              this.dialogEdit = false;
              console.log("Page1修改产业ID:"+this.Data.industryId);

//修改产业实际请求-start
              this.$axios({
                url:this.URL.BASE_URL+'/industry/update/industry',
                method:'POST',
                params:{
                  industryId:this.Data.industryId,
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
                this.Data.industryName=this.editForm.name;
                this.Data.industryRemark=this.editForm.remark;
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
              // this.Data.industryName=this.editForm.name;
              // this.Data.industryRemark=this.editForm.remark;
              // this.$refs[formName].resetFields();
            }
          });
        },
        resetForm2(formName) {
          this.dialogEdit = false;
        },
      }

    }
</script>

<style scoped>
  @import "../../../css/state.css";
  @import "../../../css/page.css";
  .page1_bar{
    width: 290px;
    height: 180px;
    background-color: white;
    border: 1px solid #cccccc;
  }



  .page1_left{
    height: 100%;
    width: 60%;
    /*background-color: pink;*/
  }
  .page1_right{
    height: 90%;
    width: 40%;
    border-left: 1px dashed rgba(204, 204, 204, 0.57);
    /*background-color: #5daf34;*/
  }

  .page1_one{
    height: 200px;
    /*background-color: white;*/
  }

  .page1_two{
    height: 620px;
    background-color: white;
  }

  .page1_img{
    width: 80px;
    height: 80px;
  }
  p,span{
  cursor: default;
  }
  .page1_numSet{
    font-size: 70px;color: #333333;font-family: 'Yu Gothic UI'
  }

  .page1_edit{
    color: #57a2ff;
    font-size: 14px;
  }

  .word{
    color: #333333;
    font-weight: 600;
    font-family: '华文中宋';
  }


</style>
