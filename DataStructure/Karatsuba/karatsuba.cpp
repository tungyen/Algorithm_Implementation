#include <iostream>
#include <sstream>
#include <string>
#include "math.h"
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int makeEqualLength(string &str1, string &str2)
{
    int len1 = str1.size();
    int len2 = str2.size();
    if (len1 < len2)
    {
        for (int i = 0 ; i < len2 - len1 ; i++)
            str1 = '0' + str1;
    }
    else if (len1 > len2)
    {
        for (int i = 0 ; i < len1 - len2 ; i++)
            str2 = '0' + str2;
    }
    if(len1%2==1){
    	str2 = '0'+str2;
    	str1 = '0'+str1;
	}
    return str1.length();
}

string longToString(unsigned long long n){
	string ans;
	while(n>0){
		char c = n%10 + '0';
		ans = c + ans;
		n /= 10;
	}
	return ans;
}

long long int karatsuba(long long int n1, long long int n2){
	if(n1<10 || n2<10){
		return n1*n2;
	}
	string s1 = longToString(n1);
	string s2 = longToString(n2);
	
	
	int l = makeEqualLength(s1, s2);
	int split = (int)l/2;
	
	long long int h1 = atoi(s1.substr(0, split).c_str());
	long long int l1 = atoi(s1.substr(l-split, split).c_str());
	long long int h2 = atoi(s2.substr(0, split).c_str());
	long long int l2 = atoi(s2.substr(l-split, split).c_str());
	
	long long int z0 = karatsuba(l1, l2);
	long long int z1 = karatsuba((l1+h1), (l2+h2));
	long long int z2 = karatsuba(h1, h2);
	return (z2*pow(10,2*split)) + ((z1-z2-z0)*pow(10, split)+z0);
	
}

int main(int argc, char** argv) {
	unsigned long long num1 = 3141592653589793238462643383279502884197169399375105820974944592;
    unsigned long long num2 = 2718281828459045235360287471352662497757247093699959574966967627;

    long long int result = karatsuba(num1, num2);
    cout<<result<<endl;
	return 0;
}
