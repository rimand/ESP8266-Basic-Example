#include <ESP8266WiFi.h>
#include <Servo.h>

WiFiServer server(80);
Servo servo;

void setup() {
  Serial.begin(115200);
  WiFi.begin("true_home2G_9f4","8cebe9f4");
  Serial.println();
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());
  Serial.println("Start Server");
  server.begin();

  servo.attach(D2);
}

int val = 0;
int Oldval = 0;

void loop() {
  WiFiClient client = server.available();
  if(!client) return;
  Serial.println("new Client");

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  if(req.indexOf("data") != -1){
    int i = req.indexOf('='); 
    i++;
    int end = req.indexOf(" H");
    /*
    Serial.print("i : ");
    Serial.println(i);
    Serial.print("end : ");
    Serial.println(end);
    */
    String valS = req.substring(i,end);
    /*Serial.print("valS : ");
    Serial.println(valS);*/
    val = valS.toInt();
    Serial.print("val : ");
    Serial.println(val);
  }
  
    
  String web = "HTTP/1.1 200 OK/r/nContent-Type:text/html\r\n\r\n";
  web += "<HTML>\r\n";
  web += "<head>\r\n";
  web += "<title>\r\n";
  web += "ESP8266 Servo\r\n";
  web += "</title>\r\n";
  
  web += "<script type='text/javascript'>\r\n";
  web += "var x;\r\n";
  web += "function check(){\r\n";
  web += "x = document.getElementById('slider1').value;\r\n";
  web += "document.getElementById('demo').innerHTML = x;}\r\n";
  web += "function enter(){\r\n";
  web += "x = document.getElementById('slider1').value;\r\n";
  web += "document.getElementById('demo').innerHTML = x;\r\n";
  web += "document.getElementById('myForm').action = 'http://192.168.1.38/data=' + x;\r\n";
  web += "document.getElementById('myForm').submit();}\r\n";
  web += "</script>\r\n";
  
  web += "</head>\r\n";
  web += "<body>\r\n";
  web += "<H1>\r\n";
  web += "<center>\r\n";
  
  web += "ESP8266 SERVO <br><br>\r\n";
  
  web += "<input id='slider1' type='range' min='0' max='180' step='18'/><br><br>\r\n";
  web += "<form id='myForm' method='post'>\r\n";
  web += "<input type='button' name='btn' value='Check' onclick='check()'>\r\n";
  web += "<input type='button' name='btn' value='Enter' onclick='enter()'>\r\n";
  web += "</form>\r\n";
  web += "<p id= 'demo'>0</p>";
  web += "Servo Data : ";
  web += val;
  
  web += "</center>\r\n";
  web += "</H1>\r\n";
  web += "</bofy>\r\n";
  web += "</HTML>\r\n";

  client.print(web);

  if(Oldval != val) servo.write(val);
  Oldval = val;
}
