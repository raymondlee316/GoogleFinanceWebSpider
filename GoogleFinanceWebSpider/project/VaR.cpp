#include"stdafx.h"
#include <string> 
#include<stdio.h>
#include <iostream>
#include <sstream> 
#include <fstream>
#include <vector>
#include <list>
#include <locale>
#include <iomanip>
#include "curl.h"
#include <locale>
#include <locale.h>
#include "stock.h"
#include "ExcelDriver\ExcelDriver.hpp"
#include <algorithm>
#include <thread>
#include <numeric>
#include "DownloadData.h"

using namespace std;

void VaR_historical(vector<double> close, double percentile) {
	vector<double> PnL_dollar;
	vector<double> PnL_return;
	double SpotPrice = close.back(); // Price at today
	for (auto itr = close.begin() + 1;itr != close.end() - 1; ++itr) {
		PnL_dollar.push_back(*itr - *(itr - 1));
		PnL_return.push_back((*itr - *(itr - 1)) / (*(itr - 1)));
	}
	sort(PnL_dollar.begin(), PnL_dollar.end());
	sort(PnL_return.begin(), PnL_return.end()); // sort P&L from loss to profit
	int ind1 = floor(percentile / 100 * PnL_dollar.size()); // find the index according to percentile
	int ind2 = floor(percentile / 100 * PnL_return.size());
	double VaR_min_dollar = PnL_dollar[ind1];
	double VaR_min_return = PnL_return[ind2] * 100;

	vector<double>::const_iterator first = PnL_dollar.begin();
	vector<double>::const_iterator last = PnL_dollar.begin() + ind1 - 1;
	vector<double> ES_vector1(first, last); // use a new vector to store losses that are beyond VaR
	double ES_min_dollar = std::accumulate(ES_vector1.begin(), ES_vector1.end(), 0.0) / ES_vector1.size(); // caculate expected shortfall (average of ES_vector)

	first = PnL_return.begin();
	last = PnL_return.begin() + ind2 - 1;
	vector<double> ES_vector2(first, last); // use a new vector to store losses that are beyond VaR
	double ES_min_return = 100 * std::accumulate(ES_vector2.begin(), ES_vector2.end(), 0.0) / ES_vector2.size(); // caculate expected shortfall (average of ES_vector)

	cout << "Minutely VaR is:" << VaR_min_dollar << "$," << VaR_min_return << "%" << endl;
	cout << "Daily VaR is:" << VaR_min_dollar * sqrt(390) << "$," << VaR_min_return * sqrt(390) << "%" << endl;
	cout << "Minutely Expected Shortfall is:" << ES_min_dollar << "$," << ES_min_return << "%" << endl;
	cout << "Daily Expected Shortfall is:" << ES_min_dollar * sqrt(390) << "$," << ES_min_return * sqrt(390) << "%" << endl << endl;
}