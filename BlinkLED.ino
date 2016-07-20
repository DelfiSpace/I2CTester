#define LED RED_LED

void setupRedLed() 
{ 
  // initialize the digital pin as an output.
  pinMode(LED, OUTPUT); 
}

void loopRedLed() 
{ 
  digitalWrite(LED, HIGH); // turn the LED on 
  delay(500); // wait for 1/2 second
  digitalWrite(LED, LOW); // turn the LED off 
  delay(500); // wait for 1/2 second  
}
