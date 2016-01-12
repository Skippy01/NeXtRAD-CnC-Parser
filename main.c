/* Example: parse a simple configuration file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Redefines what the comment prefix should be. By default it is set to ";"
//#undef  INI_INLINE_COMMENT_PREFIXES
#define INI_INLINE_COMMENT_PREFIXES "%"

#include "ini.c"

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

    // closes program if file can't be found
    configuration config;
    if (ini_parse("NeXtRAD Header.txt", handler, &config) < 0) {
        printf("Can't load 'test.ini'\n");
        return 1;
    }

    // Indicates that config file loaded successfully
    printf("Config loaded from 'test.ini': StartTime=%s, EndTime=%s\n",
        config.StartTime, config.EndTime);  //(int)time(0)
    printf("StartTimeFromEpoc=%u, EndTimeFromEpoc=%u\n",
        timeStrToInt(config.StartTime), timeStrToInt(config.EndTime));

    //retrieves parameters necessary for Asterisk recording programme
    unsigned int start = timeStrToInt(config.StartTime);
    unsigned int end = timeStrToInt(config.EndTime);
    
    sprintf(command, "./asterisk_recorder %u %u", start, end);
    system(command);

    return 0;
}

int timeStrToInt(char* time)
{
    struct tm tm;
    strptime(time, "%d-%m-%Y %H:%M:%S", &tm);
    time_t t = mktime(&tm); 
    
    return t;
}
