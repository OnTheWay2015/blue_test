
<template>
    <div>
      <h3>用户ID: {{ userId }}</h3>
      <button @click="goBack">返回</button>
    </div>
  </template>
  
  <script>
  export default {
    name: 'UserProfile',
    props: {
      userId: {
        type: String,
        required: true
      }
    },
    methods: {
      goBack() {
        this.$router.go(-1)
      }
    }
  }
  </script>
  