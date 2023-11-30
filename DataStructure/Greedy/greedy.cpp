#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <stdio.h>
using namespace std;

// This is the sorting rule of Q1 
bool cmpDiff(const pair<int, int> &p1, const pair<int, int> &p2){
	if(p1.first-p1.second==p2.first-p2.second){
		return p1.first > p2.first;
	}
	return p1.first-p1.second > p2.first-p2.second;
}

// This is the sorting rule of Q2
bool cmpRatio(const pair<int, int> &p1, const pair<int, int> &p2){

	return ((double)p1.first/(double)p1.second)> ((double)p2.first/(double)p2.second);
}


int main(int argc, char** argv) {
	
	vector<pair<int, int> > lists;
	ifstream in;
	in.open("job.txt", ios::in);
	
	int total = 10000;
	string s;

	while(getline(in, s)){
		string sub1 = s.substr(0, s.find(" "));
		string sub2 = s.substr(s.find(" ") + 1, s.length());
		int weight = atoi(sub1.c_str());
		int length = atoi(sub2.c_str());
		lists.push_back({weight, length});
	}
	
	//sort(lists.begin(), lists.end(), cmpDiff);
	sort(lists.begin(), lists.end(), cmpRatio);
	
	int lengthSum = 0;
	long long int ans = 0;
	for(int i = 0;i<lists.size();i++){
		lengthSum += lists[i].second;
		ans += lists[i].first*lengthSum;
	}
	
	cout<<ans;
	
	return 0;
}
