#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
//  sudo apt-get install libboost-filesystem1.54.0
//  sudo apt-get install libboost-filesystem1.55-dev
#include <boost/filesystem.hpp>		// Only used to test if file exists

// Redefines what the comment prefix should be. By default it is set to ";"
//#undef  INI_INLINE_COMMENT_PREFIXES
#define INI_INLINE_COMMENT_PREFIXES "%"

#include "inih/cpp/INIReader.cpp"
#include "inih/ini.c"
#include "tcpsocket/tcpconnector.h"


//functions
int timeStrToInt(const string time);
void getAllParameters(INIReader reader, char line[409]);


int main(int argc, char* argv[])
{
    // Character array containing the command to launch the Audio Recording
    char command[100];
    unsigned int StartTime, EndTime;
    string StartTimeStr, EndTimeStr;
    
    //* Variables for TCP connection
    int len;
    string message;
    char line[410];
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream = connector->connect("192.168.0.2", 9999);
    
    // Waits for 'NeXtRAD Header.txt' to appear
    while (!boost::filesystem::exists( "NeXtRAD Header.txt" ) ) 
    {
        // check every 1 second
        sleep(1);
        printf("NeXtRAD Header.txt not located.\n");
    }
    
    // Once the file has been located (from above) it then opens (below)
    INIReader reader("NeXtRAD Header.txt");
    
    // 
    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'NeXtRAD Header.txt'\n";
        return 1;
    }
    

    // Indicates that config file loaded successfully
    printf("Config loaded from 'test.ini': StartTime=%s, EndTime=%s\n", reader.Get("settings","StartTime","UNKNOWN").c_str()
        , reader.Get("settings","EndTime","UNKNOWN").c_str());  //(int)time(0)
        
    //retrieves parameters necessary for Asterisk recording programme
    StartTime = timeStrToInt(reader.Get("settings","StartTime","UNKNOWN"));
    EndTime = timeStrToInt(reader.Get("settings","EndTime","UNKNOWN"));
    
    printf("StartTimeFromEpoch=%u, EndTimeFromEpoch=%u\n",
        StartTime, EndTime);
        
	// Runs Asterisk audio recorder
    sprintf(command, "./asterisk_recorder %u %u > redirection &", StartTime, EndTime);
    system(command);
    
    
    // Connects to Rhino
    if (stream) {
        message = "Is there life on Mars?";
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
    }
    
    
    stream = connector->connect("192.168.0.2", 9999);
    if (stream) {
        char msg[409];
        
        getAllParameters(reader, msg);
        
        stream->send(msg, 409);
        printf("sent - %s\n", msg);
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
    }

/*
    stream = connector->connect("localhost", 9999);
    if (stream) {
        message = "Why is there air?";
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
    }
*/
    return 0;
}



int timeStrToInt(const string time)
{
    struct tm tm;
    strptime(time.c_str(), "%d-%m-%Y %H:%M:%S", &tm);
    time_t t = mktime(&tm); 
    
    return t;
}




void getAllParameters(INIReader reader, char line[409])
{
    int StartTime = timeStrToInt(reader.Get("settings","StartTime","UNKNOWN"));
    int EndTime = timeStrToInt(reader.Get("settings","EndTime","UNKNOWN"));
    
    // Message type
    line[0] = 'P';

    // N
    line[1] = reader.GetInteger("settings","NumberOfPulses",1);

    // M
    line[2] = (reader.GetInteger("settings","Repeats",1) >> 24) & 0xFF;
    line[3] = (reader.GetInteger("settings","Repeats",1) >> 16) & 0xFF;
    line[4] = (reader.GetInteger("settings","Repeats",1) >> 8) & 0xFF;
    line[5] = reader.GetInteger("settings","Repeats",1) & 0xFF;

    // StartTime
    line[6] = (StartTime >> 24) & 0xFF;
    line[7] = (StartTime >> 16) & 0xFF;
    line[8] = (StartTime >> 8) & 0xFF;
    line[9] = StartTime & 0xFF;

    // EndTime
    line[10] = (EndTime >> 24) & 0xFF;
    line[11] = (EndTime >> 16) & 0xFF;
    line[12] = (EndTime >> 8) & 0xFF;
    line[13] = EndTime & 0xFF;

    // Pulse Parameters
    for(int i = 0; i<32; i++)
    {
        string pulseno;
        char pulsechar[8];
        
        sprintf(pulsechar, "pulse%d", i);
        
        pulseno = pulsechar;
        
        cout << pulseno << "\n";
        
        line[14+i*12] = (reader.GetInteger(pulseno,"MBoffset",1) >> 8) & 0xFF;
        line[15+i*12] = (reader.GetInteger(pulseno,"MBoffset",1)) & 0xFF;
        
        line[16+i*12] = (reader.GetInteger(pulseno,"DIGoffset",1) >> 8) & 0xFF;
        line[17+i*12] = (reader.GetInteger(pulseno,"DIGoffset",1)) & 0xFF;
        
        line[18+i*12] = (reader.GetInteger(pulseno,"PRIoffset",1) >> 8) & 0xFF;
        line[19+i*12] = (reader.GetInteger(pulseno,"PRIoffset",1)) & 0xFF;
        
        line[20+i*12] = (reader.GetInteger(pulseno,"Frequency",1) >> 8) & 0xFF;
        line[21+i*12] = (reader.GetInteger(pulseno,"Frequency",1)) & 0xFF;
        
        line[22+i*12] = (reader.GetInteger(pulseno,"Mode",1) >> 8) & 0xFF;
        line[23+i*12] = (reader.GetInteger(pulseno,"Mode",1)) & 0xFF;
        
        line[24+i*12] = (reader.GetInteger(pulseno,"Notused",1) >> 8) & 0xFF;
        line[25+i*12] = (reader.GetInteger(pulseno,"Notused",1)) & 0xFF;
    }
    
}
