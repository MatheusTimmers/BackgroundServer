#ifndef FILE_HPP
#define FILE_HPP

#include <string>

#include <fstream>
#include <memory>
#include <vector>

using namespace std;

class FileReader {
private:
  string path_;
  unique_ptr<ifstream> file_;
  bool OpenRead();

public:
  FileReader(std::string path_file);
  ~FileReader();

  int GetNunLines();
  bool GetLines(vector<string> &lines);
  bool GetLine(string &line);
  bool IsOpen();
};

#endif
