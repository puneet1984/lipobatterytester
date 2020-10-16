/*
Lipo battery capacity tester V1.0 
By Puneet Maheshwari - EasyElectronics
This code can test any battery upto 5V
 */

#include <Wire.h>
#include <Adafruit_SSD1306.h> //oled library
#define mosfet 9

float cap = 0.0; //capacity of battery
float bat_v = 0.0;  //battery voltage
float res_v = 0.0;  //voltage at the shunt resistor
float current = 0.0;  //current in amps
float ma = 0.0; //current in mA
float temp1; //temporary variable
float temp2; //temporary variable
float res = 0.2; //value of shunt resistor
float bat_h = 4.3; //battery high Voltage
float bat_l = 2.9; //battery low voltage
float millipassed = 0;
float prevmilli = 0;
float secspassed = 0;
Adafruit_SSD1306 display(128, 32, &Wire, 4); ////oled initialise

void setup() {
  Serial.begin(9600);
  pinMode(mosfet, OUTPUT);
  digitalWrite(mosfet, LOW);
  pinMode(A0, INPUT); //battery voltage
  pinMode(A1, INPUT); //resistor voltage

  //  OLED setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
}

void loop() {

  //averaging the analog readings
  for (int i = 0; i < 100; i++) {
    temp1 += analogRead(A0);
    temp2 += analogRead(A1);
  }

  temp1 = temp1 / 100;
  temp2 = temp2 / 100;

  //transforming analog readings to voltage
  bat_v = temp1 * 0.0048828125; //  5/1024=0.0048828125
  res_v = temp2 * 0.0048828125;

  float diff_v = bat_v - res_v; //voltage difference

  current = diff_v / res; //current in amps ; I=V/R

  ma = current * 1000; //current in mA

  if (bat_v < bat_h && bat_v > bat_l) {
    digitalWrite(mosfet, HIGH);
    millipassed = millis() - prevmilli;
    secspassed = (millipassed / 3600000.0);  //converting milliseconds to hours
    cap += (ma * secspassed); //capacity = milliamps*hour
    prevmilli = millis();
    Serial.println(String("MOSFET ON"));
  }
  else if (bat_v > bat_h) {
    digitalWrite(mosfet, LOW);
    prevmilli = millis();
    Serial.println(String("battery voltage HIGH"));
  }
  else if (bat_v < bat_l) {
    digitalWrite(mosfet, LOW);
    prevmilli = millis();
    Serial.println(String("battery voltage LOW"));
  }

  //OLED display
  display.clearDisplay();
  display.setTextSize(1);       // Normal 1:1 pixel scale
  display.setTextColor(WHITE);  // Draw white text
  display.setCursor(0, 0);
  display.print(String("Bat Voltage ") + bat_v + " V");
  display.setCursor(0, 10);
  display.print(String("Current ") + ma + " mA");
  display.setCursor(0, 20);
  display.print(String("Cap ") + cap + " mAh");
  display.display();

  //serial debug
  Serial.println(String("bat V ") + bat_v + "V ");
  Serial.println(String("Current ") + ma + "mA ");
  Serial.println(String("diff_V ") + (bat_v - res_v) + "V ");
  Serial.println(String("millisecs=") + millipassed);
  Serial.println(String("capacity=") + cap + "mAh");
  Serial.println(String("------------------------------------"));

  delay(1000);
}
