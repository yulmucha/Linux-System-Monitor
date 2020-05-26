#include "linux_parser.h"
#include "processor.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
    const int IDLE = LinuxParser::IdleJiffies();
    const int ACTIVE = LinuxParser::ActiveJiffies();
    const int TOTAL = IDLE + ACTIVE;

    const float D_TOTAL = TOTAL - mPrevTotal;
    const float D_IDLE = IDLE - mPrevIdle;
    mPrevTotal = TOTAL;
    mPrevIdle = IDLE;

    return (D_TOTAL - D_IDLE) / D_TOTAL;
}