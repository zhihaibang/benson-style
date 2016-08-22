//============================================================================
// Name        : main.cpp
// Author      : zhb
// Version     : 1.0
// Description : Ragerank algorithm main function
// Reference   : http://blog.csdn.net/hguisu/article/details/7996185
//============================================================================

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include "pagerank.h"

#define kPageCount 10
#define kFactor 0.85
#define kPrecision 0.001

using namespace std;


int main(int argc, const char *argv[])
{

	PageRank* pagerank = new PageRank();

	pagerank->page_count_ = kPageCount;
	pagerank->factor_ = kFactor;
	pagerank->precision_ = kPrecision;

	if(argc > 1){
		pagerank->page_count_ = atoi(argv[1]);
	}

	pagerank->InitialMatrix();

	int n = pagerank->page_count_;
	DIntArray &page_matrix = pagerank->page_matrix_;

	cout<<"Input Matrix:"<<endl;
	pagerank->DisplayIntMatrix(page_matrix, n);

	DDoubleArray probability_matrix;
	pagerank->ProbabilityMatrix(probability_matrix);

	// cout<<"Probability Matrix:"<<endl;
	// pagerank->DisplayDoubleMatrix(probability_matrix, n);

	
	DDoubleArray transposed_matrix;
	pagerank->TransposedMatrix(probability_matrix, transposed_matrix);

	// cout<<"Transposed Matrix:"<<endl;
	// pagerank->DisplayDoubleMatrix(transposed_matrix, n);

	DDoubleArray en_matrix;
	pagerank->ENMatrix(en_matrix);

	// cout<<"EN Matrix:"<<endl;
	// pagerank->DisplayDoubleMatrix(en_matrix, n);


	DDoubleArray a_matrix;
	pagerank->AMatrix(transposed_matrix, en_matrix, a_matrix);

	// cout<<"A Matrix:"<<endl;
	// pagerank->DisplayDoubleMatrix(a_matrix, n);

	vector<double> x_array;
	vector<double> pagerank_array;
	//假设初始每个网页的 PageRank值均为1
	for(int i=0; i<n; ++i)x_array.push_back(1);

	while(1)
	{
		pagerank_array.clear();
		pagerank->PageRankArray(a_matrix, x_array, pagerank_array);

		bool ret = pagerank->DifferenceOfArray(x_array, pagerank_array);

		if(ret)break;

		x_array.clear();

		for(int i=0;i<n;++i)x_array.push_back(pagerank_array[i]);
	}

	
	cout<<"PageRank Result:"<<endl;
	pagerank->DisplayArray(pagerank_array, n);

	return 0;
}
