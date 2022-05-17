<template>
  <div class="piping">
    <div class="overlay" v-if="loading">
      {{ timer }}
    </div>
    <transition name="bounce">
      <div class="overlay" v-if="isPaused">
        <img src="../assets/resume.png" @click="onResume" />
      </div>
    </transition>
    <el-row class="progress">
      <div
        class="dot"
        v-for="item in pipingArr"
        :key="item + Math.random()"
        :class="item ? 'finished' : 'noFinished'"
      ></div>
    </el-row>
    <div class="sample">
      <h1>
        {{
          Math.floor(this.sampleTime / 1000) +
          "." +
          (Math.floor((this.sampleTime % 1000) / 10) == 0
            ? "00"
            : Math.floor((this.sampleTime % 1000) / 10))
        }}
      </h1>
      <div class="btnArea">
        <el-button type="primary" @click="onPause" size="large"
          >Pause</el-button
        >
        <el-button type="primary" @click="onBack" size="large"
          >Back to Home</el-button
        >
      </div>
    </div>
  </div>
</template>
<script>
export default {
  data() {
    return {
      sampleTime: 0,
      loading: false,
      timer: 3,
      pipingArr: [],
      sampleTimer: "",
      isPaused: false,
    };
  },
  mounted() {
    let piping = this.$route.params.time;
    // console.log(piping);
    if (piping && piping > 0) {
      this.pipingArr = new Array(parseInt(piping)).fill(false);
    }
    this.sampleTime = localStorage.getItem("sample")
      ? localStorage.getItem("sample") * 1000
      : 0;
    // this.sampleTime = 15 * 1000;
    this.countDown().then((res) => {
      if (res) {
        this.sampleing();
      }
    });
  },
  methods: {
    countDown() {
      this.loading = true;
      return new Promise((res) => {
        let count = setInterval(() => {
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
    singleSample() {
      return new Promise((res) => {
        this.sampleTimer = setInterval(() => {
          if (this.sampleTime > 0) {
            this.sampleTime -= 10;
          } else {
            // clearInterval(this.sampleTimer);
            // this.pipingArr[i] = true;
            // i = i + 1;
            res(true);
          }
        }, 10);
      });
    },
    async sampleing() {
      if (this.pipingArr.length > 0) {
        let originTime = this.sampleTime;
        let i = 0;
        while (i < this.pipingArr.length) {
          if (originTime > 0) {
            this.sampleTime = originTime;
            const res = await this.singleSample();
            if (res) {
              clearInterval(this.sampleTimer);
              this.$socket.send("vibrate");
              this.pipingArr[i] = true;
              i = i + 1;
            }
          }
        }
      }
    },
    onPause() {
      clearInterval(this.sampleTimer);
      this.isPaused = true;
    },
    onResume() {
      this.isPaused = false;
      this.sampleing();
    },
    onBack() {
      this.$router.push("/");
    },
  },
};
</script>

<style lang="scss" scoped>
.piping {
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
  .progress {
    justify-content: space-evenly;
    padding-top: 2rem;
    .dot {
      // background-color: #e7a67e;
      height: 10px;
      width: 10px;
      border-radius: 10px;
    }
  }
  .sample {
    display: flex;
    justify-content: center;
    align-items: center;
    font-size: 4rem;
    font-weight: bold;
    flex-direction: column;
  }
}
.btnArea {
  display: flex;
  flex-direction: row;
}
.finished {
  background-color: #e7a67e;
}
.noFinished {
  background-color: #edd9cc;
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
