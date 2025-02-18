#include "BackgroundServer.hpp"
#include <bits/stdc++.h>
#include <cctype>
#include <iostream>
#include <ostream>
#include <sstream>

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

  int T, TP, TA;
  string line;
  vector<string> lines;

  // Le todas as linhas do arquivo
  if (!this->file_->GetLines(lines)) {
    cerr << "Erro ao ler as linhas do arquivo" << endl;
    return;
  }

  size_t n_index = 0;

  while (n_index < lines.size()) {
    istringstream iss(lines[n_index]);
    iss >> T >> TP >> TA;

    if (T == 0 && TP == 0 && TA == 0) {
      cout << "Fim do arquivo" << endl;
      return;
    }

    this->periodic_tasks_.clear();
    this->aperiodic_tasks_.clear();

    n_index++; // Já leu a info da primeira linha. Proxima
    n_index++; // Pula a proxima linha. Tarefa servidora

    if (!ReadPeriodicTasks(n_index, lines, TP)) {
      cerr << "Erro na leitura de tarefas periodicas" << std::endl;
      return;
    }

    // FIXME: Caso tenha a mesma prioridade deveria usar aquele apresentado
    // primeiro, não sei se faz isso
    sort(this->periodic_tasks_.begin(), this->periodic_tasks_.end(), RmSort);

    if (!ReadAperiodicTasks(n_index, lines, TA)) {
      cerr << "Erro na leitura de tarefas aperiodicas" << std::endl;
      return;
    }

    if (!lines[n_index].empty()) {
      cerr << "Erro na leitura: Proxima linha deveria ser vazia" << endl;
      return;
    }

    n_index++; // Passa a linha em branco

    this->Run(T);

    cout << endl;
  }
}

void BackgroundServer::Run(int simulation_time) {
  std::vector<Task> tasks;
  int preemptions = 0;
  int context_switches = 0;
  int pid = 0;
  char symbol;
  this->grid_ = "";

  for (const auto &task : this->periodic_tasks_) {
    symbol = 'A' + (pid % 26);
    pid++;
    tasks.push_back({pid, symbol, task.computationTime, task.period,
                     task.deadline, task.period, task.deadline,
                     task.computationTime, -1, true});
  }

  for (const auto &task : aperiodic_tasks_) {
    symbol = 'A' + (pid % 26);
    pid++;
    tasks.push_back({pid, symbol, task.computationTime, -1, -1, -1, -1,
                     task.computationTime, task.arrivalTime, false});
  }

  Task *current_task = nullptr;
  int last_comp = 0;           // Para rastrear trocas de contexto
  char last_task_symbol = '.'; // Para rastrear trocas de contexto

  for (int t = 0; t < simulation_time; ++t) {
    // Como a lista esta ordenada, pega a primeira tarefa que ainda tem
    // computação para rodar
    current_task = nullptr;
    for (auto &task : tasks) {
      if (task.remaining_time > 0) {
        // incrementa troca de contextos
        if (last_task_symbol != task.symbol) {
          context_switches++;

          if (last_comp > 0) {
            preemptions++;
          }
        }

        if (task.is_periodic) {
          current_task = &task;
          break;
        } else if (task.arrival_time <= t) {
          current_task = &task;
          break;
        }
      }
    }

    // Processa essa task
    if (current_task) {
      if (current_task->is_periodic) {
        // é periodica, valida deadline
        if (current_task->absolute_deadline <= t) {
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
      if (task.absolute_period == t + 1) {
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

bool BackgroundServer::ReadPeriodicTasks(size_t &n_index, vector<string> &lines,
                                         int TP) {
  for (int i = 0; i < TP; ++i) {
    if (n_index >= lines.size()) {
      return false;
    }

    istringstream task(lines[n_index]);

    int Ci, Pi, Di;
    task >> Ci >> Pi >> Di;
    this->periodic_tasks_.push_back({Ci, Pi, Di});

    // avança para proxima linha
    n_index++;
  }
  return true;
}

bool BackgroundServer::ReadAperiodicTasks(size_t &n_index,
                                          vector<string> &lines, int TA) {
  for (int i = 0; i < TA; ++i) {
    if (n_index >= lines.size()) {
      return false;
    }

    istringstream task(lines[n_index]);

    int Ai, Ci;
    task >> Ai >> Ci;
    this->aperiodic_tasks_.push_back({Ai, Ci});

    // Avança para proxima linha
    n_index++;
  }
  return true;
}
