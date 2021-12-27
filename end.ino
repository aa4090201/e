#include "WiFi.h"
#include "WebServer.h"

#include "DHT.h"  //引入函式庫
#define DHT11PIN 27 //溫溼度

DHT dht(DHT11PIN, DHT11); 

// SSID & Password
const char* ssid = "0";
const char* password = "yohaneNO1";

#define PIN_RED  13 //紅燈腳
#define PIN_GREEN  12 //綠燈腳
#define PIN_BLUE  14  //藍燈腳
int L_RED;  //紅燈rgb值
int L_GREEN;  //綠燈rgb值
int L_BLUE; //藍燈rgb值
int L_RED_map;  //紅燈用亮度轉換
int L_GREEN_map;  //綠燈用亮度轉換
int L_BLUE_map;   //藍燈用亮度轉換
int ledstate=0;   //led是否開啟
int rgbstate=1;   //led是否自動決定顏色
int lightstate=1; //led是否自動決定亮度
int input =0 ;    //光敏電阻輸入值
int input_map  =0;  //轉換光敏電阻輸入值
int triggerPin = 26; //用來trigger
int echoPin = 25; //用來接收echo
long duration, cm, inches;
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

void setup() {
  Serial.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  //建立server的路徑
  createWebServer();
  
  //server啟動
  server.begin();
  Serial.println("HTTP server started");
  ledcSetup(0, 5000, 8);  
  ledcAttachPin(PIN_RED, 0);  //紅燈用頻道
   ledcSetup(1, 5000, 8);
  ledcAttachPin(PIN_GREEN, 1);
   ledcSetup(2, 5000, 8);     //綠燈用頻道
  ledcAttachPin(PIN_BLUE, 2);  //藍燈用頻道
  dht.begin();
   pinMode(33, INPUT);  //光敏電阻
   pinMode(32, INPUT);
   pinMode(triggerPin, OUTPUT); //發送超音波
   pinMode(echoPin, INPUT);  //讀取超音波

}

