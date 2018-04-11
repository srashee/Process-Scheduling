/***************************************************************************
 * Notes:
 * simulating proirity scheduling of processes on a single-processor
 * system, without preemption.
 * When a process ends its CPU burst, it is suceeded by the
 * highest-priority process that is waiting.
 *
 * ************************************************************************/
#include "scheduler.h"
#include "process.h"

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

//Gloval Variables
int MAX_TIME = 500;
int AT_ONCE = 5;
int QUEUE_SIZE = 20;
int ARRAY_SIZE = 10;
int HOW_OFTEN = 25;

//Variables
int Timer = 0; //Timer
int idleTime = 0; //Idle count
int pCount = 0; //Process counter
int sCount = 0; //Stopped processes counter

// Queues
priority_queue<Process*, vector<Process*>, QueueCompare> rQueue, iQueue, oQueue, tQueue;
queue<Process*> eQueue;
queue<Process*> t2Queue; 

//Pointers
Process* Active = 0;
Process* IActive = 0;
Process* OActive = 0;

Process::Process(){
//set everything to 0?
};



int main(){
	readFile(); //Reading file
	cout << "Beginning Priority Process Scheduling Simulation!!!" << endl; //Beginning program output

	while(Timer < MAX_TIME ) {
		//while (!eQueue.empty() && tCount == eQueue.front()->ArrivalTime) {
		while(!eQueue.empty() && pCount < AT_ONCE && Timer >= eQueue.front()->ArrivalTime) {
		cout << endl<< "Process " << eQueue.front()->ProcessName << " moved from Entry Queue to Ready Queue at time " << Timer << endl;
		rQueue.push(eQueue.front() ); //Moving from entry queue to ready priority queue
		eQueue.pop(); //pop from entry queue
		pCount++; //incrememnt process counter
		}

	if (Timer % HOW_OFTEN == 0) { //if statements to print out what processes are in which state
		cout << endl << endl <<"Status at time " << Timer << endl;
		if (Active == 0) //Active state
			cout << "No Current Active Process, CPU Idle" << endl;
		else
			cout << "Active is " << Active->ProcessName << endl;
		if (IActive == 0) //Input active state
			cout << "No Current IActive Process, Input Idle" << endl;
		else
			cout << "IActive is " << IActive->ProcessName << endl;
		if (OActive == 0) //output active state
			cout << "No Current OActive Process, Output Idle" << endl;
		else
			cout << "OActive is " << OActive->ProcessName << endl;
			printQueue(); //print queue
		}

		if (Active == 0) {
			if (!rQueue.empty()) {
				Active = rQueue.top(); 
				rQueue.pop();
				Active->CPUCount++;
			}
			else
				idleTime++; //increasing waiting time
		}

		if(Active != 0) {
			if (Active->CPUTimer < Active->History[Active->Sub].second ) {
				Active->CPUTimer++;
				Active->CPUTotal++;
				//cout << "Second:" << Active->History[Active->Sub].second << endl;
				//cout << "Timer: "  << Active->CPUTimer << endl;
			}
			else {
				Active->CPUTimer = 0;
				Active->Sub++;

				switch (Active->History[Active->Sub].first) {
				case 'N': //In case it terminates
					cout << endl <<  "Process " <<  Active->ProcessName << " has terminated at time " << Timer << endl;
					cout << "Name: " << Active->ProcessName << endl;
					cout << "Start Time: " << Active->ArrivalTime << " End Time: " << Timer << endl;
					cout << "Total CPU time = " << Active->CPUTotal << " in " << Active->CPUCount << " CPU bursts" << endl;
					cout << "Total Input time = " << Active->ITotal << " in " << Active ->ICount << " Input bursts" << endl;
					cout << "Total output time = " << Active->OTotal << " in " << Active->OCount << " Output bursts" << endl;
					cout << "Time spent waiting: " << idleTime << endl;
						delete Active;
						Active = 0;
						pCount--;
						sCount++;
					break;

				case 'I': //In case of input
					iQueue.push(Active);
					Active = 0;

					break;

				case 'O': //In case of output
					oQueue.push(Active);
					Active = 0;

					break;
				}
			}
		}

		if(IActive == 0 && !iQueue.empty()) {
			IActive = iQueue.top(); //Input active 
			iQueue.pop(); //pop from input queue
			IActive->ICount++;
		}

		if(IActive != 0){
			if(IActive->IOTimer < IActive->History[IActive->Sub].second) {
				IActive->ITotal++; //increment Input total
				IActive->IOTimer++; //increment input timer
			}
			else {
				IActive->IOTimer = 0; //set timer to 0  
				IActive->Sub++; //change subscript
				rQueue.push(IActive); //push object
				IActive = 0;
			}
		}
		if(OActive == 0 && !oQueue.empty()) {
			OActive = oQueue.top(); //top member of outpue queue
			oQueue.pop(); //remove output queue top member
			OActive->OCount++; 
		}

		if (OActive != 0) {
			if (OActive->IOTimer < OActive->History[OActive->Sub].second) {
				OActive->OTotal++; //increment output total
				OActive->IOTimer++; //increment output timer
			}
			else {
				OActive->IOTimer = 0;
				OActive->Sub++;  //increment subscript
				rQueue.push(OActive); //push to queue
				OActive = 0;
			}
		}
		Timer++; //increment the timer
	if (sCount == 12) //if all processes have been compelted break
		break;
}


	cout << endl <<"The run has ended." << endl << "The final value of the Timer was: " //final print 
		<< Timer << endl << "The amount of time the CPU spent idle was: " << idleTime
		<< endl << "Number of terminated processes = " << sCount << endl << endl;
		if (Active == 0) //Active state
                        cout << "No Current Active Process, CPU Idle" << endl;
                else
                        cout << "Active is " << Active->ProcessName << endl;
                if (IActive == 0) //Input active state
                        cout << "No Current IActive Process, Input Idle" << endl;
                else
                        cout << "IActive is " << IActive->ProcessName << endl;
                if (OActive == 0) //output active state
                        cout << "No Current OActive Process, Output Idle" << endl;
 
		printQueue();
return 0;
}


