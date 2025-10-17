<template>
    <div class="input-container">
      <input 
        v-model="inputValue"
        type="text"
        placeholder="请输入内容"
        @keyup.enter="handleSubmit"
      />
      <button 
        :disabled="!inputValue"
        @click="handleSubmit"
      >
        提交
      </button>
      <p v-if="submittedValue">已提交: {{ submittedValue }}</p>
    </div>
  </template>
  
  <script setup lang="ts">
  import { ref } from 'vue'
  
  const inputValue = ref('')
  const submittedValue = ref('')
  
  const handleSubmit = () => {
    if (inputValue.value.trim()) {
      submittedValue.value = inputValue.value
      inputValue.value = ''
    }
  }
  </script>
  
  <style scoped>
  .input-container {
    display: flex;
    gap: 10px;
    margin: 20px;
  }
  input {
    padding: 8px;
    border: 1px solid #ccc;
    border-radius: 4px;
  }
  button {
    padding: 8px 16px;
    background-color: #42b983;
    color: white;
    border: none;
    border-radius: 4px;
    cursor: pointer;
  }
  button:disabled {
    background-color: #cccccc;
    cursor: not-allowed;
  }
  </style>
  
