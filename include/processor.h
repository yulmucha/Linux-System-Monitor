#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor() : mPrevTotal(0), mPrevIdle(0) {}
  float Utilization();

 private:
  long mPrevTotal;
  long mPrevIdle;
};

#endif