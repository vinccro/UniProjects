// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");

  // turn on motor
  motor1.setSpeed(200);
    motor2.setSpeed(200);
      motor3.setSpeed(200);
        motor4.setSpeed(200);
 
  motor1.run(RELEASE);
   motor2.run(RELEASE);
    motor3.run(RELEASE);
     motor4.run(RELEASE);
}

void loop() {
  uint8_t i;
char key = Serial.read();
//motor1
if( key == 'q' ){
  motor1.run(FORWARD);
}
if( key == 'a' ){
  motor1.run(RELEASE);
  
}
if( key == 'z'){
  motor1.run(BACKWARD);
}
//motor2
if( key == 'w' ){
  motor2.run(FORWARD);
}
if( key == 's' ){
  motor2.run(RELEASE);
  
}
if( key == 'x'){
  motor2.run(BACKWARD);
}

//motor3
if( key == 'e' ){
  motor3.run(FORWARD);
}
if( key == 'd' ){
  motor3.run(RELEASE);
  
}
if( key == 'c'){
  motor3.run(BACKWARD);
}
//motor4
if( key == 'r' ){
  motor4.run(FORWARD);
}
if( key == 'f' ){
  motor4.run(RELEASE);
  
}
if( key == 'v'){
  motor4.run(BACKWARD);
}


}
/*
  
  Serial.print("tock");

  motor.run(BACKWARD);

  Serial.print("tech");
  motor.run(RELEASE);
  delay(1000);
}*/
