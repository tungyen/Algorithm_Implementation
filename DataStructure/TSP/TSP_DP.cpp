#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;
class TSP_DP{

public:

    TSP_DP(string path){

        ifstream in(path);
        in >> cityNum;
        M = (1 << cityNum) - 1;
        distance = vector<vector<double>>(cityNum, vector<double>(cityNum));
        DP = vector<vector<double>>(cityNum, vector<double>(M+1, -1));

        double x;
        double y;
        while(in >> x >> y){
            graph.push_back({x, y});
        }

        in.close();

        for(int i = 0;i<cityNum;i++){
            distance[i][i] = 0.0;
        }

        for(int i = 0;i<cityNum;i++){
            for(int j = i+1;j<cityNum;j++){
                double xDiff = graph[i].first-graph[j].first;
                double yDiff = graph[i].second-graph[j].second;
                double d = sqrt(xDiff*xDiff + yDiff*yDiff);
                distance[i][j] = d;
                distance[j][i] = d;
            }
        }
    }

    double minimumCost(int source, int visited){

        if(visited == M){
            return distance[source][0];
        }

        if(DP[source][visited]!=-1){
            return DP[source][visited];
        }

        double res = INT_MAX;
        for(int i = 0;i<cityNum;i++){
            if(i != source && !(visited&(1<<i))){
                double newRes = distance[source][i] + minimumCost(i, visited ^ (1<<i));
                res = min(res, newRes);
            }
        }
        //cout<<"Current result is: "<<res<<endl;
        return DP[source][visited] = res;
    }

private:
    int cityNum;
    int M;
    vector<pair<double, double>> graph;
    vector<vector<double>> distance;
    vector<vector<double>> DP;
};

int main(){

    TSP_DP tsp = TSP_DP("tsp.txt");
    double shortestDis = tsp.minimumCost(0, 1);
    cout<<"The minimim cost of the whole graph is: "<<shortestDis<<endl;

    return 0;
}



