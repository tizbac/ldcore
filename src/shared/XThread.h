#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include <iostream>
#include <map>
//#include <Log.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/times.h>
using namespace std;
class XThread{
  public:
  pthread_t tid;
  
  XThread(void *(*f)(void *),void * arg);
  void Kill();
  ~XThread();
  void Join();
};
class XMutex{
  public:
  pthread_mutex_t mid;
  XMutex();
  void Lock();

  int TimedLock(unsigned int seconds);
  void UnLock();
  void TryLock();
  ~XMutex();
};
class Condition{
  public:
    pthread_cond_t cid;
    void Broadcast();
    Condition();
    ~Condition();
    void Wait(XMutex * m);
    int TimedWait(XMutex * m,unsigned int seconds);
};
#endif