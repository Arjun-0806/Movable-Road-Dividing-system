#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3DtzaD7rJ"
#define BLYNK_TEMPLATE_NAME "movable"
#define BLYNK_AUTH_TOKEN "tDFKnbnF5Pe-PTURbskWMO0S40VplFRb"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Mallikarjuna";
char pass[] = "Mali@0806";

#include <SPI.h>
#include "Ucglib.h"

// Define an enum for directions
enum Direction {
  right,
  left
};

Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 2, /*cs=*/ 5, /*reset=*/ 4);

const int ir_1 = 34;
const int ir_2 = 35;
const int ir_3 = 32;
const int ir_4 = 33;

const int trigger_1 = 27;
const int echo_1    = 14;

const int trigger_2 = 12;
const int echo_2    = 13;

const int buzzer = 15;
const int motor_1 = 21;
const int motor_2 = 17;

int currentIndex = 0; // Index to keep track of the current position in the circular buffer
bool AUTO = 1;
BLYNK_WRITE(V0){
  int var = param.asInt();
  if(var == 1){
    AUTO = 1;
  }else{
    AUTO = 0;
  }
}

BLYNK_WRITE(V1){
  int var = param.asInt();

  if(AUTO == 0){
    if(var == 1){
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, HIGH);
      delay(1000);
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, LOW);
    }else{
      digitalWrite(motor_1, HIGH);
      digitalWrite(motor_2, LOW);
      delay(1000);
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, LOW);
   }
  }
}

BLYNK_WRITE(V2){
  int var = param.asInt();

  if(AUTO == 0){
    if(var == 1){
      digitalWrite(motor_1, HIGH);
      digitalWrite(motor_2, LOW);
      delay(1000);
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, LOW);
    }else{
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, HIGH);
      delay(1000);
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, LOW);
   }
  }
}

