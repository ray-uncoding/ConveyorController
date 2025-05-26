#include <Arduino.h>

// =============================
// 腳位設定（感測器命名）
// =============================
#define WAIT_SENSOR_PIN 2      // 等待區感測器
#define PICKUP_SENSOR_PIN 3    // 抓取區感測器
#define CLAW_SENSOR_PIN 4      // 夾爪感測器
#define RELAY_PIN 5

// =============================
// 初始化
// =============================
void setup() {
  pinMode(WAIT_SENSOR_PIN, INPUT);
  pinMode(PICKUP_SENSOR_PIN, INPUT);
  pinMode(CLAW_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // 預設輸送帶停止
}

// =============================
// 主迴圈
// =============================
void loop() {
  // 讀取紅外線感測器狀態，HIGH=有偵測到物體，LOW=沒偵測到
  int waitSensor = digitalRead(WAIT_SENSOR_PIN);      // 等待區感測器
  int pickupSensor = digitalRead(PICKUP_SENSOR_PIN);  // 抓取區感測器
  int clawSensor = digitalRead(CLAW_SENSOR_PIN);      // 夾爪感測器

  // 將三個感測器組合成一個三位元的編碼
  // 例如 waitSensor=1, pickupSensor=0, clawSensor=1 會得到 0b101 = 5
  int code = (waitSensor << 2) | (pickupSensor << 1) | clawSensor;

  // 根據不同編碼決定輸送帶動作
  // 你可以根據實際需求修改下方的判斷
  switch (code) {
    case 0b000:
      // 三個都沒偵測到
      digitalWrite(RELAY_PIN, LOW); // 停止
      break;
    case 0b001:
      // 只有夾爪感測器有偵測到
      digitalWrite(RELAY_PIN, LOW); // 停止
      break;
    case 0b010:
      // 只有抓取區感測器有偵測到
      digitalWrite(RELAY_PIN, LOW); // 停止
      break;
    case 0b011:
      // 抓取區與夾爪感測器有偵測到
      digitalWrite(RELAY_PIN, LOW); // 停止
      break;
    case 0b100:
      // 只有等待區感測器有偵測到
      digitalWrite(RELAY_PIN, LOW); // 停止
      break;
    case 0b101:
      // 等待區與夾爪感測器有偵測到
      digitalWrite(RELAY_PIN, LOW); // 停止
      break;
    case 0b110:
      // 等待區與抓取區感測器有偵測到
      digitalWrite(RELAY_PIN, LOW); // 停止
      break;
    case 0b111:
      // 三個都偵測到
      digitalWrite(RELAY_PIN, LOW); // 停止
      break;
    default:
      digitalWrite(RELAY_PIN, LOW); // 預設停止
      break;
  }

  delay(50); // 避免過度頻繁讀取
}
