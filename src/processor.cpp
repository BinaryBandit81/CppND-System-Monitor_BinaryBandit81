#include "processor.h"

#include <string>

#include "linux_parser.h"
using std::string;
#include <vector>
using std::vector;

//#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  // single values
  int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  int counter = 0;
  vector<string> cpuUtil = LinuxParser::CpuUtilization();

  user = stoi(cpuUtil[counter++]);
  nice = stoi(cpuUtil[counter++]);
  system = stoi(cpuUtil[counter++]);
  idle = stoi(cpuUtil[counter++]);
  iowait = stoi(cpuUtil[counter++]);
  irq = stoi(cpuUtil[counter++]);
  softirq = stoi(cpuUtil[counter++]);
  steal = stoi(cpuUtil[counter++]);
  guest = stoi(cpuUtil[counter++]);
  guest_nice = stoi(cpuUtil[counter++]);

  // std::cerr << "util: " << user <<" "<< nice <<" "<< system <<" "<< idle <<
  // "\n";

  // summarized values
  int usertime, nicetime, idleall, systemall, virtall, total;
  usertime = user - guest;
  nicetime = nice - guest_nice;
  idleall = idle + iowait;
  systemall = system + irq + softirq;
  virtall = guest + guest_nice;
  total = usertime + nicetime + systemall + idleall + steal + virtall;

  // calc percentage
  int deltaTotal, deltaIdle;
  deltaTotal = total - this->prevTotal;
  deltaIdle = idleall - this->prevIdle;
  this->prevTotal = total;
  this->prevIdle = idleall;

  return float(deltaTotal - deltaIdle) / deltaTotal;
}