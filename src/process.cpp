#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(
    int pid,
    std::string user,
    std::string command,
    float cpuUtilization,
    std::string ram,
    long int upTime)
    : mPid(pid)
    , mUser(user)
    , mCommand(command)
    , mCpuUtiliaztion(cpuUtilization)
    , mRam(ram)
    , mUpTime(upTime)
{
}

int Process::Pid() { return mPid; }

float Process::CpuUtilization() { return mCpuUtiliaztion; }

string Process::Command() { return mCommand; }

string Process::Ram() { return mRam; }

string Process::User() { return mUser; }

long int Process::UpTime() { return mUpTime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }