#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

Process::Process(int pid){
    Process::process_id = pid;
}

// TODO: Return this process's ID
int Process::Pid() {
    return process_id;
 }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  long seconds=(LinuxParser::UpTime())-(LinuxParser::UpTime(process_id));
  long total_time=LinuxParser::ActiveJiffies(process_id);
  float cpu_usage = ((float)total_time/sysconf(_SC_CLK_TCK))/(float)seconds;
  return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(process_id); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(process_id); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(process_id);
 }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(process_id);
 }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  bool flag = stol(LinuxParser::Ram(process_id)) > stol(LinuxParser::Ram(a.process_id)); 
  return flag;
}