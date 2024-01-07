#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class BellmanFord{

public:
    BellmanFord(string path, int s){
        ifstream in(path);

        in >> V >> E;
        source = s-1;
        graph = vector<vector<int>>(V, vector<int>(V, INT_MAX));
        dp = vector<int>(V, INT_MAX);
        dp[source] = 0;

        int u;
        int v;
        int wt;
        while(in >> u >> v >> wt){
            graph[u-1][v-1] = wt;
        }
        in.close();
    }

    bool getShortestPath(){

        bool change;
        for(int i = 0;i<V;i++){

            change = false;
            for(int src = 0;src<V;src++){
                for(int dst = 0;dst<V;dst++){
                    if(graph[src][dst] == INT_MAX || dst == source){
                        continue;
                    }

                    if(dp[src] != INT_MAX && dp[src]+graph[src][dst] < dp[dst]){
                        dp[dst] = dp[src]+graph[src][dst];
                        change = true;
                    }
                }
            }
            if(!change){
                break;
            }
        }

        for(int src = 0;src<V;src++){
            for(int dst = 0;dst<V;dst++){
                if(graph[src][dst] == INT_MAX || dst == source){
                    continue;
                }

                if(dp[src] != INT_MAX && dp[src]+graph[src][dst] < dp[dst]){
                    return true;
                }
            }
        }
        return false;
    }

    vector<int> getDistance(){
        return dp;
    }

private:
    int V;
    int E;
    int source;
    vector<vector<int>> graph;
    vector<int> dp;
};

int main(){

    BellmanFord BF = BellmanFord("g3.txt", 1);
    bool negativeCycle = BF.getShortestPath();

    //vector<int> svPath;
    vector<int> svPath = BF.getDistance();
    if(!negativeCycle){
        //svPath = BF.getDistance();
        for(int i = 0;i<svPath.size();i++){
            cout<<"The distance from src to vertex "<<i+1<<" is: "<<svPath[i]<<endl;
        }
    }
    else{
        cout<<"There exists a negative cycle in the graph."<<endl;
        for(int i = 0;i<svPath.size();i++){
            cout<<"The distance from src to vertex "<<i+1<<" is: "<<svPath[i]<<endl;
        }
    }
    return 0;
}
