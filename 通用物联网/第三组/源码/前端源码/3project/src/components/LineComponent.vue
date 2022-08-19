<template>
    <div  class="main top">
        <el-collapse accordion class="wrap">
          <el-collapse-item>
            <template slot="title">
              &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{{title}}<i class="header-icon el-icon-info"></i>
            </template>
            <div class="left mar" style="cursor: default">
              {{content}}
            </div>
          </el-collapse-item>
        </el-collapse>
      </div>
</template>

<script>
    export default {
        name: "LineComponent",
        props:{
          title:{
            type:String,
          },
          content:{
            type:String,
          },

        },
    }
</script>

<style scoped>
  @import "../css/state.css";
.main{
  width: 100%;
  background-color: white;
}
  .mar{
    margin-left: 30px;
  }
  span{
    cursor: pointer;
    font-weight: 600;
    font-size: 16px;
  }
  .wrap{
    width: 100%;
  }
</style>
