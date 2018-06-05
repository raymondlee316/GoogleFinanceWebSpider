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


using namespace std;
using std::locale;
int write_data(void *ptr, int size, int nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

struct MemoryStruct {
	char *memory;
	size_t size;
	size_t Write(void *ptr, size_t write_size) {
		memory = (char *)realloc(memory, size + write_size + 1);
		if (memory) {
			memcpy(&(memory[size]), ptr, write_size);
			size += write_size;
			memory[size] = '\0';
			return write_size;
		}
		else {
			return 0;
		}
	}
};

void *myrealloc(void *ptr, size_t size)
{
	/* There might be a realloc() out there that doesn't like reallocing
	NULL pointers, so we take care of it here */
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}


int write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;

	//mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
	//if (mem->memory) {
	//	memcpy(&(mem->memory[mem->size]), ptr, realsize);
	//	mem->size += realsize;
	//	mem->memory[mem->size] = '\0';
	//}
	//return realsize;
	return mem->Write(ptr, realsize);
}

string getTimeinSeconds(string Time)
{
	std::tm t = { 0 };
	std::istringstream ssTime(Time);
	char time[100];
	memset(time, 0, 100);
	if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
	{
		//cout << std::put_time(&t, "%c %Z") << "\n"
		//	<< std::mktime(&t) << "\n";
		sprintf_s(time, "%lld", mktime(&t));
		return string(time);
	}
	else
	{
		cout << "Parse failed\n";
		return "";
	}
}



time_t convert_string_to_time_t(const std::string & time_string)
{
	struct tm tm1;
	time_t time1;
	int i = sscanf(time_string.c_str(), "%d/%d/%d %d:%d:%d",
		&(tm1.tm_year),
		&(tm1.tm_mon),
		&(tm1.tm_mday),
		&(tm1.tm_hour),
		&(tm1.tm_min),
		&(tm1.tm_sec),
		&(tm1.tm_wday),
		&(tm1.tm_yday));

	tm1.tm_year -= 1900;
	tm1.tm_mon--;
	tm1.tm_isdst = -1;
	time1 = mktime(&tm1);

	return time1;
}

char* convert_time_t_to_string(const time_t &tick)
{
	;
	struct tm tm;
	char s[100];
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	return s;
}


//start:must be in the format of "%Y-%m-%dT%H:%M:%S"
int GetData(vector<stock>& stocklist,string start) {
	vector<stock>::iterator itr = stocklist.begin();
	
	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;

	//file pointer to create file that store the data  
	FILE *fp;

	//name of files  
	//const char outfilename[FILENAME_MAX] = "Output.txt";
	//const char resultfilename[FILENAME_MAX] = "Results.txt";

	/* declaration of an object CURL */
	CURL *handle;

	/* result of the whole process */
	CURLcode result;

	/* the first functions */
	/* set up the program environment that libcurl needs */
	curl_global_init(CURL_GLOBAL_ALL);

	/* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
	handle = curl_easy_init();

	/* if everything's all right with the easy handle... */
	if (handle)
	{
		while (true)
		{
			
			string startTime = getTimeinSeconds(start);

			
			
			string urlA = "https://www.google.com/finance/getprices?i=60&p=1000d&f=d,o,h,l,c,v&df=cpct";
			string symbol = itr->name;
			string urlB = "&ts = "+startTime;
			string url = urlA + symbol + urlB + startTime;
			const char * cURL = url.c_str();

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
			result = curl_easy_perform(handle);

			/* Check for errors */
			if (result != CURLE_OK)
			{
				/* if errors have occurred, tell us what is wrong with 'result'*/
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				//return 1;
			}
			/*
			stringstream sData;
			sData.str(data.memory);
			string line;
			getline(sData, line);
			cout << line << endl;
			for (; getline(sData, line); )
			cout << line << endl;
			*/

			stringstream sData;
			sData.str(data.memory);
			double dValue = 0;

			string line;
			//get rid of the header
			for (int i = 0; i < 7; i++) {
				getline(sData, line);
			}
			int i = 0;
			stringstream stream;
			while (getline(sData, line)) {
				itr->volumn[i] = line.substr(line.find_last_of(',') + 1);
				line.erase(line.find_last_of(','));
				itr->open[i] = line.substr(line.find_last_of(',') + 1);
				line.erase(line.find_last_of(','));
				itr->low[i] = line.substr(line.find_last_of(',') + 1);
				line.erase(line.find_last_of(','));
				itr->high[i] = line.substr(line.find_last_of(',') + 1);
				line.erase(line.find_last_of(','));
				itr->close[i] = line.substr(line.find_last_of(',') + 1);
				line.erase(line.find_last_of(','));

				string time = line.substr(1);
				if (time.length > 4)     //the first min of each day

					itr->timestamp[i] = time.substr(1);
				else {
					string result;
					stream << stol(itr->timestamp[i - 1]) + i * 60;
					stream >> result;
					itr->timestamp[i] = result;
				}

				i++;
			}

				itr++;
			
			free(data.memory);
			data.memory = NULL;
			data.size = 0;
			if (itr == stocklist.end())
				break;
		}
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}

	/* cleanup since you've used curl_easy_init */
	curl_easy_cleanup(handle);

	/* this function releases resources acquired by curl_global_init() */
	curl_global_cleanup();
	return 0;

}



void main() {
	vector<string> tickers{"GOOG","AAPL"};
	vector<stock> stockList(tickers.size());
	for (int i = 0; i < tickers.size(); i++) {
		stockList[i].name = tickers[i];
	}
	GetData(stockList, "2018 - 04 - 20T16:28 : 00");
	ExcelDriver& driver = ExcelDriver::Instance();
	typedef Vector<string, long> ExcelVector;
	NumericMatrix<double, long> matrix(stockList[0].open.size()*stockList.size(),5, 0, 0);
	list<string> row_lables;
	list<string> column_lables = { "open", "high", "low", "close", "volumn" };
	vector<stock>::iterator itr = stockList.begin();
	for (; itr != stockList.end(); itr++) {
		int n = 0;
		for (; n < stockList[0].open.size(); n++) {
			matrix(n, 0) = stod((itr->open)[n]);
			matrix(n, 1) = stod((itr->high)[n]);
			matrix(n, 2) = stod((itr->low)[n]);
			matrix(n, 3) = stod((itr->close)[n]);
			matrix(n, 4) = stod((itr->volumn)[n]);
			time_t t = convert_string_to_time_t((itr->timestamp[n]));
			char* rowlabel = convert_time_t_to_string(t);
			row_lables.push_back(rowlabel);
		}
		
		driver.AddMatrix( itr -> name, matrix, row_lables, column_lables);

	}

 }

