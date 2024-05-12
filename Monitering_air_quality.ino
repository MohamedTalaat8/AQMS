#define BLYNK_TEMPLATE_ID           "TMPL28uttif1H"
#define BLYNK_TEMPLATE_NAME         "Air Quality"
#define BLYNK_AUTH_TOKEN            "HBsZr8_449VEK__Kpbv50tnruDB1BVDS"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include<MQ135.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C  lcd(0x27,16,2);
char ssid[] = "M.T8";
char pass[] = "M.T88888";
#define GPS_RX 17
#define GPS_TX 16
TinyGPSPlus gps;
SoftwareSerial ss(GPS_RX, GPS_TX);
WiFiClient client;
#define DHTPin 4
DHT dht (DHTPin,DHT11);
double  latitude;
double longitude;
#define  MQ135pin  36

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
  Serial.println();
    Serial.println("");
    Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
 dht.begin();
  lcd.init();
  lcd.backlight();
   ss.begin(9600);
  Serial.println("Initializing GPS...");
  pinMode(MQ135pin,INPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, WiFi.SSID().c_str(), WiFi.psk().c_str());
}

void loop() {
   while (ss.available() > 0)
  {
    gps.encode(ss.read());
    if (gps.location.isUpdated())
    {
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
    }else {
        Serial.println(F("- location: INVALID"));
      }
  }
  delay(2000);
 float temp= dht.readTemperature();
  int Humidity= dht.readHumidity();
  Serial.print(F("Humidity: "));
  Serial.print(Humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.print(F("°C "));
lcd.setCursor(0,0);
  lcd.print("H:");
  lcd.print(Humidity);
  lcd.print(" %");
  lcd.setCursor(0,1);
  lcd.print("temp:");
  lcd.print(temp);
  lcd.print(" C");
  Blynk.virtualWrite(V1, Humidity);
  Blynk.virtualWrite(V2, temp);
   delay(5000);
  lcd.clear();
  int Air_Quality = analogRead(MQ135pin);
  Serial.print("PPm:");
  Serial.println(Air_Quality);
lcd.setCursor(0,0);

  lcd.print("A_Q:");
  lcd.print(Air_Quality);
  
   if ( Air_Quality==0||Air_Quality<=50){
    lcd.setCursor(0,1);
    lcd.print("Fresh_air");
  }
   else if (Air_Quality==51|| Air_Quality<=150){
    lcd.setCursor(0,1);
    lcd.print("Moderate");
  }
  else if ( Air_Quality==151||Air_Quality<=250){
    lcd.setCursor(0,1);
    lcd.print("Poor");
  }
  else if ( Air_Quality==251||Air_Quality<=399){
    lcd.setCursor(0,1);
    lcd.print("Unhealthy");
  }
  else if ( Air_Quality==400 || Air_Quality<=800){
    lcd.setCursor(0,1);
    lcd.print("Danger");
  }
  else{
    lcd.setCursor(0,1);
    lcd.print("toxic");
  }
  Blynk.virtualWrite(V3, Air_Quality);
  delay(5000);
  lcd.clear();
}
