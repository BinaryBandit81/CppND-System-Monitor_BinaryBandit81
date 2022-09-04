#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

/*using std::set;
using std::size_t;
using std::string;
using std::vector;
*/
namespace std;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  vector<Process> newProcesses;

  // Iterate over all processes
  int i = 0;
  for (auto proc : processes_) {
    // try to find pid in processes
    if (std::find(pids.begin(), pids.end(), proc.Pid()) != pids.end()) {
      // pid found in processes --> push to new list & remove from pid List
      newProcesses.push_back(proc);
      pids.erase(std::find(pids.begin(), pids.end(), proc.Pid()));
    }
    i++;
  }
  processes_.assign(newProcesses.begin(), newProcesses.end());

  // Iterate over all new pids --> create process and push it on vector
  for (auto pid : pids) {
    //Process proc(pid);
    // processes_.push_back(proc);
    processes_.emplace_back(pid);
  }

  sort(processes_.begin(), processes_.end());
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
