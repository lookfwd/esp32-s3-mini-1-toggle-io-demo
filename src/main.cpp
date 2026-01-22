/**
 * ESP32-S3-MINI-1 WiFi GPIO Control
 *
 * This program connects to WiFi and provides a web interface
 * to toggle GPIO pins IO14 and IO33
 */

#include <WiFi.h>
#include <WebServer.h>
#include "config.h"

// GPIO pins to control
#define GPIO_PIN_14 14
#define GPIO_PIN_33 33

// Web server on port 80
WebServer server(80);

// GPIO states
bool gpio14_state = false;
bool gpio33_state = false;

// HTML page
const char* html_page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32-S3 GPIO Control</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            max-width: 600px;
            margin: 50px auto;
            padding: 20px;
            background-color: #f0f0f0;
        }
        .container {
            background-color: white;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }
        h1 {
            color: #333;
            text-align: center;
        }
        .gpio-control {
            margin: 20px 0;
            padding: 20px;
            background-color: #f9f9f9;
            border-radius: 5px;
            border-left: 4px solid #4CAF50;
        }
        .gpio-control h2 {
            margin-top: 0;
            color: #555;
        }
        button {
            padding: 15px 30px;
            font-size: 16px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            transition: all 0.3s;
            width: 100%;
            margin-top: 10px;
        }
        .btn-on {
            background-color: #4CAF50;
            color: white;
        }
        .btn-on:hover {
            background-color: #45a049;
        }
        .btn-off {
            background-color: #f44336;
            color: white;
        }
        .btn-off:hover {
            background-color: #da190b;
        }
        .status {
            display: inline-block;
            padding: 5px 15px;
            border-radius: 3px;
            font-weight: bold;
            margin-top: 10px;
        }
        .status-on {
            background-color: #4CAF50;
            color: white;
        }
        .status-off {
            background-color: #f44336;
            color: white;
        }
        .info {
            text-align: center;
            color: #666;
            margin-top: 20px;
            font-size: 14px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🔌 ESP32-S3 GPIO Control</h1>

        <div class="gpio-control">
            <h2>GPIO 14</h2>
            <p>Current State: <span class="status" id="status14">Loading...</span></p>
            <button class="btn-on" onclick="toggleGPIO(14, true)">Turn ON</button>
            <button class="btn-off" onclick="toggleGPIO(14, false)">Turn OFF</button>
        </div>

        <div class="gpio-control">
            <h2>GPIO 33</h2>
            <p>Current State: <span class="status" id="status33">Loading...</span></p>
            <button class="btn-on" onclick="toggleGPIO(33, true)">Turn ON</button>
            <button class="btn-off" onclick="toggleGPIO(33, false)">Turn OFF</button>
        </div>

        <div class="info">
            <p>ESP32-S3-MINI-1 GPIO Controller</p>
            <p id="ip-info"></p>
        </div>
    </div>

    <script>
        // Update GPIO state
        function toggleGPIO(pin, state) {
            fetch('/gpio?pin=' + pin + '&state=' + (state ? '1' : '0'))
                .then(response => response.json())
                .then(data => {
                    if (data.success) {
                        updateStatus();
                    }
                })
                .catch(error => console.error('Error:', error));
        }

        // Get current status
        function updateStatus() {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    // Update GPIO 14 status
                    const status14 = document.getElementById('status14');
                    status14.textContent = data.gpio14 ? 'ON' : 'OFF';
                    status14.className = 'status ' + (data.gpio14 ? 'status-on' : 'status-off');

                    // Update GPIO 33 status
                    const status33 = document.getElementById('status33');
                    status33.textContent = data.gpio33 ? 'ON' : 'OFF';
                    status33.className = 'status ' + (data.gpio33 ? 'status-on' : 'status-off');
                })
                .catch(error => console.error('Error:', error));
        }

        // Update status on page load and every 2 seconds
        updateStatus();
        setInterval(updateStatus, 2000);

        // Display IP address
        fetch('/ip')
            .then(response => response.text())
            .then(ip => {
                document.getElementById('ip-info').textContent = 'Device IP: ' + ip;
            });
    </script>
</body>
</html>
)rawliteral";

// Handle root request
void handleRoot() {
    server.send(200, "text/html", html_page);
}

// Handle GPIO control request
void handleGPIO() {
    if (server.hasArg("pin") && server.hasArg("state")) {
        int pin = server.arg("pin").toInt();
        int state = server.arg("state").toInt();

        if (pin == GPIO_PIN_14) {
            gpio14_state = (state == 1);
            digitalWrite(GPIO_PIN_14, gpio14_state ? HIGH : LOW);
            server.send(200, "application/json", "{\"success\":true,\"pin\":14,\"state\":" + String(gpio14_state) + "}");
        } else if (pin == GPIO_PIN_33) {
            gpio33_state = (state == 1);
            digitalWrite(GPIO_PIN_33, gpio33_state ? HIGH : LOW);
            server.send(200, "application/json", "{\"success\":true,\"pin\":33,\"state\":" + String(gpio33_state) + "}");
        } else {
            server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid pin\"}");
        }
    } else {
        server.send(400, "application/json", "{\"success\":false,\"error\":\"Missing parameters\"}");
    }
}

// Handle status request
void handleStatus() {
    String json = "{\"gpio14\":" + String(gpio14_state ? "true" : "false") +
                  ",\"gpio33\":" + String(gpio33_state ? "true" : "false") + "}";
    server.send(200, "application/json", json);
}

// Handle IP request
void handleIP() {
    server.send(200, "text/plain", WiFi.localIP().toString());
}

// Handle 404
void handleNotFound() {
    server.send(404, "text/plain", "404: Not Found");
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    delay(100);

    Serial.println("\n\n=================================");
    Serial.println("ESP32-S3-MINI-1 GPIO Control");
    Serial.println("=================================\n");

    // Initialize GPIO pins
    pinMode(GPIO_PIN_14, OUTPUT);
    pinMode(GPIO_PIN_33, OUTPUT);
    digitalWrite(GPIO_PIN_14, LOW);
    digitalWrite(GPIO_PIN_33, LOW);

    Serial.println("GPIO pins initialized:");
    Serial.println("  - GPIO 14: OUTPUT (OFF)");
    Serial.println("  - GPIO 33: OUTPUT (OFF)\n");

    // Connect to WiFi
    Serial.print("Connecting to WiFi SSID: ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n\nWiFi connected successfully!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Signal Strength (RSSI): ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm\n");
    } else {
        Serial.println("\n\nFailed to connect to WiFi!");
        Serial.println("Please check your credentials in config.h");
        Serial.println("Restarting in 5 seconds...\n");
        delay(5000);
        ESP.restart();
    }

    // Setup web server routes
    server.on("/", handleRoot);
    server.on("/gpio", handleGPIO);
    server.on("/status", handleStatus);
    server.on("/ip", handleIP);
    server.onNotFound(handleNotFound);

    // Start web server
    server.begin();
    Serial.println("Web server started!");
    Serial.println("=================================");
    Serial.print("Access the control panel at: http://");
    Serial.println(WiFi.localIP());
    Serial.println("=================================\n");
}

void loop() {
    // Handle client requests
    server.handleClient();
}
