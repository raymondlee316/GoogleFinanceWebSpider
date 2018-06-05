#pragma once
#include"stdafx.h"
#include<vector>
#include<string>

using namespace std;


class stock {
public:
	vector<string> timestamp;
	string name;
	vector<double> open;
	vector<double> close;
	vector<double> high;
	vector<double> low;
	vector<double> volumn;
	vector<double> lastDayPrice;
	double VaR_min; // minutely VaR
	double ES; // Expected shortfall
public:
	double VaR(double percentile);
	//// calculate lowest price
	//double Low(vector<double> price);
	//double High(vector<double> price);
	//double std(vector<double> price);
	//double skew(vector<double> price);
	//double kurtosis(vector<double> price);
	//double sharpe(vector<double> price);
	//double informratio(vector<double> price);
	//double Jensen(vector<double> price);
	//double VaR(vector<double> price);
	//double downsideRisk(vector<double> price);
	//double riskPremium(vector<double> price);
	//// calculate return
	//vector<double> calReturn(vector<double> price);


};

