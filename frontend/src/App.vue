<template>
  <!-- <nav>
    <router-link to="/">Home</router-link> |
    <router-link to="/about">About</router-link>
  </nav> -->
  <keep-alive exclude="searchResult">
    <router-view :key="$route.fullPath"></router-view>
  </keep-alive>
</template>

<script>
export default {
  computed: {
    key() {
      return this.$route.path + Math.random();
    },
  },
};
</script>

<style lang="scss">
body {
  margin: 0;
}
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  background-color: #f2f2f2;
  height: 100vh;
  width: 100vw;
}
</style>
