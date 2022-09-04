#include "format.h"

#include <string>
#include <iomanip>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { // [[maybe_unused]]) {
  int hours, minutes;

  hours = seconds / 3600;
  seconds = seconds % 3600;
  minutes = seconds / 60;
  seconds = seconds % 60;

  std::stringstream myStringStream {};
  myStringStream << std::setw(2) << std::setfill('0') << hours << std::setw(1)
                 << ":" << std::setw(2) << std::setfill('0') << minutes
                 << std::setw(1) << ":" << std::setw(2) << std::setfill('0')
                 << seconds;

  //return std::to_string(hours) + ":" + std::to_string(minutes) + ":" +
  //       std::to_string(seconds);
  return myStringStream.str();
}