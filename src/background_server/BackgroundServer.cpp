#include "BackgroundServer.hpp"
#include <bits/stdc++.h>
#include <cctype>
#include <iostream>
#include <ostream>
#include <sstream>

using namespace std;

// Aqui tem que alterar mudou o algoritmo
bool RmSort(PeriodicTask t1, PeriodicTask t2) {
  return (t1.period <= t2.period);
};

BackgroundServer::BackgroundServer(string input_file) {
  this->file_ = make_unique<FileReader>(input_file);
  this->grid_ = "";
  this->time_ = 0;
}

void BackgroundServer::StartServer() {
  if (!this->file_->IsOpen()) {
    cerr << "Erro ao abrir o arquivo: " << endl;
    return;
  }

  // Escolher nomes melhores
  int numTask, SimTime;
  string line;
  vector<string> lines;

  size_t n_index = 0;

  while (n_index < lines.size()) {
    cin >> numTask >> SimTime;

    if (numTask == 0 && SimTime == 0) {
      cout << "Fim do arquivo" << endl;
      return;
    }

    ReadPeriodicTasks(numTask);

    this->Run(SimTime);

    cout << endl;
  }
}

void BackgroundServer::Run(int simulation_time) {
  std::vector<Task> tasks_aux;
  int preemptions = 0;
  int context_switches = 0;
  int pid = 0;
  char symbol;
  this->grid_ = "";

  for (const auto &task : this->periodic_tasks_) {
    symbol = 'A' + (pid % 26);
    pid++;
    tasks_aux.push_back({pid, symbol, task.computationTime, task.period, task.deadline, 
                         task.period, task.deadline,
                         task.computationTime});
  }

  Task *current_task = nullptr;
  int last_comp = 0;           // Para rastrear trocas de contexto
  char last_task_symbol = '.'; // Para rastrear trocas de contexto

  for (int numTask = 0; numTask < simulation_time; ++numTask) {
    // Como a lista esta ordenada, pega a primeira tarefa que ainda tem
    // computação para rodar
    current_task = nullptr;
    for (auto &task : tasks_aux) {
      if (task.remaining_computation > 0) {
        // A primeira tarefa que tem tempo de computacao restante é diferente da que ta atualizando
        if (last_task_symbol != task.symbol) {
          context_switches++;

          if (last_comp > 0) {
            preemptions++;
          }
        }

          current_task = &task;
      }
    }

    // Processa essa task
    if (current_task) {
      if (current_task->is_periodic) {
        // é periodica, valida deadline
        if (current_task->absolute_deadline <= numTask) {
          this->grid_.append(1, tolower(current_task->symbol));
        } else {
          this->grid_.append(1, current_task->symbol);
        }
      } else {
        // aperiodica só printa
        this->grid_.append(1, current_task->symbol);
      }
      current_task->remaining_time--;
      last_task_symbol = current_task->symbol;
      last_comp = current_task->remaining_time;
    } else {
      this->grid_.append(1, '.');
    }

    // Reload
    for (auto &task : tasks) {
      if (task.absolute_period == numTask + 1) {
        task.remaining_time = task.computation_time;
        task.absolute_period = task.absolute_period + task.period;
        task.absolute_deadline = task.absolute_deadline + task.deadline;
      }
    }
  }

  cout << this->grid_ << endl;
  cout << preemptions + 1 << " " << context_switches + 1 << endl;

  for (const auto &task : tasks) {
    int execution_time = 0;
    int waiting_time = 0;
    if (task.is_periodic) {
      execution_time = task.computation_time * (simulation_time / task.period);
      waiting_time = simulation_time - execution_time;
    } else {
      execution_time = task.computation_time * (simulation_time / task.period);
      waiting_time = simulation_time - execution_time;
    }

    cout << execution_time << " " << waiting_time << endl;
  }
}

void BackgroundServer::ReadPeriodicTasks(int nTask) {
  int Ci, Pi, Di;
  this->periodic_tasks_.clear();

  for (int i = 0; i < nTask; ++i) {
    cin >> Ci >> Pi >> Di;

    this->periodic_tasks_.push_back({Ci, Pi, Di});
  }

  sort(this->periodic_tasks_.begin(), this->periodic_tasks_.end(), RmSort);
}