// DecisionTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include"DecisionTree.h"
using namespace std;
int main() {
	Tree* tree = new Tree;
	cout << "Trained..........." << endl;

	//while(1)
	tree->GetOutPut();

	return 0;
}

