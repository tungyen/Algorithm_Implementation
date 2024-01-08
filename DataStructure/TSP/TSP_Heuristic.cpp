#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;
class TSP_Heuristic{

public:

    TSP_Heuristic(string path){

        ifstream in(path);
        in >> cityNum;
        visited = vector<bool>(cityNum, false);

        int index;
        double x;
        double y;
        while(in >> index >> x >> y){
            graph.push_back({x, y});
        }

        in.close();
    }

    double shortestDist(){

        visited[0] = true;
        while(numVisited < cityNum){
            for(int i = 1;i<cityNum;i++){
                if(!visited[i]){
                    double xDiff = graph[i].first-graph[source].first;
                    double yDiff = graph[i].second-graph[source].second;
                    double d = sqrt(xDiff*xDiff + yDiff*yDiff);
                    if(d < minDist){
                        minDist = d;
                        end = i;
                    }
                }
            }
            totalDist += minDist;
            visited[end] = true;
            source = end;
            minDist = DBL_MAX;
            numVisited++;
        }

        double xDiffFinal = graph[0].first-graph[source].first;
        double yDiffFinal = graph[0].second-graph[source].second;
        double dFinal = sqrt(xDiffFinal*xDiffFinal + yDiffFinal*yDiffFinal);
        totalDist += dFinal;
        return totalDist;
    }

private:
    int cityNum;
    vector<bool> visited;
    vector<pair<double, double>> graph;
    double totalDist = 0.0;
    int source = 0;
    int numVisited = 1;
    int end;
    double minDist = DBL_MAX;
};

int main(){

    TSP_Heuristic tspg = TSP_Heuristic("nn.txt");
    double shortestDist = tspg.shortestDist();
    cout << fixed;
    cout <<"The shortest path distance is: "<< shortestDist << endl;
    return 0;
}

// The answer is 1203406.501271




