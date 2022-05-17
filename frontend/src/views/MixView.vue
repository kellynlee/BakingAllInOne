<template>
  <div class="mix">
    <div class="instructionView" v-if="!isStart">
      <h1>Instructions</h1>
      <h3>
        1. Start mixing/folding once you have pressed the “Start Mixing” button.
      </h3>
      <h3>2. A warning will be shown if you are mixing/folding too fast.</h3>
      <h3>3. Press the “End” button once you have finished.</h3>
      <el-button
        type="primary"
        size="large"
        round
        @click="onStratMix"
        class="btn"
        >Start Mixing</el-button
      >
    </div>
    <div class="mixingView" v-if="isStart">
      <h1>Please move in a slow and gentle motion</h1>
      <el-row :gutter="20">
        <el-col :span="12" :offset="0">
          <el-button type="primary" size="large" @click="onPause" round
            >Pause</el-button
          >
        </el-col>
        <el-col :span="12" :offset="0">
          <el-button type="primary" size="large" round @click="onEnd"
            >End</el-button
          >
        </el-col>
      </el-row>
    </div>
  </div>
</template>

<script>
import { ElMessage } from "element-plus";

export default {
  data() {
    return {
      isStart: false,
    };
  },
  methods: {
    onStratMix() {
      this.isStart = true;
      this.onMixing();
    },
    onMixing() {
      this.$socket.send("mix");
      this.$options.sockets.onmessage = (res) => {
        if (res.data == "good") {
          ElMessage({
            message: "Good job!",
            type: "success",
            duration: "2000",
          });
        } else {
          ElMessage({
            message: "Easy, be gentle!",
            type: "warning",
            duration: "2000",
          });
        }
      };
    },
    onPause() {
      this.$socket.send("stop_mix");
    },
    onEnd() {
      this.$socket.send("stop_mix");
    },
  },
};
</script>

<style lang="scss" scoped>
.mix {
  height: 100%;
}
.instructionView {
  padding-top: 2rem;
  display: flex;
  flex-direction: column;
  align-items: flex-start;
  padding-left: 10rem;
  h3 {
    margin-left: 5rem;
  }
  .btn {
    align-self: center;
  }
}
.mixingView {
  height: 100%;
  display: flex;
  flex-direction: column;
  justify-content: center;
  h1 {
    margin-bottom: 10rem;
  }
}
</style>
