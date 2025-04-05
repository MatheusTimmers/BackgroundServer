#include "background_server/BackgroundServer.hpp"

int main() {
  BackgroundServer *bs = new BackgroundServer();

  bs->StartServer();

  delete bs;

  return 0;
}
