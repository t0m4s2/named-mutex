#include <thread>
#include <unistd.h>
#include "named_mutex.h"


int main(void) {
  tw::NamedMutex *m = tw::CreateNamedMutex("test", true);

  printf("MAIN: waiting for lock\n");
  m->Lock();
  printf("MAIN: got lock\n");

  std::thread threadObj([=]{
                          printf("  CHILD: waiting for lock\n");
                          m->Lock();
                          printf("  CHILD: got lock\n");
                          m->UnLock();
                          printf("  CHILD: unlock\n");
                        });

  printf("MAIN: sleep 5 sec\n");
  usleep(5000000);
  printf("MAIN: unlock\n");
  m->UnLock();
  printf("MAIN: waiting for child thread to finish\n");
  threadObj.join();
  delete m;
}
