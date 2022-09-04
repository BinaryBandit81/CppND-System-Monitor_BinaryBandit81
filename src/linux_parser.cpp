#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

string LinuxParser::readFile(string filename, string keytoFind) {
  string key, value;
  string line;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == keytoFind) return value;
    }
  }
  return "-1";
}
std::string LinuxParser::readLine(std::string filename, int lineToGet,
                                  int elementToGet) {
  string line;
  std::istringstream linestream;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    // get to desired line
    for (int i = 0; i < lineToGet; i++) {
      std::getline(stream, line);
    }
    if (elementToGet == -1)
      return line;
    else {
      int iCounter = 0;
      std::istringstream linestream(line);
      for (string curElement; linestream >> curElement;) {
        iCounter++;
        if (iCounter == elementToGet) return curElement;
      }
    }
  }
  return "";
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  int memTotal, memFree;
  memTotal = std::stoi(readFile(kProcDirectory + kMeminfoFilename, filterMemTotalString));
  memFree = std::stoi(readFile(kProcDirectory + kMeminfoFilename,  filterMemFreeString));
  return float(memTotal - memFree) / memTotal;
}
// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string trash;
  std::istringstream linestream(readLine(kProcDirectory + kUptimeFilename, 1));
  linestream >> uptime >> trash;
  return uptime;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string key, cpuUtilValue;
  vector<string> cpuUtil;
  std::istringstream stream(readLine(kProcDirectory + kStatFilename, 1));
  for (string cpuUtilValue; stream >> cpuUtilValue;) {
    if (cpuUtilValue != filterCpu) cpuUtil.push_back(cpuUtilValue);
  }
  return cpuUtil;
}

vector<double> LinuxParser::CpuUtilization(int pid) {
  vector<double> dummy(3, -1.0);
  return CpuUtilization(pid, dummy);
}

vector<double> LinuxParser::CpuUtilization(int pid, vector<double> lastUti) {
  vector<double> returnVector;

  int utime =
      stoi(readLine(kProcDirectory + to_string(pid) + kStatFilename, 1, 14));
  int stime =
      stoi(readLine(kProcDirectory + to_string(pid) + kStatFilename, 1, 15));
  int cutime =
      stoi(readLine(kProcDirectory + to_string(pid) + kStatFilename, 1, 16));
  int cstime =
      stoi(readLine(kProcDirectory + to_string(pid) + kStatFilename, 1, 17));
  int starttime =
      stoi(readLine(kProcDirectory + to_string(pid) + kStatFilename, 1, 22));

  double totaltime = utime + stime + cutime + cstime;
  double seconds = UpTime() - (starttime / sysconf(_SC_CLK_TCK));

  returnVector.push_back(totaltime);
  returnVector.push_back(seconds);

  if (lastUti[0] < 0.0) {
    returnVector.push_back(((totaltime / sysconf(_SC_CLK_TCK)) / seconds));
    return returnVector;  // *100;
  } else {
    returnVector.push_back(
        std::max((returnVector[0] - lastUti[0]) / sysconf(_SC_CLK_TCK) /
                     (returnVector[1] - lastUti[1]),
                 0.0));
    return returnVector;
  }
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return std::stoi(readFile(kProcDirectory + kStatFilename, filterProcesses));
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return std::stoi(readFile(kProcDirectory + kStatFilename, filterRunningProcesses));
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) {
  return readLine(kProcDirectory + to_string(pid) + kCmdlineFilename, 1);
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {  //[[maybe_unused]]) {
    // using VmRSS instead of VmSize to get the physical ram use
    return to_string(
      stoi(readFile(kProcDirectory + to_string(pid) + kStatusFilename,
                    filterProcMem)) /
      1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) {
  return readFile(kProcDirectory + to_string(pid) + kStatusFilename, filterUID);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) {
  string uidToFind = Uid(pid);

  string user, trash, uid;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> trash >> uid;
      if (uid == uidToFind) return user;
    }
  }
  return uidToFind;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long starttime =
      stol(readLine(kProcDirectory + to_string(pid) + kStatFilename, 1, 22));

  return starttime / sysconf(_SC_CLK_TCK);
}
