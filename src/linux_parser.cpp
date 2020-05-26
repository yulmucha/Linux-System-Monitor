#include <dirent.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key;
  string value;
  string memTotal;
  string memFree;
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memTotal = value;
      } else if (key == "MemFree:") {
        memFree = value;
      } else {
        continue;
      }
    }
    float totalUsedMem = stof(memTotal) - stof(memFree);
    return totalUsedMem / stof(memTotal);
  }
  return 0.0;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string systemUptime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> systemUptime;
    return stol(systemUptime);
  }
  return 0;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  enum ProcessStates { utime = 14, stime, cutime, cstime };

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    string utime;
    string stime;
    string cutime;
    string cstime;

    string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string value;
    int i = 0;
    while (true) {
      switch (i) {
        case ProcessStates::utime - 1:
          linestream >> utime;
          break;
        case ProcessStates::stime - 1:
          linestream >> stime;
          break;
        case ProcessStates::cutime - 1:
          linestream >> cutime;
          break;
        case ProcessStates::cstime - 1:
          linestream >> cstime;
          break;
        default:
          linestream >> value;
          break;
      }
      i += 1;
      if (i == ProcessStates::cstime) {
        break;
      }
    }

    return stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
  }
  return 0;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> values = LinuxParser::CpuUtilization();
  const int ACTIVE_JIFFIES = stoi(values[CPUStates::kUser_])
                           + stoi(values[CPUStates::kNice_])
                           + stoi(values[CPUStates::kSystem_])
                           + stoi(values[CPUStates::kIRQ_])
                           + stoi(values[CPUStates::kSoftIRQ_])
                           + stoi(values[CPUStates::kSteal_]);
  return ACTIVE_JIFFIES;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> values = LinuxParser::CpuUtilization();
  const long IDLE_JIFFIES = stoi(values[CPUStates::kIdle_])
                          + stoi(values[CPUStates::kIOwait_]);
  return IDLE_JIFFIES;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> values;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  return values;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key;
  string value;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return stoi(value);
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key;
  string value;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return stoi(value);
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return string();
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string key;
  string value;
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        std::stringstream ss;
        ss << (int)(stof(value) / 1000.f);
        return ss.str();
      }
    }
  }
  return string();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string key;
  string value;
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return string();
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string username;
  string pwd;
  string uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> pwd >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return username;
        }
      }
    }
  }
  return string();
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  constexpr int STARTTIME = 22;
  string value;
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < STARTTIME; i++) {
      linestream >> value;
    }
    long clockTicks = stol(value);
    return clockTicks / sysconf(_SC_CLK_TCK);
  }
  return 0;
}