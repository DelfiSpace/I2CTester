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

#include <INA226.h>
#include <PCA9550.h>

INA226 ina2(wire, 0x45);
//PCA9550 blinker1(wire, 0x60);
//PCA9550 blinker6(wire, 0x61);

void setupBlinkers()
{
//delay(5000);
  // set the shunt resistor for the INA on bus 6
    //ina2.setShuntResistor(0.04);
    
  // setup blinker on bus 1
  // set DC to ~30%
  //blinker1.setDC(0, 90);
  // set period to 305ms
  //blinker1.setPeriod(0, 0.0305);
  // enable blinker
  //blinker1.blinkLED(0, 0);
  
   //blinker1.setOutput(0, 0);
    
  // setup blinker on bus 6
  // set DC to ~25%
  //blinker6.setDC(0, 64);
  // set period to 10ms
  //blinker6.setPeriod(0, 0.01);
  // enable blinker
  //blinker6.blinkLED(0, 0);
}

void loopBlinkers()
{
  //ina2.setShuntResistor(0.04);
    
  //serial.println("INA226 present");
        
        // read the telemetry from the INA226
        //signed short i1 = ina2.getCurrent();

        serial.print("Current 2 OFF: ");
        //serial.print(i1, DEC);
        serial.println(" mA");
  delay(500);
}

