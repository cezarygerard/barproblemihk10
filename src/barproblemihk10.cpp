//============================================================================
// Name        : barproblemihk10.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Common.h"
#include "Customer.h"
#include "Assistant.h"
#include "Table.h"
#include "Landlord.h"

//#define NUM_THREADS     5

using namespace std;

pthread_mutex_t output_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_t landlordThread;


int msqid;
pthread_mutex_t beerTap, cupboard, milk, coffee, chocolate;
sem_t glasses, cups;
Table tables[NUM_TABLES];
int drink_q_id = 0;
int greeting_q_id = 0;

//used in getRand() which call rand_r(*uint);
volatile unsigned int seed =0;

int getRand()
{
	if(seed ==0 )
	{
		seed = time(NULL);
	}
	return  rand_r((unsigned int*) &seed);

}

void log(string &name,string &message)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	pthread_mutex_lock(&output_mtx);
	printf("  [%u.%06u] %s: %s \n", (unsigned int)now.tv_sec, (unsigned int)now.tv_usec, name.c_str(), message.c_str());
	pthread_mutex_unlock(&output_mtx);
}

void log(string &message)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	pthread_mutex_lock(&output_mtx);
	printf("[%u.%06u] %s \n", (unsigned int)now.tv_sec, (unsigned int)now.tv_usec, message.c_str());
	pthread_mutex_unlock(&output_mtx);
}

const char* typeAsString(OrderType type)
{
	switch (type) {
	case BEER:
		return "Beer";
	case CAPPUCCINO:
		return "Cappuccino";
	case HOT_CHOCOLATE:
		return "Hot Chocolate";
	default:
		return "";
	}
}

void init()
{
	sem_t glasses, cups;
	sem_init(&glasses, 0 , NUM_GLASSES);
	sem_init(&cups, 0 , NUM_CUPS);

	ostringstream str;
	string temp = "Bar simulation started...\n";
	log(temp);
	drink_q_id = msgget(DRINK_Q, IPC_CREAT|IPC_PRIVATE);
	if(drink_q_id == -1)
	{
		str << "failed to create drink_q errno:" << errno;
	}
	greeting_q_id = msgget(GREET_Q, IPC_CREAT|IPC_PRIVATE);
	if(greeting_q_id == -1)
	{
		str << "failed to create greet_q errno:" << errno;
	}
	else
	{
		str << "created greet_q: " << greeting_q_id;
	}
	temp = str.str();
	log(temp);
	str.str("");

	Landlord::Landlord_Thread_Args llta;

	llta.greeting_q_id = greeting_q_id;
	llta.drink_q_id = drink_q_id;
	pthread_create(&landlordThread, NULL, Landlord::run_thread, &llta);
	//pthread_t assistantThread;

	//set up the resources, Barmaid, Assistant, Landlord
	//add Barmaid and Assistant to Landlord's list (he must leave bar after everyone leaves and announce last call)
}

int main (int argc, char *argv[])
{
	int size = sizeof(int)*2 + sizeof(bool);
		ostringstream str;
	str<< " E2BIG " << E2BIG <<
			" EACCES " << EACCES <<
			" EFAULT " << EFAULT <<
			" EIDRM " << EIDRM <<
			" EINTR " << EINTR <<
			" EINVAL " << EINVAL <<
			" ENOMSG " << ENOMSG << "SIZE: " << sizeof(Landlord::Greeting_Msg_Args) << "  :  " << size;
	string temp = str.str();
	log(temp);

	assert(RATIO_BEER + RATIO_CAPPUCCINO + RATIO_HOT_CHOCOLATE == 1);
	init();


	timeval t_now, t_finish;
	gettimeofday(&t_now, NULL);
	srand(time(NULL));
	t_finish.tv_sec = t_now.tv_sec + ( TIME_UNTIL_CLOSE + TIME_UNTIL_LASTCALL) /1000 ;
	//while(t_finish.tv_sec > t_now.tv_sec)
	for (int i=0; i < 2; i++)
	{
		Customer::Cust_Thread_Args cta;
		pthread_t ct;
		cta.cust_id = i;
		cta.drink_q_id = drink_q_id;
		cta.greeting_q_id = greeting_q_id;
		cta.cust_msg_q_id = msgget(CUSTOMER_START_Q+i, IPC_CREAT);
		pthread_create(&ct, NULL, Customer::run_thread, &cta);
		usleep(TIME_INTERVAL_CUST * 1000);
		gettimeofday(&t_now, NULL);
	}

	pthread_join(landlordThread, NULL);

	temp = "Bar simulation exiting...";
	log(temp);
	exit(EXIT_SUCCESS);
}
