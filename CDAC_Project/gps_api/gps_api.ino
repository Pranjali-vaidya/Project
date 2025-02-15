#include <TinyGPS++.h>
#include<TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
/* Create object named bt of the class SoftwareSerial */
SoftwareSerial GPS_SoftSerial(4, 3);/* (Rx, Tx) */
/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps;              
int visited=0;
volatile float minutes, seconds;
volatile int degree, secs, mins;
///---------------------------------------------------------------
const char* ssid = "Crown";
const char* password = "matf5168";
const char* apiKey ="jU46NUgu3RHY";                 //"ZBcixNdSbKbU";          
const char* templateID = "110";
const char* mobileNumber = "919284229341";     //"918055992369";            
const char* var1 = "GPS TRACKER";
 char* var2 ;         //= "2000N and 4000W";
 static void fun1();
void sendSMS() {
 if (WiFi.status() == WL_CONNECTED) {
   WiFiClientSecure client; // Use WiFiClientSecure for HTTPS connections
   client.setInsecure();    // Skip certificate validation (not secure but works for development)
   HTTPClient http;
   // Build the API URL with the template ID
   String apiUrl = "https://www.circuitdigest.cloud/send_sms?ID=" + String(templateID);
   // Start the HTTPS connection with WiFiClientSecure
   http.begin(client, apiUrl);
   http.addHeader("Authorization", apiKey);
   http.addHeader("Content-Type", "application/json");
   // Create the JSON payload with SMS details
   String payload = "{\"mobiles\":\"" + String(mobileNumber) + "\",\"var1\":\"" + String(var1) + "\",\"var2\":\"" + String(var2) + "\"}";
   // Send POST request
   int httpResponseCode = http.POST(payload);
   // Check response
   if (httpResponseCode == 200) {
     Serial.println("SMS sent successfully!");
     Serial.println(http.getString());
   } else {
     Serial.print("Failed to send SMS. Error code: ");
     Serial.println(httpResponseCode);
     Serial.println("Response: " + http.getString());
   }
   http.end(); // End connection
 } else {
   Serial.println("WiFi not connected!");
 }
}
//---------------------------------------------------------------------------


const int buttonPin = 15; // Assign pin 2 to read button state

int buttonState; // Variable to store button state


//------------------------------------
void setup() {
 Serial.begin(9600);
 WiFi.begin(ssid,password);
 
 Serial.print("Connecting to wifi");
 while(WiFi.status() != WL_CONNECTED){
   delay(500);
   Serial.print(".");
 }
 Serial.println("\nConnected!");
  
 //sendSMS();
}
//------------------------------------
//void setup() {
// Serial.begin(9600);   /* Define baud rate for serial communication */
// GPS_SoftSerial.begin(9600); /* Define baud rate for software serial communication */
//}
//String total2 = Double.toString(total);
 double lat_val, lng_val, alt_m_val;
void loop() {
        buttonState = digitalRead(buttonPin);
       smartDelay(1000);     /* Generate precise delay of 1ms */
        unsigned long start;
       
        uint8_t hr_val, min_val, sec_val;
        bool loc_valid, alt_valid, time_valid;
       lat_val = gps.location.lat();     /* Get latitude data */
       loc_valid = gps.location.isValid();     /* Check if valid location data is available */
       lng_val = gps.location.lng();     /* Get longtitude data */
       alt_m_val = gps.altitude.meters();      /* Get altitude data in meters */
        alt_valid = gps.altitude.isValid();     /* Check if valid altitude data is available */
       hr_val = gps.time.hour();   /* Get hour */
       min_val = gps.time.minute();      /* Get minutes */
       sec_val = gps.time.second();      /* Get seconds */
       time_valid = gps.time.isValid();  /* Check if valid time data is available */
        if (!loc_valid)
       {         
         Serial.print("Latitude : ");
         Serial.println("*****");
         Serial.print("Longitude : ");
         Serial.println("*****");
        }
        else
        {
         DegMinSec(lat_val);
         Serial.print("Latitude in Decimal Degrees : ");
         Serial.println(lat_val, 6);
         Serial.print("Latitude in Degrees Minutes Seconds : ");
         Serial.print(degree);
         Serial.print("\t");
         Serial.print(mins);
         Serial.print("\t");
         Serial.println(secs);
         DegMinSec(lng_val); /* Convert the decimal degree value into degrees minutes seconds form */
         Serial.print("Longitude in Decimal Degrees : ");
         Serial.println(lng_val, 6);
         //------
            if(buttonState == HIGH && visited==0)
            {
              fun1();
            }
         //------
         Serial.print("Longitude in Degrees Minutes Seconds : ");
         Serial.print(degree);
         Serial.print("\t");
         Serial.print(mins);
         Serial.print("\t");
         Serial.println(secs);
        }
        if (!alt_valid)
        {
         Serial.print("Altitude : ");
         Serial.println("*****");
        }
        else
        {
         Serial.print("Altitude : ");
         Serial.println(alt_m_val, 6);   
        }
        if (!time_valid)
        {
         Serial.print("Time : ");
         Serial.println("*****");
        }
        else
        {
         char time_string[32];
         sprintf(time_string, "Time : %02d/%02d/%02d \n", hr_val, min_val, sec_val);
         Serial.print(time_string);   
        }
}
static void fun1()
{
  sprintf(var2,"%lf N , %lf E",lat_val,lng_val );
  sendSMS();
  
}
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())    /* Encode data read from GPS while data is available on serial port */
     gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)           /* Convert data in decimal degrees into degrees minutes seconds form */
{ 
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}
