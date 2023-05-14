#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Connect";
const char* password = "password123";

ESP8266WebServer server(80);

String html_template = R"(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Wi-Fi Networks</title>
    <style>
      body {
        font-family: Arial, Helvetica, sans-serif;
        margin: 0;
        padding: 0;
      }
      .header {
        background-color: #333;
        color: #fff;
        padding: 20px;
        text-align: center;
      }
      .table {
        border-collapse: collapse;
        margin: auto;
        width: 90%;
      }
      .table th, .table td {
        padding: 12px;
        text-align: left;
      }
      .table th {
        background-color: #333;
        color: #fff;
      }
      .table tr:nth-child(even) {
        background-color: #f2f2f2;
      }
      .container {
        margin: auto;
        padding: 20px;
        text-align: center;
        width: 90%;
      }
    </style>
  </head>
  <body>
    <div class="header">
      <h1>Wi-Fi Networks:</h1>
    </div>
    <div class="container">
      <table class="table">
        <tr>
          <th>SSID</th>
          <th>Signal Strength</th>
        </tr>
        %SSID_LIST%
      </table>
    </div>
  </body>
</html>
)";

void handleRoot() {
  String ssidList = "";
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    ssidList += "<tr>";
    ssidList += "<td>";
    ssidList += WiFi.SSID(i);
    ssidList += "</td>";
    ssidList += "<td>";
    ssidList += WiFi.RSSI(i);
    ssidList += " dBm</td>";
    ssidList += "</tr>";
  }
  String html = html_template;
  html.replace("%SSID_LIST%", ssidList);
  server.send(200, "text/html", html);
}

void handleNotFound() {
  String message = "404 Not Found\n\n";
  message += "The requested URL was not found on this server.";
  server.send(404, "text/plain", message);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip);
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
