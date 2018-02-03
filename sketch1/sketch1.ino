#include <UIPEthernet.h>

#define ETH_SS_PIN 10
#define RFID_SS_PIN 9
#define RFID_RST_PIN 8

EthernetClient client;
signed long next;

void setup() {

  Serial.begin(9600);

  Serial.print("setup");
  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  Ethernet.begin(mac);

  Serial.print("localIP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("subnetMask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("gatewayIP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("dnsServerIP: ");
  Serial.println(Ethernet.dnsServerIP());

  next = 0;
}

void loop() {

  if (((signed long)(millis() - next)) > 0)
    {
      next = millis() + 5000;
      Serial.println("Client connect");
      // replace hostname with name of machine running tcpserver.pl
//      if (client.connect("server.local",5000))
      if (client.connect(IPAddress(192,168,10,100),8080))
        {

          char buf[80];
          Serial.println("Client connected");
           sprintf(buf, "POST /post_test HTTP/1.0");
           /////////////////////////////////
           Serial.println(buf);
           delay(500);
           client.println(buf); // Отправляем GET запрос
           delay(100);
           client.println();
           delay(500);
           Serial.println("disconnecting.");
           client.stop(); // Завершаем соединение
          
        }
      else
        Serial.println("Client connect failed");
    }
}
