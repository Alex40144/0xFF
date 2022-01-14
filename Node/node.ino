#include <I2C_AXP192.h>
#include <LoRa.h>
#include <ArduinoNmeaParser.h>

I2C_AXP192 axp192(I2C_AXP192_DEFAULT_ADDRESS, Wire1);

const int btnPin = 38;


void onRmcUpdate(nmea::RmcData const rmc)
{
    char str[20];
    Serial.print(rmc.time_utc.second);
    LoRa.beginPacket();
    sprintf(str, "%d", rmc.time_utc.hour);
    LoRa.print(str);
    LoRa.print(":");
    sprintf(str, "%d", rmc.time_utc.minute);
    LoRa.print(str);
    LoRa.print(":");
    sprintf(str, "%d", rmc.time_utc.second);
    LoRa.print(str);
    LoRa.print(".");
    sprintf(str, "%d", rmc.time_utc.microsecond);
    LoRa.print(str);

    if (rmc.is_valid)
    {
        LoRa.print(" : LON ");
        LoRa.print(rmc.longitude);
        LoRa.print(" ° | LAT ");
        LoRa.print(rmc.latitude);
        LoRa.print(" ° | VEL ");
        LoRa.print(rmc.speed);
        LoRa.print(" m/s | HEADING ");
        LoRa.print(rmc.course);
        LoRa.print(" °");
    }

    LoRa.endPacket();
}

ArduinoNmeaParser parser(onRmcUpdate, nullptr);

void setup() {
    axp192.bitOff(0x32, (1<<7));
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

    if(digitalRead(btnPin) == 0) {
        LoRa.beginPacket();
        LoRa.print("Shutting Down");
        LoRa.endPacket();
        axp192.powerOff();
    }

    smartDelay(5000);
}

static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do 
    {
        if (Serial1.available()){
            parser.encode((char)Serial1.read());
            //Serial.write(Serial1.read()); //passthrough
        }
    } while (millis() - start < ms);
}
