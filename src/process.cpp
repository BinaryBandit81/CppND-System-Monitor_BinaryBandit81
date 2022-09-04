#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return this->pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  this->lstCpu.assign(this->curCpu.begin(), this->curCpu.end());
  this->curCpu = LinuxParser::CpuUtilization(this->pid, this->lstCpu);
  return this->curCpu[2];
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(this->pid).substr(0,40); }

// TODO: Return this process's memory utilization
string Process::Ram() {
  this->curRam = stoi(LinuxParser::Ram(this->pid));
  return to_string(this->curRam);
}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(this->pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  return LinuxParser::UpTime(this->pid) - LinuxParser::UpTime();
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return this->curCpu[2] > a.curCpu[2];
  // return this->curRam > a.curRam;
  // return this->pid < a.pid;
}