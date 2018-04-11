#ifndef PROCESS_H
#define PROCESS_H

#include <utility>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <queue>
#include <fstream>
#include <cstring>
#include <string>
#include <set>
#include <sstream>
#include <iostream>

using namespace std;

class Process {

	public:
	string ProcessName;	//the name of the process, a string.
	unsigned int Priority;	//a nonnegative integer (higher values: more important).
	int ProcessID;		//an integer, the ID number for the process.
	int ArrivalTime;	// an integer indicating when the request for this process first arrived.
	pair <char,int> History[12];	// an array of pairs
	int Sub; 		// a subscript into the array History
	int CPUTimer;		//counts clock ticks for the process until it reaches the end of a CPU burst
	int IOTimer;		// counts clock ticks for the process
	int CPUTotal;		// accumulates the number of clock ticks the process spends as Active.
	int ITotal;		// accumulates the number of clock ticks the process spends as IActive.
	int OTotal;		//accumulates the number of clock ticks the process spends as OActive.
	int CPUCount;		// counts the CPU bursts for this process.
	int ICount;		//counts the Input bursts for this process.
	int OCount;		//counts the Output bursts for this process.
	int StartTime;		//when process starts

	Process();

	/*friend bool operator<(const Process& lp , const Process& rp) {
		if (lp.Priority != rp.Priority)
		return lp.Priority < rp.Priority;
		else if (lp.ArrivalTime != rp.ArrivalTime)
		return lp.ArrivalTime < lp.ArrivalTime;
	}*/

};

struct QueueCompare {
	bool operator()(const Process* p1, const Process* p2) const {
	return p1->Priority < p2->Priority;
	}
};
#endif
