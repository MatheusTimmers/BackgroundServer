#include "BackgroundServer.hpp"

#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

void BackgroundServer::StartServer() {
  int numTask, SimTime;
  size_t n_index = 0;

  while (true) {
    cin >> numTask >> SimTime;

    if (numTask == 0 && SimTime == 0) {
      break;
    }

    ReadPeriodicTasks(numTask);

    this->Run(SimTime);

    cout << endl;
  }
}

void BackgroundServer::Run(int simulation_time) {
  int preemptions = 0;
  int context_switches = 0;

  // Para rastrear trocas de contexto
  char last_task_symbol = '.';
  int last_comp = 0;
  
  Task *current_task = nullptr;

  for (int sim_time = 1; sim_time <= simulation_time; sim_time++) {
    current_task = this->EDFSort();

    if (current_task != nullptr) {
      // Verifica se ocorreu preempção ou troca de contexto
      if (last_task_symbol != current_task->symbol) {
        context_switches++;
        last_task_symbol = current_task->symbol;

        if (last_comp > 0) {
          preemptions++;
        }
      }

      // Adiciona a tarefa ao grid
      this->AddTaskToGrid(current_task, sim_time);

      current_task->remaining_computation--;
      last_comp = current_task->remaining_computation;
    } else {
      // Adiciona Idle
      this->AddTaskToGrid(nullptr, sim_time);
      
      // Verifica se ocorreu troca de contexto
      if (last_task_symbol != '.') {
        context_switches++;
        last_task_symbol = '.';
		preemptions++;
      }
    }

    this->ReloadPeriodicTasks(sim_time, &last_task_symbol);
  }

  this->PrintResult(context_switches, preemptions);
}

void BackgroundServer::ReadPeriodicTasks(int nTask) {
  int Ci, Pi, Di;
  char symbol;
  
  this->periodic_tasks_.clear();
  this->grid_ = "";

  for (int i = 0; i < nTask; ++i) {
    cin >> Ci >> Pi >> Di;
    symbol = 'A' + (i % 26);

    this->periodic_tasks_.push_back({i, symbol, Ci, Pi, Di, Ci, Pi, Di});
  }
};

void BackgroundServer::ReloadPeriodicTasks(int sim_time, char *last_task_symbol) {
  for (auto &task : this->periodic_tasks_) {
    // Se o periodo absoluto for igual ao simulation_time e 
    // last_task_symbol for igual ao symbol da tarefa
    // significa que teve troca de contexto entre periodos
    if ((task.absolute_period == sim_time) && (task.symbol == *last_task_symbol)) {
      *last_task_symbol = ' ';
	}

    task.Reload(sim_time);
  }
}

void BackgroundServer::AddTaskToGrid(Task *current_task, int sim_time) {
  if (current_task == nullptr) {
    this->grid_.append(1, '.');
    return;
  }

  if (sim_time > current_task->absolute_deadline) {
    this->grid_.append(1, tolower(current_task->symbol));
  } else {
    this->grid_.append(1, current_task->symbol);
  }
}

string BackgroundServer::GetTestUtilization() {
  double utilization = 0.0;
  string result = "";

  for (auto &task : this->periodic_tasks_) {
    utilization += task.GetUtilization();
  }

  if (utilization <= 1.0) {
    result = "OK";
  } else {
    result = "NOK";
  }

  stringstream ss;
  ss << fixed << setprecision(4) << utilization << " " << result;
  return ss.str();
}

void BackgroundServer::PrintResult(int context_switches, int preemptions) {
  cout << this->grid_ << endl;
  cout << context_switches << " " << preemptions << endl;
  cout << this->GetTestUtilization() << endl;
}

Task* BackgroundServer::EDFSort() {
  int min_deadline = std::numeric_limits<int>::max();
  Task* current_task = nullptr;

  for (auto &task : this->periodic_tasks_) {    
    // Pega a tarefa com o deadline mais proximo e que ainda tenha computacao restante
    if (task.remaining_computation > 0) {
      if (task.absolute_deadline < min_deadline) {
        min_deadline = task.absolute_deadline;
        current_task = &task;
      }
    }
  }

  return current_task;
};
