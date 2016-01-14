/* Example: parse a simple configuration file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <iostream>

// Redefines what the comment prefix should be. By default it is set to ";"
//#undef  INI_INLINE_COMMENT_PREFIXES
#define INI_INLINE_COMMENT_PREFIXES "%"

#include "inih/cpp/INIReader.cpp"
#include "inih/ini.c"
#include "tcpsocket/tcpconnector.h"








//functions
int timeStrToInt(const string time);


// Parameters from header file that are necessary for this parser
typedef struct
{
    const char* StartTime;
    const char* EndTime;

} configuration;

// Handler used by the parser to assign values to variables.
// Variables are declared in the configuration struct
static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)user;

    //#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    //got rid of above line since sections are not necessary here. They are
    //necessary for the Rhino
    #define MATCH(n) strcmp(name, n) == 0
    if (MATCH("StartTime")) {
        pconfig->StartTime = strdup(value);
    } else if (MATCH("EndTime")) {
        pconfig->EndTime = strdup(value);
    }  else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}


int main(int argc, char* argv[])
{
    
    // Character array containing the command to launch the Audio Recording
    char command[100];
    unsigned int StartTime, EndTime;
    string StartTimeStr, EndTimeStr;

    // closes program if file can't be found
    configuration config;
    
    // Waits for 'NeXtRAD Header.txt' to appear
    while (ini_parse("NeXtRAD Header.txt", handler, &config) < 0) 
    {
        // check every 1 second
        sleep(1);
        printf("NeXtRAD Header.txt not located.\n");
    }
    
    INIReader reader("NeXtRAD Header.txt");
    
    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'NeXtRAD Header.txt'\n";
        return 1;
    }
    
    StartTimeStr = ;
    EndTimeStr   = reader.Get("settings","EndTime","UNKNOWN");

    // Indicates that config file loaded successfully
    printf("Config loaded from 'test.ini': StartTime=%s, EndTime=%s\n", reader.Get("settings","StartTime","UNKNOWN").c_str()
        , EndTimeStr.c_str());  //(int)time(0)
        
    StartTime = timeStrToInt(reader.Get("settings","StartTime","UNKNOWN"));
    EndTime = timeStrToInt(reader.Get("settings","EndTime","UNKNOWN"));
    
    printf("StartTimeFromEpoch=%u, EndTimeFromEpoch=%u\n",
        StartTime, EndTime);

    //retrieves parameters necessary for Asterisk recording programme
    
    
    
    
    
    
    sprintf(command, "./asterisk_recorder %u %u > redirection &", StartTime, EndTime);
    system(command);

    return 0;
}

int timeStrToInt(const string time)
{
    struct tm tm;
    strptime(time.c_str(), "%d-%m-%Y %H:%M:%S", &tm);
    time_t t = mktime(&tm); 
    
    return t;
}
