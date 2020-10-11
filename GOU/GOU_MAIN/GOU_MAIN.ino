/*https://blog.naver.com/intopion/221613937290 RFID 전기연결 및 코드 참조
  * 
  */
   /* Typical pin layout used:
 * -----------------------------------
 *             MFRC522      Arduino  
 *             Reader/PCD   Uno     
 * Signal      Pin          Pin      
 * -----------------------------------
 * RST/Reset   RST          9         
 * SPI SS      SDA(SS)      10        
 * SPI MOSI    MOSI         11       
 * SPI MISO    MISO         12       
 * SPI SCK     SCK          13       
  */
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>                        // i2C 통신을 위한 라이브러리
#include <LiquidCrystal_I2C.h>        // LCD 1602 I2C용 라이브러리
#define RST_PIN 9
#define SS_PIN 10
#define MAX 7
#define speaker 2
#define switch_1 8
#define sg90 4
LiquidCrystal_I2C lcd(0x3F,16,2);   

int savesize=0;
String save [MAX];
String tmp;
int angle=0;
int x;
int i;
const int Do = 262;
const int Do2 = 523;


Servo SG90; //서보모터 정의
MFRC522 rc522(SS_PIN, RST_PIN); //RFID 모듈 정의


void setup(){
  SPI.begin();
  rc522.PCD_Init(); // RFID 모듈 초기화
  SG90.attach(sg90); // 서보모터 초기화
  pinMode(8,INPUT);
  pinMode(speaker,OUTPUT);
  pinMode(6,OUTPUT); // Dir
  pinMode(5,OUTPUT); // Step
    lcd.init();                      // LCD 초기화
  // Print a message to the LCD.
  lcd.backlight();                // 백라이트 켜기
  lcd.setCursor(0,0);             // 1번째, 1라인
  lcd.print("Hello");
  lcd.setCursor(0,1);             // 1번째, 2라인
  lcd.print("Produced by GOU");
  re_start();
}

