<template>
  <div class="home">
    <div class="mb-5">
      <img
        src="../assets/cake.gif"
        class="img-fluid"
        height=""
        alt="Cupcake Animation Icon"
      />
    </div>

    <div>
      <h1 class="display-4 fw-bold">Baking All In One</h1>
    </div>

    <div>
      <el-row justify="center" class="btn">
        <el-button
          size="large"
          type="primary"
          round
          @click="onNavigate('/decoration/1')"
        >
          Decoration
        </el-button>
      </el-row>
      <el-row justify="center" class="btn">
        <el-button
          size="large"
          type="primary"
          @click="onNavigate('/mixing')"
          round
        >
          Mix
        </el-button>
      </el-row>
      <el-row justify="center" class="btn">
        <el-button size="large" type="primary" round> Pouring </el-button>
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

<style lang="scss" scoped>
.home {
  height: 100%;
  background: white;
}
.btn {
  margin-bottom: 2rem;
}
</style>
