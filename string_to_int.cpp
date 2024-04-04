#include<bits/stdc++.h>
using namespace std;


int string_to_int(string s)
{
    int ans=0;
    for(auto i : s)
    {
        ans*=10;
        ans+=i-'0';
    }
    return ans;
}
int main()
{
    string s;
    cin>>s;
    cout<<string_to_int(s);

    return 0;
}