import { createApp } from 'vue'
import router from './router/router'
import './style.css'
//import App from './App.vue'
import App from './AppWithRouter.vue'
//import App from './views/testinput.vue'

//createApp(App).mount('#app')
createApp(App)
.use(router)
.mount('#app')



