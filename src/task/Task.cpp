#include "Task.hpp"

std::string Task::ToString() const {
  std::ostringstream oss;
  oss << "(pid: " << pid
      << ", symbol: " << symbol
      << ", Cr: " << computation_time
      << ", Pr: " << period
      << ", Dr: " << deadline
      << ", Rcomp: " << remaining_computation
      << ", Pa: " << absolute_period
      << ", Da: " << absolute_deadline
      << ")";
  return oss.str();
}

void Task::Reload(int sim_time) {
  if (this->absolute_period == sim_time) {
    this->remaining_computation = this->computation_time;
    this->absolute_period += this->period;
    this->absolute_deadline += + this->period;
  }
}

double Task::GetUtilization() {
  return (double)this->computation_time / this->period;
}
