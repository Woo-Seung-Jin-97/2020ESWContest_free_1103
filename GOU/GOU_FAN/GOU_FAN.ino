#define switch_1 0
#define switch_2 1
#define LED_1    3

#include <AFMotor.h>    



AF_DCMotor motor(2);  



void setup() {
  pinMode(switch_1, INPUT);
  pinMode(switch_2, INPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(9,OUTPUT);
  motor.setSpeed(255);
 
  
}

void loop() {
  // put your main code here, to run repeatedly:

    if(digitalRead(switch_1)==0){
      digitalWrite(LED_1, LOW);
       
        // 12번을 HIGH 상태로 두고
  digitalWrite(9,HIGH); 
 
  motor.run(FORWARD);
    
    
    }

    else if(digitalRead(switch_2)==0){
      digitalWrite(LED_1, HIGH);
       // 강제 정지 기능은 LOW 상태로 두며
  digitalWrite(9,LOW);
      motor.run(RELEASE);
      
    
    }
    
}
