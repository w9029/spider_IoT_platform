<template>
  <div class="add mar2 between">
    <div class="top  width">

      <el-popover
        placement="top-start"
        v-bind:title="item.industryName"
        width="200"
        trigger="hover"
        v-bind:content="item.industryRemark">
        <h2 style="cursor: pointer" slot="reference" v-on:click="click()">{{item.industryName}}</h2>
      </el-popover>


      <div class="between " style="margin-top: 15px;">
        <span class="edit" style="cursor: pointer" v-on:click="edit()">编辑&nbsp|&nbsp</span>
        <span class="edit" style="cursor: pointer" v-on:click="del()">删除</span>
      </div>
    </div>

    <div class="between" style="cursor: pointer"  v-on:click="click()">
      <div class="top width2">
        <span class="number">采集单元数</span>
        <p style="margin-top: 20px;">{{item.industryUnitNum}}</p>
      </div>
      <div class=" width2" style="cursor: pointer">
        <span class="number">创建时间</span>
        <p style="margin-top: 20px;">{{item.industryTime}}</p>
      </div>
    </div>
  </div>
</template>

<script>

  export default {
        name: "Function",
      data() {
        return {
        }
      },
      mounted(){
          console.log("ok")
          console.log(this.item);
      },
      props:{
          item:{
            type:Object,
          },
          index:{
            type:Number,
          }
      },

      methods:{
        del(n){
            this.$emit('remove',this.index);
          },
        edit(n){
            this.$emit('editor',this.index);
        },
        click(){
            //保存当前访问的产品是哪个产品
            // localStorage.setItem("produceState", this.item.industryId);
            this.choose.setProduce(this.item.industryId);
            this.$router.push('/Home/Manage');

        }
      }
    }
</script>

<style scoped>
  @import "../../../css/state.css";
  .add{
    height: 100px;
    width: 100%;
    border: 1px solid #eee;
  }
  .mar2{
    margin-top: 25px;
  }
  .width{
    width: 200px;
  }
  .width2{
    width: 300px;
  }
  .number{
    font-size: 12px;
    color: #666666;
  }
  h2{
    color: #333333;
  }

  .edit{
    color: #57a2ff;
    font-size: 14px;
  }
</style>
