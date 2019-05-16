#include <DHT.h>

#define DHTPIN A2     // what pin we're connected the DHT output
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 48);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  dht.begin();  
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<style>");
          client.println("table, th, td {");
          client.println("  border: 1px solid black;");
          client.println("  border-collapse: collapse;");
          client.println("}");
          client.println("</style>");
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          client.println("<title>DHT11 WebServer</title>");
          client.println("<body>");
          client.println("<H3>DHT webserver</H3>");
          client.println("<br />");
          client.println("<table style='width:50%'>");
          client.println("<tr>");
          client.println("<th>Temperature</th>");
          client.println("<th>Humidity</th>");
          client.println("</tr>");
          client.println("<tr>");
          client.println("<td>");
          client.print(t);
          client.print("</td>");
          client.println("<td>");
          client.print(h);
          client.print("</td>");
          client.println("</table>");
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}
