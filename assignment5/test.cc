/*
Filename:    test.cc
Date:        11/12/2020
Author:      Garrett Strealy
Email:       gjs160430@utdallas.edu
Version:     1.0
Copyright:   2020, All Rights Reserved
Description: contains 2 functions below the main

configParse function parses the configuration file using rude

signalHandler function deletes the pid file, closes the log file, and exists.
In the main function, it uses tclap to parse command line.

-d option: run in the background, create pid file, log file, and inotify.
no -d option: runs in the foreground.

use inotify to watch the testdir directory for file modification updates.
modified files will be copied to the .versions directory
*/

#include <iostream>
#include <tclap/CmdLine.h>
#include <map>
#include <rude/config.h>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include <csignal>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <sys/inotify.h>
#include "hw5.h"
using namespace std;
using namespace TCLAP;
using namespace rude;

#define EVENT_SIZE ( sizeof (struct inotify_event))
#define BUFFER_LENGTH (1024 * (EVENT_SIZE + 16))
enum
results{DAEMON,CONFIG,VERBOSE,LOGFILE,PASSWORD,NUMVERSION,WATCHDIR};
map<int,string> tclapMap;

// void signalHandler(int signum);
// void signalHandlerU(int signum);
// void configParse();

int main(int argc, char* argv[]) {

  //define the command line object
  CmdLine cmd("CS3377.001 program5",' ',"1.0");

  SwitchArg daemonSwitch("d", "daemon", "Run in daemon mode (forks to run as a
  daemon).", cmd, false);

  UnlabeledValueArg<string>configArg("config", "The name of the configuration
  file", true, "cs3377dirmond.conf", "configuration file name", false);

  cmd.add(configArg);
  cmd.parse(argc,argv);

  //if d is in the command line
  if(daemonSwitch.getValue()) { // returns a bool
  tclapMap[DAEMON] = "true"; // converts to string
  }
  else{
  tclapMap[DAEMON]="false"; // converts to string
  }
  string configVar = configArg.getValue();
    tclapMap[CONFIG]=configVar;
    
    configParse(); // call parsing config file function

    mkdir("/home/012/w/wx/wxl180022/testdir/.versions", 0770); // system call to create
directory

    if(daemonSwitch.getValue()) { // implementing -d option (run in background)

    pid_t forkValue = fork();
    if(forkValue == -1) {
    perror( "There is an error in the fork. No child was created\n");
    return 1;
    }
    else if (forkValue == 0) {
    // create pid file by daemon
    ofstream pidFileCreate;
    // check if the pid file exists
    struct stat st;
    if(stat("cs3377dirmond.pid", &st) == 0) { // process already running
    perror( "cs3377dirmond.pid exists. Now exiting\n");
    exit(0);
    }

    pidFileCreate.open("cs3377dirmond.pid");
    
    if(!pidFileCreate){
         perror( "error. Pid file cannot be created or open\n");
         return -1;
    }
    else{
         if(tclapMap[VERBOSE]=="true"){
         pidFileCreate << "pid file is created" << endl;
         }
    }

    pidFileCreate << getpid() << endl;
        signal(SIGINT,signalHandler);
        signal(SIGTERM, signalHandler);
        signal(SIGHUP, signalHandlerU);
    pidFileCreate.close();

        // create log file as specified by the LogFile Parameter in the CONF file
        ofstream LogFileCreate;
        LogFileCreate.open(tclapMap[LOGFILE].c_str());
        // if opening the log file has failed
        if(!LogFileCreate){
        perror( "error. Log file cannot be created or open\n");
        return -1;
        }

        // start doing inotify thing
        int fileDescriptor = inotify_init();
        if (fileDescriptor < 0){// if inotify_int returns -1, error occurs
        perror("inotify_init");
        }

        int writeDescriptor = inotify_add_watch(fileDescriptor, tclapMap[WATCHDIR].c_str(),
IN_MODIFY);
        if (writeDescriptor < 0){// if add watch function returns -1, error
        perror("inotify_add_watch");
        }
        while(1){
        int i = 0;
        char buffer[BUFFER_LENGTH];

        int length = read(fileDescriptor,buffer,BUFFER_LENGTH);
        if (length < 0) {
        perror("read");
        }
        while (i< length) {
        struct inotify_event *event = ( struct inotify_event * ) &buffer[i];
        if(event->len) {
        if (event->mask & IN_MODIFY){
        if(event->mask & IN_ISDIR){ // directory is modified
        LogFileCreate << "The directory is modified" << event->name << endl;
        }
        else { // if file is modified
        LogFileCreate << "The file is modified: " << event->name << endl;
        string fileNameInDir = event->name;
        string leadToFile = tclapMap[WATCHDIR]+"/"+fileNameInDir;
        string versionsPath = tclapMap[WATCHDIR]+"/.versions/";

        time_t now = time(0); // current date/time based on current system
        tm *ltm = localtime(&now);

        int year = 1900+ ltm->tm_year; // convert year to string
        string yearFinal;
        stringstream ssYear;
        ssYear << year;
        ssYear >> yearFinal;

        int month = 1 + ltm->tm_mon; // convert month to string
        stringstream ssMonth;
        string monthFinal;
        ssMonth << month;
        ssMonth >> monthFinal;

        int day = ltm->tm_mday; //convert day to string
        stringstream ssDay;
        string dayFinal;
        ssDay << day;
        ssDay >> dayFinal;

        int hour = ltm->tm_hour;//convert hour to string
        stringstream ssHour;
        string hourFinal;
        ssHour << hour;
        ssHour >> hourFinal;

        int minute= ltm->tm_min; //convert minute to string
        stringstream ssMin;
        string minuteFinal;
        ssMin << minute;
        ssMin >> minuteFinal;

        int seconds = ltm->tm_sec; //convert seconds to string
        stringstream ssSec;
        string secondFinal;
        ssSec << seconds;
        ssSec >> secondFinal;

        string newFile = versionsPath + fileNameInDir + "." + yearFinal + "." + monthFinal
        + "." + dayFinal + "-" + hourFinal + ":" + minuteFinal + ":" + secondFinal;

        string finalFile = "cp " + leadToFile + " " + newFile;
        FILE *VP = popen(finalFile.c_str(),"r"); //copies files to .versions
        pclose(VP);
        }
        }
        }

        i += EVENT_SIZE + event->len;
} //end of while

} // end of infinite loop
(void) inotify_rm_watch(fileDescriptor, writeDescriptor);
(void) close(fileDescriptor);

// end of inotify
return 0;
} // end of forkValue == 0

}// end of -d option if statement
    else { // no -d option
    // create log file as specified by the LogFile Parameter in the CONF file
    ofstream LogFileCreate;
    LogFileCreate.open(tclapMap[LOGFILE].c_str());
    // if opening the log file has failed
    if(!LogFileCreate){
    perror( "error. Log file cannot be created or open\n");
    return -1;
    }

    // start doing inotify thing
    int fileDescriptor = inotify_init();
    if (fileDescriptor < 0) { // if inotify_int returns -1, error occurs
    perror("inotify_init");
    }
    int writeDescriptor = inotify_add_watch(fileDescriptor, tclapMap[WATCHDIR].c_str(),
IN_MODIFY);
    if (writeDescriptor < 0) { // if add watch function returns -1, error
    perror("inotify_add_watch");
    }
    while(1) {
    int i = 0;
    char buffer[BUFFER_LENGTH];

    int length = read(fileDescriptor,buffer,BUFFER_LENGTH);
    if (length < 0) {
    perror("read");
    }
    while (i< length) {
    struct inotify_event *event = ( struct inotify_event * ) &buffer[i];
    if(event->len){
    if (event->mask & IN_MODIFY){
    if(event->mask & IN_ISDIR){ // directory is modified
    LogFileCreate << "The directory is modified" << event->name << endl;
    }
    else{ // if file is modified
        LogFileCreate << "The file is modified: " << event->name << endl;
        string fileNameInDir = event->name;
        string leadToFile = tclapMap[WATCHDIR]+"/"+fileNameInDir;
        string versionsPath = tclapMap[WATCHDIR]+"/.versions/";
        
        time_t now = time(0); // current date/time based on current system
        tm *ltm = localtime(&now);

        int year = 1900+ ltm->tm_year; // convert year to string
        string yearFinal;
        stringstream ssYear;
        ssYear << year;
        ssYear >> yearFinal;

        int month = 1 + ltm->tm_mon; // convert month to string
        stringstream ssMonth;
        string monthFinal;
        ssMonth << month;
        ssMonth >> monthFinal;

        int day = ltm->tm_mday; //convert day to string
        stringstream ssDay;
        string dayFinal;
        ssDay << day;
        ssDay >> dayFinal;

        int hour = ltm->tm_hour;//convert hour to string
        stringstream ssHour;
        string hourFinal;
        ssHour << hour;
        ssHour >> hourFinal;

        int minute= ltm->tm_min; //convert minute to string
        stringstream ssMin;
        string minuteFinal;
        ssMin << minute;
        ssMin >> minuteFinal;

        int seconds = ltm->tm_sec; //convert seconds to string
        stringstream ssSec;
        string secondFinal;
        ssSec << seconds;
        ssSec >> secondFinal;

        string newFile = versionsPath + fileNameInDir + "." + yearFinal + "." + monthFinal
        + "." + dayFinal + "-" + hourFinal + ":" + minuteFinal + ":" + secondFinal;

        string finalFile = "cp " + leadToFile + " " + newFile;
        FILE *VP = popen(finalFile.c_str(),"r"); //copies files to .versions
        pclose(VP);

        }
        }
        }

i += EVENT_SIZE + event->len;
} //end of while

}// end of infinite loop
    (void) inotify_rm_watch(fileDescriptor, writeDescriptor);
    (void) close(fileDescriptor);
    // end of inotify
} // end of d option
return 0;
}

void signalHandler(int signum) {
    remove("cs3377dirmond.pid");
    if(!remove("cs3377dirmond.pid")) {
        perror( "File deletion failed\n");
    }

ofstream LogFileCreate;
LogFileCreate.open("cs3377dirmond.log", ios::app);

LogFileCreate.close();
exit (signum);
}

void signalHandlerU(int signum) {
    configParse();
    cout << "Press enter to exit" << endl;
}

void configParse() {
    // now parse config
    // use config file
    Config config; // create config
    //load a configuration file
    if (config.load(tclapMap[CONFIG].c_str())) {
    //read information
    if (config.setSection("Parameters")) {
    // if one of the definitions is missing
    string verbose = "Verbose";
    if (config.exists(verbose.c_str())) {
    tclapMap[VERBOSE] = config.getStringValue("Verbose");
    cout << "tclapMap[VERBOSE] value is " << tclapMap[VERBOSE] << endl;
    }
    else {
    perror("verbose definition is missing. Now exiting\n");
    exit(0);
    }
    string logFile = "LogFile";
    if (config.exists(logFile.c_str())) {
    tclapMap[LOGFILE] = config.getValue("LogFile");
    cout << "tclap[LOGFILE] value is " << tclapMap[LOGFILE] << endl;
    }
    else {
    perror("log file definition is missing. Now exiting\n");
    exit(0);
    }
    string password = "Password";
    if (config.exists(password.c_str()))
    {
    if (tclapMap[PASSWORD] == "") {
        tclapMap[PASSWORD] = config.getStringValue("Password");
        cout << "tclapMap[PASSWORD] value " << tclapMap[PASSWORD] << endl;
    }
    else {
        string forbiddenS = config.getStringValue("Password");
        if (forbiddenS.compare(tclapMap[PASSWORD]) != 0) {
        perror("password doesn't match the original.\n");
        }
}
}
    else { // if password def does not exists
    perror("password definition is missing. Now exiting\n");
    exit(0);
}

// end of password exists
string numVersion = "NumVersions";
if (config.exists(numVersion.c_str())) {
    tclapMap[NUMVERSION] = config.getStringValue("NumVersions");
    cout << "tclapMap[NUMVERSION] value " << tclapMap[NUMVERSION] << endl;
    }
    else
    {
    perror("numVersion definition is missing. Now exiting\n");
    exit(0);
    }

    string watchDir = "WatchDir";
    if (config.exists(watchDir.c_str())) {
    if (tclapMap[WATCHDIR] == "") {
        tclapMap[WATCHDIR] = config.getStringValue("WatchDir");
        cout << "tclapMap[WATCHDIR] value " << tclapMap[WATCHDIR] << endl;
    }
    else {
        string forbiddenS = config.getStringValue("WatchDir");
        if (forbiddenS.compare(tclapMap[WATCHDIR]) != 0) {
        perror("WatchDir doesn't match the original\n");
        }
    }
}
else {
    perror("tclapMap[WATCHDIR] definition is missing. Now exiting\n");
    exit(0);
}

cout << "Done parsing with the config file" << endl;
// save changes
config.save();
} // end of if parameter section exists
else // if parameter section is empty
{
perror("Parameter section is missing. Now exiting\n");
exit(0);
}

} // end of loading conf file
else { // if conf file is missing
perror("conf file is missing. Error\n");
exit(0);
}

return;
} // end of parsing function
