#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "Thread.h"


using namespace std;

/////////////////////////////////////////////////////////////////////////////
//  Sleep
//  sleep in milliseconds
//
void SleepMilis(long millis)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}


/////////////////////////////////////////////////////////////////////////////
//  Thread
//  base class for simple thread wrapper
//

//  Constructor
//  
Thread::Thread() 
{
	_Thread = 0;
	
	ThreadStopped = false;
	ThreadRunning = false;
}


//  Destructor
//  
Thread::~Thread()
{
	//  stop running thread before destruction
	if ( ThreadRunning )
	{
		//  ASSERT - this is bad, you should stop before destruction
		Cancel();
	}

	//  clean up the memory
	if ( _Thread != 0 )
		delete _Thread;
}



//  Run
//  This is the thread run function
//  it takes a reference to the Thread class, and calls the Thread::RunFunction() for each cycle
//  note, if you have a process that never quits, you should add a sleep in your RunFunction to prevent race condition
//  TODO:  Room For Improvement, I have read that sleep or yeild is not necessary on modern machines, is this true ?
//
void Run(Thread& thread)
{
	while ( thread.IsRunning() )
	{
		thread.RunFunction();

		//  use sleep or yeild once per cycle to avoid race condition locking up UI
		//  TODO: this won't compile, there is some question if it is necessary ?
		//  std::this_thread::yeild();
	}

	//  we are out of the run loop, set is stopped flag
	thread.SetIsStopped();
	
	return;
}


//  Start
//  
void Thread::Start()
{
	//  stop thread if it is running
	if ( _Thread != 0 )
	{
		//  ASSERT - this is bad, you should stop running thread before starting new one
		Cancel();
	}

	//  initialize thread run variables
	ThreadRunning = true;
	ThreadStopped = false;

	//  start the thread
	_Thread = new std::thread(Run,std::ref(*this));
	
	sched_param sch;
	int policy; 
	pthread_getschedparam(_Thread->native_handle(), &policy, &sch);
	sch.sched_priority = 99;
	if (pthread_setschedparam(_Thread->native_handle(), SCHED_FIFO, &sch)) 
	{
		//
		int i = 0;
	}


	return;
}



//  Cancel
//  Stop the thread
//  this function will wait until the thread has exited before returning
//
void Thread::Cancel()
{
	ThreadRunning = false;

	if ( _Thread )
	{
		//  a proper cancel thread function should have an interrupt here, to kill the thread if it is sleeping
		//  unfortunately, std::thread can not be interrupted (I believe?)
		//  use the BoostThread class if you want an interruptable thread

		//  wait for the thread to stop
		while ( ! ThreadStopped )
		{
			usleep(50000);
#ifdef DEBUG
			printf("Thread::Cancel - Waiting for thread to stop... \n");
#endif
		}

		//  join to the thread to ensure that thread shuts down before exit
		_Thread->join();

		//  clean up our mess
		delete _Thread;
		_Thread = 0;
	}

	return;
}
