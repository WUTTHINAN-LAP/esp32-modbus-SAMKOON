#include <ModbusRTU.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_INA219.h>
#include <Adafruit_NeoPixel.h>

#define ON  LOW
#define OFF HIGH

#define SLAVE_ID 1
#define TX_PIN 21
#define RX_PIN 20 

// --- NeoPixel Setup ---
#define PIXEL_PIN 7
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

ModbusRTU mb;
Adafruit_AHTX0 aht;
Adafruit_INA219 ina219;

// ฟังก์ชันแปลง Float เป็น 2 Registers (32-bit)
void setFloatToIreg(uint16_t start_reg, float value) {
    uint32_t data;
    memcpy(&data, &value, sizeof(float));
    mb.Ireg(start_reg, (uint16_t)(data >> 16));    
    mb.Ireg(start_reg + 1, (uint16_t)(data & 0xFFFF)); 
}

void setup() {
  Serial.begin(115200);

  // เริ่มต้น NeoPixel
  pixels.begin();
  pixels.setBrightness(50);
  pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // สีน้ำเงินตอนเริ่มเครื่อง
  pixels.show();
  
  // เริ่มต้น I2C Sensors
  Wire.begin(); 
  if (!aht.begin()) Serial.println("AHT10/20 Error!");
  if (!ina219.begin()) Serial.println("INA219 Error!");

  pinMode(2, OUTPUT); pinMode(3, OUTPUT);
  pinMode(0, INPUT_PULLUP); pinMode(1, INPUT_PULLUP);

  Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  mb.begin(&Serial1);
  mb.slave(SLAVE_ID);

  // Digital Coils (0x)
  mb.addCoil(0); mb.addCoil(1); // Relays
  mb.addCoil(2); mb.addCoil(3); // Switches

  // Input Registers (3x) แบบ Floating Point (ใช้ช่องละ 2 IDs)
  mb.addIreg(100, 2); // Temp: 100-101
  mb.addIreg(102, 2); // Hum:  102-103
  mb.addIreg(104, 2); // Volt: 104-105
  mb.addIreg(106, 2); // Current: 106-107
}

void loop() {
  // เก็บสถานะก่อนหน้าเพื่อเช็คการเปลี่ยนแปลง (ใช้ตรวจจับ Master)
  static uint16_t lastCoil0 = 0;
  static uint16_t lastCoil1 = 0;
  static unsigned long ledActiveTime = 0;

  mb.task(); // ประมวลผล Modbus

  // --- ระบบตรวจจับการสื่อสารจาก Master (Simple Heartbeat) ---
  // ถ้า Master เขียนค่าใหม่ลงใน Coil หรือมีการร้องขอ ข้อมูลจะเปลี่ยน
  if (mb.Coil(0) != lastCoil0 || mb.Coil(1) != lastCoil1) {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // ไฟสีเขียวแวบหนึ่งเมื่อ Master สั่งงาน
    pixels.show();
    ledActiveTime = millis();
    lastCoil0 = mb.Coil(0);
    lastCoil1 = mb.Coil(1);
  } 
  
  // จัดการไฟสถานะ (ถ้าไม่มีการสั่งงานนานเกิน 300ms ให้กลับเป็นสีน้ำเงินแบบ Breathing)
  if (millis() - ledActiveTime > 300) {
    float breath = (sin(millis() * 0.004) * 127) + 128;
    pixels.setPixelColor(0, pixels.Color(0, 0, (int)breath / 2)); // สีน้ำเงินวูบวาบ
    pixels.show();
  }

  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 1000) {
    sensors_event_t h, t;
    aht.getEvent(&h, &t); 

    float busVoltage = ina219.getBusVoltage_V();
    float current_mA = ina219.getCurrent_mA();

    setFloatToIreg(100, t.temperature);
    setFloatToIreg(102, h.relative_humidity);
    setFloatToIreg(104, busVoltage);
    setFloatToIreg(106, current_mA);

    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 2000) {
      Serial.println("--- MODBUS DATA UPDATE ---");
      Serial.printf("Temp: %.2f C | Hum: %.2f %%\n", t.temperature, h.relative_humidity);
      Serial.printf("Volt: %.2f V | Curr: %.2f mA\n", busVoltage, current_mA);
      Serial.printf("Relay: %d, %d | SW: %d, %d\n", mb.Coil(0), mb.Coil(1), !digitalRead(0), !digitalRead(1));
      lastDebug = millis();
    }
    lastUpdate = millis();
  }

  // อัปเดตสถานะ Coil 2-3 จากปุ่มกดจริง
  mb.Coil(2, !digitalRead(0));
  mb.Coil(3, !digitalRead(1));

  // ควบคุม Relay 0-1 จากค่าใน Modbus
  digitalWrite(2, mb.Coil(0) ? ON : OFF);
  digitalWrite(3, mb.Coil(1) ? ON : OFF);

  delay(1);
}
