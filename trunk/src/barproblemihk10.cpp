//============================================================================
// Name        : The Bar Problem
// Author      : Jeff Jankowski, Jonathan Mann
// Description : Parallelized Simulation of a Pub
//============================================================================

#include "Common.h"
#include "Landlord.h";
#include "Table.h";
#include "Barmaid.h"
#include "Assistant.h"

using namespace std;

pthread_mutex_t output_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_t landlordThread;
pthread_t assistantThread;
pthread_t barmainThread;
pthread_t clockThread;

pthread_mutex_t beerTap, milk, coffee, chocolate;
sem_t glasses, cups;

Table tables[NUM_TABLES];
//placeholder values
int drink_q_id = 0;
int greeting_q_id = 0;

volatile bool bLastCall, bClose;

//used in getRand() which call rand_r(*uint);
volatile unsigned int seed = 0;

int getRand()
{
	//seed only once
	if (seed == 0)
		seed = time(NULL);

	return  rand_r((unsigned int*) &seed);
}

void log(string name,string message)
{
	//format log message with name
	struct timeval now;
	gettimeofday(&now, NULL);
	pthread_mutex_lock(&output_mtx);
	printf("  [%u.%06u] %s: %s \n", (unsigned int)now.tv_sec, (unsigned int)now.tv_usec, name.c_str(), message.c_str());
	pthread_mutex_unlock(&output_mtx);
}

void log(string message)
{
	//format log message without sender name
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

void* run_clock(void *dptr)
{
	usleep(TIME_UNTIL_LASTCALL * 1000);
	bLastCall = true;

	string temp = "\n------------------------------------------  LAST CALL! ------------------------------------------\n";
	log(temp);

	return 0;
}

void init()
{
	pthread_mutex_init(&beerTap, NULL);
	pthread_mutex_init(&milk, NULL);
	pthread_mutex_init(&coffee, NULL);
	pthread_mutex_init(&chocolate, NULL);
	sem_init(&glasses, 0 , NUM_GLASSES);
	sem_init(&cups, 0 , NUM_CUPS);

	bLastCall = false;
	bClose = false;

	ostringstream str;
	string temp = "Bar simulation started...";
	log(temp);

	//We need to create the drink queue and then destroy it to remove leftover junk
	drink_q_id = msgget(DRINK_Q, 0666|IPC_CREAT|IPC_PRIVATE);
	msgctl(drink_q_id, IPC_RMID, (struct msqid_ds *) 0);
	drink_q_id = msgget(DRINK_Q, 0666|IPC_CREAT|IPC_PRIVATE);
	if(drink_q_id == -1)
	{
		str << "ERROR: Failed to create drink_q errno:" << errno;
	}
	else
	{
		str << "Created drink_q: " << drink_q_id;
	}

	//We need to create the greeting queue and then destroy it to remove leftover junk
	greeting_q_id = msgget(GREET_Q, 0666|IPC_CREAT|IPC_PRIVATE);
	msgctl(greeting_q_id, IPC_RMID, (struct msqid_ds *) 0);
	greeting_q_id = msgget(GREET_Q, 0666|IPC_CREAT|IPC_PRIVATE) ;
	if(greeting_q_id == -1)
	{
		str << "\n                    ERROR: Failed to create greet_q errno:" << errno;
	}
	else
	{
		str << "\n                    Created greet_q: " << greeting_q_id;
	}
	temp = str.str();
	log(temp);
	str.str("");

	/*sleep after each thread is created to give time for initialization*/
	Landlord::Landlord_Thread_Args llta;
	llta.greeting_q_id = greeting_q_id;
	llta.drink_q_id = drink_q_id;
	pthread_create(&landlordThread, NULL, Landlord::run_thread, &llta);
	usleep(10000);

	Assistant::Assistant_Thread_Args ata;
	ata.greeting_q_id = greeting_q_id;
	pthread_create(&assistantThread, NULL, Assistant::run_thread, &ata);
	usleep(10000);

	Barmaid::Barmaid_Thread_Args bta;
	bta.greeting_q_id = greeting_q_id;
	bta.drink_q_id = drink_q_id;
	pthread_create(&assistantThread, NULL, Barmaid::run_thread, &bta);
	usleep(10000);

	pthread_create(&clockThread, NULL, run_clock, NULL);
}

int main (int argc, char *argv[])
{
	//Error message data for reference
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

	//make sure drink ratios match up
	assert(RATIO_BEER + RATIO_CAPPUCCINO + RATIO_HOT_CHOCOLATE == 1);
	init();

	srand(time(NULL));
	//didn't work with i=0... that's super weird.
	int i = 1;
	//create customers at set interval until last call
	while(!bLastCall)
	{
		Customer::Cust_Thread_Args cta;
		pthread_t ct;
		cta.cust_id = i;
		cta.drink_q_id = drink_q_id;
		cta.greeting_q_id = greeting_q_id;
		pthread_create(&ct, NULL, Customer::run_thread, &cta);
		usleep(TIME_INTERVAL_CUST * 1000);
		i++;
	}

	pthread_join(landlordThread, NULL);

	temp = "Bar simulation exiting...";
	log(temp);
	exit(EXIT_SUCCESS);
}
