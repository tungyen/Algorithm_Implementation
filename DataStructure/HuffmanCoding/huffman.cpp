#include <iostream>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

class Huffman{

    struct HuffNode{
        int label;
        unsigned wt;
        HuffNode* left;
        HuffNode* right;

        HuffNode(int label, unsigned wt){
            left = nullptr;
            right = nullptr;
            this->label = label;
            this->wt = wt;
        }
    };

    struct Cmp{
        bool operator()(HuffNode* n1, HuffNode* n2) {
            return n1->wt > n2->wt;
        }
    };

public:

    Huffman(string path){

        ifstream in(path);
        in >> numSymbols;

        int wt;
        while(in >> wt){
            HuffNode* tmp = new HuffNode(wt, wt);
            pq.push(tmp);
        }
    }

    pair<int, int> getDepth(){

        while(pq.size() != 1){

            struct HuffNode* left = pq.top();
            pq.pop();

            struct HuffNode* right = pq.top();
            pq.pop();

            struct HuffNode* top = new HuffNode(-1, left->wt + right->wt);
            top->left = left;
            top->right = right;

            pq.push(top);
        }

        return {maxDepth(pq.top()), minDepth(pq.top())};
    }

    int maxDepth(HuffNode* root){

        if(!root){
            return -1;
        }

        int l = maxDepth(root->left);
        int r = maxDepth(root->right);
        return max(l, r) + 1;
    }

    int minDepth(HuffNode* root){

        if(!root){
            return -1;
        }

        int l = minDepth(root->left);
        int r = minDepth(root->right);
        return min(l, r) + 1;
    }

private:
    int numSymbols;
    priority_queue<HuffNode*, vector<HuffNode*>, Cmp> pq;

};

int main(){

    Huffman huff = Huffman("huffman.txt");
    pair<int, int> HuffDepth = huff.getDepth();
    cout<<"The maximum depth of the Huffman is: "<<HuffDepth.first<<endl;
    cout<<"The minimum depth of the Huffman is: "<<HuffDepth.second<<endl;
    return 0;
}

// The maximum depth is 19
// The minimum depth is 9
