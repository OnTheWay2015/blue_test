
<template>
    <div>
      <h2>首页</h2>
      <router-link :to="{ name: 'UserProfile', params: { userId: 123 } }">
        用户资料
      </router-link>
    </div>
  </template>
  
  <script lang="ts">
  import { defineComponent } from 'vue'
  
  export default defineComponent({
    name: 'HomeView',
  })
  </script>