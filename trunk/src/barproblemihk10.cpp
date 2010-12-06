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
#include <string>
#include <sys/msg.h>
#include <sys/time.h>
#include "Common.h"
#include "Customer.h"


//#define NUM_THREADS     5

using namespace std;

//pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
//sem_t sem1;

int msqid, currentCustID;
pthread_mutex_t beerTap, cupboard, milk, coffee, chocolate;
sem_t glasses, cups, tables[NUM_TABLES];


void log(string &name,string &message)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	printf("  [%u.%06u] %s: %s\n", now.tv_sec, now.tv_usec, name.c_str(), message.c_str());
}

void log(string &message)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	printf("[%u.%06u] %s\n", now.tv_sec, now.tv_usec, message.c_str());
}

const char* typeAsString(OrderType type)
{
	switch (type) {
	case BEER:
		return "Beer";
	case CAPPUCCINO:
		return "Cappuccnio";
	case HOT_CHOCOLATE:
		return "Hot Chocolate";
	default:
		return "NO CHOICE";
	}
}

void init()
{
	string temp = "Bar simulation started...";
	log(temp);
	currentCustID = 0;

	//set up the resources, Barmaid, Assistant, Landlord
	//add Barmaid and Assistant to Landlord's list (he must leave bar after everyone leaves and announce last call)
}



//void *TaskCode(void *argument)
//{
//   int tid;
//
//   tid = *((int *) argument);
//
//   for(int i = 0; i < 25; i++)
//   {
//	   pthread_mutex_lock(&mutex1);
//	   cout << "Hello World ";
//	   usleep(1);
//	   cout << tid;
//	   usleep(1);
//	   cout << endl;
//	   pthread_mutex_unlock(&mutex1);
//	   usleep(1);
//   }
//
//   /* optionally: insert more useful stuff here */
//
//   return NULL;
//}

int main (int argc, char *argv[])
{
	assert(RATIO_BEER + RATIO_CAPPUCCINO + RATIO_HOT_CHOCOLATE == 1);
	init();


	srand(time(NULL));
	for (int i=0; i < 5; i++)
	{
		Customer* cust = new Customer();
		cust->run();
		currentCustID++;

		delete cust;
	}

//	pthread_t threads[NUM_THREADS];
//   int thread_args[NUM_THREADS];
//   int rc, i;
//
//   sem_init(&sem1, false, 5);
//
//   /* create all threads */
//   for (i=0; i<NUM_THREADS; ++i) {
//      thread_args[i] = i;
//      printf("In main: creating thread %d\n", i);
//      rc = pthread_create(&threads[i], NULL, TaskCode, (void *) &thread_args[i]);
//      assert(0 == rc);
//   }
//
//   /* wait for all threads to complete */
//   for (i=0; i<NUM_THREADS; ++i) {
//      rc = pthread_join(threads[i], NULL);
//      assert(0 == rc);
//   }


	string temp = "Bar simulation exiting...";
	log(temp);
	exit(EXIT_SUCCESS);
}
