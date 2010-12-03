//============================================================================
// Name        : barproblemihk10.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/msg.h>


#define NUM_THREADS     5

using namespace std;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
sem_t sem1;



void *TaskCode(void *argument)
{
   int tid;

   tid = *((int *) argument);

   for(int i = 0; i < 25; i++)
   {
	   pthread_mutex_lock(&mutex1);
	   cout << "Hello World ";
	   usleep(1);
	   cout << tid;
	   usleep(1);
	   cout << endl;
	   pthread_mutex_unlock(&mutex1);
	   usleep(1);
   }

   /* optionally: insert more useful stuff here */

   return NULL;
}

int main (int argc, char *argv[])
{

	asster(beerRatio + capuchinnoRatio + chocolateRatio == 1);
   pthread_t threads[NUM_THREADS];
   int thread_args[NUM_THREADS];
   int rc, i;

   sem_init(&sem1, false, 5);

   /* create all threads */
   for (i=0; i<NUM_THREADS; ++i) {
      thread_args[i] = i;
      printf("In main: creating thread %d\n", i);
      rc = pthread_create(&threads[i], NULL, TaskCode, (void *) &thread_args[i]);
      assert(0 == rc);
   }

   /* wait for all threads to complete */
   for (i=0; i<NUM_THREADS; ++i) {
      rc = pthread_join(threads[i], NULL);
      assert(0 == rc);
   }

   exit(EXIT_SUCCESS);
}
