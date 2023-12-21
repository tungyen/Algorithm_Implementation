#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Knapsack{

public:

    Knapsack(string path){

        ifstream in(path);
        in >> W >> n;

        dp = vector<vector<long long>>(2, vector<long long>(W+1));
        for(int i = 0;i<=W;i++){
            dp[0][i] = 0;
        }

        int v;
        int w;

        while(in >> v >> w){
            Data.push_back({v, w});
        }
    }

    int getOptimal(){

        for(int i = 1;i<=n;i++){
            for(int j = 0;j<=W;j++){
                if(Data[i-1].second > j){
                    dp[1][j] = dp[0][j];
                }
                else{
                    dp[1][j] = max(dp[0][j], dp[0][j-Data[i-1].second]+Data[i-1].first);
                }
            }
            for(int j = 0;j<=W;j++){
                dp[0][j] = dp[1][j];
                dp[1][j] = 0;
            }
        }

        return dp[0][W];
    }

private:
    int W;
    int n;
    vector<pair<int, int>> Data;
    vector<vector<long long>> dp;
};

int main(){

    Knapsack kp = Knapsack("knapsack_big.txt");
    int ans = kp.getOptimal();
    cout<<"The answer of Knapsack big is: "<<ans<<endl;
}

// The answer is 4243395
