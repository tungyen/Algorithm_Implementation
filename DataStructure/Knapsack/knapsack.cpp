#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Knapsack{

public:

    Knapsack(string path){

        ifstream in(path);
        in >> W >> n;

        dp = vector<vector<int>>(n+1, vector<int>(W+1));
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
                    dp[i][j] = dp[i-1][j];
                }
                else{
                    dp[i][j] = max(dp[i-1][j], dp[i-1][j-Data[i-1].second]+Data[i-1].first);
                }
            }
        }

        return dp[n][W];
    }

private:
    int W;
    int n;
    vector<pair<int, int>> Data;
    vector<vector<int>> dp;
};

int main(){

    Knapsack kp = Knapsack("knapsack1.txt");
    int ans = kp.getOptimal();
    cout<<"The answer of Knapsack small is: "<<ans<<endl;
}

// The answer is 2493893
