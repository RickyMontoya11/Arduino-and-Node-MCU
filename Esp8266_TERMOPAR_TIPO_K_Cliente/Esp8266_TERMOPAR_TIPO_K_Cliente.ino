#include <max6675.h>
#include <ESP8266WiFi.h>

int thermoDO = D0;
int thermoCS = D1;
int thermoCLK = D2;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

const char* ssid = "LIA";
const char* password = "2G5-K0W-L4B5";
const char* host = "192.168.0.115";

void setup(){

  Serial.begin(9600);
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

}


void loop()
{
  float cel;
  cel = (thermocouple.readCelsius());
  Serial.print("Celsius: ");
  Serial.println(cel);

  float fah;
  fah = (thermocouple.readFahrenheit());
  Serial.print("Fahrenheit: ");
  Serial.println(fah);

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URL for the request
  String url = "/termopar1.php";
  String key = "?pass=LIA123456";
  String dato1 = "&Celsius=";
  String dato2 = "&Fahrenheit=";
  

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + key + dato1 + cel + dato2 + fah + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

  delay(60000);
 
}
