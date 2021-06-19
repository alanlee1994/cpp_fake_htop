#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::vector;
using std::fixed;
using std::setprecision;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key,value,line;
  float totalMemory=0.0;
  float freeMemory=0.0;
  float default_result  = 0.0;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key=="MemTotal:"){
        totalMemory = stof(value);
      }
      if (key=="MemFree:"){
        freeMemory = stof(value);
      }
    }
    return (totalMemory-freeMemory)/totalMemory;
  }
  return default_result;
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string ut, line;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if (stream.is_open()){
     std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> ut;
    return stol(ut);
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long total_jiffles =0.0;
  vector<string> cpu_utilization = LinuxParser::CpuUtilization();
  for (int i=kUser_; i<kSteal_;i++){
    total_jiffles += stol(cpu_utilization[i]);
  }
  return total_jiffles;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long active_jiffles=0.0;
  string value, line;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    int count=0;
    while(linestream >>value){
      if (count > 12 && count <17){
        active_jiffles += stol(value);
      }
      count += 1;
    }
  }
  return active_jiffles;
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
	 return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long idlejiffies = 0;
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  for(int i = kIdle_; i <= kIOwait_; i++) {
    idlejiffies += stol(cpuUtilization[i]);
  }
  return idlejiffies; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_utilization;
  string key, line;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream>>key;
      if (key == "cpu"){
        while(linestream>>key){
          cpu_utilization.push_back(key);
        }
      }
    }
  }
  return cpu_utilization;
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string key, value, line;
    std::ifstream stream(kProcDirectory+kStatFilename);
    if (stream.is_open()){
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> key >> value;
        if (key == "processes"){
          if (value!=""){
              return stoi(value);
          }
        }
    }
    return 0;
  }
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string key, value, line;
    std::ifstream stream(kProcDirectory+kStatFilename);
    if (stream.is_open()){
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> key >> value;
        if (key == "procs_running"){
          if (value!=""){
              return stoi(value);
          }
        }
    }
    return 0;
  }
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
	string command;
    std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if (stream.is_open()) {
      std::getline(stream, command);
      return command;
    }
    return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::stringstream ram;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
       ram << std::fixed << std::setprecision(3) << stof(value) / 1000; 
       return ram.str();
      }
    }
  }
  return "0";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line, key, _ , value;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
   while (std::getline(stream, line)) {
     std::replace(line.begin(), line.end(), ':', ' ');
     std::istringstream linestream(line);
     linestream >> key >> _ >> value;
     if (value == uid) {
       return key;
     }
   }
  }
  return string();   
} 
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
	int i = 0;
    while (linestream >> value) {
      if (i == 21) {
        return stol(value) / sysconf(_SC_CLK_TCK);
      }
      i+=1;
    }
  }
  return 0;
}