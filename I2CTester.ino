/*
 * Copyright (c) 2016 by Stefano Speretta <s.speretta@tudelft.nl>
 *
 * I2CTester: this is an application designed to test the 
 * functionality of the I2C Tester setup. The setup includes several 
 * devices on different boards to perform a long duration test on 
 * the hardware and software implementation. It is designed to use 
 * Energia (the Arduino port for MSP microcontrollers) on an MSP432.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 
 * version 3, both as published by the Free Software Foundation.
 *
 */
 
#include <DWire.h>
#include <DSerial.h>
#include <I2CScanner.h>
#include <INA226.h>
#include "I2CTester.h"

DWire wire;
DSerial serial;

INA226 ina(wire, 0x40);

unsigned char dev[128];
unsigned char devicesCount;

void deviceFound(unsigned char device)
{
  dev[devicesCount] = device;
  devicesCount++;
}

void listDevices()
{
  if (devicesCount == 0)
  {
    serial.println("No device found");
    return;
  }
  
  for(int i = 0; i < devicesCount; i++)
  {
    if (dev[i] != 0)
    {
      serial.print("Found Address 0x");
      if (dev[i] < 15) 
      {
        serial.print('0');
      }
      serial.print(dev[i], HEX);
      serial.println();
    }
  }
}

void setup()
{
  // Initialize I2C master
  wire.setStandardSpeed();
  wire.begin();
  
  // initialize debug UART
  serial.begin();
  
  // set the direction to output and enable all buses
  pinMode(BUS1, OUTPUT);
  pinMode(BUS2, OUTPUT);
  pinMode(BUS3, OUTPUT);
  pinMode(BUS4, OUTPUT);
  pinMode(BUS5, OUTPUT);
  pinMode(BUS6, OUTPUT);
  
  serial.println();
  serial.println("-----------------------------------------------------");
  serial.println("------------------    I2CTester    ------------------");
  serial.println("-----------------------------------------------------");
  serial.println();
  
  // initialize the GPIOs used to control the power buses high
  digitalWrite(BUS1, HIGH); 
  digitalWrite(BUS2, HIGH);
  digitalWrite(BUS3, HIGH);
  digitalWrite(BUS4, HIGH);
  digitalWrite(BUS5, HIGH);
  digitalWrite(BUS6, HIGH);
  
  // ensure the system is ready
  delay(10);
  
  ina.setShuntResistor(0.04);
}

void loop()
{
  // scan the bus and count the available devices
  serial.println("Scanning bus... ");
  unsigned char c = I2CScanner::scan(wire), deviceFound);
  serial.println("Scan completed");
  
  serial.println("INA226 present");
        
        // read the telemetry from the INA226
        signed short i1 = ina.getCurrent();

        serial.print("Current OFF: ");
        serial.print(i1, DEC);
        serial.println(" mA");
  
  delay(2354);
}
