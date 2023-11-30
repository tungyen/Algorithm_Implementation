#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <sstream>
#include <unordered_set>
using namespace std;

bool cmp(const vector<int> &v1, const vector<int> &v2){

    return v1[2] > v2[2];
}

void build(string filename, vector<vector<int> >& lists){
    ifstream in;
    in.open(filename.c_str(), ios::in);
    string s;
    while(getline(in, s)){
        stringstream ss;
        ss << s;
        string token;
        vector<int> tmp;
        while(getline(ss,token,' ')){
            int ele = atoi(token.c_str());
            tmp.push_back(ele);
        }
        lists.push_back(tmp);
    }
}

void show(vector<vector<int> >& lists){
    for(int i = 0;i<lists.size();i++){
        cout<<lists[i][0]<<" "<<lists[i][1]<<" "<<lists[i][2]<<endl;
    }
}

int main(int argc, char** argv) {

    vector<vector<int> > lists;
    string filename = "edge.txt";
    build(filename, lists);
    int n = lists.size();
    int index = rand() % n;

    int ans = lists[index][2];
    unordered_set<int> X;
    X.insert(lists[index][0]);
    X.insert(lists[index][1]);
    lists[index][2] = INT_MAX;

    sort(lists.begin(), lists.end(), cmp);

    while(X.size()!=500){
        for(int i = 0;i<lists.size();i++){
            if(lists[i][2]==INT_MAX){
                continue;
            }
            if(X.count(lists[i][1]) && X.count(lists[i][0])){
                continue;
            }
            if(!X.count(lists[i][1]) && !X.count(lists[i][0])){
                continue;
            }
            if(X.count(lists[i][0])){
                X.insert(lists[i][1]);
            }
            else{
                X.insert(lists[i][0]);
            }
            ans += lists[i][2];
        }
    }

    cout<<ans;


    return 0;
}
