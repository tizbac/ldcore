#include "XThread.h"
#include <time.h>
#include <sys/time.h>
XThread::XThread(void *(*f)(void *),void * arg)
{
  //sLog.outDebug("Spawning new thread");
  int err = pthread_create(&tid,NULL,f,arg);
  if ( err != 0)
  {
    //sLog.outError("FATAL: Thread failed to spawn");
    //abort();
  }
  
}
void XThread::Join()
{
  pthread_join(tid,NULL);
}
void XThread::Kill()
{
  pthread_cancel(tid);
}
XThread::~XThread()
{

}
void XThread::SendSignal(int signal)
{
  pthread_kill(tid,signal);
}

XMutex::XMutex()
{
  if (pthread_mutex_init(&mid,NULL) != 0)
  {
    //sLog.outError("Cannot create mutex");
  }
}
XMutex::~XMutex()
{
  pthread_mutex_destroy(&mid);
}
void XMutex::Lock()
{
  pthread_mutex_lock(&mid);
}
void XMutex::UnLock()
{
  pthread_mutex_unlock(&mid);
}
void XMutex::TryLock()
{
   pthread_mutex_trylock(&mid);
}
int XMutex::TimedLock(unsigned int seconds)
{
   struct timeval tv;

  gettimeofday(&tv,NULL);
  struct timespec t;
  t.tv_sec = tv.tv_sec;
  t.tv_nsec = tv.tv_usec * 1000;
  t.tv_sec += seconds;
   pthread_mutex_timedlock(&mid,&t);
}


Condition::Condition()
{
  pthread_cond_init(&cid,NULL);
}
int Condition::TimedWait(XMutex* m, unsigned int seconds)
{
  struct timeval tv;

  gettimeofday(&tv,NULL);
  struct timespec t;
  t.tv_sec = tv.tv_sec;
  t.tv_nsec = tv.tv_usec * 1000;
  t.tv_sec += seconds;
  return pthread_cond_timedwait(&cid,&m->mid,&t);
}
void Condition::Wait(XMutex* m)
{
  pthread_cond_wait(&cid,&m->mid);
}
void Condition::Broadcast()
{
  pthread_cond_broadcast(&cid);
}

Condition::~Condition()
{
  pthread_cond_destroy(&cid);
}

