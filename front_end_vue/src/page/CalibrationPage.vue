<template>
  <div>
    <div class="d-flex flex-row pb-5">
      <a href="Home.html" class="btn btn-md btn-link"
        ><i class="fa-solid fa-chevron-left fa-2xl"> Back to home</i></a
      >
    </div>

    <div class="mx-5 mb-5">
      <div class="position-relative m-4 text-center">
        <div class="progress" style="height: 5px">
          <div
            class="progress-bar"
            role="progressbar"
            style="width: 0%"
            aria-valuenow="0"
            aria-valuemin="0"
            aria-valuemax="100"
          ></div>
        </div>
        <button
          type="button"
          class="position-absolute top-0 start-0 translate-middle btn btn-lg btn-primary rounded-pill"
        >
          1
        </button>
        <button
          class="btn comp position-absolute start-0 translate-middle mt-4 pt-5"
        >
          Calibration
        </button>

        <button
          type="button"
          class="position-absolute top-0 start-50 translate-middle btn btn-lg btn-secondary rounded-pill"
        >
          2
        </button>
        <button
          class="btn incom position-absolute start-50 translate-middle mt-4 pt-5"
        >
          Recording Sample
        </button>

        <button
          type="button"
          class="position-absolute top-0 start-100 translate-middle btn btn-lg btn-secondary rounded-pill"
        >
          3
        </button>
        <button
          class="btn incom position-absolute start-100 translate-middle mt-4 pt-5"
        >
          Implement
        </button>
      </div>
    </div>

    <div class="d-flex flex-column align-items-center" id="content">
      <div
        class="d-flex align-items-center justify-content-center"
        id="cal-pic"
      >
        <div class="spinner-grow" role="status">
          <span class="visually-hidden">Loading...</span>
        </div>
      </div>
      <h3 class="mt-5 pt-3">Calibrating wirstbands, please stay still.</h3>
    </div>
  </div>
</template>
<script>
export default {
  data() {
    return {};
  },
};
</script>

<style scoped></style>
