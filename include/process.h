#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(
    int pid,
    std::string user,
    std::string command,
    float cpuUtilization,
    std::string ram,
    long int upTime);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
  int mPid;
  std::string mUser;
  std::string mCommand;
  float mCpuUtiliaztion;
  std::string mRam;
  long int mUpTime;
};

#endif