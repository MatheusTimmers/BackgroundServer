#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <string>
#include <sstream>

class Task {
public:
  int pid;
  char symbol;
  int computation_time;
  int period;
  int deadline;
  // aux
  int remaining_computation;
  int absolute_period;
  int absolute_deadline;

  std::string ToString() const;
  double GetUtilization();
  void Reload(int sim_time);
};

#endif