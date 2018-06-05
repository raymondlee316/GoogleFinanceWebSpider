////stock.cpp
//#include "stock.h"
#include"stdafx.h"
//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <algorithm>
//#include <iomanip>
//#include <fstream>
//#include <string>
//#include <climits>
//#include <cstdint>
//#include <typeinfo>
////#include <ql/math/statistics/generalstatistics.hpp>
//using namespace std;
////using namespace QuantLib;
//
//double stock::VaR(double percentile) {
//	vector<double> PnL;
//	double SpotPrice = close.back(); // Price at today
//	for (auto itr = close.begin()+1;itr != close.end()-1; ++itr) {
//		PnL.push_back(*itr-*(itr-1));
//	}
//	sort(PnL.begin(), PnL.end()); // sort P&L from loss to profit
//	int ind = floor(percentile / 100 * PnL.size()); // find the index according to percentile
//	VaR_min = PnL[ind];
//	vector<double>::const_iterator first = PnL.begin();
//	vector<double>::const_iterator last = PnL.begin() + ind - 1;
//	vector<double> ES_vector(first, last); // use a new vector to store losses that are beyond VaR
//	//ES = std::accumulate(ES_vector.begin(), ES_vector.end(), 0.0) / ES_vector.size(); // caculate expected shortfall (average of ES_vector)
//};
//
////double VaR::risk_VaR(const vector<double> & pl, double percentile)
////{
////	/*
////	this function returns the Value-at-Risk at certain percentile,
////	and print out the expected shortfall, given the vector of return distribution
////	typically, when there is a loss, VaR is reported as a negative number
////	input:
////	pl: profit&loss distribution
////	percentile: the 0-100 percentile Domer chooses
////	(eg: 99% VaR: percentile=1; 95% VaR: percentile=5)
////	*/
////	vector<double> v = pl;
////	sort(v.begin(), v.end()); // sort P&L from loss to profit
////	int ind = floor(percentile / 100 * pl.size()); // find the index according to percentile
////	vector<double>::const_iterator first = v.begin();
////	vector<double>::const_iterator last = v.begin() + ind - 1;
////	vector<double> ES_vector(first, last); // use a new vector to store losses that are beyond VaR
////	double ES = std::accumulate(ES_vector.begin(), ES_vector.end(), 0.0) / ES_vector.size(); // caculate expected shortfall (average of ES_vector)
////	cout << "Expected shortfall in " << (100 - percentile) << "% is " << ES << endl; // print out ES
////	return v[ind]; // return the value of VaR
////}
////
//
//
//
//
//
//
//
////vector<double> stock::calreturn(vector<double> price) {
////	vector<double> ::iterator itr;
////	vector<double> return;
////	for (itr = price.begin(); itr < price.end - 1; itr++)
////	{
////		double r = *(itr + 1) / *itr - 1;
////		return.push_back(r*250*390);
////}
////	return return;
////};
////
////double stock::low(vector<double> price) {
////	generalstatistics stats;
////	stats.addsequence(lastdayprice.begin(), lastdayprice.end());
////		return stats.min();
////};
////double stock::high(vector<double> price) {
////	generalstatistics stats;
////	stats.addsequence(lastdayprice.begin(), lastdayprice.end());
////	return stats.max();
////};
////double stock::std(vector<double> price) {
////	generalstatistics stats;
////	stats.addsequence(lastdayprice.begin(), lastdayprice.end());
////	return stats.standarddeviation();
////};
////double stock::skew(vector<double> price) {
////	generalstatistics stats;
////	stats.addsequence(lastdayprice.begin(), lastdayprice.end());
////	return stats.skewness;
////};
////double stock::kurtosis(vector<double> price) {
////	generalstatistics stats;
////	stats.addsequence(lastdayprice.begin(), lastdayprice.end());
////	return stats.kurtosis();
////};
////double stock::sharpe(vector<double> price) {
////	vector<double> return= calreturn(price);
////	double rf = 0.02946;
////	generalstatistics stats;
////	stats.addsequence(return.begin(), return.end());
////	double er = stats.mean();
////	double std = this->std(return);
////	return (er - rf) / std;
////};
////double stock::informratio(vector<double> price) {};
////double stock::jensen(vector<double> price) {};
////double stock::var(vector<double> price);
////double stock::es(vector<double> price);
////double stock::downsiderisk(vector<double> price);
////double stock::riskpremium(vector<double> price);