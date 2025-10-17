
import { createRouter, createWebHistory, NavigationFailureType } from 'vue-router'
//import Home from './views/home.vue'
import Home from '../views/home.vue'
import About from '../views/about.vue'
import UserProfile from '../views/user.vue'
import TestComponents from '../views/TestComponents.vue'
import t_input from '../components/t_input.vue'
import calcInCome  from '../views/testinput.vue'

const routes = [
  {
    path: '/test_components',
    component: TestComponents,
    children: [
      {
        path: 't_input',
        component: t_input 
      }
    ]
  },
  {
    path: '/calcIncome',
    component:calcInCome  
  },
  {
    path: '/t_input',
    component: t_input 
  },
  {
    path: '/',
    name: 'Home',
    component: Home
  },
 {
   path: '/about',
   name: 'About',
   component: About,
 },
  {
    path: '/user/:userId',
    name: 'UserProfile',
    component: UserProfile,
    props: true
  },
//  {
//    path: '/admin',
//    name: 'Admin',
//    component: Admin,
//    meta: { requiresAuth: true }   //配置元数据，可在中间件做特别处理
//  },
  {
    path: '/:pathMatch(.*)*',
    redirect: '/'
  }
]

const router = createRouter({
  history: createWebHistory( import.meta.env.BASE_URL),
  routes
})

router.beforeEach((to, from, next) => {
  if (to.meta.requiresAuth && !isAuthenticated()) {
    next('/')
  } else {
    next()
  }
})

router.afterEach((to, from, failure:any) => {
  if (!failure){
    return;
  }
  if (failure.type === NavigationFailureType.duplicated) {
    console.log('重复导航被阻止', to.fullPath)
  }

  if (from){
    console.log( `failure.type[${failure.type}]  to.fullPath[${to.fullPath}] from.fullPath[${from.fullPath}]   `  )
  }else{
    console.log( `failure.type[${failure.type}]  to.fullPath[${to.fullPath}] `  )
  }


})


function isAuthenticated() {
  return localStorage.getItem('auth') === 'true'
}





export default router
