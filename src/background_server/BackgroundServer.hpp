#ifndef _BACKGROUND_SERVER_HPP_
#define _BACKGROUND_SERVER_HPP_

#include "../task/Task.hpp"
#include <vector>
#include <string>

class BackgroundServer {
private:
  std::string grid_;
  std::vector<Task> periodic_tasks_;

  Task* EDFSort();
  void Run(int simulation_time);

  void ReadPeriodicTasks(int nTask);
  void ReloadPeriodicTasks(int sim_time);
  std::string GetTestUtilization();

  void AddTaskToGrid(Task *current_task, int sim_time);

  void PrintResult(int context_switches, int preemptions);

public:
  void StartServer();
};

#endif