void loop() {
  server.handleClient();  //handleClient要寫在loop
  float humi = dht.readHumidity();  //DHT讀取濕度
  float temp = dht.readTemperature(); //DHT讀取溫度
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("ºC ");
  Serial.print("Humidity: ");
  Serial.println(humi);
  Serial.println(input);
  Serial.println(input_map);
   Serial.println(cm);
  
   
   //發送超音波
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);  // 給 Trig 高電位，持續 10微秒
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  

  //接收超音波
  duration = pulseIn(echoPin, HIGH);   // 收到高電位時的時間

  //將回傳時間換算成距離 cm 或 inch 
  cm = (duration/2) / 29.1;   
 
  if(cm <10 ){  //當距離小於10公分時開啟led
    ledstate= 1;
  }
  if(ledstate== 0){ //關閉led
    L_BLUE=0;
    L_RED=0;
    L_GREEN=0;
    ledcWrite(0, L_RED);    
    ledcWrite(1, L_GREEN);
    ledcWrite(2, L_BLUE);
  }
  if(ledstate== 1){ //開啟led
    L_RED_map = map(input_map, 0, 255, 0, L_RED);
    ledcWrite(0, L_RED-L_RED_map); 
    L_GREEN_map = map(input_map, 0, 255, 0, L_GREEN);
    ledcWrite(1, L_GREEN-L_GREEN_map);
    L_BLUE_map = map(input_map, 0, 255, 0, L_BLUE);
    ledcWrite(2, L_BLUE-L_BLUE_map);
  if(rgbstate == 1){  //開啟led自動跟隨溫度改變顏色
  if(temp>26){  //大於26度led藍紅黃
    if(lightstate == 1){ //led自動隨光敏電阻改變亮度
    input = analogRead(33);
    input_map = map(input, 0, 300, 0, 255);}
    L_BLUE=255; 
    L_RED=0;
    L_GREEN=0;
    L_RED_map = map(input_map, 0, 255, 0, L_RED);
    ledcWrite(0, L_RED-L_RED_map); 
    L_GREEN_map = map(input_map, 0, 255, 0, L_GREEN);
    ledcWrite(1, L_GREEN-L_GREEN_map);
    L_BLUE_map = map(input_map, 0, 255, 0, L_BLUE);
    ledcWrite(2, L_BLUE-L_BLUE_map);
    delay(1800);
    if(lightstate == 1){ //led自動隨光敏電阻改變亮度
    input = analogRead(33);
    input_map = map(input, 0, 300, 0, 255);}
    L_RED=255;
    L_GREEN=0;
    L_BLUE=0;
    L_RED_map = map(input_map, 0, 255, 0, L_RED);
    ledcWrite(0, L_RED-L_RED_map); 
    L_GREEN_map = map(input_map, 0, 255, 0, L_GREEN);
    ledcWrite(1, L_GREEN-L_GREEN_map);
    L_BLUE_map = map(input_map, 0, 255, 0, L_BLUE);
    ledcWrite(2, L_BLUE-L_BLUE_map);
    delay(1800);
    if(lightstate == 1){ //led自動隨光敏電阻改變亮度
    input = analogRead(33);
    input_map = map(input, 0, 300, 0, 255);}
    L_RED=255;
    L_GREEN=255;
    L_BLUE=0;
    L_RED_map = map(input_map, 0, 255, 0, L_RED);
    ledcWrite(0, L_RED-L_RED_map); 
    L_GREEN_map = map(input_map, 0, 255, 0, L_GREEN);
    ledcWrite(1, L_GREEN-L_GREEN_map);
    L_BLUE_map = map(input_map, 0, 255, 0, L_BLUE);
    ledcWrite(2, L_BLUE-L_BLUE_map);
    delay(1800);}
    if(temp<=26){  //小於26度led紫粉綠
    if(lightstate == 1){ //led自動隨光敏電阻改變亮度
    input = analogRead(33);
    input_map = map(input, 0, 300, 0, 255);}
    L_RED=138;
    L_GREEN=43;
    L_BLUE=226;
    L_RED_map = map(input_map, 0, 255, 0, L_RED);
    ledcWrite(0, L_RED-L_RED_map); 
    L_GREEN_map = map(input_map, 0, 255, 0, L_GREEN);
    ledcWrite(1, L_GREEN-L_GREEN_map);
    L_BLUE_map = map(input_map, 0, 255, 0, L_BLUE);
    ledcWrite(2, L_BLUE-L_BLUE_map);
    delay(1800);
    if(lightstate == 1){ //led自動隨光敏電阻改變亮度
    input = analogRead(33);
    input_map = map(input, 0, 300, 0, 255);}
    L_RED=255;
    L_GREEN=192;
    L_BLUE=203;
    L_RED_map = map(input_map, 0, 255, 0, L_RED);
    ledcWrite(0, L_RED-L_RED_map); 
    L_GREEN_map = map(input_map, 0, 255, 0, L_GREEN);
    ledcWrite(1, L_GREEN-L_GREEN_map);
    L_BLUE_map = map(input_map, 0, 255, 0, L_BLUE);
    ledcWrite(2, L_BLUE-L_BLUE_map);
    delay(1800);
    if(lightstate == 1){ //led自動隨光敏電阻改變亮度
    input = analogRead(33);
    input_map = map(input, 0, 300, 0, 255);}
    L_RED=0;
    L_GREEN=255;
    L_BLUE=0;
    L_RED_map = map(input_map, 0, 255, 0, L_RED);
    ledcWrite(0, L_RED-L_RED_map); 
    L_GREEN_map = map(input_map, 0, 255, 0, L_GREEN);
    ledcWrite(1, L_GREEN-L_GREEN_map);
    L_BLUE_map = map(input_map, 0, 255, 0, L_BLUE);
    ledcWrite(2, L_BLUE-L_BLUE_map);
    delay(1800);}
}
}
   if(lightstate == 1){ //led自動隨光敏電阻改變亮度
    input = analogRead(33);
    input_map = map(input, 0, 300, 0, 255);
    

}
}


//----設定HTML資料----
String htmlContent = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 300px; height: 70px; margin: 30px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <a href='/led/on'>LED ON</a>\
      <a href='/led/off'>LED OFF</a>\
      <a href='/led/autorgb'>autorgb</a>\
      <a href='/led/autolight'>autolight</a>\
      <a href='/led/WHITE'>WHITE</a>\
      <a href='/led/RED'>RED</a>\
      <a href='/led/GREEN'>GREEN</a>\
      <a href='/led/BLUE'>BLUE</a>\
      <a href='/led/ORANGE'>ORANGE</a>\
      <a href='/led/YELLOW'>YELLOW</a>\
      <a href='/led/PURPLE'>PURPLE</a>\
      <form method='get' action='/led/setLightBrightness'>\
        <label>Led Brightness</label>\
        <input type='number' name='lightBrightness'><input type='submit'>\
      </form>\
    </body>\
  </html>";
  String LEDautorgbPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>autorgb</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";
  String LEDautolightPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>autolight</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";
String LEDPURPLEPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>PURPLE</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";
String LEDYELLOWPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>YELLOW</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";
  String LEDORANGEPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>ORANGE</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";
  String LEDBLUEPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>BLUE</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";
  String LEDGREENPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>GREEN</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";
  String LEDREDPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>RED</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";
String LEDWHITEPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>WHITE</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";

String LEDOpenPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
         a { display: flex; flex-direction: row;justify-content: center;background: #0d0177;color: #a09e9e; width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>LED OPEN</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";

String LEDCLOSEPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
      <style>\
        body{ display: flex; flex-direction: column;align-items: center; background:url(images/bg.jpg);}\
        a {display: flex; flex-direction: row;justify-content: center; background: #0d0177; color: #a09e9e;width: 200px; height: 70px; margin: 20px; font-size: 50pt; text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <h1>LED CLOSE</h1>\
      <a href='/'>Back</a>\
    </body>\
  </html>";
  
//------設定路徑------
void createWebServer()
{
  //初始頁面
  server.on("/", []() {
      server.send(200, "text/html", htmlContent);
  });
  server.on("/led/on", []() {
    Serial.println("LED On");
    ledstate =1;
    rgbstate =1;
    lightstate =1; //add your code here
    
    String response = "{\"LED On Success\"}";
    server.send(200, "text/html", LEDOpenPage);
  });
  server.on("/led/autolight", []() {
    Serial.println("LED auto light");
    lightstate = 1;//add your code here
    
    String response = "{\"LED auto light Success\"}";
    server.send(200, "text/html", LEDautolightPage);
  });
  server.on("/led/autorgb", []() {
    Serial.println("LED auto rgb");
    rgbstate =1;
    String response = "{\"LED auto rgb Success\"}";
    server.send(200, "text/html", LEDautorgbPage);
  });
  server.on("/led/off", []() {
    Serial.println("LED Off");
    ledstate =0;
      //add your code here
    
    String response = "{\"LED Off Success\"}";
    server.send(200, "text/html", LEDCLOSEPage);
  });
   server.on("/led/WHITE", []() {
    Serial.println("LED WHITE");
    rgbstate =0;
    L_RED=255;
    L_GREEN=255;
    L_BLUE=255;//add your code here
    
    String response = "{\"LED WHITE Success\"}";
    server.send(200, "text/html", LEDWHITEPage);
  });
   server.on("/led/RED", []() {
    Serial.println("LED RED");
    rgbstate =0;
    L_RED=255;
    L_GREEN=0;
    L_BLUE=0;//add your code here
    
    String response = "{\"LED RED Success\"}";
    server.send(200, "text/html", LEDREDPage);
  });
     server.on("/led/GREEN", []() {
    Serial.println("LED GREEN");
    rgbstate =0;
    L_GREEN=255;
    L_RED=0;
    L_BLUE=0;//add your code here
    
    String response = "{\"LED GREEN Success\"}";
    server.send(200, "text/html", LEDGREENPage);
  });
       server.on("/led/BLUE", []() {
    Serial.println("LED BLUE");
    rgbstate =0;
    L_BLUE=255;
    L_RED=0;
    L_GREEN=0;//add your code here
    
    String response = "{\"LED BLUE Success\"}";
    server.send(200, "text/html", LEDBLUEPage);
  });
  server.on("/led/ORANGE", []() {
      Serial.println("LED ORANGE");
      rgbstate =0;
    L_BLUE=16;
    L_RED=255;
    L_GREEN=160;//add your code here
    
    String response = "{\"LED ORANGE Success\"}";
    server.send(200, "text/html", LEDORANGEPage);
  });
  server.on("/led/YELLOW", []() {
      Serial.println("LED YELLOW");
      rgbstate =0;
    L_BLUE=32;
    L_RED=255;
    L_GREEN=224;//add your code here
    
    String response = "{\"LED YELLOW Success\"}";
    server.send(200, "text/html", LEDYELLOWPage);
  });
  server.on("/led/PURPLE", []() {
      Serial.println("LED PURPLE");
      rgbstate =0;
    L_BLUE=255;
    L_RED=160;
    L_GREEN=32;//add your code here
    
    String response = "{\"LED PURPLE Success\"}";
    server.send(200, "text/html", LEDPURPLEPage);
  });
  server.on("/led/setLightBrightness", []() {
    //以server.arg&引數名稱，取得form傳來的資料
    int lightBrightness = server.arg("lightBrightness").toInt();
    //判斷條件    
    if(lightBrightness>=0 && lightBrightness<=255){
      Serial.print("Set Brightness: ");
      Serial.println(lightBrightness);
      lightstate =0;
      input_map=255-lightBrightness;

 
        
 //add your code here
      
      String response = "Set Brightness: " + (String)lightBrightness;
      server.send(200, "application/json", response);
    } 
    else {
      String response = "{\"Error\":\"over range 0-1024\"}";
      server.send(404, "application/json", response);
    }
  });
