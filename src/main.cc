#include <thread>
#include <unistd.h> // usleep
#include <iostream>
#include "named_mutex.h"


int main(void) {

  const char* THREAD_LOCK_BANNER = R"banner(
 _   _                        _   _            _
| |_| |__  _ __ ___  __ _  __| | | | ___   ___| | __
| __| '_ \| '__/ _ \/ _` |/ _` | | |/ _ \ / __| |/ /
| |_| | | | | |  __/ (_| | (_| | | | (_) | (__|   <
 \__|_| |_|_|  \___|\__,_|\__,_| |_|\___/ \___|_|\_\

)banner";

  std::cout << THREAD_LOCK_BANNER;

  auto tm = tw::CreateNamedMutex("test", false);
  if (tm->Init())
  {
    std::cout << "Mutex initialization failed" << std::endl;
    return -1;
  }
  std::cout << "MAIN: created mutex \"test\'" << std::endl;

  std::cout << "MAIN: test->lock" << std::endl;
  tm->Lock();
  std::cout << "MAIN: got lock" << std::endl;

  std::thread threadObj([=]{
                          std::cout << "  CHILD: test->lock" << std::endl;
                          tm->Lock();
                          std::cout << "  CHILD: got lock" << std::endl;
                          tm->UnLock();
                          std::cout << "  CHILD: test->unlock" << std::endl;
                        });

  std::cout << "MAIN: sleep 5 sec" << std::endl;
  usleep(5000000);
  std::cout << "MAIN: test->unlock" << std::endl;
  tm->UnLock();
  std::cout << "MAIN: waiting for child thread to finish" << std::endl;
  threadObj.join();

  const char* PROCESS_LOCK_BANNER = R"banner(

                                     _            _
 _ __  _ __ ___   ___ ___  ___ ___  | | ___   ___| | __
| '_ \| '__/ _ \ / __/ _ \/ __/ __| | |/ _ \ / __| |/ /
| |_) | | | (_) | (_|  __/\__ \__ \ | | (_) | (__|   <
| .__/|_|  \___/ \___\___||___/___/ |_|\___/ \___|_|\_\
|_|

)banner";

  std::cout << PROCESS_LOCK_BANNER;

  auto pm = tw::CreateNamedMutex("test", false);
  if (pm->Init())
  {
    std::cout << "Mutex initialization failed" << std::endl;
    return -1;
  }
  std::cout << "MAIN: created mutex \"test\'" << std::endl;

  std::cout << "MAIN: test->lock" << std::endl;
  pm->Lock();
  std::cout << "MAIN: got lock" << std::endl;

  std::thread threadObj2([=]{
                          std::cout << "  CHILD: test->lock" << std::endl;
                          pm->Lock();
                          std::cout << "  CHILD: got lock" << std::endl;
                          pm->UnLock();
                          std::cout << "  CHILD: test->unlock" << std::endl;
                        });

  std::cout << "MAIN: sleep 5 sec" << std::endl;
  usleep(5000000);
  std::cout << "MAIN: test->unlock" << std::endl;
  pm->UnLock();
  std::cout << "MAIN: waiting for child thread to finish" << std::endl;
  threadObj2.join();

}
