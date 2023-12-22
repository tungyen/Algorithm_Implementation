#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;


class Kosaraju{

public:

    Kosaraju(string path){

        adjOri = vector<vector<int>>(V);
        adjRev = vector<vector<int>>(V);
        visited = vector<bool>(V, false);
        ifstream in(path);

        int u;
        int v;

        while(in >> u >> v){
            adjOri[u-1].push_back(v-1);
            adjRev[v-1].push_back(u-1);
        }
    }

    void DfsOri(int x){

        visited[x] = true;
        for(int u:adjOri[x]){
            if(!visited[u]){
                DfsOri(u);
            }
        }
        st.push(x);
    }

    void DfsRev(int x, vector<int>* scc){

        visited[x] = true;
        for(int u:adjRev[x]){
            if(!visited[u]){
                DfsRev(u, scc);
            }
        }
        scc->push_back(x);
    }

    vector<int> getTopSize(int top){

        for(int i = 0;i<V;i++){
            if(!visited[i]){
                DfsOri(i);
            }
        }

        for(int i=0;i<V;i++){
            visited[i] = false;
        }

        while(!st.empty()){

            int u = st.top();
            st.pop();

            if(!visited[u]){
                vector<int> scc;
                DfsRev(u, &scc);
                int Size = scc.size();
                pq.push(Size);
            }
        }

        vector<int> topSize;
        while(!pq.empty()){
            int size = pq.top();
            pq.pop();
            topSize.push_back(size);
        }
        return topSize;
    }

private:
    int V = 875714;
    vector<vector<int>> adjOri;
    vector<vector<int>> adjRev;
    vector<bool> visited;
    stack<int> st;
    priority_queue<int> pq;
};

int main(){

    int top = 5;
    Kosaraju ksrj = Kosaraju("SCC.txt");
    vector<int> top5 = ksrj.getTopSize(top);

    for(int i = 0;i<top;i++){
        cout<<"The current top 5 size is: "<<top5[i]<<endl;
    }
    return 0;
}

// The Answer is 434821, 968, 459, 313, 211
