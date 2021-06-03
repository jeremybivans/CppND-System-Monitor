#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {    
  
  	processes_.clear();

    vector<int> pIDs = LinuxParser::Pids();
  
	for(auto pID : pIDs) {
       
      Process thisPid(pID);
      
      if(thisPid.Command()=="") {
        
        continue;
        
      }
      else if(thisPid.Ram() == "0") {
      
      	continue;
      
      }
     
      else if(thisPid.CpuUtilization() == 0.00) {
      
      	continue;
      
      }
      
      else {
      
        processes_.emplace_back(thisPid);
        
      }
   
    }
  
    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { 

  string kernel = LinuxParser::Kernel();
  
  return kernel; 
}

// DONE: Return the system's memory utilization
float System::MemoryUtilization() {
 
  float systemMemUtil = LinuxParser::MemoryUtilization();
  
  return systemMemUtil;
}

// DONE: Return the operating system name
std::string System::OperatingSystem() {
 
  string systemOs = LinuxParser::OperatingSystem();
  
  return systemOs;
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() {
  
  int runningProcesses = LinuxParser::RunningProcesses();
  
  return runningProcesses;
}

// DONE: Return the total number of processes on the system
int System::TotalProcesses() {
  
  int totalProcesses = LinuxParser::TotalProcesses();
  
  return totalProcesses;
}

// DONE: Return the number of seconds since the system started running
long int System::UpTime() {
  
  long int upTime = LinuxParser::UpTime();
  
  return upTime;
}