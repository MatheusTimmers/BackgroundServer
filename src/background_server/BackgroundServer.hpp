#ifndef _BACKGROUND_SERVER_HPP_
#define _BACKGROUND_SERVER_HPP_

#include "../enums/enums.cpp"
#include "../file_reader/FileReader.hpp"
#include <string>
#include <vector>

using namespace std;

class BackgroundServer {
private:
  unique_ptr<FileReader> file_;
  int time_;
  string grid_;

  vector<PeriodicTask> periodic_tasks_;

  void Run(int simulation_time);

  void ReadPeriodicTasks(int nTask);

public:
  BackgroundServer(string input_file);
  void StartServer();
};

#endif
