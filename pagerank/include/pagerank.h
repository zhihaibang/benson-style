/*
 * pagerank.h
 *
 *  Created on: 2015/11/05
 *  Author: zhb
 */
#ifndef PAGERANK_H_
#define PAGERANK_H_

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

#define random(x) (rand()%x)


typedef vector<vector<int> > DIntArray;
typedef vector<vector<double> > DDoubleArray;
		

class PageRank{

	public:
		void InitialMatrix();
		void DisplayIntMatrix(DIntArray data, int m);
		void DisplayDoubleMatrix(DDoubleArray data, int m);
		void DisplayArray(vector<double> data, int m);

		void ProbabilityMatrix(DDoubleArray &probability_matrix);
		void TransposedMatrix(DDoubleArray &input, DDoubleArray &output);
		void ENMatrix(DDoubleArray &en_matrix);
		void AMatrix(DDoubleArray transposed_matrix, DDoubleArray en_matrix, DDoubleArray &a_matrix);
		void PageRankArray(DDoubleArray a_matrix, vector<double> x_array, vector<double> &pagerank_array);
		bool DifferenceOfArray(vector<double> x_array, vector<double> pagerank_array);
		
		int page_count_;
		DIntArray page_matrix_;
		double factor_;
		double precision_;
		
};

#endif /* PAGERANK_H_ */
