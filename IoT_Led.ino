#include <ESP8266WiFi.h>
  
int ledler[5] = {5,0, 2, 4, 16};
int durum[5] = {0,0,0,0,0};
const char* ssid = "Your Device Name";
const char* password = "Your Device Password";
bool req = false, deger = false;

int value = LOW;
WiFiServer server(80);
//SETUP
void setup() {
Serial.begin(9600);
delay(10);
 for(int i = 0; i<5;i++){
    pinMode(ledler[i], OUTPUT);
 }
 
// Wifi bağlantısı
Serial.print("\n\n");
Serial.print("Bağlanılıyor ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi ağına bağlanıldı.");
 
// server baslatılıyor
server.begin();
Serial.println("Server başlatıldı.");
 
// baglantı saglandıktan sonra serial monitorde bize ip adresini gösterecek
Serial.print("Bağlanmak için bu URL yi kullanın: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
 
}
 
void loop() {
// bir client istemci baglı olup olmadığını kontrol ediyoruz
WiFiClient client = server.available();
if (!client) {
  if(value == HIGH){
    KaraSimsek();
  }   
return;
}

// client ın bir data gondermesini bekliyoruz
Serial.println("new client");
while(!client.available()){
delay(1);
}
// gelen istekleri okuyoruz
String request = client.readStringUntil('\r');
Serial.println(request);
//client.flush();  //VERİ OKUNANA KADAR BEKLE
 

if (request.indexOf("/BASLAT") != -1) {
  KaraSimsek();
  value = HIGH;
}
else if (request.indexOf("/DURDUR") != -1) {
  value = LOW;
}
else{
  digitalWrite(ledler[request.substring(1,1).toInt()],(durum[request.substring(1,1).toInt()] == 0)? HIGH: LOW);
}

client.println(""); 
client.println("<!DOCTYPE HTML>");
client.println("<html>");

client.println("<style> .buton{ width:50px; height:50px; background:black; forecolor:white; border:none; ");
for(int i=0;i<5;i++){
  client.print("<a href=\"/");
  client.print(i);
  client.print("\"><button class=");
  if(durum[i] == 1){
    client.print("\"open\">OPEN</button></a>");
  }
  else{
    client.print("\"buton\">CLOSE</button></a>");
  }
}
client.println("<a href=\"/BASLAT\"\"><button id=\"ss\">ledi ac </button></a>");
client.println("<a href=\"/DURDUR\"\"><button>ledi sondur </button></a>"); 
client.println("</html>");
 
delay(1);
Serial.println("Client disonnected");
Serial.println("");
 
}

void KaraSimsek(){
  for(int i = 0; i<5;i++){
    digitalWrite(ledler[i], HIGH);
    delay(200);
    digitalWrite(ledler[i], LOW);
  }
}
