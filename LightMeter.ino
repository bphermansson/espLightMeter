/*
* Measure light level with an IR-transistor and send the value to a webserver. Here the value is sent to an Emoncms 
* installation but that can be altered.
 *
 *©Patrik Hermansson 2015
 */

#include <ESP8266WiFi.h>

const char* ssid = "dd-wrt";
const char* password = "your-password";  // Wifi password, if any

// Values for Emoncms
const char* host = "192.168.1.3";
String apikey = "b0660d27cc88ee1a916b7bbb4b9f8baf";
String nodeid = "24";
String url = "/emoncms/input/post.json?apikey="+apikey+"&node="+nodeid+"&csv=";

// Remote Emoncms 
const char*  remotedomain="emoncms.org";
String remoteemoncmspath = "emoncms";
String remoteapikey = "bdd96271de3ff345c087afa276d1a619";
String remotenodeid = "24";
String remoteurl = "/input/post.json?node="+nodeid+"&csv=";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup Gpio 2
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW); 
}

void loop() {

  // Set output high
  Serial.println("Gpio 2 high");
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  // Wait for capacitor to charge
  delay(500);          
  // Set Gpio2 to input
  digitalWrite(2, LOW);
  pinMode(2, INPUT);

  // Wait while capacitor discharges and count how long it takes
  unsigned long startTime = micros();
  // Measure discharge time and use a timeout. If its really dark, we get stuck here otherwise. 
  while (digitalRead(2) == HIGH && micros()-startTime < 1000000) {
  }
  unsigned int time = micros() - startTime;
  // Print results
  Serial.print ("Light level: ");
  Serial.println(time);

  // Add count to url
  String newurl="";
  newurl=url + String(time);
  String newremoteurl="";
  newremoteurl=remoteurl + String(time) + "&apikey="+remoteapikey;

  // Connect to host
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print("Requesting URL: ");
  Serial.println(newurl);
  
  // This will send the request to the server
  client.print(String("GET ") + newurl + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(100);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");

  // Connect to remote Emoncms
    // Connect to host
  Serial.print("connecting to ");
  Serial.println(remotedomain);
  
  if (!client.connect(remotedomain, httpPort)) {
    Serial.println("Remote connection failed");
    return;
  }
  
  Serial.print("Requesting URL: ");
  Serial.println(newremoteurl);
  
  // This will send the request to the server
  client.print(String("GET ") + newremoteurl + " HTTP/1.1\r\n" +
               "Host: " + remotedomain + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(100);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");

  // Wait x minutes to midnight (or to next measurement). Here we could/should go into deep sleep...
  Serial.println("Wait...");
  delay(600000);

}

