#include <Arduino.h>

#define PICKUP_SENSOR_PIN 3    // 抓取區感測器
#define CLAW_SENSOR_PIN 4      // 夾爪感測器
#define RELAY_PIN 13

int pickupSensor = 0;
int clawSensor   = 0;
int last_pickupSensor = 0;
int last_clawSensor   = 0;

int counter = 0;

void setup() {
  pinMode(PICKUP_SENSOR_PIN, INPUT);
  pinMode(CLAW_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // 預設輸送帶停止
  Serial.begin(9600);
}

void loop() {

  clawSensor = !digitalRead(CLAW_SENSOR_PIN);      // 夾爪感測器
  pickupSensor = !digitalRead(PICKUP_SENSOR_PIN);  // 抓取區感測器
 
  int code = (clawSensor << 1) | pickupSensor;

  Serial.print("clawSensor: ");
  Serial.print(clawSensor);
  Serial.print(", pickupSensor: ");
  Serial.print(pickupSensor);

  if (code == 0b10){  //夾爪進入抓取區
    digitalWrite(RELAY_PIN, 1); // 啟動傳送帶
    counter = 0;
  }
  else if(code == 0b11){  //夾爪和物體都進入抓取區
    if (counter < 100){
      float timer = counter * 0.1;

      digitalWrite(RELAY_PIN, 0); // 停下傳送帶
      Serial.print(", time counter (wait until 10s) : ");
      Serial.print(timer);
      Serial.print("s");  
      counter++;
    }
    else{
      digitalWrite(RELAY_PIN, 1); // 啟動傳送帶
      Serial.print(", time's up, active for 3s ...");
      delay(3000);
      counter = 0;
    }
  }
  else{ //夾爪正在做動中
    digitalWrite(RELAY_PIN, 0); // 停下傳送帶
    counter = 0;
  }

  last_pickupSensor = pickupSensor;
  last_clawSensor = clawSensor;
 
  Serial.println();
  delay(100);
}

bool is_pullUp(int last_action, int action){
  if(last_action == 0 && action == 1) return true;
  else return false;
}
bool is_pullDown(int last_action, int action){
  if(last_action == 1 && action == 0) return true;
  else return false;
}
