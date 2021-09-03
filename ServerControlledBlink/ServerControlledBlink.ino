#include<WiFi.h>
#include<WebServer.h>

/* SSID and Password  */
const char* ssid = "ESP32";
const char* password = "123456";

/*  IP Address Details  */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

    /* webserver port */
WebServer server(80);  

    /*  LED1  */
// 8-bit unsigned integer 
uint8_t LED1pin = 22; 
bool LED1status = LOW; 

uint8_t delays = 0;

void setup(){
  Serial.begin(9600);
  pinMode(LED1pin, OUTPUT);

    // set ssid and password for the softAP wifi
  WiFi.softAP(ssid, password); 
    // IP address setup
  WiFi.softAPConfig(local_ip, gateway, subnet); 
  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led1on1", handle_led1on1);
  server.on("/led1on2", handle_led1on2);

 
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(){
  server.handleClient();
  if(LED1status)
  {
    if(delays){
        digitalWrite(LED1pin, HIGH);
        delay(delays);

        digitalWrite(LED1pin, LOW);
        delay(delays);
    }
    else{
      digitalWrite(LED1pin, HIGH);
    }    
  }
  else{
    digitalWrite(LED1pin, LOW);
  }

}

  /*  defined functions */

  void handle_OnConnect(){
    LED1status = LOW;
    delays = 0;
    Serial.println("Pin Status are LOW");
    server.send(200, "text/html", SendHTML(LED1status, delays));
  }

  void handle_led1on(){
    LED1status = HIGH;
    delays = 0;
    Serial.println("Pin 18 Status: ON");
    server.send(200, "text/html", SendHTML(true, delays));
  }
  void handle_led1off(){
    LED1status = LOW;
    delays = 0;
    Serial.println("Pin 18 Status: ON");
    server.send(200, "text/html", SendHTML(false, delays));
  }

  void handle_led1on1(){
    LED1status = HIGH;
    delays = 200;
    Serial.println("Pin 18 Status: ON");
    server.send(200, "text/html", SendHTML(true, delays));
  }
  

  void handle_led1on2(){
    LED1status = HIGH;
    delays = 80;
    Serial.println("Pin 18 Status: ON");
    server.send(200, "text/html", SendHTML(true, delays));
  }

  void handle_NotFound(){
    server.send(404, "text/plain","Not Found");
  }

      // Sending the html file as a string
  
  String SendHTML(uint8_t led1stat,uint8_t rate){
    String ptr = "<!DOCTYPE html> <html>\n";
    ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr +="<title>LED Control</title>\n";
    ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n.container{dispalay:grid; place-items:center;width:100vw;}";
    ptr +=".led-head{text-decoration:underline;line-space:2px;font-size:40px;}\n";
    ptr +=".button{height: 20px;width: 60px;background-color: grey;padding: 10px;border-radius: 5px;margin-left:10px;color: white;font-weight:bold;}\n.button-switch{margin-bottom:20px;background-color:#cc0000;}\n.button-switch-on{margin-bottom:20px;background-color:green;}\n.button-div{margin-top: 20px;position:relative;width:100vw;height:auto;display:flex;text-align:center;margin-left:40%;}\n.button-on{background-color:green;}\n";
    ptr +="</style>\n";
    ptr +="</head>\n";
    ptr +="<body>\n";
    ptr +="<div class=\"container\">\n";
    ptr +="<h1>ESP32 Web Server</h1>\n";
    ptr +="<h1>Set the blinking Rate</h1>\n";
    
  
   if(led1stat)
    {
      if(rate == 200){
        ptr +=" <p class=\"led-head\">Led at rate-1</p>\n";
        ptr +="<a class=\"button button-switch-on\" href=\"/led1off\">ON</a>\n";
        ptr +="<div class=\"button-div\">\n";
        ptr +="<a class=\"button button-on\" href=\"/led1off\">1</a>\n";
        ptr +="<a class=\"button button-off\" href=\"/led1on2\">2</a>\n";
        ptr +="</div>\n";        
        }
      else if(rate == 80){
        ptr +=" <p class=\"led-head\">Led at rate-2</p>\n";
        ptr +="<a class=\"button button-switch-on\" href=\"/led1off\">ON</a>\n";
        ptr +="<div class=\"button-div\">\n";
        ptr +="<a class=\"button button-off\" href=\"/led1on1\">1</a>\n";
        ptr +="<a class=\"button button-on\" href=\"/led1off\">2</a>\n";
        ptr +="</div>\n";       
       }
      else{
          ptr +=" <p class=\"led-head\" >Led ON</p>\n";
          ptr +="<a class=\"button button-switch-on\" href=\"/led1off\">ON</a>\n";
          ptr +="<div class=\"button-div\" >\n";
          ptr +="<a class=\"button button-off\" href=\"/led1on1\">1</a>\n";
          ptr +="<a class=\"button button-off\" href=\"/led1on2\">2</a>\n";
          ptr +="</div>\n";    
      }
     }
  else
    {
      ptr +=" <p class=\"led-head\">Led is off</p>\n";
      ptr +="<a class=\"button button-switch\" href=\"/led1on\">OFF</a>\n";
      ptr +="<div class=\"button-div\">\n";
      ptr +="<a class=\"button button-off\" >1</a>\n";
      ptr +="<a class=\"button button-off\">2</a>\n";
      ptr +="</div>\n";    
     }
    ptr +="</div>\n";
    ptr +="</body>\n";
    ptr +="</html>\n";
    return ptr;
  }
  
