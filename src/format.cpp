#include <sstream>
#include <string>

#include "format.h"

using std::string;
using std::stringstream;
using std::to_string;

string Format::ElapsedTime(long seconds) {
  int h = seconds / 60 / 60;
  string HH = (h < 10) ? "0" + to_string(h) : to_string(h);
  int m = (seconds / 60) - (h * 60);
  string MM = (m < 10) ? "0" + to_string(m) : to_string(m);
  int s = seconds - (seconds / 60 * 60);
  string SS = (s < 10) ? "0" + to_string(s) : to_string(s);
  
  stringstream ss;
  ss << HH << ":" << MM << ":" << SS;
  
  return ss.str();
}