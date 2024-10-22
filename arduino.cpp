#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <HTTPClient.h>

MPU6050 mpu;

const char* ssid = "YOUR_WIFI_SSID";  
const char* password = "YOUR_WIFI_PASSWORD";  
const char* autoremoteUrl = "https://autoremotejoaomgcd.appspot.com/sendmessage"; 

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi!");

  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
}

void loop() {
  
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);


  if (abs(ax) > 15000 || abs(ay) > 15000 || abs(az) > 15000) {
    Serial.println("Motion detected! Sending signal...");

    
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String message = "your_autoremote_key"; 
      String url = autoremoteUrl + "?key=" + message + "&message=gyro_trigger";
      http.begin(url);
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } else {
        Serial.println("Error on sending request");
      }

      http.end();
    } else {
      Serial.println("WiFi not connected");
    }
  }

  delay(1000);  
}
