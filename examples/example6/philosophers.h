#ifndef CS414_PHILOSOPHERS_H
#define CS414_PHILOSOPHERS_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// a simple enumeration for the philosophers states
// feel free to add more, but make sure to keep and use
// these states.
enum PHIL_STATE {
    THINKING = 'T',
    HUNGRY = 'H',
    EATING = 'E'
};

// returns the approximate elapsed time since the first call
// (in seconds)
float elapsedTime() {
  timeval tv;
  gettimeofday(&tv, NULL);
  static timeval startTime = tv;
  return ( (tv.tv_sec - startTime.tv_sec) + float(tv.tv_usec-startTime.tv_usec) / 1E6f );
}

// a very simple representation of a chopstick.
// it has two states: up and down.
class Chopstick {
public:
  void Pickup();
  void PutDown();
  bool isDown();
private:
  bool is_down;
};

// callback function for pthread_create
void* start_philosopher(void* phil_pointer);

// our philosopher representation. All of the methods
// have been provided, but you will need to prevent
// deadlocks and starvation (livelocks)
class Philosopher {
public:
  Philosopher();
  // initialize all the paramters of this philosopher
  void Init(int _id,
	    Chopstick* _left, Chopstick* _right,
	    Philosopher* _left_phil, Philosopher* right_phil,
	    float _avg_wait = 0.5f,
	    bool _do_random = true);
  // start the whole process of 
  // thinking, getting hungry, and eating
  void Start();
  // sleep for a little while based on
  // avg_wait and do_random
  //
  // if do_random is set, the actual number of seconds
  // will be: 
  //
  // 2*X*avg_wait, where X is a random variable in [0.0, 1.0]
  void Sleep();

  // gets the current state that this philosopher is in.
  // this MUST be in the PHIL_STATE enum
  char GetState();

  // changes this philosopher's state to the next one
  // This new state should be in the PHIL_STATE enum,
  // which you can modify if you need more states
  void ChangeState(char new_state);

  // picks up the left chopstick
  void PickupLeft();
  // puts down the left chopstick
  void PutDownLeft();

  // picks up the right chopstick
  void PickupRight();
  // puts down the right chopstick
  void PutDownRight();

  // here's a good place to put all the thread variables
  pthread_t thread_id;
  pthread_attr_t thread_attr;


private:
  // this should only be used for debugging purposes, 
  // as each philosopher shouldn't know anything about her id.
  // 
  // in other words, a solution based on certain id's acting
  // differently is not acceptable.
  int id;

  // the current PHIL_STATE for this philosopher
  char state;

  // pointers to the two chopsticks that this philosopher
  // may pick up.
  Chopstick *left, *right;

  // pointers to the two neighboring philosophers
  Philosopher *left_phil, *right_phil;

  // the average number of seconds the philosopher waits between
  // thinking and getting hungry, or between eating and putting down
  //
  // See Sleep()
  float avg_wait;

  // should we sleep for (avg_wait) each time, or add some
  // randomness?
  bool do_random;
};
#endif // CS414_PHILOSOPHERS_H
