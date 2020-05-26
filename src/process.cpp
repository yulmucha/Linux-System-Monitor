#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : mPid(pid), mPrevTotal(0), mPrevActive(0) {
  mUser = LinuxParser::User(pid);
  mCommand = LinuxParser::Command(pid);
}

// Return this process's ID
int Process::Pid() { return mPid; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  const long ACTIVE = LinuxParser::ActiveJiffies(mPid) / sysconf(_SC_CLK_TCK);
  const long TOTAL = LinuxParser::UpTime() - LinuxParser::UpTime(mPid);

  const float D_ACTIVE = ACTIVE - mPrevActive;
  const float D_TOTAL = TOTAL - mPrevTotal;
  mPrevActive = ACTIVE;
  mPrevTotal = TOTAL;

  mCpuUtilization = D_ACTIVE / D_TOTAL;
  return mCpuUtilization;
}

// Return the command that generated this process
string Process::Command() { return mCommand; }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(mPid); }

// Return the user (name) that generated this process
string Process::User() { return mUser; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(mPid); }

bool Process::operator<(const Process& a) const {
  if (this->mCpuUtilization > a.mCpuUtilization) {
    return true;
  } else {
    return false;
  }
}