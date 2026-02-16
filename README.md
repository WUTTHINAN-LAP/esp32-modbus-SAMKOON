🚀 ESP32-C3 Modbus RTU Server (Slave)
Smart IoT Controller with AHT20 & INA219 Sensing
โปรเจกต์นี้เป็นการพัฒนาอุปกรณ์ Modbus RTU Slave โดยใช้ ESP32-C3 เป็นหน่วยประมวลผลหลัก รองรับการควบคุม Digital Output (Relay), อ่านค่า Digital Input (Switch) และส่งค่าจากเซนเซอร์ความละเอียดสูง (อุณหภูมิ, ความชื้น, แรงดันไฟฟ้า, กระแสไฟฟ้า) ในรูปแบบ 32-bit Floating Point
________________________________________
🛠 Features
•	Communication: Modbus RTU ผ่าน RS485 (Baudrate 9600)
•	Digital Control: Relay 2 ช่อง (Address 0x00 - 0x01)
•	Digital Input: สวิตช์ 2 ช่อง (Address 0x02 - 0x03)
•	Sensor Integration:
o	AHT20: วัดอุณหภูมิและความชื้น
o	INA219: วัดแรงดันไฟฟ้า (V) และกระแสไฟฟ้า (mA)
•	Precision Data: ส่งค่าเซนเซอร์แบบ 32-bit Float (IEEE 754) โดยใช้ 2 Registers ต่อ 1 ค่า
•	Debug System: แสดงผลสถานะการทำงานและค่าเซนเซอร์ผ่าน Serial Monitor (115200)
________________________________________
📋 Modbus Register Mapping
Coils (Read/Write - Function Code 01, 05, 15)
Address	Parameter	Description
0x00	Relay 0	ควบคุม Relay ตัวที่ 1
0x01	Relay 1	ควบคุม Relay ตัวที่ 2
0x02	Switch 0	สถานะสวิตช์ 1 (Read Only จากฝั่ง Master)
0x03	Switch 1	สถานะสวิตช์ 2 (Read Only จากฝั่ง Master)
Input Registers (Read Only - Function Code 04)
Address	Data Type	Parameter	Unit
100-101	Float 32	Temperature	°C
102-103	Float 32	Humidity	%
104-105	Float 32	Bus Voltage	V
106-107	Float 32	Current	mA
หมายเหตุ: การอ่านค่า Float 32-bit จะใช้ 2 Register ต่อกัน (Big-Endian: High Word ก่อน)
________________________________________
🔌 Hardware Configuration
ESP32-C3 Pin	Function	Device
GPIO 2	Digital Output	Relay 1
GPIO 3	Digital Output	Relay 2
GPIO 0	Digital Input	Switch 1 (Pull-up)
GPIO 1	Digital Input	Switch 2 (Pull-up)
GPIO 20	RX1	RS485 RO
GPIO 21	TX1	RS485 DI
Default I2C	SDA/SCL	AHT20 / INA219
________________________________________
📦 Requirements
ต้องติดตั้ง Library ต่อไปนี้ใน Arduino IDE:
1.	Modbus-ESP32 (by Bert Melis)
2.	Adafruit AHTX0
3.	Adafruit INA219
________________________________________
🚀 Quick Start
1.	ต่อวงจรตามตาราง Hardware Configuration
2.	เปิดโปรแกรม Arduino IDE และคัดลอกโค้ดจากไฟล์ .ino ไปวาง
3.	เลือกบอร์ดเป็น ESP32-C3 Dev Module
4.	ทำการ Upload และเปิด Serial Monitor ที่ 115200 เพื่อดู Log การทำงาน
5.	ใช้โปรแกรม Modbus Master (เช่น Modbus Poll หรือ HMI) เชื่อมต่อผ่าน RS485 ที่ Baudrate 9600

