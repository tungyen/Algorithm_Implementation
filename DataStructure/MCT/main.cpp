#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class MinCutter{

    public:

        MinCutter(string path){
            string line;
            fstream in;
            in.open(path.c_str());

            while(getline(in,line)){

                int vertex;
                vector<int> edges;
                stringstream ss(line);
                string token;
                int time = 0;

                while(ss >> token){
                    if(time == 0){
                        vertex = stoi(token);
                    }
                    else{
                        edges.push_back(stoi(token));
                    }
                    time++;
                }

                graph[vertex] = edges;
                total_edges += edges.size();
            }
            in.close();
        }

        pair<int, int> pickRandom(){

            int randEdge = rand() % total_edges;

            for(auto ele:graph){
                int vertex = ele.first;
                vector<int> edges = ele.second;
                if(edges.size() <= randEdge){
                    randEdge -= edges.size();
                }
                else{
                    int fromVertex = vertex;
                    int toVertex = edges[randEdge];
                    return {fromVertex, toVertex};
                }
            }
            return {0, 0};
        }

        int findMinCut(){

            int minCut = 0;
            while(graph.size() > 2){

                pair<int, int> randomEdge = pickRandom();
                int v1 = randomEdge.first;
                int v2 = randomEdge.second;
                total_edges -= graph[v1].size();
                total_edges -= graph[v2].size();

                // Extending the v2 into v1
                std::vector<int> edgesNew = graph[v1];
                edgesNew.insert(edgesNew.end(), graph[v2].begin(), graph[v2].end());
                graph[v1] = edgesNew;

                for(auto ele:graph[v2]){
                    // Remove v2 from all edge that contains v2
                    graph[ele].erase(remove(graph[ele].begin(), graph[ele].end(), v2), graph[ele].end());
                    // Add v1 to all edges that contains v2
                    graph[ele].push_back(v1);
                }

                // Remove the redundant v1 from v1 edge
                graph[v1].erase(remove(graph[v1].begin(), graph[v1].end(), v1), graph[v1].end());
                total_edges += graph[v1].size();
                graph.erase(v2);
            }
            for(auto ele:graph){
                minCut = ele.second.size();
            }
            return minCut;
        }

    private:
        unordered_map<int, vector<int>> graph;
        int total_edges = 0;

};

int main()
{
    int minCut = INT_MAX;
    int maxTime = 100;

    for(int i = 0;i<maxTime;i++){
        MinCutter mc = MinCutter("minCut.txt");
        int minCutResult = mc.findMinCut();
        if(minCutResult < minCut){
            minCut = minCutResult;
        }
    }
    cout<<"The minCut is end of "<<minCut<<endl;
    return 0;
}

// Result is 17
