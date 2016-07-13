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
#include <PCA9550.h>
#include <INA226.h>
#include "I2CTester.h"

DWire wire;
DSerial serial;

#define one true
#define four false
#define six false

#if one
  INA226 ina_bus1(wire, 0x40);
#endif
#if four
  INA226 ina_bus4(wire, 0x44);
#endif
#if six
  INA226 ina_bus6(wire, 0x45);
#endif

PCA9550 blinker1(wire, 0x60);
PCA9550 blinker6(wire, 0x61);

unsigned char dev[128];
unsigned char devicesCount;

void deviceFound(unsigned char device)
{
  dev[devicesCount] = device;
  devicesCount++;
}

void scanBus()
{
  devicesCount = 0;
  // scan the bus and count the available devices
  serial.println();
  serial.println("Scanning bus... ");
  I2CScanner::scan(wire, deviceFound);
  
  for(int i = 0; i < devicesCount; i++)
  {
    if (dev[i] != 0)
    {
      serial.print("-> Found Address 0x");
      if (dev[i] < 15) 
      {
        serial.print('0');
      }
      serial.print(dev[i], HEX);
      serial.println();
    }
  }
  
  if (devicesCount == 0)
  {
    serial.println("No device found");
  }
  
  serial.println("Scan completed");
  serial.println();
}

void setup()
{
  // Initialize I2C master
  wire.setStandardMode();
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
  
  // ensure the serial port is initialized
  delay(20);
  
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
  
  // ensure power buses are all high
  delay(10);
  
  // setup blinker on bus 1
  // set DC to ~10%
  /*blinker1.setDC(0, 25);
  // set period to 0.5s
  blinker1.setPeriod(0, 0.5);
  // enable LED0 blinker on PWM channel 0
  blinker1.blinkLED(0, 0); //-> stuck here
  */
  // setup blinker on bus 6
  // set DC to ~10%
  blinker6.setDC(0, 25);
  // set period to 2s
  blinker6.setPeriod(0, 2);
  // enable LED0 blinker on PWM channel 0
  serial.println("Blink");
  blinker6.blinkLED(0, 0);
  serial.println("Blink OK");
  #if one
    ina_bus1.setShuntResistor(0.04);
    delay(20);
  #endif
  #if four
    ina_bus4.setShuntResistor(0.04);
    delay(20);
  #endif
  #if six
    ina_bus6.setShuntResistor(0.04);
    delay(20);
  #endif
  serial.println("setup done");
  //serial.print(NUM_SKETCHES, DEC);
}

void loop()
{
   
  scanBus();
  
  serial.println("INA226 present");

  #if one
    signed short i1 = ina_bus1.getCurrent();
    if ((i1 > 1500) || (i1 < 5))
    {
        serial.print("Current BUS 1: ");
        serial.print(i1, DEC);
        serial.println(" mA");
    }
  #endif
  
  #if four
    signed short i4 = ina_bus4.getCurrent();
    if ((i4 > 1500) || (i4 < 5))
    {
        serial.print("Current BUS 4: ");
        serial.print(i4, DEC);
        serial.println(" mA");
    }
  #endif
  
  #if six
    signed short i6 = ina_bus6.getCurrent();
    if ((i6 > 1500) || (i6 < 5))
    {
        serial.print("Current BUS 6: ");
        serial.print(i6, DEC);
        serial.println(" mA");
    }
  #endif
  
   //serial.print(blinker6.readRegister(REG_LS0), HEX);
   //serial.println();
  //delay(354);
}
