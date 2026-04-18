#include <WiFi.h>
#include <WebServer.h>

#define PIR_PIN 13
#define LDR_PIN 34
#define RELAY_PIN 26

const char* ssid = "Vaibhavi";        
const char* password = "16271116"; 

WebServer server(80);


int threshold = 2000;
unsigned long lastMotionTime = 0;
unsigned long delayTime = 10000;


void handleRoot() {
  int ldr = analogRead(LDR_PIN);
  int motion = digitalRead(PIR_PIN);

  String page = "<html><body style='text-align:center;'>";
  page += "<h2>Smart Light IoT</h2>";

  page += "<p>LDR Value: " + String(ldr) + "</p>";
  page += "<p>Motion: " + String(motion) + "</p>";

  page += "<a href='/on'><button>Turn ON</button></a><br><br>";
  page += "<a href='/off'><button>Turn OFF</button></a>";

  page += "</body></html>";

  server.send(200, "text/html", page);
}

void handleOn() {
  digitalWrite(RELAY_PIN, LOW);
  server.send(200, "text/html", "<h3>Light ON</h3><a href='/'>Back</a>");
}

void handleOff() {
  digitalWrite(RELAY_PIN, HIGH);
  server.send(200, "text/html", "<h3>Light OFF</h3><a href='/'>Back</a>");
}

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH); 


  WiFi.begin(ssid, password);

  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected ✅");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();
}

void loop() {
  server.handleClient();

  
  int ldrValue = analogRead(LDR_PIN);
  int motion = digitalRead(PIR_PIN);

  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print(" | Motion: ");
  Serial.println(motion);

  
  if (ldrValue < threshold) {
    if (motion == HIGH) {
      digitalWrite(RELAY_PIN, LOW); 
      lastMotionTime = millis();
    }

    if (millis() - lastMotionTime > delayTime) {
      digitalWrite(RELAY_PIN, HIGH); 
    }
  } else {
    digitalWrite(RELAY_PIN, HIGH); 
  }

  delay(200);
}