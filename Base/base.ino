#include <LoRa.h>
#include <I2C_AXP192.h>

I2C_AXP192 axp192(I2C_AXP192_DEFAULT_ADDRESS, Wire1);

const int btnPin = 38;



void setup() {
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, 34, 12, false, 1000);;
    delay(1000);

    Wire1.begin(21, 22);

        I2C_AXP192_InitDef initDef = {
        .EXTEN  = true,
        .BACKUP = true,
        .DCDC1  = 2500,
        .DCDC2  = 0,
        .DCDC3  = 3300,
        .LDO2   = 3300,
        .LDO3   = 3300,
        .GPIO0  = -1,
        .GPIO1  = -1,
        .GPIO2  = -1,
        .GPIO3  = -1,
        .GPIO4  = -1,
    };
    axp192.begin(initDef);

    pinMode(btnPin, INPUT);

    LoRa.setPins(18, 23, 26);

    if (!LoRa.begin(868E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

void loop() {

    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // received a packet
        Serial.print("Received packet '");

        // read packet
        while (LoRa.available()) {
        Serial.print((char)LoRa.read());
        }

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
    }

    smartDelay(0);
}

static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do 
    {
            
    } while (millis() - start < ms);
}
