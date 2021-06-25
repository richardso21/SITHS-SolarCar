#include "LoraSerial.hpp"

void LoraSerial::sendData(String msg)
{
    int size = msg.length();
    String res = "AT+SEND=0," + String(size) + "," + msg + "\r\n";
    LoraSerial::print(res);
}

// void LoraSerial::sendData(String msgs[])
// {
//     String res = "";
//     int arrLength = sizeof(msgs) / sizeof(msgs[0]);
//     for (int i = 0; i < arrLength; i++)
//     {
//         res += msgs[i];
//         if (i != (arrLength - 1))
//             res += ":";
//     }

//     LoraSerial::sendData(res);
// }

String LoraSerial::parseData()
{
    String msg = LoraSerial::readString();
    String parsed[5];
    int n = 0;
    int length = msg.length();
    String tmp;

    // split raw data using commas
    for (int i = 0; i < length; i++)
    {
        char c = msg[i];
        if (c == ',')
        {
            parsed[n] = tmp;
            tmp = "";
            n++;
        }
        else
        {
            tmp += String(c);
        }
    }
    // return 3rd separated element (the message)
    return parsed[2];
}