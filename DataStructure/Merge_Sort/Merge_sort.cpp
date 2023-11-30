#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

using namespace std;

vector<int> mergeSort(vector<int>& l1, vector<int>& l2, long long &count){
	int n1 = l1.size();
	int n2 = l2.size();
	
	vector<int> ans;
	
	int i = 0;
	int j = 0;
	
	while(i<n1 && j <n2){
		if(l1[i]<l2[j]){
			ans.push_back(l1[i]);
			i++;
		}
		else{
			ans.push_back(l2[j]);
			j++;
			count += (long long)n1-i;
		}
	}
	while(i<n1){
		ans.push_back(l1[i++]);
	}
	while(j<n2){
		ans.push_back(l2[j++]);
	}
	return ans;
}

vector<int> sort(vector<int>& vec, long long &count){
	int n = vec.size();
	if(n > 1){
		int mid = n / 2;
		vector<int> first(vec.begin(), vec.begin()+mid);
		vector<int> second(vec.begin()+mid, vec.end());
		
		vector<int> B = sort(first, count);
		vector<int> C = sort(second, count);
		vector<int> D = mergeSort(B, C, count);
		return D;
	}
	else{
		return vec;
	}
}

int main(int argc, char** argv) {
	
	ifstream in;
	in.open("Inversion.txt", ios::in);
	
	vector<int> lists;
	int element;
	while(in>>element){
		lists.push_back(element);
	}
	
	long long count = 0;
	vector<int> result = sort(lists, count);
	cout<<count<<endl;
	
	return 0;
}
