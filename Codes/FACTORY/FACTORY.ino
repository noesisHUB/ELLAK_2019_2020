#include <Wire.h>
#include <Adafruit_INA219.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver(2000, 11, 10, 8, false); // Default speed is 2000bps; RX pin = 11; TX pin = 12; Ptt pin =  10; PTT inverted = false
Adafruit_INA219 ina219_A;
Adafruit_INA219 ina219_B(0x41);
Adafruit_INA219 ina219_D(0x45);

float shunt_voltage;
float bus_voltage;
float current_mA;
float load_voltage;

void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello!");
  ina219_A.begin();  // Initialize first board (default address 0x40)
  ina219_B.begin();  // Initialize second board with the address 0x41
  ina219_D.begin();  // Initialize second board with the address 0x45
  if (!driver.init())
     Serial.println("init failed");

}

void loop() {
  read_print(ina219_A, "Sun");
  read_print(ina219_B, "Wind");
  read_print(ina219_D, "electrolysis");
  int sensorValue = analogRead(A0);
  Serial.print("Hydrogen concentration: ");
  Serial.println(sensorValue);
  if (sensorValue > 150) 
  {
    Serial.println("Hydrogen detected, sending signal to car!");
    const char *msg = "Move_car";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(1000);  
  }
  Serial.println("");
  Serial.println("");
  Serial.println("");
  delay(1000);
}

void read_print(Adafruit_INA219 ina_219, const String& ina_219_assigned_letter) {
  shunt_voltage = 0;
  bus_voltage = 0;
  current_mA = 0;
  load_voltage = 0;

  shunt_voltage = ina_219.getShuntVoltage_mV();
  bus_voltage = ina_219.getBusVoltage_V();
  current_mA = ina_219.getCurrent_mA();
  load_voltage = bus_voltage + (shunt_voltage / 1000);

//  Serial.print("INA_219_"); Serial.print(ina_219_assigned_letter); Serial.print(" Bus Voltage:   "); Serial.print(bus_voltage); Serial.println(" V");
//  Serial.print("INA_219_"); Serial.print(ina_219_assigned_letter); Serial.print(" Shunt Voltage: "); Serial.print(shunt_voltage); Serial.println(" mV");
  Serial.print("INA_219_"); Serial.print(ina_219_assigned_letter); Serial.print(" Load Voltage:  "); Serial.print(load_voltage); Serial.println(" V");
  Serial.print("INA_219_"); Serial.print(ina_219_assigned_letter); Serial.print(" Current:       "); Serial.print(current_mA); Serial.println(" mA");
}
