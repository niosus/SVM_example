#include "svm.h"
#include <iostream>
using namespace std;

int main()
{
	svm_problem problem;
	problem.l=10;
	cout<<"labels "<<problem.l<<endl;
	return 0;
}