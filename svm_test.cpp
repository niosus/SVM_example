#include "svm.h"
#include <ctype.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;


#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

struct svm_parameter param;		// set by parse_command_line
struct svm_problem prob;		// set by read_problem
struct svm_model *model;
struct svm_node *x_space;

vector<int> generateLabels(int labelsSize)
{
	std::vector<int> labels;
	for (int i=0; i<labelsSize; ++i)
	{
		labels.push_back(i%2+1);
	}
	return labels;
}

vector<vector<double> > generateData(int problemSize, int featureNum)
{
	std::vector<std::vector<double> > data;
	for (int i=0; i<problemSize; ++i)
	{
		std::vector<double> featureSet;
		for (int j=0; j<featureNum; ++j)
		{
			cout<<"feature pushed"<<endl;
			featureSet.push_back(j);
		}
		data.push_back(featureSet);
	}
	return data;
}

int main()
{
	//here I will create a small artificial problem just for illustration
	int sizeOfProblem = 3; //number of lines with labels
	int elements = 4; //number of features for each data vector

	vector<vector<double> > data = generateData(sizeOfProblem,elements);
	vector<int> labels = generateLabels(sizeOfProblem);

	cout<<"data size = "<<data.size()<<endl;
	cout<<"labels size = "<<labels.size()<<endl;
	//initialize the size of the problem with just an int	
	prob.l = sizeOfProblem;
	//here we need to give some memory to our structures
	// @param prob.l = number of labels
	// @param elements = number of features for each label
	prob.y = Malloc(double,prob.l); //space for prob.l doubles
	prob.x = Malloc(struct svm_node *, prob.l); //space for prob.l pointers to struct svm_node
	x_space = Malloc(struct svm_node, (elements+1) * prob.l); //memory for pairs of index/value

	//here we are going to initialize it all a bit

	
	//initialize the different lables with an array of labels
	for (int i=0; i < prob.l; ++i)
	{
		prob.y[i] = labels[i];
		cout<<"prob.y["<<i<<"] = "<<prob.y[i]<<endl;
	}
	//initialize the svm_node vector with input data array as follows:
	int j=0; //counter to traverse x_space[i];
	for (int i=0;i < prob.l; ++i)
	{
		//set i-th element of prob.x to the address of x_space[j]. 
		//elements from x_space[j] to x_space[j+data[i].size] get filled right after next line
		prob.x[i] = &x_space[j];
		for (int k=0; k<data[i].size(); ++k, ++j)
		{
			x_space[j].index=k+1; //index of value
			x_space[j].value=data[i][k]; //value
			cout<<"x_space["<<j<<"].index = "<<x_space[j].index<<endl;
			cout<<"x_space["<<j<<"].value = "<<x_space[j].value<<endl;
		}
		x_space[j].index=-1;//state the end of data vector
		x_space[j].value=0;
		cout<<"x_space["<<j<<"].index = "<<x_space[j].index<<endl;
		cout<<"x_space["<<j<<"].value = "<<x_space[j].value<<endl;
		j++;

	}

	//ok, let's try to print it
	for (int i = 0; i < prob.l; ++i)
	{
		cout<<"line "<<i<<endl;
		cout<<prob.y[i]<<"---";
		for (int k = 0; k < elements; ++k)
		{
			int index = (prob.x[k])->index;
			double value = (prob.x[k])->value; 
			cout<<index<<":"<<value<<" ";
		}
		cout<<endl;
	}
	cout<<"all ok"<<endl;
	return 0;
}