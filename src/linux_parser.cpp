#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string str, kb;
  string line;
  int i = 0;
  long int Total{}, value{}, free{};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) &&
           i < 2)  // Reading only first 2 lines
    {
      std::istringstream linestream(line);
      while (linestream >> str >> value >> kb) {
        if (str == "MemTotal:")
          Total = value;
        else if (str == "MemFree:")
          free = value;
      }
      i++;
    }
  }
  return float(Total - free) / (Total);
}


// TODO: Read and return the system uptime
long LinuxParser::UpTime() {  
 
  long upTime = 0;											//var for uptime needs to be long as the function is long
  
  string line;												//
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);	// src dir for upTime
  
  if (stream.is_open()) {
    
    std::getline(stream, line);
    
    std::istringstream linestream(line);
    
    linestream >> upTime;
    
  }
  
return upTime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {   
  	
  	string line, data;
  
    double value = 0.0;
  
    long jiffies = 0.0;
  
	std::ifstream stream(kProcDirectory + kStatFilename);
  
 	if (stream.is_open()) {
    
    	std::getline(stream, line);
    
     	std::istringstream linestream(line);
    
    	linestream >> data;
    
    	while (linestream >> value) {
     
      		jiffies += value;
   
    	}
 
 	}
  
 return jiffies;

}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {   
  
  long int value;
  
  long int activePidJiffies = 0.0;
  
  string line, key;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if (stream.is_open()) {
  
    std::getline(stream, line); 
    
    std::istringstream linestream(line);
    
    for (int i = 0; i < 13; ++i) linestream >> key;
    
    for (int i = 0; i < 4; ++i) {
     
      linestream >> value;
      
      activePidJiffies += value;
    
    }
    
    return activePidJiffies;
  
  }
 
  return activePidJiffies; 

}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {   
  
  string line, key;
  
  double value = 0.0;
 
  vector<int> data;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()) {
  
    std::getline(stream, line);
    
    std::istringstream linestream(line);
    
    linestream >> key;
    
    while (linestream >> value) {
      data.push_back(value);
    }
  
  }
  
  long activeJiffies =
     
    data.at(CPUStates::kUser_) + data.at(CPUStates::kNice_) +
     
    data.at(CPUStates::kSystem_) + data.at(CPUStates::kIRQ_) +
     
    data.at(CPUStates::kSoftIRQ_) + data.at(CPUStates::kSteal_);
  
  return activeJiffies;

}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
  string line, key;
  
  double value = 0.0;
  
  vector<int> data;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()) {
  
    std::getline(stream, line);
    
    std::istringstream linestream(line);
    
    linestream >> key;
    
    while (linestream >> value) {
      data.push_back(value);
    }
  
  }
  
  long idleJiffies = data.at(CPUStates::kIdle_) + data.at(CPUStates::kIOwait_);
  
  return idleJiffies;

}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 

  string line, key;
  
  int totalProc = 0;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()) {
  
  	while (std::getline(stream, line)){
    
    	std::istringstream linestream(line);
      	while (linestream >> key >> totalProc){
        
        	if (key == "processes") {
            return totalProc;
            }
          
       }
      
    }
   
  }
  
return totalProc; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
 
  string line, key;
  
  int runProc = 0;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()) {
  
  	while (std::getline(stream, line)){
    
    	std::istringstream linestream(line);
      	while (linestream >> key >> runProc){
        
        	if (key == "procs_running") {
            return runProc;
            }
        }
      
    }
   
  }
  
return runProc; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {  
  
  string line{};
 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  
  if(stream.is_open())
  
   	std::getline(stream,line);

  return line; 

}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
  string line, key, value;
  
  vector<string> cputime{};
 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  
  if (stream.is_open()) {
    
    while (std::getline(stream, line)) {
     
      std::istringstream linestream(line);
      
      while (linestream >> key >> value) {
       
        if (key == "VmSize:") 
        return value;
        break;
     
      }
    
    }
 
  }

  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { return 0; }
