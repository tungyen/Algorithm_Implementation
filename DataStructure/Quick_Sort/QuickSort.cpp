#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

using namespace std;

int getMedian(vector<int> &lists, int low, int high){
	int first = lists[low];
	int final = lists[high];
	int median = lists[(high+low)/2];
	int large = max(first, max(final, median));
	int small = min(first, min(final, median));
	if(first != small && first != large){
		return low;
	}
	else if(final != small && final != large){
		return high;
	}
	else if(median != small && median != large){
		return (high+low)/2;
	}
	return -1;
}

void swap(vector<int>& lists , int pos1, int pos2){
	int temp;
	temp = lists[pos1];
	lists[pos1] = lists[pos2];
	lists[pos2] = temp;
}

int partition(vector<int>& lists, int low, int high, int pivot){
	int i = low;
	int j = low;
	while( j<= high){
		if(lists[j] > pivot){
			j++;
		}
		else{
			swap(lists,i, j);
			i++;
			j++;
		}
	}
	swap(lists, low, i-1);
	return i-1;
}

void quickSort(vector<int>& lists, int low, int high, int &count){
	if(low < high){
		count += high-low;
		int mLoc = getMedian(lists, low, high);
		
		// For Question 1
		//int pivot = lists[low];
		
		// For Question 2
		//int pivot = lists[high];
		//swap(lists, low, high);
		
		// For Question 3
		int pivot = lists[mLoc];
		swap(lists, low, mLoc);
		
		int pos = partition(lists, low, high, pivot);
		quickSort(lists, low, pos-1, count);
		quickSort(lists, pos+1, high, count);
	}
}


int main(int argc, char** argv) {
	
	ifstream in;
	in.open("QuickSort.txt", ios::in);
	
	vector<int> lists;
	int element;
	while(in>>element){
		lists.push_back(element);
	}
	
	
	int count = 0;
	quickSort(lists, 0, lists.size()-1, count);
	
	for(int i = 0;i<lists.size();i++){
		cout<<lists[i]<<endl;
	}
	
	cout<<"Total Count is: "<<count<<endl;
}
