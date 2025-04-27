#include <WebServer.h>
#include <WiFi.h>

const char *ssid     = ""; // Network Name
const char *password = ""; // Network Password

const int relayPin = 25;

WebServer server(80);

const char *htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Garage Control</title>
  <style>
    body { font-family: sans-serif; text-align: center; background: #f0f0f0; padding-top: 50px; }
    button { font-size: 24px; padding: 15px 30px; background-color: #4CAF50; color: white; border: none; border-radius: 10px; }
    button:hover { background-color: #45a049; }
  </style>
</head>
<body>
  <h1>Garage Door Control</h1>
  <button onclick="fetch('/open')">Open Garage</button>
</body>
</html>
)rawliteral";

void handleRoot() { server.send(200, "text/html", htmlPage); }

void handleOpen() {
	digitalWrite(relayPin, HIGH); // Activate relay (depends on relay logic)
	delay(500);		      // Simulate button press
	digitalWrite(relayPin, LOW);  // Deactivate relay
	server.send(200, "text/plain", "Garage door triggered");
}

void setup() {
	Serial.begin(115200);
	pinMode(relayPin, OUTPUT);
	digitalWrite(relayPin, LOW); // Relay off (depends on your module)

	WiFi.begin(ssid, password);
	Serial.print("Connecting to WiFi");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("\nConnected! IP address: ");
	Serial.println(WiFi.localIP());

	server.on("/", handleRoot);
	server.on("/open", handleOpen);

	server.begin();
	Serial.println("HTTP server started");
}

void loop() { server.handleClient(); }
