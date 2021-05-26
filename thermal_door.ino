#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>


LiquidCrystal_I2C lcd(0x27,20,4);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float temp=0.0;
int openDoor = 2;
int closeDoor = 3;
String doorStatus = "close";
long actuatorDelay = 20000;
int buzzer = 4;
String doorState = "close";
long cm = 0.0;
long duration = 0.0;
boolean hasObject = false;

const int triggerPin = 24; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 22; // Echo Pin of Ultrasonic Sensor

void setup(){
  lcd.init();
  lcd.clear();         
  lcd.backlight(); 

  pinMode(openDoor, OUTPUT);
  pinMode(closeDoor, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  mlx.begin();

  constantScreen();

  closeDoorFunc();
}

void loop(){
  temp = mlx.readObjectTempC();
  printToLCD(0,2, (String) temp + " C");

  if(temp > 32 && temp < 37 && hasObject){
    openDoorFunc();
    printToLCD(0,3, "Door Status: "+doorStatus+"  ");
    alarmNotif();
    delay(20000);
  }else{
    closeDoorFunc(); 
    printToLCD(0,3, "Door Status: "+doorStatus+"  ");
    alarmNotif();
  }

  if(temp > 37 && hasObject){
    alarmSound();  
  }

  detectObjectDistance();
  printToLCD(10,2, "CM:    ");
  printToLCD(10,2, "CM: "+(String)cm + "  ");

  if(cm < 40){
    hasObject = true;  
  }else{
    hasObject = false;
  }
  
}


void constantScreen(){
  printToLCD(0,0, "Thermal Door System");
  printToLCD(0,1, "Reading Temperature:");
  printToLCD(0,2, (String) temp + " C");
  printToLCD(0,3, "Door Status: close");
  printToLCD(10,2, "CM: "+(String)cm + "  ");
}

void printToLCD(int x, int y, String text){
  lcd.setCursor(x,y);
  lcd.print(text);
}

void openDoorFunc(){
     digitalWrite(closeDoor, HIGH);
     digitalWrite(openDoor, LOW);
     doorStatus = "open";
}

void closeDoorFunc(){
   digitalWrite(closeDoor, LOW);
   digitalWrite(openDoor, HIGH);
   doorStatus = "close";
}

void alarmSound(){
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
}

void alarmNotif(){
  if(doorState != doorStatus){
    doorState = doorStatus;
    alarmSound();  
  }
}


long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}


void detectObjectDistance(){
   digitalWrite(triggerPin, LOW);
   delayMicroseconds(2);
   digitalWrite(triggerPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(triggerPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);
}
  
