#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

class Kclustering{

public:
    Kclustering(string path){

        ifstream in(path);
        in >> V;

        parents = vector<int>(V);
        ranks = vector<int>(V, 0);

        for(int i = 0;i<V;i++){
            parents[i] = i;
        }

        edges.clear();


        while (!in.eof()) {
            int v1, v2, wt;
            in >> v1 >> v2 >> wt;
            edges.push_back({wt, {v1-1, v2-1}});

        }

        in.close();
    }

    int getNumCluster(){

        int count = 0;
        for(int i = 0;i<edges.size();i++){
            if(i==parents[i]){
                count++;
            }
        }
        return count;
    }

    int find(int i){
        if(i==parents[i]){
            return i;
        }
        else{
            return find(parents[i]);
        }
    }

    void Union(int v1, int v2){

        if(ranks[v1] > ranks[v2]){
            parents[v2] = parents[v1];
            ranks[v1]++;
        }
        else if(ranks[v2] > ranks[v1]){
            parents[v1] = parents[v2];
            ranks[v2]++;
        }
        else{
            parents[v2] = parents[v1];
            ranks[v1]++;
        }
    }

    int Clustering(int K){
        int v1;
        int v2;

        int index = 0;

        sort(edges.begin(), edges.end());


        while(K != getNumCluster()){

            v1 = find(edges[index].second.first);
            v2 = find(edges[index].second.second);


            if(v1 != v2){
                Union(v1, v2);
            }
            index++;

        }

        do {
            v1 = find(edges[index].second.first);
            v2 = find(edges[index].second.second);
            index++;
        } while (v1 == v2);
        return edges[index-1].first;
    }

private:
    vector<pair<int, pair<int, int>>> edges;
    int V;
    vector<int> parents;
    vector<int> ranks;
};

int main()
{
    string path = "clustering1.txt";
    int K = 4;
    Kclustering KC = Kclustering(path);
    int maxSpacingK = KC.Clustering(K);

    cout<<"The Max Spacing of K is: "<<maxSpacingK<<endl;
    return 0;
}

//The answer of cluster_small is 106
