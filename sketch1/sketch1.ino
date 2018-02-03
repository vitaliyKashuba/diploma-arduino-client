#include <UIPEthernet.h>

#define ETH_SS_PIN 10
#define RFID_SS_PIN 9
#define RFID_RST_PIN 8

EthernetClient client;
signed long next;

void setup() {

  Serial.begin(9600);

  digitalWrite(ETH_SS_PIN,LOW);
  digitalWrite(RFID_SS_PIN, HIGH);

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

void doPost()
{
  if (client.connect(IPAddress(192,168,10,100),8080))
        {
          char buf[100];
          Serial.println("Client connected");
          char a[] = "123";
           sprintf(buf, "POST /post_test HTTP/1.0\r\nContent-Type: application/raw\r\nContent-Length: 3\r\n\r\n%s", a);
           /////////////////////////////////
           Serial.println(buf);
           delay(1000);
           client.println(buf); // Отправляем GET запрос
           delay(200);
           client.println();
           delay(1000);
           Serial.println("disconnecting.");
           client.stop(); // Завершаем соединение
          
        }
      else
      {
        Serial.println("Client connect failed");
      }
}

void loop() {

  if (((signed long)(millis() - next)) > 0)
    {
      next = millis() + 10000;
      Serial.println("Client connect");
      doPost();
    }
}
