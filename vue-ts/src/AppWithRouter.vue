
<template>
    <div>
      <nav>
        <router-link to="/calcInCome"> calcInCome </router-link>
        <router-link to="/t_input"> t_input </router-link>
        <router-link to="/test_components"> test_components </router-link>
        <router-link to="/about"> about </router-link>
        <router-link :to="{ name: 'UserProfile', params: { userId: 123 } }">
        用户资料
      </router-link>
        <router-link to="/">首页</router-link>
      </nav>
      <router-view/>
    </div>
  </template>
  
  <style>
  #app {
    font-family: Avenir, Helvetica, Arial, sans-serif;
    text-align:left;
  }
  nav {
    padding: 30px;
  }
  nav a {
    margin: 0 10px;
  }
  </style>
  