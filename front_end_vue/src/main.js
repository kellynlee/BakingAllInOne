import Vue from "vue";
import App from "./App.vue";
import webSocketService from "./service/webSocketServer";

Vue.config.productionTip = false;

new Vue({
  render: (h) => h(App),
}).$mount("#app");
Vue.use(webSocketService, {
  url: "ws://localhost:44444",
});
