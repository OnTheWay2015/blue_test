<template>
    <div class="user-card" :class="{ 'is-premium': isFlag }">
      <div class="avatar-container">
        <img 
          :src="avatarUrl" 
          :alt="`${userName}'s avatar`" 
          class="avatar"
          @error="handleImageError"
        />
        <span v-if="isFlag" class="verified-badge">✓</span>
      </div>
      
      <div class="user-info">
        <h3>{{ displayName }}</h3>
        <p class="bio">{{ truncatedBio }}</p>
      </div>
      
      <button 
        class="follow-btn" 
        :disabled="isFollowingDisabled"
        @click="handleFollow"
      >
        {{ followButtonText }}
      </button>
    </div>
  </template>
  
  <script lang="ts">
  import { defineComponent, computed, ref } from 'vue'
  
  export default defineComponent({
    name: 'NameTestAll',
    props: {
      userId: {
        type: Number,
        //Vue Router 的 params 传参机制会将所有参数强制转换为字符串类型，而组件 props 中定义的 Number 类型会校验失败
        //type: [Number, String], // 允许两种类型
        required: true
      },
      userName: {
        type: String,
        required: true,
        validator: (value: string) => value.length >= 3 
            /*
                validator 属性可以指定一个自定义验证函数，该函数接收字段值作为参数，返回布尔值或错误信息字符串。
                当返回false或非空字符串时，表示验证失败
            */ 
      },
      count: {
        type: Number,
        default: 0 
      },
      avatarUrl: {
        type: String,
        default: 'https://picsum.photos/100/100?random=1'
      },
      isFlag: {
        type: Boolean,
        default: false
      }
    },
    emits: { //响应 emit 事件
      'follow-status-change': (payload: { userId: string, newStatus: boolean }) => {
        return typeof payload.newStatus === 'boolean'
      },
      'avatar-error': null
    },

 /*
 setup() 
  若返回一个对象：则对象中的属性、方法等，在模板中均可以直接使用。
  若返回一个函数：则可以自定义渲染内容。 

  Vue3 提供了一个 setup 的语法糖，允许我们将 setup 配置独立到一个 <script> 标签中，而不需要在主 <script> 标签中声明 setup 函数
 */ 
    //setup(props:Readonly<LooseRequired<Props>>, { emit }) {
    setup(props, { emit }) {
      const isFollowing = ref(true)
      const avatarLoadError = ref(false)
      const count = ref(props.count)
      
      const displayName = computed(() => {
        let n =  props.isFlag? `⭐ ${props.userName}` : props.userName  
        return n + "/" + props.userId
      })
  
      const truncatedBio = computed(() => {
        return props.userName.length > 100 
          ? `${props.userName.substring(0, 100)}...` 
          : props.userName
      })
  
      const followButtonText = computed(() => {
        return isFollowing.value ? 'Following' : 'Follow'
      })
  
      const isFollowingDisabled = computed(() => {
        return count.value>= 10000
      })
  
      const handleFollow = () => {
        count.value ++
        if (isFollowingDisabled.value) return
        
        isFollowing.value = !isFollowing.value
        emit('follow-status-change', { 
          userId: props.userId, 
          newStatus: isFollowing.value 
        })
      }
  
      const handleImageError = () => {
        avatarLoadError.value = true
        emit('avatar-error')
      }
  
      return {
        displayName,
        truncatedBio,
        followButtonText,
        isFollowingDisabled,
        handleFollow,
        handleImageError
      }
    }
  })
  </script>
  
  <style scoped>
  .user-card {
    /* 样式内容省略 */
  }
  </style>
  