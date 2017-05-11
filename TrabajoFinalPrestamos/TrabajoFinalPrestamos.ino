#include <SPI.h>
#include <Ethernet.h>
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0F, 0x3A, 0xDC
};
char server[] = "www.mundoviceversa.com";
//IPAddress server(74, 208, 236, 116);
//IPAddress ip(192,168,2,8); //
EthernetClient client;

void setup() {
  Serial.begin(9600);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    //Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println("connecting...");

}


void loop() {
    
    

}

void prueba(){
  if (client.connect(server, 80)) {
      Serial.println("connected");
      client.println("GET /APrest.php?Doc=8163847&Ref=1&Cant=120&Prest=1 HTTP/1.1");
      client.println("Host: www.mundoviceversa.com");
      client.println("Connection: close");
      client.println();
    } else {
      Serial.println("connection failed");
    }
    delay(1000);

    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
    }
    while (true){};
}

