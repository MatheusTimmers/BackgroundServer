struct PeriodicTask {
  int computationTime;
  int period;
  int deadline;
};

struct AperiodicTask {
  int arrivalTime;
  int computationTime;
};

struct Task {
  int pid;
  char symbol;
  // periodic
  int computation_time;
  int period;
  int deadline;
  int absolute_period;
  int absolute_deadline;
  // aux
  int remaining_time;
  // aperiodic
  int arrival_time;
  bool is_periodic;
};
