#include <Usb.h>
#include <adk.h>
#include <Servo.h>

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

USB Usb;
ADK adk(&Usb,"Raszewski", // Manufacturer Name
"MEGA_ADK", // Model Name
"Description", // Description (user-visible string)
"1.0", // Version
"http://www.raszewski.info", // URL (web page to visit if no installed apps support the accessory)
"0000000012345678"); // Serial Number (optional)

#define LED 10 // Pin 13 is occupied by the SCK pin on a normal Arduino (Uno, Duemilanove etc.), so use a different pin

#define DEBUG false // print out debug output over Serial Monitor

void setup()
{
  Serial.begin(115200);
  Serial.print("\r\nQC Kamikaze start");
  if (Usb.Init() == -1) {
    Serial.print("\r\nOSCOKIRQ failed to assert");
    while(1); //halt
  }
  pinMode(LED, OUTPUT);

  calibrateECS();
}

void loop()
{    
  Usb.Task();
  if(adk.isReady()) {
    uint8_t msg[4];
    uint16_t len = sizeof(msg);
    uint8_t rcode = adk.RcvData(&len, msg);
    if(rcode && rcode != hrNAK)
      USBTRACE2("Data rcv. :", rcode);
    if(len > 0) {
      if(DEBUG){
        Serial.println(F("\r\nData Packet: "));
        Serial.println(msg[0]);
        Serial.println(msg[1]);
        Serial.println(msg[2]);
        Serial.println(msg[3]);
      }
      motor1.write(msg[0]);
      motor2.write(msg[1]);
      motor3.write(msg[2]);
      motor4.write(msg[3]);
      digitalWrite(LED,HIGH);
    }
  } 
  else
    digitalWrite(LED, LOW); 
}

void calibrateECS(){
  // teach the controller and arm
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(500);               // wait for a second
  motor1.attach(7,1000,2000);
  motor2.attach(6,1000,2000);
  motor3.attach(5,1000,2000);
  motor4.attach(4,1000,2000);
  Serial.println("Attached PIN 7 for ESC for calibration");
  all_motors_speed(40);
  delay(5000);
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("Connect battery pack to ESC");

  Serial.println("Wait for 2 s...");
  delay(5000);
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  all_motors_speed(0);

  Serial.println("Should hear as many short beeps as many cels your battery is");

  for(int seconds = 0; seconds <5 ; seconds++) 
  { 
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);               // wait for a second
    digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    delay(500);
  }

  Serial.println("ESC calibrated");
}

void all_motors_speed(int speed){
  motor1.write(speed);
  motor2.write(speed-1);
  motor3.write(speed);
  motor4.write(speed);

}






