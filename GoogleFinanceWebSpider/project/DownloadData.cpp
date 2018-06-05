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

	mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = '\0';
	}
	return realsize;
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
	int i = sscanf_s(time_string.c_str(), "%d/%d/%d %d:%d:%d",
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




//start:must be in the format of "%Y-%m-%dT%H:%M:%S"
int GetData(vector<stock>& stocklist) {
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
			string urlA = "https://www.google.com/finance/getprices?i=";
			string urlB = "60"; // get 1 min Data: 60;  get 5 min Data: 300; get 15 min Data: 900 etc.
			string urlC = "&p=";
			string urlD = "15d"; // how many days
			string urlE = "&f=d,o,h,l,c,v&df=cpct&q=";
			string symbol = itr->name;
			string url = urlA + urlB + urlC + urlD + urlE + symbol;
			const char * cURL = url.c_str();
			curl_easy_setopt(handle, CURLOPT_URL, cURL);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
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
				if (line[0] == 'T')
					continue;
				itr->volumn.push_back(stod(line.substr(line.find_last_of(',') + 1)));
				line.erase(line.find_last_of(','));
				itr->open.push_back(stod(line.substr(line.find_last_of(',') + 1)));
				line.erase(line.find_last_of(','));
				itr->low.push_back(stod(line.substr(line.find_last_of(',') + 1)));
				line.erase(line.find_last_of(','));
				itr->high.push_back(stod(line.substr(line.find_last_of(',') + 1)));
				line.erase(line.find_last_of(','));
				itr->close.push_back(stod(line.substr(line.find_last_of(',') + 1)));
				line.erase(line.find_last_of(','));
				string time;
				if (line[0] == 'a')
					time = line.substr(1);
				else
					time = line;
				if (time.length() > unsigned int(4))     //the first min of each day

					itr->timestamp.push_back(time);
				else {
					string result;
					string k= (itr->timestamp)[i - 1];
					stream << stol(k) +  60;
					stream >> result;
					stream.clear();
					itr->timestamp.push_back(result);
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






