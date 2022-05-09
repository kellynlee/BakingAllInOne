
// Adafruit MPU6050 - Version: Latest 
#include <Adafruit_MPU6050.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Kellynnnnn-project-1_inferencing.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>

#define FREQUENCY_HZ        60
#define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))

// objeto da classe Adafruit_MPU6050
Adafruit_MPU6050 mpu;

// Constants for WiFi
const char *SSID = "your_wifi_ssid";
const char *PWD = "your_wifi_pwd";

WebServer server(80);

float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
size_t feature_ix = 0;

// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];


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

void setupRouting() {     
  Serial.print("Start listenting:");
  server.on("/calibrate", onStartCalibrate);
  server.on("/flip", onDetectFlip);
  server.on("/vibrate", onStartVibrate);

  // start server    
  server.begin();    
}

void onStartCalibrate () {
  Serial.println("start calibration");
  createMsgJson("calibrate successful!");
  server.send(200, "application/json", buffer);
}

void onDetectFlip () {
  Serial.println("start detecting flip");
  boolean result = checkMovement("Flip");
  if (result) {
    createMsgJson("detect fliping wrist!");
    server.send(200, "application/json", buffer);
  } else {
    createMsgJson("nothing found! try again");
    server.send(404, "application/json", buffer);
  }
}

void onStartVibrate () {
  Serial.println("start vibrating motor!");
  createMsgJson("vibrate successful!");
  server.send(200, "application/json", buffer);
}
void createMsgJson(char *msg) {  
  jsonDocument.clear();  
  jsonDocument["message"] = msg;
  serializeJson(jsonDocument, buffer);
}

boolean checkMovement(char* type) {
  sensors_event_t a, g, temp;
  for (int i = 0; i < 500; i++) {
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
        return false;
      }

      EI_IMPULSE_ERROR res = run_classifier(&signal, &result, true);

      if (res != 0) return false;

      for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        if (result.classification[ix].value > 0.6) {
          if (result.classification[ix].label == type)
          {
            Serial.println("circle");
            return true;
            
          } else if (result.classification[ix].label == type)
          {
            Serial.println("flip");
            return true;
          }else
          {
            Serial.println("nothing");
            return false;
          }
        } else {
          //nothing found
          return false;
        }
      }
      feature_ix = 0;
      }
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
  setupMpu();
  connectToWifi();
  setupRouting();
  delay(100);

}

void loop() {
  server.handleClient();
}
