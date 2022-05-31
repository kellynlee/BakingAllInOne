<template>
  <div class="mix">
    <transition name="bounce">
      <div class="overlay" v-if="pause">
        <img src="../assets/resume.png" @click="onPause" />
      </div>
    </transition>
    <div class="instructionView" v-if="!isStart">
      <h1>Instructions</h1>
      <h3>
        1. Start stiring/folding once you have pressed the “Start Mixing”
        button.
      </h3>
      <h3>2. A warning will be shown if you are stir/folding too fast.</h3>
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
      <img
        v-if="folding"
        src="../assets/mixing.gif"
        class="img-fluid"
        alt="Cupcake Animation Icon"
      />
      <img
        v-if="stiring"
        src="../assets/stir.gif"
        class="img-fluid"
        alt="Cupcake Animation Icon"
      />
      <h1>{{ instruction }}</h1>
      <el-row :gutter="20">
        <el-col :span="12" :offset="0">
          <el-button type="primary" size="large" @click="onPause" round>{{
            pause ? "Resume" : "Pause"
          }}</el-button>
        </el-col>
        <el-col :span="12" :offset="0">
          <el-button type="primary" size="large" round @click="onEnd"
            >End</el-button
          >
        </el-col>
      </el-row>
      <audio :src="audio.src" ref="audio"></audio>
    </div>
  </div>
</template>

<script>
import { ElMessage } from "element-plus";

export default {
  data() {
    return {
      isStart: false,
      instruction: "Please move in a slow and gentle motion to start",
      audio: {
        src: require("../assets/bad.mp3"),
      },
      folding: false,
      stiring: false,
      pause: false,
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
        if (res.data == "mix") {
          this.folding = true;
          this.stiring = false;
          this.instruction = "Detecting folding. Good job, keep going!";
        } else if (res.data == "circle") {
          this.folding = false;
          this.stiring = true;
          this.instruction = "Detecting stiring. Good job, keep going!";
        } else {
          this.instruction = "Easy! Be gentle, you are breaking the bubbles!";

          this.$refs.audio.play();
          ElMessage({
            message: "Easy, be gentle!",
            type: "warning",
            duration: "2000",
          });
        }
      };
    },
    onPause() {
      if (this.pause) {
        this.pause = false;
        this.$socket.send("mix");
      } else {
        this.pause = true;
        this.$socket.send("stop_mix");
      }
    },
    onEnd() {
      this.$socket.send("stop_mix");
      this.$router.go(-1);
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
  img {
    padding: 3rem;
    height: 500px;
  }
}
.overlay {
  position: absolute;
  height: 100%;
  width: 100%;
  z-index: 1000;
  background-color: #e7a67e;
  color: white;
  font-size: 4rem;
  display: flex;
  justify-content: center;
  align-items: center;
}
.bounce-enter-active {
  animation: bounce-in 0.5s;
}
.bounce-leave-active {
  animation: bounce-in 0.5s reverse;
}
@keyframes bounce-in {
  0% {
    transform: scale(0);
  }
  100% {
    transform: scale(1);
  }
}
</style>
