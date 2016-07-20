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
#include <MAX1237.h>
#include "I2CTester.h"

DWire wire;
DSerial serial;

// enable the use of serial port: it is useful 
// for debugging but it slows the system down
bool enablePrint = false;

#if INA_bus_one
  INA226 ina_bus1(wire, 0x40);
#endif
#if INA_bus_four
  INA226 ina_bus4(wire, 0x44);
#endif
#if INA_bus_six
  INA226 ina_bus6(wire, 0x45);
#endif

#if PCA9550_bus_one
  PCA9550 blinker1(wire, 0x60);
#endif
#if PCA9550_bus_six
  PCA9550 blinker6(wire, 0x61);
#endif

#if MAX1237_bus_four
  MAX1237 adc(wire);
#endif

unsigned char dev[128];
unsigned char devicesCount;

void deviceFound(unsigned char device)
{
  dev[devicesCount] = device;
  devicesCount++;
}

void scanBus(bool print)
{
  devicesCount = 0;
  // scan the bus and count the available devices
  if (print)
  { 
     serial.println();
     serial.println("Scanning bus... ");
  }
  
  I2CScanner::scan(wire, deviceFound);
  
  if (print)
  {   
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
}

void setup()
{
  // Initialize I2C master
  wire.setFastMode();
  wire.begin();
  
  // initialize debug UART
  serial.begin();
  
  // ensure the serial port is initialized
  delay(20);
  
  serial.println();
  serial.println("-----------------------------------------------------");
  serial.println("------------------    I2CTester    ------------------");
  serial.println("-----------------------------------------------------");
  serial.println();
}

void loop()
{
  scanBus(enablePrint);
  
  #if INA_bus_one
    signed short i1 = ina_bus1.getCurrent();
    if (enablePrint)
    {
      if ((i1 > 1500) || (i1 < 2))
      {
          serial.print("Current BUS 1: ");
          serial.print(i1, DEC);
          serial.println(" mA");
      }
    }
  #endif
  
  #if INA_bus_four
    signed short i4 = ina_bus4.getCurrent();
    if (enablePrint)
    {
      if ((i4 > 1500) || (i4 < 2))
      {
          serial.print("Current BUS 4: ");
          serial.print(i4, DEC);
          serial.println(" mA");
      }
    }
  #endif
  
  #if INA_bus_six
    signed short i6 = ina_bus6.getCurrent();
    if (enablePrint)
    {
      if ((i6 > 1500) || (i6 < 2))
      {
          serial.print("Current BUS 6: ");
          serial.print(i6, DEC);
          serial.println(" mA");
      }
    }
  #endif
  
  #if MAX1237_bus_four
    unsigned short val = adc.readSingleChannel() >> 1;
  #endif
}
