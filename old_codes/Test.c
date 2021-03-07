#include<iostream>
using namespace std;
void Test()
{
	cout<<"hello world"<<endl;
	vector<int> v1;
	int i;
	v1.push_back(2);
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(4);
	for(i=0; i<v1.size(); i++)
	{
		cout<<i<<"\t"<<v1[i]<<endl;
	}
	
}
