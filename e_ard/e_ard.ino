#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>                                                // firebase library
#include <DHT.h>                                                            // dht11 temperature and humidity sensor library

#define FIREBASE_HOST "e-irigation.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "cFVsCHZfOsK24RilpI9cdr84WxN6rMWWVi6ibcBV"            // the secret key generated from firebase

#define WIFI_SSID "network_name"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "password"                                    //password of wifi ssid

#define MOIST A0                                                           //  Analog pin connected to water level sensor 
#define DHTPIN D4                                                           // digital pin connected to DHT sensor
#define DHTTYPE DHT11                                                       // select dht type as DHT 11 or DHT22
DHT dht(DHTPIN, DHTTYPE);                                                     

void setup() {
  Serial.begin(9600);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to "); 
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
  dht.begin();                                                               //Start reading dht sensor
}

void loop() { 
  float m = analogRead(MOIST)
  float h = dht.readHumidity();                                              // Reading temperature or humidity takes about 250 milliseconds!
  float t = dht.readTemperature();                                           // Read temperature as Celsius (the default)
    
  if (isnan(h) || isnan(t)) {                                                // Check if any reads failed and exit early (to try again).
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("Water Level: "); Serial.print(m);
  String SoilMoist= String(m) + String("cm");
  Serial.print("Humidity: ");  Serial.print(h);
  String Humid = String(h) + String("%");                                         //convert integer humidity to string humidity 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("Â°C ");
  String Temp = String(t) + String("Â°C");                                                     //convert integer temperature to string temperature
  delay(4000);

  Firebase.pushString("/Sensor/Moisture", SoilMoist);                               // setup path and push water level value to database 
  Firebase.pushString("/Sensor/Humidity", Humid);                                  //setup path and push Humidity value to database
  Firebase.pushString("/Sensor/Temperature",Temp);                                //setup path and push Temperature value to database
   
}
