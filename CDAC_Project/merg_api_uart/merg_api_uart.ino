#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include <WiFiClientSecure.h>

//// WiFi credentials
//const char* ssid = "black berry";
//const char* password = "12345678901";
//--------------------------------------------------------------------------------------------
const char* ssid = "black berry";
const char* password = "12345678901";
const char* apiKey = "jU46NUgu3RHY";
const char* templateID = "110";
const char* mobileNumber = "919284229341";
const char* var1 = "GPS TRACKER";
char* var2 ;
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

//------------------------------------------------------------------------------------------------------


//Serial monitor usart recevier
#define RX_PIN D1  // Connect this to the TX pin of STM32
#define TX_PIN D2  // Connect this to the RX pin of STM32
SoftwareSerial mySerial(RX_PIN, TX_PIN);


void setup() {
  // Start Serial Monitor
  Serial.begin(9600);
  Serial.println();

  
  mySerial.begin(9600); //Set baud rate match with STM32 UART settings
  //digitalWrite(D0, LOW);
  Serial.println("Setup completed.");

  WiFi.begin(ssid,password);
  Serial.print("Connecting to wifi");
  while(WiFi.status() != WL_CONNECTED){
  delay(500);
  Serial.print(".");
  }
  Serial.println("\nConnected!");
 // sendSMS();             // add condition if data available then only send sms

 }
//void loop() {
// // put your main code here, to run repeatedly:
//}

//// Define 
//String str = "This is my string"; 
//
//// Length (with one extra character for the null terminator)
//int str_len = str.length() + 1; 
//
//// Prepare the character array (the buffer) 
//char char_array[str_len];
//
//// Copy it over 
//str.toCharArray(char_array, str_len);


void loop() {

   if (mySerial.available() > 0) {                                        //check data on serail monitor from stm32
    String  receivedData = mySerial.readStringUntil('\r');
    
    int len=receivedData.length()+1;

    char char_array[len];

    receivedData.toCharArray(char_array,len);
    var2 = char_array;
    Serial.println(char_array);
    
    sendSMS();
     
    //receivedData.trim(); // Remove any extra whitespace 
    Serial.print("Message from STM32: ");
    Serial.println(receivedData); // Print the received message to Serial Monitor
    
    Serial.print("Length of the received string: ");
    int length = receivedData.length();
    Serial.println(length);
    delay(1000);

   }


}
