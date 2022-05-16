<template>
  <div class="record">
    <div class="overlay">
      {{ timer }}
    </div>
    <div v-if="!isRecorded">
      <el-row justify="center" class="timer">
        <h1>{{ timer + "seconds" }}</h1>
      </el-row>
      <div class="img">
        <img src="../assets/flip-w.png" />
      </div>
      <span>{{ instruction }}</span>
    </div>
    <el-row v-else class="confirm" justify="center">
      <el-icon :size="48" color="#ffffff" class="icon">
        <CircleCheck />
      </el-icon>
      <span>{{ timer + " seconds collected, click Confirm to continue" }}</span>
      <el-row :gutter="20">
        <el-col :span="12" :offset="0"
          ><el-button type="primary" size="default" @click="onConfirm"
            >Confirm</el-button
          ></el-col
        >
        <el-col :span="12" :offset="0"
          ><el-button type="primary" size="default" @click="onRetry"
            >Retry</el-button
          ></el-col
        >
      </el-row>
    </el-row>
  </div>
</template>

<script>
export default {
  data() {
    return {
      isRecorded: true,
      loading: false,
      counter: 0,
      timer: 0,
      start: "0",
      instruction:
        "Please turn your left wrist in a quick motion to start sampling",
    };
  },
  mounted() {
    if (!this.isRecorded) {
      this.onFlip();
    }
  },
  methods: {
    countDown() {
      this.loading = true;
      let count = setInterval(() => {
        console.log(this.timer);
        if (this.timer == "Start!") {
          clearInterval(count);
          this.loading = false;
        } else {
          this.timer = this.timer - 1;
          if (this.timer === 0) {
            this.timer = "Start!";
          }
        }
      }, 1000);
    },
    onFlip() {
      let count = 0;
      let intervalId;
      this.$socket.send("flip");
      this.$options.sockets.onmessage = () => {
        if (count == 0) {
          console.log(1);
          this.counter = 3;
          this.countDown();
          this.instruction =
            "Please turn your left wrist in a quick motion again to end sampling";
          intervalId = setInterval(() => {
            this.timer++;
          }, 1000);
          count++;
        } else {
          clearInterval(intervalId);
          this.isRecorded = true;
          console.log(2);
        }
      };
    },
    onConfirm() {
      localStorage.setItem("sample", this.timer);
      this.$router.push("/decoration/3");
    },
    onRetry() {},
  },
};
</script>

<style scoped lang="scss">
.record {
  background-color: #e7a67e;
  height: 100%;
  width: 100%;
}
.timer {
  color: white;
}
.confirm {
  padding-top: 5rem;
  flex-direction: column;
  flex-grow: 1;
  align-items: center;
  .icon {
    margin-bottom: 1.5rem;
  }
  span {
    color: white;
    margin-bottom: 1.5rem;
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
</style>
