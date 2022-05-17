<template>
  <div class="record">
    <div class="overlay" v-if="loading">
      {{ timer }}
    </div>
    <el-row justify="flex-start" class="backHome">
      <el-button type="primary" size="large" round @click="onHome">
        <el-icon>
          <HomeFilled />
        </el-icon>
      </el-button>
    </el-row>
    <div v-if="!isRecorded" class="recordContent">
      <el-row justify="center" class="timer" v-if="startSample">
        <span class="time">{{ recording }}</span>
        <span> secs</span>
      </el-row>
      <div class="instruction">
        <div class="img">
          <img src="../assets/flip.png" />
        </div>
        <span>{{ instruction }}</span>
      </div>
    </div>
    <el-row v-else class="confirm" justify="center">
      <el-icon :size="200" color="#E7A67E" class="icon">
        <CircleCheck />
      </el-icon>
      <span>{{
        recording + " seconds collected, click Confirm to continue"
      }}</span>
      <el-row :gutter="20">
        <el-col :span="12" :offset="0"
          ><el-button type="primary" size="large" @click="onConfirm" round
            >Confirm</el-button
          ></el-col
        >
        <el-col :span="12" :offset="0"
          ><el-button type="primary" size="large" @click="onRetry" round
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
      isRecorded: false,
      loading: false,
      timer: 3,
      recording: 0,
      instruction:
        "Please flip your left wrist in a quick motion to start sampling",
      startSample: false,
    };
  },
  mounted() {
    if (!this.isRecorded) {
      this.onFlip();
    }
  },
  methods: {
    countDown() {
      return new Promise((res) => {
        let count = setInterval(() => {
          console.log(this.timer);
          if (this.timer == "Start!") {
            clearInterval(count);
            this.loading = false;
            res(true);
          } else {
            this.timer = this.timer - 1;
            if (this.timer === 0) {
              this.timer = "Start!";
            }
          }
        }, 1000);
      });
    },
    onFlip() {
      let count = 0;
      let intervalId;
      this.$socket.send("flip");
      this.$options.sockets.onmessage = () => {
        if (count == 0) {
          this.loading = true;
          this.startSample = true;
          console.log(1);
          this.countDown().then((res) => {
            if (res) {
              this.instruction =
                "Please flip your left wrist in a quick motion again to end sampling";
              intervalId = setInterval(() => {
                this.recording++;
              }, 1000);
              count++;
            }
          });
        } else {
          clearInterval(intervalId);
          this.isRecorded = true;
          console.log(2);
        }
      };
    },
    onConfirm() {
      localStorage.setItem("sample", this.recording);
      this.$router.push("/decoration/3");
    },
    onRetry() {},
    onHome() {
      this.$router.push("/");
    },
  },
};
</script>

<style scoped lang="scss">
.record {
  display: flex;
  height: 100%;
  width: 100%;
  align-items: center;
  flex-direction: column;
}
.backHome {
  padding-left: 1rem;
  padding-top: 2rem;
  margin-bottom: 2rem;
  align-self: flex-start;
}
.timer {
  // color: white;
  font-size: 1.5rem;
  align-items: baseline;
  .time {
    font-size: 10rem;
    font-weight: bold;
  }
}
.instruction {
  display: flex;
  align-items: baseline;
  img {
    margin-right: 10rem;
  }
  span {
    font-size: 1rem;
  }
}
.confirm {
  flex-direction: column;
  flex-grow: 1;
  align-items: center;
  .icon {
    margin-bottom: 1.5rem;
  }
  span {
    // color: white;
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
