<template>
    <div>
      <h2>TestComponents</h2>

    <!-- to 要写绝对路径, 要么写 {name: } 的数据通过命名路由  -->
    <router-link to="/test_components/t_input"> t_input </router-link>
    <!-- 有  router-link 的地方必有  router-view 显示对应的路由效果 -->
      <router-view/>
    </div>
  </template>
  
  <script>
  export default {
    name: 'HomeView'
  }
  </script>
  