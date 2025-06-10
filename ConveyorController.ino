#include <Arduino.h>

// =============================
// 腳位設定（感測器命名）
// =============================
#define WAIT_SENSOR_PIN 2      // 等待區感測器
#define PICKUP_SENSOR_PIN 3    // 抓取區感測器
#define CLAW_SENSOR_PIN 4      // 夾爪感測器
#define RELAY_PIN 13

int waitSensor   = 0;
int pickupSensor = 0;
int clawSensor   = 0;
int last_waitSensor   = 0;
int last_pickupSensor = 0;
int last_clawSensor   = 0;

// =============================
// 初始化
// =============================
void setup() {
  pinMode(WAIT_SENSOR_PIN, INPUT);
  pinMode(PICKUP_SENSOR_PIN, INPUT);
  pinMode(CLAW_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // 預設輸送帶停止
  Serial.begin(9600);
}

// =============================
// 主迴圈
// =============================
void loop() {
  // 讀取紅外線感測器狀態，HIGH=沒偵測到物體，LOW=有偵測到
  waitSensor = digitalRead(WAIT_SENSOR_PIN);      // 等待區感測器
  pickupSensor = digitalRead(PICKUP_SENSOR_PIN);  // 抓取區感測器
  clawSensor = digitalRead(CLAW_SENSOR_PIN);      // 夾爪感測器

  // 將三個感測器組合成一個三位元的編碼
  // 例如 waitSensor=1, pickupSensor=0, clawSensor=1 會得到 0b101 = 5
  int code = (waitSensor << 2) | (pickupSensor << 1) | clawSensor;

  Serial.print("waitSensor: ");
  Serial.print(waitSensor);
  Serial.print(", pickupSensor: ");
  Serial.print(pickupSensor);
  Serial.print(", clawSensor: ");
  Serial.println(clawSensor);

  // 根據不同編碼決定輸送帶動作
  // 可以根據實際需求修改下方的判斷
  switch (code) {
    case 0b000:
      // 三個都有偵測到
      digitalWrite(RELAY_PIN, 0); // 停止
      break;
    case 0b001:
      // 只有夾爪感測器沒偵測到
      digitalWrite(RELAY_PIN, 0); // 停止
      break;
    case 0b010:
      // 只有抓取區感測器沒偵測到
      digitalWrite(RELAY_PIN, 0); // 停止
      break;
    case 0b011:
      // 抓取區與夾爪感測器有偵測到
      digitalWrite(RELAY_PIN, 0); // 停止
      break;
    case 0b100:
      // 只有等待區感測器沒偵測到
      digitalWrite(RELAY_PIN, 0); // 停止
      break;
    case 0b101:
      // 等待區與夾爪感測器沒偵測到
      digitalWrite(RELAY_PIN, 0); // 停止
      break;
    case 0b110:
      // 等待區與抓取區感測器沒偵測到
      digitalWrite(RELAY_PIN, 1); // 運行
      break;
    case 0b111:
      // 三個都沒偵測到
      digitalWrite(RELAY_PIN, 1); // 運行
      break;
    default:
      digitalWrite(RELAY_PIN, 1); // 當以上狀態都失效(系統當機時)，停止
      break;636
      +
  }

  last_waitSensor = waitSensor;
  last_pickupSensor = pickupSensor;
  last_clawSensor = clawSensor;
  
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