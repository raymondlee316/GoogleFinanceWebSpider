#pragma once
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

using namespace std;
using std::locale;

void VaR_historical(vector<double> close, double percentile);

int GetData(vector<stock>& stocklist);

time_t convert_string_to_time_t(const std::string & time_string);

static int convert_time_t_to_string(std::string &strDateStr, const time_t &timeData)
{
	char chTmp[40];
	memset(chTmp, 0, sizeof(chTmp));

	struct tm *p;
	p = localtime(&timeData);
	p->tm_year = p->tm_year + 1900;
	p->tm_mon = p->tm_mon + 1;
	snprintf(chTmp, sizeof(chTmp), "%04d-%02d-%02d-%02d:%02d:%02d",
		p->tm_year, p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

	strDateStr = chTmp;
	return 0;
};
