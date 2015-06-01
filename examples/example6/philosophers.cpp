#include "philosophers.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

// we will lock STDOUT so our output isn't garbled
pthread_mutex_t console_lock;
pthread_mutex_t chopstick_lock;
pthread_cond_t  chopstick_cond;
int chopstick_count = 5;

void Chopstick::Pickup()  { is_down = false; }
void Chopstick::PutDown() { is_down = true;  }
bool Chopstick::isDown()  { return is_down;  }

Philosopher::Philosopher() : id(-1) {}
void Philosopher::Init(int _id, Chopstick* _left, Chopstick* _right,
		       Philosopher* _left_phil, Philosopher* _right_phil,
		       float _avg_wait, bool _do_random) {
  id = _id;
  left = _left; right = _right;
  left_phil = _left_phil; right_phil = _right_phil;
  avg_wait = _avg_wait;
  do_random = _do_random;
  state = THINKING;
  // explicitly make our thread joinable
  pthread_attr_init(&thread_attr);
  pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

  // Start the Thread - this requires a little explanation:
  // pthread_create cannot take a class method, so we have
  // defined start_philosopher, that takes a pointer to 
  // a Philosopher. start_philosopher then calls Start() on
  // that Philosopher.
  pthread_create(&thread_id, &thread_attr, start_philosopher, this);
}

// start the whole process of 
// thinking, getting hungry, and eating
void Philosopher::Start() {
  while(true) {
    // I want to think ...
    Sleep();
    // I'm hungry, I want to eat ...
    ChangeState(HUNGRY);
    // gotta pick up the chopsticks before I can eat
    pthread_mutex_lock(&chopstick_lock);
    if (chopstick_count < 2) {
        pthread_cond_wait(&chopstick_cond, &chopstick_lock);
    }
    //while (!left->isDown()) Sleep(); // wait until left is available
    PickupLeft();
    chopstick_count--;
    //while (!right->isDown()) Sleep(); // wait until right is available
    PickupRight();
    chopstick_count--;
    pthread_mutex_unlock(&chopstick_lock);

    // FOOD!
    ChangeState(EATING);
    // wait while I finish this rice
    Sleep();
    // okay, put the chopsticks down
    pthread_mutex_lock(&chopstick_lock);
    PutDownLeft();
    chopstick_count++;
    PutDownRight();
    chopstick_count++;
    if (chopstick_count >= 2) {
        pthread_cond_signal(&chopstick_cond);
    }
    pthread_mutex_unlock(&chopstick_lock);
    // with my stomach full, I can finally think again
    ChangeState(THINKING);
  }
}

char Philosopher::GetState() { return state; }
void Philosopher::ChangeState(char new_state) {
  state = new_state;

  pthread_mutex_lock(&console_lock);
  std::cout << "(t=" << elapsedTime() << ") "
	    << id << ": " << state << std::endl;
  pthread_mutex_unlock(&console_lock);
}

void Philosopher::PickupLeft() {
  left->Pickup();
  pthread_mutex_lock(&console_lock);
  std::cout << "(t=" << elapsedTime() << ") "
	    << id << ": " << "LEFT UP" << std::endl;
  pthread_mutex_unlock(&console_lock);
}

void Philosopher::PickupRight()
{
  right->Pickup();
  pthread_mutex_lock(&console_lock);
  std::cout << "(t=" << elapsedTime() << ") "
	    << id << ": " << "RIGHT UP" << std::endl;
  pthread_mutex_unlock(&console_lock);
}

void Philosopher::PutDownLeft()
{
  left->PutDown();
  pthread_mutex_lock(&console_lock);
  std::cout << "(t=" << elapsedTime() << ") "
	    << id << ": " << "LEFT DOWN" << std::endl;
  pthread_mutex_unlock(&console_lock);
}

void Philosopher::PutDownRight()
{
  right->PutDown();
  pthread_mutex_lock(&console_lock);
  std::cout << "(t=" << elapsedTime() << ") "
	    << id << ": " << "RIGHT DOWN" << std::endl;
  pthread_mutex_unlock(&console_lock);
}

void Philosopher::Sleep()
{
  float wait_time_ms = avg_wait * 1E3f;

  if (do_random) {
    wait_time_ms = (rand() / (float(RAND_MAX)+1.0f))*2.0*wait_time_ms;
  }

#ifdef WIN32
  _sleep(int(wait_time_ms));
#else
  usleep(int(wait_time_ms * 1E3f)); // convert from ms to us
#endif
}

void* start_philosopher(void* phil_pointer)
{
  ((Philosopher*) phil_pointer)->Start();

  pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
  // sets up the initial time to be as close as
  // possible to the start of the application
  elapsedTime(); 
  int num_philosophers;
  float avg_wait_seconds;
  bool do_random;
  // process command line args
  bool params_invalid = false;
  if (argc < 4) {
    params_invalid = true;
  } else {
    num_philosophers = atoi(argv[1]);
    avg_wait_seconds = atof(argv[2]);
    do_random = (atoi(argv[3]) != 0);
  }
  if (num_philosophers < 1) {
    cerr << "Error: num_philosophers must be > 0" << endl;
    params_invalid = true;
  }
  if (avg_wait_seconds < 0) {
    cerr << "Error: avg_wait_seconds must be >= 0" << endl;
    params_invalid = true;
  }
  if (params_invalid) {
    cerr << "Usage: " << argv[0] << " num_philosophers avg_wait_seconds "
	 << "use_randomness" << endl << "Example: " << argv[0] << " 5 0.5 1" << endl;
    exit(1);
  }

  // initialize the console lock
  pthread_mutex_init(&console_lock, NULL);  
  pthread_mutex_init(&chopstick_lock, NULL);
  pthread_cond_init(&chopstick_cond, NULL);
  // create the philosophers
  Philosopher phils[num_philosophers];
  Chopstick chops[num_philosophers];

  // chopstick i is the chopstick to the left of 
  // philosopher i
  for (int i = 0; i < num_philosophers; i++) {
    Philosopher *left_phil = &phils[(i+num_philosophers-1) % num_philosophers];
    Philosopher *right_phil = &phils[(i+1) % num_philosophers];
    // remember, the philosophers spawn a new thread
    // inside their Init functions, so this creates a new thread
    phils[i].Init(i, &chops[i], &chops[(i+1)%num_philosophers],
		  left_phil, right_phil, avg_wait_seconds, do_random);
  }


  // now wait for each philosopher to finish
  for (int i = 0; i < num_philosophers; i++) {
    pthread_join(phils[i].thread_id, NULL);
  }
}
