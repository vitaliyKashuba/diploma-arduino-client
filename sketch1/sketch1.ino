#include <UIPEthernet.h>
#include <SPI.h>
#include <RFID.h>

#define ETH_SS_PIN 10
#define RFID_SS_PIN 9
#define RFID_RST_PIN 8

RFID rfid(RFID_SS_PIN, RFID_RST_PIN);

int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;

EthernetClient client;

void setup() {

  Serial.begin(19200);

  digitalWrite(ETH_SS_PIN,LOW);   //activates eherned module
  digitalWrite(RFID_SS_PIN, HIGH);//

  Serial.print("setup");
  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  Ethernet.begin(mac);

  char a[] = "123";
  Serial.print("localIP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("subnetMask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("gatewayIP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("dnsServerIP: ");
  Serial.println(Ethernet.dnsServerIP());

  digitalWrite(ETH_SS_PIN,HIGH);  //activates rfid module
  digitalWrite(RFID_SS_PIN, LOW); //
  
  SPI.begin(); //needs for rfid
  rfid.init();
}

void doPost()
{
  digitalWrite(ETH_SS_PIN,LOW);
  digitalWrite(RFID_SS_PIN, HIGH);
  
  if (client.connect(IPAddress(192,168,10,100),8080))
  {
    char buf[100];
    Serial.println("Client connected");
    char a[] = "123";
    sprintf(buf, "POST /post_test HTTP/1.0\r\nContent-Type: application/raw\r\nContent-Length: 14\r\n\r\n%X %X %X %X %X", serNum0, serNum1, serNum2, serNum3, serNum4);
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

void loop() 
{
  digitalWrite(ETH_SS_PIN,HIGH);
  digitalWrite(RFID_SS_PIN, LOW);
  
  if (rfid.isCard()) 
  {
        if (rfid.readCardSerial()) 
        {
            if (rfid.serNum[0] != serNum0
                && rfid.serNum[1] != serNum1
                && rfid.serNum[2] != serNum2
                && rfid.serNum[3] != serNum3
                && rfid.serNum[4] != serNum4
            ) 
            {
                Serial.println("Card found");
                serNum0 = rfid.serNum[0];
                serNum1 = rfid.serNum[1];
                serNum2 = rfid.serNum[2];
                serNum3 = rfid.serNum[3];
                serNum4 = rfid.serNum[4];

                doPost();
             } else 
             {
               /* If we have the same ID, just write a dot. */
               Serial.print(".");
             }
          }
    }
    rfid.halt();
}
