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
using std::locale;
void main() {
	vector<string> tickers{ ".DJI",".IXIC",".INX","RUT" }; // stock ticker list
	vector<stock> stockList(tickers.size());
	for (int i = 0; i < tickers.size(); i++) {
		stockList[i].name = tickers[i];
	}
	cout << "Retrieve historical price data for all stocks." << endl;
	GetData(stockList);
	double percentile = 5; // 95% percentile
	for (auto itr = stockList.begin(); itr != stockList.end(); ++itr) {
		cout << endl << "Ticker:" << itr->name << endl;
		VaR_historical(itr->close, 5);
	}
	//Output to Excel (Note this will make the program run slower)
	ExcelDriver& driver = ExcelDriver::Instance();
	driver.MakeVisible(true);
	typedef Vector<string, long> ExcelVector;
	NumericMatrix<double, long> matrix(stockList[0].open.size(), 5, 0, 0);
	list<string> row_labels;
	list<string> column_labels = { "open", "high", "low", "close", "volumn" };
	vector<stock>::iterator itr = stockList.begin();
	for (; itr != stockList.end(); itr++) {
		int n = 0;
		for (; n < stockList[0].open.size(); n++) {
			matrix(n, 0) = (itr->open[n]);
			matrix(n, 1) = (itr->high[n]);
			matrix(n, 2) = (itr->low[n]);
			matrix(n, 3) = (itr->close[n]);
			matrix(n, 4) = (itr->volumn[n]);
			time_t t = (time_t)(stol(itr->timestamp[n]));
			string rowlabel;
			convert_time_t_to_string(rowlabel,t);
			row_labels.push_back(string(rowlabel));
		}
		
			driver.AddMatrix(itr->name, matrix, row_labels, column_labels);
	}

	system("pause");
}