// project.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include<list>
#include <iostream>
#include <fstream>
#include <string>
#include "stock.h"
#include "GetData.h"
#include"helper.h"
#include"classify.h"
#include "ExcelDriver\VectorsAndMatrices\Vector.hpp"
#include "ExcelDriver\ExcelDriver.hpp"
#include "ExcelDriver\ExcelMechanisms.hpp"
#include "ExcelDriver\VectorsAndMatrices\Matrix.hpp"


using namespace std;

void display(vector<stock> data) {
	vector<stock>::iterator itr = data.begin();
	for (; itr != data.end(); itr++) {
		cout << itr->name << "  " <<tmtost(itr->reportdate,"%Y-%m-%dT16:00:00")<<"  "<< itr->earning << "  " << itr->group << endl;

	}
}
void displayAAR(vector<double> data) {
	vector<double>::iterator itr = data.begin();
	for (; itr != data.end(); itr++) {
		cout <<*itr<< endl;

	}
}

Vector<double, long> TransToExcelVec(vector<double>& vec) {
	Vector<double, long> exvec = Vector<double, long>(vec.size(), 0);
	for (size_t i = 0; i < vec.size(); i++) {
		exvec[i] = vec[i];
	}
	return exvec;
}


int main()
{//first get data from csv
	ifstream fin;
	fin.open("SP500data.csv");
	if (!fin.is_open()) {
		cerr << "Load SP500data.csv Failed" << endl;
		system("pause");
		return 1;
	}
	vector<stock> stockList;
	string lineStr;
	getline(fin, lineStr);
	while (getline(fin,lineStr)){
		istringstream ss(lineStr);
		string field = "";
		getline(ss, field, ' ');
		string name = field;
		getline(ss, field, ',');
		getline(ss, field, ',');
		string reportdate = field; 
		getline(ss, field, ',');
		double earning = stod(field); 
		getline(ss, field, ',');
		double est_earning = stod(field);
		stock temp(name, reportdate,earning,est_earning);
		stockList.push_back(temp);
	}
	//display(stockList);
	//getdata from yahoo and store in stocklist
	stock Spy = Getspy();
	//cout << Spy.date.size() << endl;
	Spy.setR();
	GetData(stockList, Spy);
	//initialize stock return
	vector<stock>::iterator itr = stockList.begin();
	//cout << stockList.size() << endl;
	for (; itr != stockList.end(); itr++) {
		itr->setR();
		itr->calAR(Spy);
	}
	//classify to three groups
	classify beat(stockList, 1);
	cout <<"size of beat stocks: "<< beat.stockvec.size() << endl;
	beat.calAAR(30);
	beat.calCAAR();
	classify meet(stockList, 0);
	cout <<"size of meet stocks: "<< meet.stockvec.size() << endl;
	meet.calAAR(30);
	meet.calCAAR();
	classify miss(stockList, -1);
	cout << "size of miss stocks: "<<miss.stockvec.size() << endl;
	miss.calAAR(30);
	miss.calCAAR();

	//put result together in a tensor "result"
	vector<vector<vector<double>>> result;
	result.resize(3);
	for (int i = 0; i < 3; i++) {//three groups in row
		result[i].resize(2); //AAR and CAAR in column
	}
	result[0][0] = beat.AAR;
	result[0][1] = beat.CAAR;
	result[1][0] = meet.AAR;
	result[1][1] = meet.CAAR;
	result[2][0] = miss.AAR;
	result[2][1] = miss.CAAR;

	//output in excel
	typedef Vector<double, long> ExcelVector;
	NumericMatrix<double, long> caar_matrix(beat.CAAR.size(), 6, 0, 0);//result matrix shown in sheet2
	list<string> row_lables;
	list<string> column_lables = { "Beat AAR","Beat CAAR", "Meet AAR", "Meet CAAR", "Miss AAR", "Miss CAAR" };


	for (size_t i = 0; i < beat.CAAR.size(); i++) {
		long day_num = i - 59;
		row_lables.push_back(to_string(day_num));

		caar_matrix(i, 0) = beat.AAR[i];
		caar_matrix(i, 1) = beat.CAAR[i];

		caar_matrix(i, 2) = meet.AAR[i];
		caar_matrix(i, 3) = meet.CAAR[i];

		caar_matrix(i, 4) =miss.AAR[i];
		caar_matrix(i, 5) = miss.CAAR[i];

	}
	ExcelDriver& driver = ExcelDriver::Instance();
	driver.AddMatrix("results", caar_matrix, row_lables, column_lables);

	//Plot in Excel
	list<ExcelVector> y;
	y.push_back(TransToExcelVec(beat.CAAR));
	y.push_back(TransToExcelVec(meet.CAAR));
	y.push_back(TransToExcelVec(miss.CAAR));

	list<string> group;
	group.push_back("Beat");
	group.push_back("Meet");
	group.push_back("Miss");

	ExcelVector x(beat.CAAR.size(), 0);
	for (int i = 0; i < 120; i++) {
		x[i] = i -59;
	}

	printInExcel(x, group, y, "CAAR", "Days to report day", "Cumulated Average Abnomal Return");


	//Destroy Excel COM object
	//ExcelDriver::DestroyInstance();
	system("pause");
	return 0;
}

