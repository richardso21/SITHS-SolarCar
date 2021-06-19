#include "Lora.hpp"

Lora::Lora(SoftwareSerial *ss)
{
    loraSerial = ss;
}

void Lora::begin(long baud)
{
    loraSerial->begin(baud);
    // String commands[5] = {
    //     "AT+RESET",
    //     "AT+IPR=" + String(baud),
    //     "AT+ADDRESS=" + String(address),
    //     "AT+NETWORKID=12",
    //     "AT+PARAMETER=12,4,1,7"
    // };
    // for (int i = 0; i < 5; i++)
    // {
    //     loraSerial->print(commands[i] + "\r\n");
    //     delay(100);
    // }
}

void Lora::send(String message)
{
    int size = message.length();
    String res = "AT+SEND,0," + String(size) + "," + message + "\r\n";
    loraSerial->print(res);
}

bool Lora::available()
{
    return loraSerial->available();
}
char Lora::read()
{
    return loraSerial->read();
}
String Lora::readString()
{
    return loraSerial->readString();
}