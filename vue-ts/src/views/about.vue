

 
 <template>
    <div>
      <h2>about</h2>
      <button @click="goBack">about 返回</button>
    </div>
  </template>
  
  <script lang="ts">
  import { defineComponent } from 'vue'
  
  export default defineComponent({
    name: 'about',
    methods: {
      goBack(): void {
        // 使用 router.go(-1) 返回上一页
        this.$router?.go(-1)
        
        // 或者使用 router.back()
        // this.$router?.back()
        
        // 或者直接跳转到指定路由
        // this.$router?.push('/home')
      }
    }
  })
  </script>
  