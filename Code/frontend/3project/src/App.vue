<template>
  <div id="app">
    <router-view/>
  </div>
</template>

<script>

export default {
  name: 'App',

  mounted:function () {
    this.$router.push("/Main");
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: white;


  /*background-color: black;*/
}

body::-webkit-scrollbar {
  display: none;
}

.BMap_cpyCtrl {
  display: none;
}

.anchorBL {
  display: none;
}


</style>