void setup() {
  // Display setup:
  Serial.begin(115200);
  pinMode(ir_1, INPUT);
  pinMode(ir_2, INPUT);
  pinMode(ir_3, INPUT);
  pinMode(ir_4, INPUT);
  
  pinMode(trigger_1, OUTPUT);
  pinMode(echo_1, INPUT);

  pinMode(trigger_2, OUTPUT);
  pinMode(echo_2, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(motor_1, OUTPUT);
  pinMode(motor_2, OUTPUT);
   //ucg.setRotate90();
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  //ucg.begin(UCG_FONT_MODE_SOLID);
  ucg.clearScreen();
  ucg.setFont(ucg_font_ncenR12_tr);
  ucg.setColor(255, 255, 255);
  //ucg.setColor(0, 255, 0);
  ucg.setColor(1, 255, 0,0);
  
  ucg.setPrintPos(10,25);
  ucg.print("Welcome To");
  ucg.setPrintPos(10,50);
  ucg.print("Road Divider");
  ucg.setPrintPos(30,75);
  ucg.print("Project");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  delay(5000);
}

void loop() {
  Blynk.run();
  int ir_val1 = digitalRead(ir_1);
  int ir_val2 = digitalRead(ir_2);
  int ir_val3 = digitalRead(ir_3);
  int ir_val4 = digitalRead(ir_4);
  Serial.println("IR1 : "+String(ir_val1));
  Serial.println("IR2 : "+String(ir_val2));
  Serial.println("IR3 : "+String(ir_val3));
  Serial.println("IR4 : "+String(ir_val4));
  bool dist1 = check_ultra(Direction::left);
  bool dist2 = check_ultra(Direction::right);
  
  if (ir_val1 == LOW && ir_val2 == LOW) {
    if(dist1 == 1) {
      Serial.println("Someone found in between divider. Beware!");
      ucg.clearScreen();
      ucg.setFont(ucg_font_ncenR12_tr);
      ucg.setColor(255, 255, 255);
      //ucg.setColor(0, 255, 0);
      ucg.setColor(1, 255, 0,0);
  
      ucg.setPrintPos(0,25);;
      ucg.println("Someone found");
      ucg.setPrintPos(10,50);
      ucg.println("in between");
      ucg.setPrintPos(0,75);
      ucg.println("divider, Beware!!!");
      for (int i = 0; i < 5; i++) {
        digitalWrite(buzzer, 1); // Start tone with 1000Hz frequency
        delay(100); // Short delay for fast beep
        digitalWrite(buzzer, 0); // Stop tone
        delay(100);
      }
    }else if(AUTO == 1){
      //tft.fillScreen(ST7735_BLACK);
      Serial.println("Road is Moving Right");
      ucg.clearScreen();
      ucg.setFont(ucg_font_ncenR12_tr);
      ucg.setColor(255, 255, 255);
      //ucg.setColor(0, 255, 0);
      ucg.setColor(1, 255, 0,0);
  
      ucg.setPrintPos(0,25);
      ucg.println("Road is");
      ucg.setPrintPos(20,50);
      ucg.println("Moving");
      ucg.setPrintPos(20,75);
      ucg.println("Right!!!");
      
      digitalWrite(buzzer, 1); // Start tone with 1000Hz frequency
      delay(3000);
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, HIGH);
      Serial.println("Motor is Driving");
      Blynk.virtualWrite(V3, 1);
      delay(1000);
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, LOW);
      digitalWrite(buzzer, 0);
      delay(5000);
      while(digitalRead(ir_1) == LOW && digitalRead(ir_2) == LOW);
      Blynk.virtualWrite(V3, 0);
      digitalWrite(buzzer, 1);
      ucg.clearScreen();
      ucg.setFont(ucg_font_ncenR12_tr);
      ucg.setColor(255, 255, 255);
      //ucg.setColor(0, 255, 0);
      ucg.setColor(1, 255, 0,0);
  
      ucg.setPrintPos(0,25);
      ucg.println("Road is");
      ucg.setPrintPos(20,50);
      ucg.println("Moving");
      ucg.setPrintPos(20,75);
      ucg.println("Normal Position");
      
      delay(3000);
      digitalWrite(motor_1, HIGH);
      digitalWrite(motor_2, LOW);
      delay(1000);
      Serial.println("Motor is Driving");
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, LOW);
      digitalWrite(buzzer, 0);
    }
  }else{
    ucg.clearScreen();
    ucg.setFont(ucg_font_ncenR12_tr);
    ucg.setColor(255, 255, 255);
    ucg.setColor(1, 255, 0,0);
    ucg.setPrintPos(10,25);
    ucg.println("Wishing You");
    ucg.setPrintPos(30,50);
    ucg.println("Happy");
    ucg.setPrintPos(40,75);
    ucg.println("&");
    ucg.setPrintPos(20,100);
    ucg.println("Safe Journey");
  }
  if (ir_val3 == LOW && ir_val4 == LOW) {
    if(dist2 == 1) {
      Serial.println("Someone found in between divider");
       ucg.clearScreen();
      ucg.setFont(ucg_font_ncenR12_tr);
      ucg.setColor(255, 255, 255);
      //ucg.setColor(0, 255, 0);
      ucg.setColor(1, 255, 0,0);
      
      ucg.setPrintPos(0,25);
      ucg.println("Someone found");
      ucg.setPrintPos(20,50);
      ucg.println("in between");
      ucg.setPrintPos(0,75);
      ucg.println("divider, Beware!!!");
      
      for (int i = 0; i < 5; i++) {
        digitalWrite(buzzer, 1); // Start tone with 1000Hz frequency
        delay(100); // Short delay for fast beep
        digitalWrite(buzzer, 0); // Stop tone
        delay(100);
      }
    }
    else if (AUTO == 1) {
      Serial.println("Road is Moving Left");
      ucg.clearScreen();
      ucg.setFont(ucg_font_ncenR12_tr);
      ucg.setColor(255, 255, 255);
      //ucg.setColor(0, 255, 0);
      ucg.setColor(1, 255, 0,0);
  
      ucg.setPrintPos(0,25);
      ucg.println("Road is ");
      ucg.setPrintPos(20,50);
      ucg.println("Moving");
      ucg.setPrintPos(20,75);
      ucg.println("Left");
      
      digitalWrite(buzzer, 1); // Start tone with 1000Hz frequency; // Stop ton
      delay(3000);
      digitalWrite(motor_1, HIGH);
      digitalWrite(motor_2, LOW);
      Blynk.virtualWrite(V4, 1);
      delay(1000);
      digitalWrite(buzzer, 0);
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, LOW);
      ucg.clearScreen();
      ucg.setFont(ucg_font_ncenR12_tr);
      ucg.setColor(255, 255, 255);
      //ucg.setColor(0, 255, 0);
      ucg.setColor(1, 255, 0,0);
  
      ucg.setPrintPos(0,25);
      ucg.println("Road is");
      ucg.setPrintPos(20,50);
      ucg.println("Moving");
      ucg.setPrintPos(20,75);
      ucg.println("Normal Position");
      
      delay(5000);
      while(digitalRead(ir_3) == LOW && digitalRead(ir_4) == LOW);
      
      Blynk.virtualWrite(V4, 0);
      
      digitalWrite(buzzer, 1);

      delay(3000);
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, HIGH);
      delay(1000);
      digitalWrite(motor_1, LOW);
      digitalWrite(motor_2, LOW);
      digitalWrite(buzzer, 0);
    }
  }else{
    ucg.clearScreen();
    ucg.setFont(ucg_font_ncenR12_tr);
    ucg.setColor(255, 255, 255);
    ucg.setColor(1, 255, 0,0);
    ucg.setPrintPos(10,25);
    ucg.println("Wishing You");
    ucg.setPrintPos(30,50);
    ucg.println("Happy");
    ucg.setPrintPos(40,75);
    ucg.println("&");
    ucg.setPrintPos(20,100);
    ucg.println("Safe Journey");
  }
  delay(100);
}

bool check_ultra(Direction side) {
  if (side == Direction::right) {
    digitalWrite(trigger_2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger_2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_2, LOW);
    
    // Read the echoPinRight, and calculate distance
    long duration = pulseIn(echo_2, HIGH);
    int distance = duration * 0.034 / 2;
    Serial.println("Right Distance:"+ String(distance));
    if (distance < 5) {
        digitalWrite(motor_1, LOW);
        digitalWrite(motor_2, LOW);
      return true;
    } else {
      return false;
    }
  } else if (side == Direction::left) {
      digitalWrite(trigger_1, LOW);
      delayMicroseconds(2);
      digitalWrite(trigger_1, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigger_1, LOW);
      // Read the echoPinLeft, and calculate distance
      long duration = pulseIn(echo_1, HIGH);
      int distance = duration * 0.034 / 2;
      Serial.println("Left Distance:"+ String(distance));
      if (distance < 5) {
        digitalWrite(motor_1, LOW);
        digitalWrite(motor_2, LOW);
        return true;
      } else {
        return false;
      }
   }
}
