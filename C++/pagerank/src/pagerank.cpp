/*
 * pagerank.cpp
 *
 *  Created on: 2015/11/05
 *  Author: zhb
 */
#include <iostream>
#include "pagerank.h"
using namespace std;

void PageRank::InitialMatrix()
{
	int n = this->page_count_;
	srand((int)time(NULL));
	for(int i=0 ; i<n; ++i){
		vector<int> array;
		for(int j=0; j<n; ++j){
			array.push_back(random(2));
		}
		this->page_matrix_.push_back(array);
	}	
}


void PageRank::DisplayIntMatrix(DIntArray data, int m)
{
	for(int i=0 ; i<m; ++i){
		for(int j=0; j<m; ++j){
			cout<<data[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;	
}

void PageRank::DisplayDoubleMatrix(DDoubleArray data, int m)
{
	for(int i=0 ; i<m; ++i){
		for(int j=0; j<m; ++j){
			cout<<setprecision(4)<<data[i][j]<<", ";
		}
		cout<<endl;
	}
	cout<<endl;
}


void PageRank::DisplayArray(vector<double> data, int m)
{
	for(int i=0 ; i<m; ++i){
		cout<<setprecision(4)<<data[i]<<"  ";
	}
	cout<<endl;
}


void PageRank::ProbabilityMatrix(DDoubleArray &probability_matrix)
{
	int s = 0 ;
	int n = this->page_count_;
	for(int i=0; i<n; ++i){
		vector<double> array;
		s = 0;
		for(int j=0; j<n; ++j){
			s += this->page_matrix_[i][j];
		}
		for(int j=0; j<n; ++j){
			double p = 0;
			if(s > 0)p = this->page_matrix_[i][j]/(double)s;
			array.push_back(p);
		}
		probability_matrix.push_back(array);
	}
}


void PageRank::TransposedMatrix(DDoubleArray &input, DDoubleArray &output)
{
	int n = this->page_count_;
	for(int i=0; i<n; ++i){
		vector<double> array;
		for(int j=0; j<n; ++j){
			array.push_back(input[j][i]);
		}		
		output.push_back(array);
	}
}


void PageRank::ENMatrix(DDoubleArray &en_matrix)
{
	int n = this->page_count_;
	for(int i=0; i<n; ++i){
		vector<double> array;
		for(int j=0; j<n; ++j){
			double p = 1/(double)n;
			array.push_back(p);
		}		
		en_matrix.push_back(array);
	}
}


void PageRank::AMatrix(DDoubleArray transposed_matrix, DDoubleArray en_matrix, DDoubleArray &a_matrix)
{
	double factor = this->factor_;
	int n = this->page_count_;
	for(int i=0; i<n; ++i){
		vector<double> array;
		for(int j=0; j<n; ++j){
			double p = factor*transposed_matrix[i][j] + (1-factor)*en_matrix[i][j];
			array.push_back(p);
		}
		a_matrix.push_back(array);
	}
}

void PageRank::PageRankArray(DDoubleArray a_matrix, vector<double> x_array, vector<double> &pagerank_array)
{
	int n = this->page_count_;
	double s;
	for(int i=0; i<n; ++i){
		s = 0;
		for(int j=0; j<n; ++j){
			s += a_matrix[i][j]*x_array[j];
		}
		pagerank_array.push_back(s);
	}
}


bool PageRank::DifferenceOfArray(vector<double> x_array, vector<double> pagerank_array)
{
	double precision = this->precision_;
	int n = this->page_count_;
	for(int i=0;i<n;++i){
		double d = fabs(x_array[i]-pagerank_array[i]);
		if(d > precision)return false;
	}
	return true;
}

