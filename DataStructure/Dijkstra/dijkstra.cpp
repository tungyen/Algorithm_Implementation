#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include<sstream>

using namespace std;

class Dijkstra{

    // pair<int, int> p -> p.first is distance, p.second is vertex.

    static bool cmp(pair<int, int> p1, pair<int, int> p2){
        if(p1.first == p2.first){
            return p1.second > p2.second;
        }
        else{
            return p1.first > p2.first;
        }
    }

public:

    Dijkstra(string path, int s){

        start = s;
        graph = vector<vector<int>>(V, vector<int>(V, -1));
        parents = vector<int>(V, -1);
        dis = vector<int>(V, INT_MAX);
        visited = vector<bool>(V, false);
        dis[start-1] = 0;
        pq.push({0, start-1});

        ifstream in(path);
        string input;

        while(getline(in, input)){

            istringstream iss(input);
            int Vstart;
            iss >> Vstart;

            int Vend;
            int wt;
            char comma;
            while(iss >> Vend >> comma >> wt){
                graph[Vstart-1][Vend-1] = wt;
            }
        }
    }

    vector<int> getShortestPath(vector<int> desiredVertex){

        while(!pq.empty()){
            pair<int, int> cur = pq.top();
            int u = cur.second;
            int distance = cur.first;
            pq.pop();
            if(!visited[u]){
                visited[u] = true;
                for(int v = 0;v<V;v++){
                    if(visited[v]){
                        continue;
                    }
                    if(graph[u][v] ==-1 || u == v){
                        continue;
                    }
                    if(distance+graph[u][v] < dis[v]){
                        dis[v] = distance+graph[u][v];
                        parents[v] = u;
                        pq.push({dis[v], v});
                    }
                }

            }
        }

        vector<int> shortestPath;
        for(int ele:desiredVertex){
            shortestPath.push_back(dis[ele-1]);
        }
        return shortestPath;
    }

private:

    int V = 200;
    int start;
    vector<int> parents;
    vector<int> dis;
    vector<bool> visited;
    vector<vector<int>> graph;
    //priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(&cmp)> pq;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
};

int main(){

    Dijkstra dijkstra = Dijkstra("dijkstraData.txt", 1);
    vector<int> desire = {7, 37, 59, 82, 99, 115, 133, 165, 188, 197};
    vector<int> shortestPathDesire = dijkstra.getShortestPath(desire);
    for(int i = 0;i<desire.size();i++){
        cout<<"The shortest path of vertex "<<desire[i]<<" is: "<<shortestPathDesire[i]<<endl;
    }
    return 0;
}
