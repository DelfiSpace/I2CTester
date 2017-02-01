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

#include "I2CTester.h"

// matrix containing all the possible bus configurations for bus 1, 4 and 6
unsigned char busStatus[8][3] = {
// BUS1  BUS4  BUS6  
  { LOW,  LOW,  LOW},
  { LOW,  LOW, HIGH},
  { LOW, HIGH,  LOW},
  { LOW, HIGH, HIGH},
  {HIGH,  LOW,  LOW},
  {HIGH,  LOW, HIGH},
  {HIGH, HIGH,  LOW},
  {HIGH, HIGH, HIGH}};
  
// status variable
unsigned char currentStatus = 0;

void setupPowerController()
{
  // Initialize I2C master
  wire.setFastMode();
  wire.begin();
  
  // initialize debug UART
  serial.begin();
  
  // set the direction to output
  pinMode(BUS1, OUTPUT);
  pinMode(BUS2, OUTPUT);
  pinMode(BUS3, OUTPUT);
  pinMode(BUS4, OUTPUT);
  pinMode(BUS5, OUTPUT);
  pinMode(BUS6, OUTPUT);
}

void loopPowerController()
{
  // enable the busses in all possible combinations
  digitalWrite(BUS1, busStatus[currentStatus][0]);
  digitalWrite(BUS4, busStatus[currentStatus][1]);
  digitalWrite(BUS6, busStatus[currentStatus][2]);

  if (enablePrint)
  {
    serial.println();  
    serial.print("----->   Status: ");
    serial.print(currentStatus, DEC);
    serial.println();
  }
  
  // give enough time to the switch to turn on the device
  // delayMicroseconds is used because it is based on a loop
  // and not on an OS timer: this ensures the task is not
  // paused waiting for the delay to be over. This makes sure
  // preemption is only happening at the end of the loop function.
  delayMicroseconds(10000);
  
  #if PCA9550_bus_one
    // setup blinker on bus 1
    // set DC to ~10%
    blinker1.setDC(0, 25);
    // set period to 0.5s
    blinker1.setPeriod(0, 0.5);
    // enable LED0 blinker on PWM channel 0
    blinker1.blinkLED(0, 0); 
  #endif
  
  #if PCA9550_bus_six
    // setup blinker on bus 6
    // set DC to ~10%
    blinker6.setDC(0, 25);
    // set period to 2s
    blinker6.setPeriod(0, 2);
    // enable LED0 blinker on PWM channel 0
    blinker6.blinkLED(0, 0);
  #endif
  
  #if INA_bus_one
    ina_bus1.setShuntResistor(0.04);
  #endif
  #if INA_bus_four
    ina_bus4.setShuntResistor(0.04);
  #endif
  #if INA_bus_six
    ina_bus6.setShuntResistor(0.04);
  #endif
  
  #if MAX1237_bus_four
    // setup the ADC
    adc.writeRegister(0x80 | 0x02 | (0x05 << 4));
    adc.writeRegister(CS2 | SCAN3 | 1);
  #endif
  
  // make sure the INAs have enough time to sample at least
  // one current value. delayMicroseconds is used to make sure
  // preemption is only happening at the end of the loop function.
  delayMicroseconds(10000);
  
  currentStatus = (currentStatus + 1) % 8;

  delay(3500);
}

