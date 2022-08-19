// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import App from './App'
import router from './router'

import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';

import VueAwesomeSwiper from 'vue-awesome-swiper'
// require styles
import 'swiper/dist/css/swiper.css'

import axios from 'axios'

import choose from './js/choose'

import animated from 'animate.css' // npm install animate.css --save安装，在引入

import fullscreen from 'vue-fullscreen'

import infiniteScroll from 'vue-infinite-scroll'

// 引入echarts
import echarts from 'echarts'

import url from './js/url'
import url2 from './js/url2'

import crypto from 'crypto'

Vue.prototype.URL = url
Vue.prototype.URL2 = url2

Vue.prototype.$echarts = echarts

Vue.use(infiniteScroll)

Vue.use(fullscreen)

Vue.use(animated)

Vue.prototype.choose=choose

Vue.prototype.$axios = axios;

Vue.use(VueAwesomeSwiper, /* { default global options } */)

Vue.use(ElementUI);

Vue.config.productionTip = false

/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  components: { App },
  template: '<App/>'
});

