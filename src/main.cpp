#include "background_server/BackgroundServer.hpp"

int main() {
  BackgroundServer *bs = new BackgroundServer("test/teste.txt");

  bs->StartServer();

  delete bs;

  return 0;
}
