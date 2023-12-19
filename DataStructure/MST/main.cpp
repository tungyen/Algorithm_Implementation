#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <sstream>
#include <numeric>

using namespace std;

class MST{

    public:

        MST(string path){
            ifstream in;
            in.open(path.c_str(), ios::in);
            string s;
            int time = 0;
            while(getline(in, s)){

                stringstream ss;
                ss << s;
                string token;
                vector<int> tmp;
                while(getline(ss,token,' ')){
                    int ele = atoi(token.c_str());
                    tmp.push_back(ele);
                }

                if(time != 0){
                    edges.push_back(tmp);
                }
                else{
                    V = tmp[0];
                    E = tmp[1];
                }
                time++;
            }
            visited = vector<bool>(V, false);
        }

        static bool cmp(const vector<int> &v1, const vector<int> &v2){

            return v1[2] < v2[2];
        }

        int getminCost(){

            int index = rand() % E;
            int minCost = edges[index][2];
            visited[edges[index][0]] = true;
            visited[edges[index][1]] = true;
            edges.erase(edges.begin()+index);
            sort(edges.begin(), edges.end(), cmp);

            while(accumulate(visited.begin(), visited.end(), 0) < V-1){
                for(int i = 0;i<edges.size();i++){
                    int v1 = edges[i][0];
                    int v2 = edges[i][1];
                    int wt = edges[i][2];

                    if(visited[v1] && visited[v2]){
                        continue;
                    }
                    if(!visited[v1] && !visited[v2]){
                        continue;
                    }
                    minCost += wt;
                    visited[v1] = true;
                    visited[v2] = true;
                    edges.erase(edges.begin()+i);
                    break;
                }
            }
            return minCost;
        }

    private:
        vector<vector<int>> edges;
        int V;
        int E;
        vector<bool> visited;


};

int main(int argc, char** argv) {

    string filename = "edge.txt";
    MST mst = MST(filename);
    int minCost = mst.getminCost();

    cout<<"The cost of the Minimum Spanning Tree is: "<<minCost<<endl;
    return 0;
}
