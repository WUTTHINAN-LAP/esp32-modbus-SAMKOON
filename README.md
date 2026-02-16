🚀 ESP32-C3 Modbus RTU Server (Industrial IoT Node)
[Relay Control | Sensor Monitoring | Samkoon HMI Integration]
โครงการพัฒนาอุปกรณ์ Modbus RTU Slave (Server) ประสิทธิภาพสูงโดยใช้ ESP32-C3 เพื่อเป็นตัวกลางในการควบคุมและจัดเก็บข้อมูลจากเซนเซอร์อุตสาหกรรม ส่งข้อมูลแบบทศนิยมแม่นยำสูง (Floating Point) ไปยังจอ Samkoon HMI (SK-070FE)
________________________________________
✨ Features (คุณสมบัติเด่น)
•	Dual-Core Processing Simulation: ใช้เทคโนโลยี Modbus-ESP32 จัดการสื่อสารแบบ Real-time
•	Precision Sensing:
o	AHT20: อ่านค่าอุณหภูมิและความชื้นสัมพัทธ์ผ่าน I2C
o	INA219: ตรวจสอบแรงดัน (V) และกระแสไฟฟ้า (mA) แบบ DC ความแม่นยำสูง
•	Visual Status Feedback:
o	NeoPixel (GPIO 7): แสดงสถานะการทำงาน (สีน้ำเงิน: Standby / สีเขียว: Modbus Active)
•	Industrial Connectivity: เชื่อมต่อผ่าน RS485 (Baudrate 9600) รองรับระยะไกลและป้องกันสัญญาณรบกวน
________________________________________
📋 Modbus Register Mapping (ตารางรีจิสเตอร์)
🟢 Digital I/O (Coils - 0x)
Address	Function	Description	Access
0x00	Relay 1	ควบคุมรีเลย์ช่องที่ 1 (GPIO 2)	R/W
0x01	Relay 2	ควบคุมรีเลย์ช่องที่ 2 (GPIO 3)	R/W
0x02	Switch 1	สถานะปุ่มกดช่องที่ 1 (GPIO 0)	Read Only
0x03	Switch 2	สถานะปุ่มกดช่องที่ 2 (GPIO 1)	Read Only
🔵 Analog Data (Input Registers - 3x)
ส่งข้อมูลรูปแบบ IEEE 754 Floating Point (32-bit) ใช้ 2 Registers ต่อ 1 ค่า
Address	Parameter	Unit	Memory Range
100	Temperature	°C	100 - 101
102	Humidity	%	102 - 103
104	Bus Voltage	V	104 - 105
106	Current	mA	106 - 107
________________________________________
🔌 Hardware Configuration (การต่อสาย)
ESP32-C3 Pin	Function	Device Connection
GPIO 2	Digital Output	Relay 1 (Active LOW)
GPIO 3	Digital Output	Relay 2 (Active LOW)
GPIO 0	Digital Input	Push Button 1 (Pull-up)
GPIO 1	Digital Input	Push Button 2 (Pull-up)
GPIO 20	Serial RX	RS485 RO (Receiver Output)
GPIO 21	Serial TX	RS485 DI (Driver Input)
GPIO 7	Data Out	WS2812B NeoPixel Status LED
GPIO 8/9	SDA / SCL	AHT20 & INA219 (I2C Bus)
________________________________________
🖥️ Samkoon HMI Setup (การตั้งค่าจอ SK-070FE)
สำหรับการแสดงผลบนจอ Samkoon ให้ตั้งค่าดังนี้เพื่อให้ค่า Floating Point แสดงผลได้ถูกต้อง:
1.	Communication Setup:
o	Link Type: Modbus RTU (Slave)
o	Baud Rate: 9600, 8, N, 1
2.	Numerical Display:
o	Address: 3x 100 (สำหรับอุณหภูมิ)
o	Data Type: 32-bit Float
o	สำคัญ: หากค่าแสดงผลผิดเพี้ยน ให้เปิดฟังก์ชัน Word Swap ใน Link Properties
3.	Bit Button/Lamp:
o	ใช้ Address 0x 0 ถึง 0x 3 ตามตาราง Coil ด้านบน
________________________________________
📦 Requirements (สิ่งที่ต้องติดตั้ง)
1.	Arduino IDE พร้อมบอร์ดจัดการ ESP32 (C3)
2.	Libraries:
o	Modbus-ESP8266 (โดย Bert Melis)
o	Adafruit_AHTX0
o	Adafruit_INA219
o	Adafruit_NeoPixel

