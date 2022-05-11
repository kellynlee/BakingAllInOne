
// Adafruit MPU6050 - Version: Latest 
#include <Adafruit_MPU6050.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Kellynnnnn-project-1_inferencing.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define FREQUENCY_HZ        60
#define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))

// objeto da classe Adafruit_MPU6050
Adafruit_MPU6050 mpu;
int vib = 26;

// Constants for WiFi
const char *SSID = "Kellyn‘s Iphone";
const char *PWD = "jellyapple";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
StaticJsonDocument<100> doc; // Allocate a static JSON document

float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
size_t feature_ix = 0;

// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];

int endFlip = 0;
int startFlip = 0;
int decMode = 0;
int mixMode = 0;

static unsigned long last_interval_ms = 0;

void connectToWifi () {
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }

  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("RSSI ");
  Serial.println(WiFi.RSSI());
}

void notifyClients(String msg) {
  ws.textAll(msg);
}

void onStartCalibrate () {
  Serial.println("start calibration");
  // createMsgJson("calibrate successful!");
  notifyClients("calibrating");
}

void onDetectFlip () {
  Serial.println("start detecting flip");
  decMode = 1;
  startFlip = 1;
  
}

void onStartVibrate () {
  digitalWrite(vib, HIGH);
  delay(200);
  digitalWrite(vib, LOW);
  Serial.println("start vibrating motor!");
  notifyClients("vibrating");
}

void onStartMixing () {
  Serial.println("start detecting mixing&circling");
  mixMode = 1;
}

void onEndMixing() {
  Serial.println("end mixing, stop detecting");
  mixMode = 0;
}

//disable for now, maybe future needed
// void vibrate(int x){
//   for (int i=0; i<x; i++){
//     digitalWrite(vib, HIGH);
//     delay(200);
//     digitalWrite(vib, LOW);
//     delay(300);
//   }
// }

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  // Serial.print((char*)data);
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.print("websocket on listenting:");
    if (strcmp((char*)data, "calibrate") == 0) {
       onStartCalibrate();
    } else if (strcmp((char*)data, "flip") == 0) {
       onDetectFlip();
    } else if (strcmp((char*)data, "vibrate") == 0) {
      onStartVibrate();
    } else if (strcmp((char*)data, "mix") == 0) {
      onStartMixing();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void createMsgJson(char *msg) {  
  jsonDocument.clear();  
  jsonDocument["message"] = msg;
  serializeJson(jsonDocument, buffer);
}

void checkMovement(char* type) {
  sensors_event_t a, g, temp;
  if (millis() > last_interval_ms + INTERVAL_MS) {
    last_interval_ms = millis();

    mpu.getEvent(&a, &g, &temp);

    features[feature_ix++] = a.acceleration.x;
    features[feature_ix++] = a.acceleration.y;
    features[feature_ix++] = a.acceleration.z;

    if (feature_ix == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
      Serial.println("Running the inference...");
      signal_t signal;
      ei_impulse_result_t result;
      int err = numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
      if (err != 0) {
        ei_printf("Failed to create signal from buffer (%d)\n", err);
        return;
      }

      EI_IMPULSE_ERROR res = run_classifier(&signal, &result, true);

      if (res != 0) return;
      for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        if (result.classification[ix].value > 0.6) {
          if (type == "Flip") {
            if (result.classification[ix].label == type) {
              // Flip action's case, only capture flip
              notifyClients("detect flip");
              if (endFlip == 0) {
                endFlip = 1;
              } else {
                endFlip = 0;
                startFlip = 0;
              }
            }
          } else {
            // Mix action's case, need to capture both normal and abnormal actions
            if (result.classification[ix].label == "Circle" || result.classification[ix].label == "Mix"){
               notifyClients("good");
            } else if (result.classification[ix].label == "Mix_abnormal" || result.classification[ix].label == "Circle_abnormal") {
                notifyClients("bad");
            }
          }
        }
      }
      feature_ix = 0;
    }
  }
}

void setupMpu() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }
  
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }

}

void setup() {
  Serial.begin(115200);
  pinMode(vib, OUTPUT);
  setupMpu();
  connectToWifi();
  initWebSocket();
  server.begin();
  delay(100);

}

void loop() {
  ws.cleanupClients();
  if (decMode) {
    if (startFlip == 1 && endFlip == 0) {
    checkMovement("Flip");
  }
  if (startFlip == 1 && endFlip == 1) {
    checkMovement("Flip");
    } 
  }
  if (mixMode) {
    checkMovement("MixHandling");
  }
}
