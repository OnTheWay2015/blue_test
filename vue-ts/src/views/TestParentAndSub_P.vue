
<template>
    <TestParentAndSub_S @act123="handleStatusChange" @act321="handleStatusChange1" />
    <p>
        TestParentAndSub_P 
    </p>
</template>



<script setup lang="ts">
import TestParentAndSub_S from "./TestParentAndSub_S.vue"

const handleStatusChange = (payload: { active: boolean }) => {
  console.log('Status:', payload.active)
}

const handleStatusChange1 = (payload: { active: boolean }) => {
  console.log('Status:', payload.active)
}

console.log('TestParentAndSub_P setup()')
</script>


<!-- 老写法

<script lang="ts">
    import TestParentAndSub_S from "./TestParentAndSub_S.vue"
  import { defineComponent, computed, ref } from 'vue'
  
  export default defineComponent({
  components: {
    TestParentAndSub_S
  },
  methods: {
    handleStatusChange(payload: { active: boolean }) {
      console.log('Status:', payload.active)
    },
    handleStatusChange1(payload: { active: boolean }) {
      console.log('Status:', payload.active)
    }
  },
  setup(props, { emit }) {
    console.log('TestParentAndSub_P setup()')
  }
})
</script>

-->