void readFile(){
	ifstream file;
	file.open("data.txt");

	if(file){
		string lineString;
		int processCount = 100;

		while(getline(file, lineString)) {
			istringstream ifs;	//iss to iterate through string
		        Process* process = new Process();	//initializing a process object
			ifs.str(lineString);
			ifs >> process->ProcessName;		//process name

			if(process->ProcessName == "STOPHERE") //stop at this name
				break;

			ifs >> process->Priority; //set priority
			ifs >> process->ArrivalTime; //set arrival time 
			processCount++; //increase processes
			process->ProcessID = processCount; 

			getline(file, lineString); //get line 
			//cout << lineString;
			ifs.clear(); //clearing stream for use
			ifs.str(lineString); //wtf why wont you work
			for(int i = 0 ; i < 10; i++){ //putting values of pair into history
				ifs >> process->History[i].first; //Letter
				ifs >> process->History[i].second; //Burst
				//cout << "First: " <<  process->History[i].first << endl;
				//cout << "Second: " << process->History[i].second << endl;
			}
			process->Sub = 0;

			//Testing output
/*
			cout << "Name; " << process->ProcessName << endl;
			cout << "priority: " << process->Priority << endl;
			cout << "arrival: " << process->ArrivalTime << endl; */
			eQueue.push(process); //Pushing process to entry queue
		}
	}
}

void printQueue() {
	cout << endl << "Contents of the Entry Queue:" << endl; //printing contents
	if (eQueue.empty()) 
		cout << "(EMPTY)" << endl; //print empty if empty
	else {
		t2Queue = eQueue;
		while (!t2Queue.empty()) { 
			cout<<t2Queue.front()->ProcessName << "  ";
			t2Queue.pop(); //pop from temp2 queue
		}
		cout << endl;
	}

	cout << endl << "Contents of the Ready Queue:" << endl; //ready queue print
	if (rQueue.empty())
		cout << "(EMPTY)" << endl; //print empty if empty
	else {
		tQueue = rQueue; //setting queue
		printStatus();  //printing queue
	}

	cout << endl << "Contents of the Input Queue:" << endl;
	if (iQueue.empty())
		cout << "(EMPTY)" << endl; //empty if empty
	else {
		tQueue = iQueue; //setting queue
		printStatus(); //printing queue
	}

	cout << endl << "Contents of the Output Queue:" << endl;
	if (oQueue.empty())
		cout << "(EMPTY)" << endl;
	else {
		tQueue = oQueue; //setting queue
		printStatus(); //printing queue
	}
}

void printStatus(){
	while(!tQueue.empty()) {//until its empty
		cout << tQueue.top()->ProcessName << "  "; //print
		tQueue.pop(); //remove after printing
	}
}


