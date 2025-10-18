
<template>
    <div>
      <!-- 声明式导航 -->
      <router-link to="/about">关于页面</router-link>
      <router-link :to="{ name: 'UserProfile', params: { userId: 100} }">
            跳转用户 100 </router-link> 
      <router-link :to="{ name: 'UserProfile', params: { userId: 456 } }">
            跳转用户 456</router-link> 
      <!-- 编程式导航按钮 -->
      <button @click="goToAbout">跳转到关于页</button>
      <button @click="goToUser">代码跳转用户页(带参数)</button>
      <button @click="goBack">返回上一页</button>
      <button @click="goForward">前进一页</button>
    </div>
  </template>
  
  <script setup>
  import { useRouter } from 'vue-router'
  
  const router = useRouter()
  
  // 基本跳转
  const goToAbout = () => {
    router.push('/about')
  }
  
  // 带参数跳转
  const goToUser = () => {
    //需要用路由名 name 来找路由 
    router.push({
      name: 'UserProfile',
      params: { userId: 666, name: '张三' } // 查询参数
    })



  }
  
  // 导航控制
  const goBack = () => {
    router.back()
  }
  
  const goForward = () => {
    router.go(1)
  }
  </script>
  