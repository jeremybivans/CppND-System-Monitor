#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { pid_ = pid; }
// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {  
  
  float cpuUsage = LinuxParser::CpuUtilization(pid_);
  
  return cpuUsage;
  
}

// DONE: Return the command that generated this process
string Process::Command() {
  
  string command = LinuxParser::Command(pid_);
  
  return command;
  
}

// DONE: Return this process's memory utilization
string Process::Ram() {
  
  string ram = LinuxParser::Ram(pid_);
 
  return ram;
  
}

// DONE: Return the user (name) that generated this process
string Process::User() {
 
  string user = LinuxParser::User(pid_);
  
  return user;
  
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
  
  long int upTime = LinuxParser::UpTime(pid_);
  
  return upTime;
  
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const &a) const {
  
  if (LinuxParser::CpuUtilization(a.pid_) < LinuxParser::CpuUtilization(pid_)) {
    return true;
  }
 
  return false;
  
}
