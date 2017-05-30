#include "worker_pool.h"
#include "stdlib.h"

/*
void worker_pool::run(worker_task task) {
   jq->push(task);
}
*/

void *worker_pool::per_worker_task(void *arg) {
   printf("thread %ld running\n", (long)pthread_self());
   worker_pool *wp = (worker_pool *)arg;
   /*
   if (wp->worker_init_callback) {
      wp->worker_init_callback(NULL);
   } 
   */
   while (wp->running) {
      printf("%ld waiting element from queue\n", pthread_self());
      queue_element* qe = (queue_element *)wp->jq->pop();
      printf("%ld getting element from queue\n", pthread_self());
      //printf("%ld getting element : %d\n", pthread_self(), qe->fd);
      if (wp->worker_init_callback) {
         //printf("here?\n");
         wp->worker_init_callback(NULL);
      } else {
         //wp->exe_work((void *)qe);
         qe->_cb(qe->arg);
         delete qe;
      }
      //TODO
      //wp->worker_init_callback(qe);
   }
   return NULL;
}

void thread_worker_pool::start() {
   threads.resize(worker_num);
   running = true;
   printf("in thread_worker_pool start\n");
   for (int i = 0; i < worker_num; ++i) {
      pthread_create(&(threads[i]), NULL, per_worker_task, this);
   }
}

void thread_worker_pool::stop() {
   void *status;
   for (int i = 0; i < worker_num; ++i) {
      pthread_join(threads[i], &status);
   }
}
