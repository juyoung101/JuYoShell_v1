#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include <chrono>

class Chronos
{
public:
	Chronos(void);
	~Chronos(void);
	std::vector<unsigned int> getTimes();
	void clear();
	void start();
	void stop();
	//template<class T> 
	//void meter(T);
	void meter(std::function<void(void)>);
	void meter2(void);
	std::vector<unsigned int> times;
	void print();
	double sum();
	double mu();
	double sigma();	
private:
	std::chrono::steady_clock::time_point t1;
};

std::ostream& operator<<(std::ostream&, Chronos);
