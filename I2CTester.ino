/*
 * Copyright (c) 2016 by Stefano Speretta <s.speretta@tudelft.nl>
 *
 * I2CTester: this is an application designed to test the 
 * functionality of he I2C Tester setup. The setuo includes several 
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

DWire wire(EUSCI_B1_BASE);
DSerial serial;

INA226 ina(wire, 0x40);

void deviceFound(unsigned char device)
{
  serial.print("Found Address 0x");
  if (device < 15) 
  {
      serial.print('0');
  }
  serial.print(device, HEX);
  serial.println();
}

void setup()
{
  // Initialize I2C master
  // TODO: speed?????
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
  unsigned char c = I2CScanner::scan(wire, deviceFound);
  serial.println("Scan completed");
  
  serial.println("INA226 present");
        
        // read the telemetry from the INA226
        signed short i1 = ina.getCurrent();

        serial.print("Current OFF: ");
        serial.print(i1, DEC);
        serial.println(" mA");

  
  
  
  delay(2354);
}
