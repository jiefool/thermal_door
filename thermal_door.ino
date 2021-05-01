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

void setup(){
  lcd.init();
  lcd.clear();         
  lcd.backlight(); 

  pinMode(openDoor, OUTPUT);
  pinMode(closeDoor, OUTPUT);
  pinMode(buzzer, OUTPUT);

  mlx.begin();

  constantScreen();

  closeDoorFunc();
}

void loop(){
  temp = mlx.readObjectTempC();
  printToLCD(0,2, (String) temp + " C");

  if(temp > 32 && temp < 37){
    openDoorFunc();
    printToLCD(0,3, "Door Status: "+doorStatus+"  ");
    alarmNotif();
  }else{
    closeDoorFunc(); 
    printToLCD(0,3, "Door Status: "+doorStatus+"  ");
    alarmNotif();
  }

  if(temp > 37){
    alarmSound();  
  }
  
  delay(100);
}


void constantScreen(){
  printToLCD(0,0, "Thermal Door System");
  printToLCD(0,1, "Reading Temperature:");
  printToLCD(0,2, (String) temp + " C");
  printToLCD(0,3, "Door Status: close");
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
