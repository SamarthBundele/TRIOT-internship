#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif

#define LED1 D5

const char* ssid = "TRIOT"; // Replace with your preferred AP name
const char* password = ""; // Replace with your password (optional)

const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<title>Samarth</title>
<center>
<h1>TASK 3 TRIOT</h1>
<h3> LED 1 </h3>
<button id="led1OnBtn">On</button>
<button id="led1OffBtn">Off</button>
<span id="led1Status"></span>
</center>
<script>
  document.getElementById("led1OnBtn").addEventListener("click", function() {
    fetch("/led1/on").then(response => {
      if (response.ok) {
        return response.text();
      } else {
        throw new Error("LED control failed");
      }
    }).then(data => {
      document.getElementById("led1Status").textContent = "LED1: On";
    }).catch(error => {
      console.error(error);
      document.getElementById("led1Status").textContent = "Error turning LED1 on";
    });
  });

  document.getElementById("led1OffBtn").addEventListener("click", function() {
    fetch("/led1/off").then(response => {
      if (response.ok) {
        return response.text();
      } else {
        throw new Error("LED control failed");
      }
    }).then(data => {
      document.getElementById("led1Status").textContent = "LED1: Off";
    }).catch(error => {
      console.error(error);
      document.getElementById("led1Status").textContent = "Error turning LED1 off";
    });
  });
</script>
</body>
</html>
)=====";

#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Page Not found");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.println("AP started");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());

  if (!MDNS.begin("esp")) {
    Serial.println("Error starting mDNS responder");
  } else {
    Serial.println("mDNS responder started");
  }

  server.on("/", [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", webpage);
  });

  server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(LED1, HIGH);
    request->send_P(200, "text/html", webpage);
  });

  server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(LED1, LOW);
    request->send_P(200, "text/html", webpage);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // You can add code here to monitor the LED state or perform other tasks (optional)
}
