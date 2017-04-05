#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#include <LiquidCrystal_I2C.h>
#include <Servo.h>    

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

Servo myservo;

int IN3 = 4; 
int IN4 = 5;

int descargas;

const int buttonPin = 10;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status


// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
void setup()
{
  Wire.begin();
  Serial.begin(9600);

  //contador de descargas de comida
  descargas=0;
  
  //motor
  pinMode (IN4, OUTPUT);    // Input4 conectada al pin 4 
  pinMode (IN3, OUTPUT);    // Input3 conectada al pin 5

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object    
  myservo.write(80);

  pinMode(buttonPin, INPUT);

  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(00,51,20,7,04,04,17);

  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight

// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  


//-------- Write characters on the display ------------------
// NOTE: Cursor Position: (CHAR, LINE) start at 0  
  lcd.setCursor(0,0); //Start at character 4 on line 0
  lcd.print("Comedor Max v1.4");
  lcd.setCursor(0,1); //Start at character 4 on line 0
  lcd.print("Iniciando...");
  delay(3000);
  lcd.clear();
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,byte *minute,byte *hour,byte *dayOfWeek,byte *dayOfMonth,byte *month,byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void loop(){

  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

  imprimeLcdserial(second,minute,hour,dayOfWeek,dayOfMonth,month,year);

  buttonState = digitalRead(buttonPin);
  Serial.println("STATE:");
  Serial.println(buttonState);
  
  if(hour==9 && minute==00 && second==00){
    virarOn();
    abrePuerta();
    vibrarOff();
    cierraPuerta();
    descargas++;
  }
  
   if(hour==14 && minute==00 && second==00){
    virarOn();
    abrePuerta();
    vibrarOff();
    cierraPuerta();
    descargas++;
  }

   if(hour==20 && minute==00 && second==00){
    virarOn();
    abrePuerta();
    vibrarOff();
    cierraPuerta();
    descargas++;
  }

  if(buttonState == HIGH){
    virarOn();
    abrePuerta();
    vibrarOff();
    cierraPuerta();
  }

  lcd.setCursor(0,1);
  lcd.print("N-Descarga:");
  lcd.print(" ");
  if(descargas>=9){
     lcd.print("Empy");
  }else{
    lcd.print(descargas);
  }
  
}


void imprimeLcdserial(byte second,byte minute,byte hour,byte dayOfWeek,byte dayOfMonth,byte month,byte year){
   lcd.setCursor(0,0);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  lcd.print(hour);
  lcd.print(":");
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  
  if (minute<10)
  {
    Serial.print("0");
    lcd.print("0");
  }
  Serial.print(minute, DEC);
  lcd.print(minute);
  Serial.print(":");
  lcd.print(":");
  
  if (second<10)
  {
    Serial.print("0");
    lcd.print("0");
  }
  
  Serial.print(second, DEC);
  lcd.print(second);
  
  Serial.print(" ");
  lcd.print(" ");
  
  Serial.print(dayOfMonth, DEC);
  lcd.print(dayOfMonth);
  lcd.print("/");
  Serial.print("/");
  Serial.print(month, DEC);
  lcd.print(month);
  Serial.print("/");
  lcd.print("/");
  Serial.print(year, DEC);
  lcd.print(year);
  Serial.print(" Day of week: ");
}

void abrePuerta(){
  myservo.write(140);
  delay(900);
}

void cierraPuerta(){
  myservo.write(80);
  delay(900);
}

void virarOn(){
   // Motor gira en un sentido
  digitalWrite (IN4, HIGH);
  digitalWrite (IN3, LOW); 
  delay(4000);
  // Motor no gira
  digitalWrite (IN4, LOW); 
  delay(500);
  // Motor gira en sentido inverso
  digitalWrite (IN3, HIGH);
  delay(4000);
}

void vibrarOff(){
  // Motor no gira
  digitalWrite (IN3, LOW); 
}

