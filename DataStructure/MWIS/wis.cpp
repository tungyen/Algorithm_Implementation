#include <iostream>
#include <vector>
#include <fstream>
#include <set>
using namespace std;

class WIS{

public:

    WIS(string path){

        ifstream in(path);
        in >> V;

        wt = vector<int>(V);
        int index = 0;
        int w;

        while(in>>w){
            wt[index] = w;
            index++;
        }

        dp = vector<int>(V+1);
        dp[0] = 0;
        dp[1] = wt[0];
    }

    set<int> DP(){

        for(int j = 2;j<=V;j++){
            dp[j] = max(dp[j-1], dp[j-2]+wt[j-1]);
        }

        set<int> s;
        int index = V;
        while(index > 1){
            if(dp[index-1] >= dp[index-2]+wt[index-1]){
                index--;
            }
            else{
                s.insert(index);
                index -= 2;
            }
        }

        if(index == 1){
            if(dp[index] == wt[index-1]){
                s.insert(index);
            }
        }

        return s;
    }

private:
    int V;
    vector<int> wt;
    vector<int> dp;

};

int main(){

    WIS wis = WIS("mwis.txt");
    set<int> vertexForMaxWeight = wis.DP();

    int candidates[8] = {1, 2, 3, 4, 17, 117, 517, 997};
    string ans = "";

    for(int i = 0;i<8;i++){
        int num = candidates[i];
        if(vertexForMaxWeight.count(num)){
            ans += '1';
        }
        else{
            ans += '0';
        }
    }

    cout<<"The binary representation of maximum weight set is: "<<ans<<endl;
    return 0;
}
