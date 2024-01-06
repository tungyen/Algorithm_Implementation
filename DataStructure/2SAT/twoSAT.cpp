#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

using namespace std;


class twoSAT{

public:

    void addNode(int a, int b){
        graph[a].push_back(b);
        graphInv[b].push_back(a);
    }

    twoSAT(string path){

        ifstream in(path);

        in >> n;
        mx = 2 * n + 1;
        graph = vector<vector<int>>(mx);
        graphInv = vector<vector<int>>(mx);
        visited = vector<bool>(mx, false);
        visitedInv = vector<bool>(mx, false);
        SCC = vector<int>(mx);

        int a;
        int b;
        while(in >> a >> b){

            if(a > 0 && b > 0){
                addNode(a+n, b);
                addNode(b+n, a);
            }
            else if(a > 0 && b < 0){
                addNode(a+n, n-b);
                addNode(-b, a);
            }
            else if(a < 0 && b > 0){
                addNode(-a, b);
                addNode(b+n, n-a);
            }
            else{
                addNode(-a, n-b);
                addNode(-b, n-a);
            }
        }
        in.close();
    }

    void dfs(int cur){
        if(visited[cur]){
            return;
        }
        visited[cur] = true;
        for(int i = 0;i<graph[cur].size();i++){
            dfs(graph[cur][i]);
        }
        s.push(cur);
    }

    void dfsInv(int cur){
        if(visitedInv[cur]){
            return;
        }
        visitedInv[cur] = true;
        for(int i = 0;i<graphInv[cur].size();i++){
            dfsInv(graphInv[cur][i]);
        }
        SCC[cur] = count;
    }

    bool checkSatisfied(){
        for(int i = 1;i<mx;i++){
            if(!visited[i]){
                dfs(i);
            }
        }

        while(!s.empty()){
            int top = s.top();
            s.pop();

            if(!visitedInv[top]){
                dfsInv(top);
                count++;
            }
        }

        for(int i = 1;i<=n;i++){
            if(SCC[i] == SCC[i+n]){
                return false;
            }
        }
        return true;
    }

private:
    int n;
    int mx;
    int count = 1;
    vector<vector<int>> graph;
    vector<vector<int>> graphInv;
    vector<bool> visited;
    vector<bool> visitedInv;
    stack<int> s;
    vector<int> SCC;
};

int main(){

    for(int i = 1;i<=6;i++){
        string path = "2sat" + to_string(i) + ".txt";
        twoSAT twoSat = twoSAT(path);
        bool res = twoSat.checkSatisfied();
        cout<<"The "<<i<<" data is: "<<res<<endl;
    }

    return 0;
}