void loop(){
  tmp="";
  digitalWrite(6,HIGH); // Set Dir high
    for(x = 0; x <= 200; x++) // Loop 200 times
  {
    digitalWrite(5,HIGH); // Output high
    delay(7); // Wait
    digitalWrite(5,LOW); // Output low
    delay(7); // Wait
    if(digitalRead (switch_1)==0){
      break;
    }
    }
    delay(500);
  //step 1 ) 카드 또는 ID 가 읽히지 않으면 return을 통해 다시 시작하게 됩니다.
  if ( !rc522.PICC_IsNewCardPresent() || !rc522.PICC_ReadCardSerial() ) {
      //모터있던곳
      delay(500);
  digitalWrite(6,HIGH); // Set Dir high
    for(x = x+1; x <= 200; x++) // Loop 200 times
  {
    digitalWrite(5,HIGH); // Output high
    delay(7); // Wait
    digitalWrite(5,LOW); // Output low
    delay(7); // Wait
    }
    delay(500);
    return;
    }
 //step 2 ) 카드가 읽히면 아래의 실행문 실행
  else{
 //2 - 1 ) 받아들인 uid를 string으로 변환하여 tmp에 저장
    for (byte i = 0; i < 4; i++) {
    tmp += rc522.uid.uidByte[i];
  } // uid 출력 
      lcd.init();
      lcd.setCursor(0,0); // 1번째, 1라인
      lcd.print("Card contact!"); 
      lcd.setCursor(0,1); //2번째, 2라인
      lcd.print("ID : ");
      lcd.print(tmp); //작업중
      Serial.println(tmp);

  // 받아들인 uid가 이미 존재하면 save 배열에서 제거하고 루프 다시 시작
    for(byte i=0; i<MAX; i++){
    if(save[i]==tmp){
      lcd.init();
      lcd.setCursor(0,0); // 1번째, 1라인
      lcd.print("UNLOCK"); 
      lcd.setCursor(0,1); //2번째, 2라인
      lcd.print("Take Umbrella"); 
      delay(100);
       digitalWrite(6,HIGH); // Set Dir high   
    if(x<25*(i+1)){
      for(x = x+1; x <= ((25*i)+25); x++) // Loop 200 times
          {
            digitalWrite(5,HIGH); // Output high
            delay(7); // Wait
            digitalWrite(5,LOW); // Output low
            delay(7); // Wait
          }
         for(int j=0;j<90;j++){
        SG90.write(j);
        delay(10);
      }//문열림
      tone(speaker,Do2,250);
      delay(5000);
      for(int j=90;j>=0;j--){
        SG90.write(j);
        delay(10);
      }//문닫힘
          digitalWrite(6,LOW); // Set Dir high
          for( x = 0; x <= ((25*i)+25); x++) // Loop 200 times
          {
            digitalWrite(5,HIGH); // Output high
            delay(7); // Wait
            digitalWrite(5,LOW); // Output low
            delay(7); // Wait
          }
  delay(500);
  }
  else{
    for(x = x+1; x <= (200+(25*(i+1))); x++) // Loop 200 times
          {
            digitalWrite(5,HIGH); // Output high
            delay(7); // Wait
            digitalWrite(5,LOW); // Output low
            delay(7); // Wait
          }
          for(int j=0;j<90;j++){
        SG90.write(j);
        delay(10);
      }//문열림
      tone(speaker,Do2,250);
      delay(5000);
      for(int j=90;j>=0;j--){
        SG90.write(j);
        delay(10);
      }//문닫힘
          digitalWrite(6,LOW); // Set Dir high
            for( x = 0; x <= ((25*i)+25); x++) // Loop 200 times
          {
            digitalWrite(5,HIGH); // Output high
            delay(7); // Wait
            digitalWrite(5,LOW); // Output low
            delay(7); // Wait
          }
          delay(500);
      }
      save[i]="";
      savesize--;
      lcd.init();
      lcd.setCursor(0,0); // 1번째, 1라인
      lcd.print("Remaining count"); 
      lcd.setCursor(0,1); //2번째, 2라인
      lcd.print(savesize);       
      return;
    }
  }

  // 받아들인 uid가 save배열에 없으면, 빈 부분에 추가
  if(savesize<MAX){
      for(i=0; i<MAX; i++){
        if(save[i]==""){
          save[i]=tmp;
          digitalWrite(6,HIGH); // Set Dir high
          if(x<25*(i+1)){
    for(x = x+1; x <= ((25*i)+25); x++) // Loop 200 times
  {
    digitalWrite(5,HIGH); // Output high
    delay(7); // Wait
    digitalWrite(5,LOW); // Output low
    delay(7); // Wait
  }
  for(int j=0;j<90;j++){
        SG90.write(j);
        delay(10);
      }//문열림
      tone(speaker,Do2,250);
      delay(5000);
      for(int j=90;j>=0;j--){
        SG90.write(j);
        delay(10);
      }//문닫힘
          digitalWrite(6,LOW); // Set Dir high
    for(x = 0; x <= ((25*i)+25); x++) // Loop 200 times
  {
    digitalWrite(5,HIGH); // Output high
    delay(7); // Wait
    digitalWrite(5,LOW); // Output low
    delay(7); // Wait
  }
  delay(500);
  }
  else{
    for(x = x+1; x <= (200+(25*(i+1))); x++) // Loop 200 times
  {
    digitalWrite(5,HIGH); // Output high
    delay(7); // Wait
    digitalWrite(5,LOW); // Output low
    delay(7); // Wait
  }
  for(int j=0;j<90;j++){
        SG90.write(j);
        delay(10);
      }//문열림
      tone(speaker,Do2,250);
      delay(5000);
      for(int j=90;j>=0;j--){
        SG90.write(j);
        delay(10);
      }//문닫힘
   digitalWrite(6,LOW); // Set Dir high
    for(x = 0; x <= ((25*i)+25);  x++) // Loop 200 times
  {
    digitalWrite(5,HIGH); // Output high
    delay(7); // Wait
    digitalWrite(5,LOW); // Output low
    delay(7); // Wait
  }
  delay(500);
  }
      savesize++;
      lcd.init();
      lcd.setCursor(0,0); // 1번째, 1라인
      lcd.print("Remaining count"); 
      lcd.setCursor(0,1); //2번째, 2라인
      lcd.print(savesize);  //모터있던곳
        break;
        }
        }
  }

  // save배열이 다 찼으면 루프 다시 시작
  else{
     lcd.init();
      lcd.setCursor(0,0); // 1번째, 1라인
      lcd.print("FULL"); 
      return;
  }
}
}

void re_start(){
  SG90.write(0);
  tone(speaker,Do,250);
  delay(1000);
      for(int j=0;j<90;j++){
        SG90.write(j);
        delay(10);
      }
      for(int j=90;j>=0;j--){
        SG90.write(j);
        delay(10);
      }
  delay(1000);
  return;
}
