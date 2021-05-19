#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<fstream>
#include<queue>
#include<vector>

using namespace std;

class worker{
	private:
		float floor;
		float wt;
	
	public:
		worker(){
			floor = 0;
			wt = 0;
		}
		float setFloor(float x)
		{	floor = x;	}
		float getFloor()
		{	return floor;	}
		float setWt(float k)
		{	wt = k;	}
		float getWt()
		{	return wt;	}	
};

int main(){
	
	float elevArr[4][4] = {0, 1.0, 1.5, 1.75, // 2d array of elevator time
						   1.0, 0, 0.5, 0.75,
						   1.5, 0.5, 0, 0.5,
						   1.75, 1.5, 1.0, 0};
	
	float elevOpen = 0.5;
	float queTime;
	float interArrival = 0.1667;
	int people;
	worker suit;
	vector<worker> que;
	vector<worker> elevator;
	vector<float> waitTime;
	vector<float> totAvgWait;
	vector<int> pplWalk;
	vector<int> walkers;
	vector<int> halfWork;
	vector<int> threeWork;
	vector<int> fullWork;
	vector<float> lastWork;
	int timeArr[3] = {0};
	vector<int> queSize;
	int queIndex = 5;
	
	for (int i = 0; i < 25; i++){
		waitTime.clear();
		int ppw = 0;
		bool half = true;
		bool three = true;
		bool full = true;
		float totTime = 0.0;
		float exTime = 0;
		int halfnum = 0;
		int threenum = 0;
		int fullnum = 0;
		
		srand(i+5); // random seed for floor number
		float stFloor;
		for (int i = 0; i < 3; i++){ // start simulation with three people in elevator
			stFloor = (float) rand()/RAND_MAX;
			suit.setFloor(stFloor);
			elevator.push_back(suit);
		}
		
		while(totTime <= 60 || !que.empty()){
			bool second = false;
			bool third = false;
			bool fourth = false;
			float elevTime = 0.0;
			for (int i = 0; i < elevator.size(); i++){
				if(elevator[i].getFloor() <= 0.33)
					second = true;
				else if (elevator[i].getFloor() > 0.33 && elevator[i].getFloor() <= 0.66)
					third = true;
				else
					fourth = true;
			}
			if (second && third && fourth) // go through each possible combination and get elevator time
				elevTime = elevArr[0][1] + elevArr[1][2] + elevArr[2][3] + (3 * elevOpen) + elevArr[3][0] + elevOpen;
			if (second && !third && !fourth)
				elevTime = elevArr[0][1] + elevOpen + elevArr[1][0] + elevOpen;
			if (third && !second && !fourth)
				elevTime = elevArr[0][2] + elevOpen + elevArr[2][0] + elevOpen;
			if (fourth && !second && !third)
				elevTime = elevArr[0][3] + elevOpen + elevArr[3][0] + elevOpen;
			if (second && fourth && !third)
				elevTime = elevArr[0][1] + elevArr[1][3] + (2 * elevOpen) + elevArr[3][0] + elevOpen;
			if (second && third && !fourth)
				elevTime = elevArr[0][1] + elevArr[1][2] + (2 * elevOpen) + elevArr[2][0] + elevOpen;
			if (third && fourth && !second)
				elevTime = elevArr[0][2] + elevArr[2][3] + (2 * elevOpen) + elevArr[3][0] + elevOpen;
			totTime = totTime + elevTime;
			if (totTime >= 30 && half){ 
				halfWork.push_back(que.size());
				halfnum = que.size();
				half = false;
			}
			if (totTime >= 45 && three){ // chech que size at specific times
				threeWork.push_back(que.size());
				threenum = que.size();
				three = false;
			}
			if (totTime >= 60 && full){
				fullWork.push_back(que.size());
				fullnum = que.size();
				full = false;
			}
			elevator.clear();
			if (totTime >= queIndex && totTime < 70){
				queIndex = queIndex + 5;
				queSize.push_back(que.size());
			}
			
			people = elevTime/interArrival; // calculate new people that join que and waiting times
			worker wait;
			if (que.empty()){
				srand((unsigned)time(NULL));
				for (int i = 1; i <= people; i++){
					stFloor = (float) rand()/RAND_MAX;
					wait.setFloor(stFloor);
					wait.setWt(elevTime - (interArrival * i));
					que.push_back(wait);
				}
			}
			else{
				for (int i = 0; i < que.size(); i++){
					que[i].setWt(que[i].getWt() + elevTime);
				}
				if (totTime <= 60){
					srand((unsigned)time(NULL));
					for (int i = 1; i <= people; i++){
						stFloor = (float) rand()/RAND_MAX;
						wait.setFloor(stFloor);
						wait.setWt(elevTime - (interArrival * i) + exTime); // time is added b/c worker
						que.push_back(wait);								// wait time interval
					}
				}
			}
			exTime = elevTime - (interArrival * people);
			
			float walk;
			int walkcount = 0;
			if (que.size() > 12){ // check to see who walks while elevator moves
				srand(i+7);
				for (int i = 0; i < que.size(); i++){
					walk = (float) rand()/RAND_MAX;
					if (que[i].getFloor() <= 0.33 && walk <= 0.5){
						waitTime.push_back(que[i].getWt());
						que.erase(que.begin()+i);
						walkcount++;
					}
					if (que[i].getFloor() > 0.33 && que[i].getFloor() <= 0.66 && walk <= 0.33){
						waitTime.push_back(que[i].getWt());
						que.erase(que.begin()+i);
						walkcount++;
					}
					if (que[i].getFloor() > 0.66 && walk <= 0.1){
						waitTime.push_back(que[i].getWt());
						que.erase(que.begin()+i); // erase walker from que
						walkcount++;
					}		
				}
			}
			ppw = ppw + walkcount; // add to total people who walk
			
			int index = 0;
			if (que.size() <= 12){
				while (index < que.size()){
					waitTime.push_back(que[index].getWt());
					index++;
				}
			}
			else{
				while (index < 12){
					waitTime.push_back(que[index].getWt());
					elevator.push_back(que[index]);
					index++;
				}
			}
			
			que.erase(que.begin(), que.begin()+index);
		}
		float sum = 0;
		for (int i = 0; i < waitTime.size(); i++)
			sum = sum + waitTime[i];
		
		totAvgWait.push_back((float) sum/waitTime.size());
		pplWalk.push_back(ppw);
		lastWork.push_back(totTime);
		timeArr[0] = timeArr[0] + halfnum;
		timeArr[1] = timeArr[1] + threenum;
		timeArr[2] = timeArr[2] + fullnum;
	}
	
//	ofstream myFile; // write output to csv file for Rstudio
//	myFile.open("elevator_sim_total2.csv");
//	myFile << "Wait,Walk,8:30,8:45,9:00,Last";
//	myFile << endl;
//	for (int j = 0; j < 25; j++){
//		myFile << totAvgWait[j] << "," << pplWalk[j] << "," << halfWork[j] << ","
//			   << threeWork[j] << "," << fullWork[j] << "," << lastWork[j];
//		myFile << endl;
//	}
//	myFile.close();
	
	float waitSum = 0;
	for (int i = 0; i < totAvgWait.size(); i++){
		waitSum = waitSum + totAvgWait[i];
	}
	int pplsum = 0;
	for (int i = 0; i < pplWalk.size(); i++)
		pplsum = pplsum + pplWalk[i];
	
	int halfsum = 0;
	for (int i = 0; i < halfWork.size(); i++)
		halfsum = halfsum + halfWork[i];
	
	int threesum = 0;
	for (int i = 0; i < threeWork.size(); i++)
		threesum = threesum + threeWork[i];
	
	int fullsum = 0;
	for (int i = 0; i < fullWork.size(); i++)
		fullsum = fullsum + fullWork[i];
	
	float lastsum = 0;
	for (int i = 0; i < lastWork.size(); i++)
		lastsum = lastsum + lastWork[i];
	float avgLastSum = lastsum/lastWork.size();
	float minTime = avgLastSum - 60;
	int realtime = (int) (minTime + 0.5);
	cout << "Total wait time average: " << (float) waitSum/totAvgWait.size() << endl;
	cout << "Average people who walk per day: " << (int) pplsum/pplWalk.size() << endl;
	cout << "Average people waiting at 8:30: " << (int) halfsum/halfWork.size() << endl;
	cout << "Average people waiting at 8:45: " << (int) threesum/threeWork.size() << endl;
	cout << "Average people waiting at 9:00: " << (int) fullsum/fullWork.size() << endl;
	cout << "Average time last person leaves: 9:" << realtime;

//	int avgArr[3];
//	avgArr[0] = timeArr[0] / 25;
//	avgArr[1] = timeArr[1] / 25;
//	avgArr[2] = timeArr[2] / 25;
//	
//	ofstream newFile;
//	newFile.open("exponential.csv");
//	newFile << "Time";
//	newFile << endl;
//	for (int i = 0; i < queSize.size(); i++){
//		newFile << queSize[i];
//		newFile << endl;
//	}
//	
	return 0;
}






