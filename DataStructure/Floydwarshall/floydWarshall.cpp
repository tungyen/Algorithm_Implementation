#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class FloydWarshall{

public:

    FloydWarshall(string path){

        ifstream in(path);
        in >> V;
        in >> E;
        graph = vector<vector<int>>(V, vector<int>(V, INT_MAX));
        for(int i = 0;i<V;i++){
            graph[i][i] = 0;
        }

        int u;
        int v;
        int length;
        while(in >> u >> v >> length){
            graph[u-1][v-1] = length;
        }
    }

    bool shortestPath(){

        for(int k = 0;k<V;k++){
            for(int i = 0;i<V;i++){
                for(int j = 0;j<V;j++){
                    if(graph[i][k] == INT_MAX || graph[k][j] == INT_MAX){
                        continue;
                    }
                    else if(graph[i][k]+graph[k][j] < graph[i][j]){
                        graph[i][j] = graph[i][k]+graph[k][j];
                    }
                }
            }
        }

        for(int i = 0;i<V;i++){
            if(graph[i][i] < 0){
                return true;
            }
        }

        return false;
    }

    vector<vector<int>> getAPSP(){
        return graph;
    }

private:
    int V;
    int E;
    vector<vector<int>> graph;
};

int main(){

    for(int i = 1;i<=3;i++){
        string path = "g" + to_string(i) + ".txt";
        FloydWarshall FW = FloydWarshall(path);
        bool negativeCycle = FW.shortestPath();
        if(negativeCycle){
            cout<<"Graph "<<i<<" contains a negative cycle."<<endl;
        }
        else{
            vector<vector<int>> APSP = FW.getAPSP();
            int shortest = INT_MAX;
            for(int src = 0;src<APSP.size();src++){
                for(int dst = 0;dst<APSP[0].size();dst++){
                    shortest = min(shortest, APSP[src][dst]);
                }
            }
            cout<<"The shortest distance between any two vertexs in Graph "<<i<<" is: "<<shortest<<endl;
        }
    }

    return 0;
}

