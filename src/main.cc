#include <unistd.h> //getpid()
#include <sys/types.h>
#include <sys/wait.h> // waitpid()
#include <iostream>
#include <thread>
#include <chrono>
#include "named_mutex.h"

using namespace std::chrono_literals;

void test1()
{
  const char* TEST1_BANNER = R"banner(
===============================================
2 x THREADS, SINGLE NAMED MUTEX OBJ
===============================================
)banner";

  std::cout << TEST1_BANNER;

  auto tm = tw::CreateNamedMutex("test", tw::NamedMutex::THREAD);
  if (tm->Init())
  {
    std::cout << "Mutex initialization failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  tm->UnLock();
  tm->Lock();
  std::cout << "MAIN: test->lock" << std::endl;

  std::thread threadObj([=]{
                          std::cout << "  CHILD: waiting for lock" << std::endl;
                          tm->Lock();
                          std::cout << "  CHILD: test->lock" << std::endl;
                          std::cout << "  CHILD: sleep 1 sec" << std::endl;
                          std::this_thread::sleep_for(1s);
                          std::cout << "  CHILD: test->unlock" << std::endl;
                          tm->UnLock();
                        });

  std::cout << "MAIN: sleep 5 sec" << std::endl;
  std::this_thread::sleep_for(5s);
  std::cout << "MAIN: test->unlock" << std::endl;
  tm->UnLock();
  std::cout << "MAIN: waiting for child thread to finish" << std::endl;
  threadObj.join();
  std::cout << "MAIN: done" << std::endl;
}

void test2()
{
  const char* TEST1_BANNER2 = R"banner(
===============================================
2 x THREADS, 2 x NAMED MUTEX OBJs
===============================================
)banner";

  std::cout << TEST1_BANNER2;

  auto tm = tw::CreateNamedMutex("test", tw::NamedMutex::THREAD);
  if (tm->Init())
  {
    std::cout << "Mutex initialization failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  tm->UnLock();
  tm->Lock();
  std::cout << "MAIN: test->lock" << std::endl;

  std::thread threadObjA([=]{
                           auto tm = tw::CreateNamedMutex("test", tw::NamedMutex::THREAD);
                          if (tm->Init())
                          {
                            std::cout << "Mutex initialization failed" << std::endl;
                            exit(EXIT_FAILURE);
                          }
                          std::cout << "  CHILD: waiting for lock" << std::endl;
                          tm->Lock();
                          std::cout << "  CHILD: test->lock" << std::endl;
                          std::cout << "  CHILD: sleep 1 sec" << std::endl;
                          std::this_thread::sleep_for(1s);
                          std::cout << "  CHILD: test->unlock" << std::endl;
                          tm->UnLock();
                        });

  std::cout << "MAIN: sleep 5 sec" << std::endl;
  std::this_thread::sleep_for(5s);
  std::cout << "MAIN: test->unlock" << std::endl;
  tm->UnLock();
  std::cout << "MAIN: waiting for child thread to finish" << std::endl;
  threadObjA.join();
  std::cout << "MAIN: done" << std::endl;
}

void test3()
{
  const char* TEST3_BANNER = R"banner(
===============================================
2 x THREADS, SINGLE NAMED MUTEX OBJ, SHARED MEM
===============================================
)banner";

  std::cout << TEST3_BANNER;

  auto pm = tw::CreateNamedMutex("test", tw::NamedMutex::PROCESS_OWNER);
  if (pm->Init())
  {
    std::cout << "Mutex initialization failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  pm->UnLock();
  pm->Lock();
  std::cout << "MAIN: test->lock" << std::endl;

  std::thread threadObj2([=]{
                           std::cout << "  CHILD: waiting for lock" << std::endl;
                           pm->Lock();
                           std::cout << "  CHILD: test->lock" << std::endl;
                           std::cout << "  CHILD: sleep 1 sec" << std::endl;
                           std::this_thread::sleep_for(1s);
                           std::cout << "  CHILD: test->unlock" << std::endl;
                           pm->UnLock();
                         });

  std::cout << "MAIN: sleep 5 sec" << std::endl;
  std::this_thread::sleep_for(5s);
  std::cout << "MAIN: test->unlock" << std::endl;
  pm->UnLock();
  std::cout << "MAIN: waiting for child thread to finish" << std::endl;
  threadObj2.join();
  std::cout << "MAIN: done" << std::endl;
}

void test4()
{
  const char* TEST4_BANNER = R"banner(
===============================================
2 x THREADS, 2 x NAMED MUTEX OBJ, 2 x PROCESSES
===============================================
)banner";

  std::cout << TEST4_BANNER;
  pid_t cpid, w;
  int wstatus;

  cpid = fork();
  if (cpid == -1)
  {
    std::cout << "Creating child process failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  if(cpid == 0)
  {
    // child process
    std::cout << "  CHILD_P: PID=" << getpid() << std::endl;
    std::cout << "  CHILD_P: sleep 1 sec" << std::endl;
    std::this_thread::sleep_for(1s);

    auto pm = tw::CreateNamedMutex("proc_test", tw::NamedMutex::PROCESS);
    if (pm->Init())
    {
      std::cout << "  CHILD_P: Mutex initialization failed" << std::endl;
      exit(EXIT_FAILURE);
    }
    std::cout << "  CHILD_P: waiting for lock" << std::endl;
    pm->Lock();
    std::cout << "  CHILD_P: test->lock" << std::endl;
    std::cout << "  CHILD_P: got lock, sleep 1 sec" << std::endl;
    std::this_thread::sleep_for(1s);
    std::cout << "  CHILD_P: test->unlock" << std::endl;
    pm->UnLock();

    printf("EXITING\n");
    exit(EXIT_SUCCESS);
  }
  else
  {
    // parent process
    std:: cout << "PARENT_P: PID=" << getpid() << std::endl;

    auto pm = tw::CreateNamedMutex("proc_test", tw::NamedMutex::PROCESS_OWNER);
    if (pm->Init())
    {
      std::cout << "PARENT_P: Mutex initialization failed" << std::endl;
      exit(EXIT_FAILURE);
    }
    pm->UnLock();
    pm->UnLock();
    pm->Lock();
    std::cout << "PARENT_P: test->lock" << std::endl;
    std::cout << "PARENT_P: sleep 5 sec" << std::endl;
    std::this_thread::sleep_for(5s);
    std::cout << "PARENT_P: test->unlock" << std::endl;
    pm->UnLock();

    std::cout << "PARENT_P: waiting for child process to finish" << std::endl;
    do {
      w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
      if (w == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
      }

      if (WIFEXITED(wstatus)) {
        std::cout << "exited, status=" << WEXITSTATUS(wstatus) << std::endl;
      } else if (WIFSIGNALED(wstatus)) {
        std::cout << "killed by signal " << WTERMSIG(wstatus) << std::endl;
      } else if (WIFSTOPPED(wstatus)) {
        std::cout << "stopped by signal " <<  WSTOPSIG(wstatus) << std::endl;
      } else if (WIFCONTINUED(wstatus)) {
        std::cout << "continued" << std::endl;
      }
    } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

    std::cout << "MAIN: done" << std::endl;
  }
}


int main(void) {
  test1();
  test2();
  test3();
  test4();

  exit(EXIT_SUCCESS);
}
