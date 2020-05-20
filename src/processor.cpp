#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

using std::stoi;
using std::string;
using std::vector;

float Processor::Utilization() {
    vector<string> values = LinuxParser::CpuUtilization();
    const int USER_VAL = stoi(values[0]);
    const int NICE_VAL = stoi(values[1]);
    const int SYSTEM_VAL = stoi(values[2]);
    const int IDLE_VAL = stoi(values[3]);
    const int IOWAIT_VAL = stoi(values[4]);
    const int IRQ_VAL = stoi(values[5]);
    const int SOFTIRQ_VAL = stoi(values[6]);
    const int STEAL_VAL = stoi(values[7]);
    // const int GUEST_VAL = stoi(values[8]);
    // const int GUEST_NICE_VAL = stoi(values[9]);

    const int IDLE = IDLE_VAL + IOWAIT_VAL;
    const int NON_IDLE = USER_VAL + NICE_VAL + SYSTEM_VAL + IRQ_VAL + SOFTIRQ_VAL + STEAL_VAL;
    const int TOTAL = IDLE + NON_IDLE;
    const float D_TOTAL = TOTAL - mPrevTotal;
    const float D_IDLE = IDLE - mPrevIdle;
    const float UTIL_PERCENTAGE = (D_TOTAL - D_IDLE) / D_TOTAL;

    mPrevTotal = TOTAL;
    mPrevIdle = IDLE;
    return UTIL_PERCENTAGE;
}