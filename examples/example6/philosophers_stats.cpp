#include <iostream>
#include <fstream>
#include <vector>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


using namespace std;

// for keeping some stats
vector<float> eatingTimes;
vector<float> hungryTimes;
vector<float> thinkingTimes;
vector<float> lastStateChangeTimes;

float lastTime = 0.0;

void printStats()
{
  cout << endl << endl << "Total Thinking Times: " << endl;
  for (unsigned int i = 0; i < thinkingTimes.size(); i++) {
    cout << "philosopher " << i << ": " << thinkingTimes[i] << endl;
  }

  cout << endl << endl << "Total Hungry Times: " << endl;
  for (unsigned int i = 0; i < hungryTimes.size(); i++) {
    cout << "philosopher " << i << ": " << hungryTimes[i] << endl;
  }

  cout << endl << endl << "Total Eating Times: " << endl;
  for (unsigned int i = 0; i < eatingTimes.size(); i++) {
    cout << "philosopher " << i << ": " << eatingTimes[i] << endl;
  }
}

/**
 * a handler for SIGINT so that you can stop 
 * a program with CTRL+C and still get the results.
 */
void handle_signal(int signum) 
{
  switch(signum) {
  case SIGINT:
    printStats();
    exit(0);
    break;
  }
}

/**
 * reads all the input either from a file or
 * from stdin
 */
void readStream(istream& fin, bool showOutput)
{
  // read the input
  char ch;
  
  float t;
  int phil;
  string action;


  string line;
  int rParenIdx, colonIdx, actionIdx;

  while (!fin.eof()) {
    getline(fin, line);
    
    if (showOutput) {
      // print the line back to stdout
      cout << line << endl;
    }

    // yeah, not the best error detection...
    if (line.size() <= 0 || line[0] != '(')
      continue;

    rParenIdx = line.find(')');
    colonIdx = line.find(':');
    actionIdx = colonIdx + 2;

    if (rParenIdx <= 0 || rParenIdx == string::npos ||
	colonIdx <= 0 || colonIdx == string::npos ||
	actionIdx <= 0 || actionIdx == string::npos) {
      cerr << "\nError: couldn't pare line (" << line << ")\n";
      continue;
    }

    t = atof(line.substr(3, rParenIdx-1).c_str());
    phil = atoi(line.substr(rParenIdx+1, colonIdx-rParenIdx-1).c_str());

//     cout << "actionIdx: " << actionIdx
// 	 << ", paramIdx: " << paramIdx 
// 	 << ", string::npos " << string::npos
// 	 << ", line.size() " << line.size() << endl;

    action = line.substr(actionIdx, line.size() - actionIdx);


//     cout << "Got: t=" << t << ", phil=" << phil
// 	 << ", action=\"" << action << "\", param=\""
// 	 << param << "\"" << endl;


    while (phil >= lastStateChangeTimes.size()) {
      lastStateChangeTimes.push_back(0.0);
    }

    if (t < lastStateChangeTimes[phil]) {
      cerr << "non-chronological time for philosopher " << phil
	   << " (t=" << t << ") on action \"" << action << "\""
	   << endl;
    }


    if (action == "H") {
      while (phil >= thinkingTimes.size()) {
	thinkingTimes.push_back(0.0);
      }
      thinkingTimes[phil] += (t-lastStateChangeTimes[phil]);
    } else if (action == "E") {
      while (phil >= hungryTimes.size()) {
	hungryTimes.push_back(0.0);
      }
      hungryTimes[phil] += (t-lastStateChangeTimes[phil]);
    } else if (action == "T") {
      while (phil >= eatingTimes.size()) {
	eatingTimes.push_back(0.0);
      }
      eatingTimes[phil] += (t-lastStateChangeTimes[phil]);
    } else {
//       if (action != "LEFT UP" && action != "LEFT DOWN" &&
// 	  action != "RIGHT UP" && action != "RIGHT DOWN")
// 	cerr << "Unrecognized action: " << action << endl;
	  
      // don't mark the time for anything else
      continue;
    }


    lastStateChangeTimes[phil] = t;
    lastTime = t;

  }
}


int main(int argc, char* argv[])
{
  signal(SIGINT, handle_signal);

  bool showOutput = false;
  bool haveFile = false;
  int fileArg = -1;

  // go through all args and get options / filename
  for (int i = 1; i < argc; i++) {
    if ( (strcmp(argv[i], "-o") == 0) ||
	 (strcmp(argv[i], "-O") == 0))
      showOutput = true;
    else if (argv[i][0] != '-') {
      // this must be the filename
      
      if (haveFile) {
	cerr << "Too many arguments." << endl
	     << "Usage: " << argv[0] << " [-o] [filename]" << endl;
	exit(1);
      }

      fileArg = i;
      haveFile = true;
    }
  }


  if (haveFile) {
    ifstream fin(argv[1]);
    if (!fin) {
      cerr << "Bad filename: " << argv[1] << endl;
      exit(0);
    }
    readStream(fin, showOutput);
  } else {
    readStream(cin, showOutput);
  }


  printStats();
}
