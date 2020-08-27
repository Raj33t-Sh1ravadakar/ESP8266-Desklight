#include <ESP8266WiFi.h>
 


const char* ssid = "SINGTEL-XXXX(2.4G)"; //change this to your wifi ssid make sure its 2.4ghz band as esp8266 only supports 2.4ghz
const char* password = "Pass1234";//change this to your wifi password

int ledPin = D2;// connect gnd of led to this 

WiFiServer server(80);
IPAddress ip(192, 168, 1, 222); // this is the static IP address
IPAddress gateway(192, 168, 1, 254); // set gateway to match your network
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  Serial.print(F("Setting static ip to : "));
  Serial.println(ip);
 

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
 
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 

  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, LOW);
    value = LOW;
  } 
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
 

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Table Light is now: ");
 
  if(value == HIGH) {
    client.print("off");  
  } else {
    client.print("on");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn Table Light ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn Table Light OFF<br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
 
}
