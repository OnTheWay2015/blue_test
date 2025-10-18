<template>
    <div>
      <h2>about</h2>
      <button @click="triggerEvent">触发事件</button>
      <button @click="triggerEvent1">触发事件</button>
    </div>
  </template>
  
  <script setup lang="ts">
  const emit = defineEmits<{
    (e: 'act123', payload: { active: boolean }): void
    (e: 'act321', payload: { active: false}): void
  }>()
  
  const triggerEvent = () => {
    emit('act123', { active: true })
  }
  const triggerEvent1 = () => {
    emit('act321', { active: false})
  }
  </script>


<!--  老写法

<script lang="ts">
export default {
  methods: {
    triggerEvent() {
      this.$emit('act123', { active: true })
    },
    triggerEvent1() {
      this.$emit('act321', { active: false})
    }
  }
}
</script>
-->