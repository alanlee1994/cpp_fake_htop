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

Process::Process(int pid){Process::pid_ = pid;}
// TODO: Return this process's ID
int Process::Pid() { return pid_; }

float Process::CpuUtilization() { 
  long total_time=LinuxParser::ActiveJiffies(pid_);
  long seconds=(LinuxParser::UpTime())-(LinuxParser::UpTime(pid_));
  float cpu_usage = ((float)total_time/sysconf(_SC_CLK_TCK))/(float)seconds;
  return cpu_usage;
  
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return stol(LinuxParser::Ram(pid_)) > stol(LinuxParser::Ram(a.pid_)); 
}