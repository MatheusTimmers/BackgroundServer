#include "FileReader.hpp"

#include <iostream>

FileReader::FileReader(std::string path_file) {
  this->path_ = path_file;
  this->OpenRead();
}

FileReader::~FileReader() {
  if (this->IsOpen()) {
    file_->close();
  }
}

bool FileReader::OpenRead() {
  this->file_ = make_unique<ifstream>(this->path_, ios::in);

  if (!this->IsOpen()) {
    std::cerr << "erro ao abrir arquivo " << this->path_ << endl;
    return false;
  }

  return true;
}

int FileReader::GetNunLines() {
  int count = 0;
  string line;

  if (!this->IsOpen())
    return false;

  while (getline(*this->file_, line))
    count++;

  return count;
}

bool FileReader::GetLines(vector<string> &lines) {
  try {
    if (!this->IsOpen())
      return false;

    this->file_->seekg(0, ios::beg);

    string line;
    while (getline(*this->file_, line)) {
      lines.push_back(line);
    }

    return true;
  } catch (const exception &e) {
    cerr << "Erro: " << e.what() << endl;
    return false;
  }
}

bool FileReader::GetLine(string &line) {
  try {
    if (!this->IsOpen())
      return false;

    if (!getline(*this->file_, line)) {
      this->file_->close();
      return false;
    }

    return true;
  } catch (const exception &e) {
    cerr << "Erro: " << e.what() << endl;
    return false;
  }
}

bool FileReader::IsOpen() { return this->file_ && this->file_->is_open(); }
