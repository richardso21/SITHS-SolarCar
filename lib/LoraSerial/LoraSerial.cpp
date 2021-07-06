#include "LoraSerial.hpp"

void LoraSerial::sendData(
    int speed,
    int vBatt,
    double vAux,
    double aMotor,
    double aShunt)
{
    int dataSize = (String(speed) +
                    String(vBatt) +
                    String(vAux) +
                    String(aMotor) +
                    String(aShunt))
                       .length() +
                   4;

    print("AT+SEND=0,");
    print(dataSize);
    print(",");
    print(speed);
    print(";");
    print(vBatt);
    print(";");
    print(vAux, 1);
    print(";");
    print(aMotor, 1);
    print(";");
    print(aShunt, 1);
    print("\r\n");
}

// void LoraSerial::sendData(String msg)
// {
//     int size = msg.length();
//     String res = "AT+SEND=0," + String(size) + "," + msg + "\r\n";
//     LoraSerial::print(res);
// }

// void LoraSerial::sendData(
//     time_t unixTime,
//     int speed,
//     int vBatt,
//     double vAux,
//     double aMotor,
//     double aShunt,
//     int temp)
// {
//     String msg =
//         String(unixTime) + ";" +
//         String(speed) + ";" +
//         String(vBatt) + ";" +
//         String(vAux) + ";" +
//         String(aMotor) + ";" +
//         String(aShunt) + ";" +
//         String(temp);

//     LoraSerial::sendData(msg);
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