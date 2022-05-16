<template>
  <div class="decoration">
    <el-row justify="flex-start">
      <el-button type="primary" :icon="ArrowLeft">Back to home</el-button>
    </el-row>
    <el-row :gutter="20">
      <el-col :offset="0">
        <el-progress
          :stroke-width="24"
          :percentage="decPercentatge"
          width="200"
        />
      </el-col>
    </el-row>
    <el-row :gutter="20">
      <el-col :span="8" :offset="0"> Calibration </el-col>
      <el-col :span="8" :offset="0"> Record Sample </el-col>
      <el-col :span="8" :offset="0"> Confirm </el-col>
    </el-row>
    <div v-loading="isCalibrating" v-if="this.mode === '1'">
      <div id="cal-pic">
        <img
          src="https://i.ibb.co/fxvvdK8/calibration.png"
          class="img-fluid h-100"
          alt="Hand calibration picture"
        />
      </div>

      <h3>Please keep your hands steady with plams down as illustrated.</h3>
      <span>{{ clibrateProgress }}</span>
    </div>
    <div v-else-if="this.mode === '2'">
      <h3>Intruction:</h3>
      <h5>
        1. Turn your left wrist in a quick motion to start the sampling process.
      </h5>
      <h5>
        2. You will have three seconds of prep time before the recording starts.
      </h5>
      <h5>3. Start piping your sample on “Start”.</h5>
      <h5>
        4. Turn your left wrist in a quick motion again to end the sampling
        process.
      </h5>
      <el-button type="primary" size="default" @click="onStartSample"
        >Start Sampling</el-button
      >
    </div>
    <div v-else-if="this.mode === '3'">
      <el-row justify="space-between" class="inputArea">
        <el-col :span="10" :offset="0"><h1>I’d Like to Pipe</h1></el-col>
        <el-col :span="3" :offset="0"
          ><el-input
            v-model="pipingTime"
            placeholder="Please input"
            clearable
            size="large"
            :input-style="{ width: '5rem' }"
        /></el-col>
        <el-col :span="10" :offset="0"><h1>Time(s)</h1></el-col>
      </el-row>
      <el-row class="instructionArea">
        <h3>Instruction:</h3>
        <h5>1. Start piping after the first vibration.</h5>
        <h5>2. Stop when the wristband vibates againe.</h5>
        <el-button type="primary" size="default" @click="onStartPiping"
          >Start Piping</el-button
        >
      </el-row>
    </div>
    <el-dialog v-model="isFailed" title="Calibration Failed">
      <div class="dialog">
        <span>Calibration Failed! Please try again.</span>
        <span class="dialogFooter">
          <el-button @click="onReCalibrate">Try again</el-button>
        </span>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import { ElMessage } from "element-plus";

export default {
  data() {
    return {
      isCalibrating: false,
      decPercentatge: "33",
      isFailed: false,
      clibrateProgress: "",
      calibrateFinished: false,
      mode: "",
      pipingTime: "",
    };
  },
  mounted() {
    //simple enum coding for decoration
    //1: calibrate
    //2:sample
    //3:comfirm
    this.mode = this.$route.params.id;
    switch (this.mode) {
      case "1": {
        setTimeout(() => {
          this.clibrateProgress = "Starting Calibration";
          this.isCalibrating = true;
          this.onClibrate()
            .then(() => {
              console.log("calibrate successful");
              this.clibrateProgress = "Calibration Successful!";
              this.isCalibrating = false;
              this.decPercentatge = "66";
              this.calibrateFinished = true;
              ElMessage({
                message: "Calibration Successful!",
                type: "success",
                duration: "2000",
              });
            })
            .catch((err) => {
              this.isFailed = true;
              this.isCalibrating = false;
              console.log(err);
            });
        }, 2000);
        break;
      }
      case "2": {
        this.decPercentatge = "66";
        break;
      }
      case "3": {
        this.decPercentatge = "100";
        break;
      }
    }
  },
  methods: {
    onReCalibrate() {
      this.isFailed = false;
      this.onClibrate()
        .then(() => {
          this.clibrateProgress = "Calibration Successful!";
          this.isCalibrating = false;
          this.decPercentatge = "66";
          this.calibrateFinished = true;
          this.mode = 2;
          ElMessage({
            message: "Calibration Successful!",
            type: "success",
            duration: "2000",
          });
        })
        .catch((err) => {
          this.isFailed = true;
          this.isCalibrating = false;
          console.log(err);
        });
    },
    onClibrate() {
      return new Promise((res, rej) => {
        this.$socket.send("calibrate");
        this.$options.sockets.onmessage = (data) => {
          if (data.data === "calibrated") {
            res(data);
          }
          if (data.data === "moved") {
            rej("Calibration Failed!");
          }
        };
      });
    },
    onStartSample() {
      this.$router.push("/record");
    },
    onStartPiping() {
      this.$router.push("/piping/" + this.pipingTime);
    },
  },
};
</script>

<style scoped lang="scss">
.inputArea {
  align-items: center;
}
.instructionArea {
  flex-direction: column;
  text-align: left;
}
.dialog {
  display: flex;
  flex-direction: column;
  .dialogFooter {
    margin-top: 1rem;
  }
}
</style>
