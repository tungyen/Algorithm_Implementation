#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class BellmanFord{

public:
    BellmanFord(vector<vector<int>> adj){

        V = adj.size()+1;

        graph = adj;
        for(int row = 0;row<graph.size();row++){
            graph[row].push_back(0);
        }
        vector<int> newRow = vector<int>(V, 0);
        graph.push_back(newRow);
        dp = vector<int>(V, INT_MAX);
        dp[V-1] = 0;
    }

    bool getShortestPath(){

        bool change;
        for(int i = 0;i<V;i++){

            change = false;
            for(int src = 0;src<V;src++){
                for(int dst = 0;dst<V;dst++){
                    if(graph[src][dst] == INT_MAX || dst == V-1){
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
                if(graph[src][dst] == INT_MAX || dst == V-1){
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
    vector<vector<int>> graph;
    vector<int> dp;
};

class JohnsonAlgorithm{

public:

    JohnsonAlgorithm(string path){

        ifstream in(path);
        in >> V >> E;
        graph = vector<vector<int>>(V, vector<int>(V, INT_MAX));
        for(int i = 0;i<V;i++){
            graph[i][i] = 0;
        }

        int u;
        int v;
        int wt;
        while(in >> u >> v >> wt){
            graph[u-1][v-1] = wt;
        }
        in.close();
    }

    vector<int> pathFromS(){
        BellmanFord BF = BellmanFord(graph);
        BF.getShortestPath();
        return BF.getDistance();
    }

    void updateGraph(){

        vector<int> sPath = pathFromS();
        for(int src = 0;src<V;src++){
            for(int dst = 0;dst<V;dst++){
                if(graph[src][dst] == INT_MAX){
                    continue;
                }
                graph[src][dst] += sPath[src] - sPath[dst];
            }
        }
    }

    vector<vector<int>> APSP(){

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
                cout<<"There is still a negative cycle."<<endl;
                break;
            }
        }
        return graph;
    }

private:
    int V;
    int E;
    vector<vector<int>> graph;
};


int main(){

    JohnsonAlgorithm JA = JohnsonAlgorithm("g3.txt");
    JA.updateGraph();
    vector<vector<int>> apsp = JA.APSP();
    for(int row = 0;row<apsp.size();row++){
        for(int col = 0;col<apsp[0].size();row++){
            cout<<"The distance between vertex "<<row+1<<" and "<<col+1<<" is: "<<apsp[row][col]<<endl;
        }
    }
    return 0;
}
