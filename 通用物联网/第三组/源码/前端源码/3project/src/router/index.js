import Vue from 'vue'
import Router from 'vue-router'
import Main from '@/view/Main'
import Home from '@/view/Home'
import Center from '@/view/Center'
import EditPassword from '@/view/EditPassword'
import EditEmail from '@/view/EditEmail'

import Manage from '@/view/HomeChildren/Manage'

import AddProduce from '../view/HomeChildren/AddProduce'
import Page1 from '../view/HomeChildren/ManageChild/Page1'
import Page2 from '../view/HomeChildren/ManageChild/Page2'
import Page3 from '../view/HomeChildren/ManageChild/Page3'
import Page4 from '../view/HomeChildren/ManageChild/Page4'
import Page5 from '../view/HomeChildren/ManageChild/Page5'
import Page6 from '../view/HomeChildren/ManageChild/Page6'
import Page7 from '../view/HomeChildren/ManageChild/Page7'
import Page8 from '../view/HomeChildren/ManageChild/Page8'
import Device from '../view/HomeChildren/ManageChild/Device'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/Main',
      name: 'Main',
      component: Main,
    },
    {
      path: '/Center',
      name: 'Center',
      component: Center,
    },
    {
      path: '/EditPassword',
      name: 'EditPassword',
      component: EditPassword,
    },
    {
      path: '/EditEmail',
      name: 'EditEmail',
      component: EditEmail,
    },
    {
      path: '/Home',
      name: 'Home',
      component: Home,
      children:[
        {
          path: '/',
          name: 'AddProduce',
          component: AddProduce,
        },
        {
          path: 'Manage',
          name: 'Manage',
          component: Manage,
          children:[
            {
              path: '/',
              name: 'Page1',
              component: Page1,
            },
            {
              path: 'Page2',
              name: 'Page2',
              component: Page2,
            },
            {
              path: 'Page3',
              name: 'Page3',
              component: Page3,
            },
            {
              path: 'Page4',
              name: 'Page4',
              component: Page4,
            },
            {
              path: 'Page5',
              name: 'Page5',
              component: Page5,
            },
            {
              path: 'Page6',
              name: 'Page6',
              component: Page6,
            },
            {
              path: 'Page7',
              name: 'Page7',
              component: Page7,
            },
            {
              path: 'Page8',
              name: 'Page8',
              component: Page8,
            },
            {
              path: 'Device',
              name: 'Device',
              component: Device,
            },
          ],
        },
      ],
    },

  ]
})
