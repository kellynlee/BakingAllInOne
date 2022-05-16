import { createApp } from "vue";
import App from "./App.vue";
import router from "./router";
import ElementPlus from "element-plus";
import "element-plus/dist/index.css";
import "./styles/index.scss";

import store from "./store/index";
// import webSocketService from "./services/webSocketServer";

import VueNativeSock from "vue-native-websocket-vue3";

import * as ElementPlusIconsVue from "@element-plus/icons-vue";

const app = createApp(App);
for (const [key, component] of Object.entries(ElementPlusIconsVue)) {
  app.component(key, component);
}

app
  .use(router)
  .use(ElementPlus)
  .use(store)
  .use(VueNativeSock, "ws://172.20.10.8/ws")
  .mount("#app");
