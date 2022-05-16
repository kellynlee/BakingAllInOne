<template>
  <div class="home">
    <div class="mb-5">
      <img
        src="https://i.ibb.co/6sGFkTz/Cakegif.gif"
        class="img-fluid"
        height=""
        alt="Cupcake Animation Icon"
      />
    </div>

    <div>
      <h1 class="display-4 fw-bold">Baking All In One</h1>
    </div>

    <div>
      <el-row justify="center" gutter="10">
        <el-button @click="onNavigate('/decoration/1')"> Decoration </el-button>
      </el-row>
      <el-row justify="center">
        <el-button> Mix </el-button>
      </el-row>
      <el-row justify="center">
        <el-button> Pouring </el-button>
      </el-row>
    </div>
  </div>
</template>

<script>
// @ is an alias to /src

export default {
  name: "HomeView",
  data() {
    return {};
  },
  methods: {
    onNavigate(name) {
      this.$router.push(name);
    },
  },
};
</script>
