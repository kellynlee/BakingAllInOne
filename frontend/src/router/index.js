import { createRouter, createWebHistory } from "vue-router";
import HomeView from "../views/HomeView.vue";

const routes = [
  {
    path: "/",
    name: "home",
    component: HomeView,
  },
  {
    path: "/decoration/:id",
    name: "decoration",
    component: () => import("../views/DecorationView.vue"),
  },
  {
    path: "/record",
    name: "record",
    component: () => import("../views/RecordView.vue"),
  },
  {
    path: "/piping/:time",
    name: "piping",
    component: () => import("../views/PipingView.vue"),
  },
  {
    path: "/mixing",
    name: "mixing",
    component: () => import("../views/MixView.vue"),
  },
];

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes,
});

export default router;
