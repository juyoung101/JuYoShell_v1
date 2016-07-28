#include <vector>
#include <cmath>
#include <chrono>
#include <iostream>
#include <iomanip>
#include "Chronos.hpp"

Chronos::Chronos(void)
{
	this->times.clear();
}

Chronos::~Chronos(void)
{
}

void Chronos::start()
{
	this->t1 = std::chrono::steady_clock::now();
}

void Chronos::stop()
{
	auto t2 = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - this->t1);
	this->times.push_back(diff.count());
}

std::vector<unsigned int> Chronos::getTimes()
{
	return this->times;
}

void Chronos::clear(){
	this->times.clear();
	this->times.resize(0, 0);
}

void Chronos::meter(std::function<void(void)> f)
{
	auto start = std::chrono::steady_clock::now();
	f();
	auto end = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start); //delta times
	this->times.push_back(diff.count());
}

void Chronos::meter2(void)
{
	auto start = std::chrono::steady_clock::now();
	for(int i = 0; i < 1000; ++i){printf("%d ", i);}; //sprinkles
	auto end = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start); //delta times
	times.push_back(diff.count());
}

double Chronos::sum(void)
{
	auto sum = 0.0;

	auto size = this->times.size();
	for(int i = 0; i < size; ++i) //simple summation
	{
		sum += this->times[i];
	}

	return sum;
}
 
double Chronos::mu(void)
{
	auto mu = sum() / this->times.size(); //get sum, over n
	return mu;
}

double Chronos::sigma(void)
{
	auto stddev = 0.0;
	double avg = mu();

	std::vector<double> vars;
	auto size = this->times.size();
	for(int i = 0; i < size; ++i)
	{
		vars.push_back( avg - this->times[i] ); //difference
		vars[i] = vars[i] * vars[i]; //square
	}

	double var = 0.0;
	for(int i = 0; i < size; ++i)
	{
		var += vars[i];	//sum of variances
	}

	var /= size; //sum over n
	stddev = sqrt(var); //sqrt variance

	return stddev;
}

std::ostream& operator<<(std::ostream& os, Chronos c)
{
	os << std::setprecision(4);

	os << std::endl << "\tTimes: " << std::endl;
	auto v = c.getTimes();
	os << "\t\t";
	for(unsigned int i = 1; i < v.size(); ++i){
		os << "Run #" << i << ":  " << std::fixed << v[i]/1000000000.0 << " seconds";
		if(i % 2 == 0) os << std::endl << "\t\t";
		else os << "  ";
	}

	os << std::endl;
	os << "\tSum: " << c.sum()/1000000000.0 <<" seconds" << std::endl;
	os << "\tAvg: " << c.mu()/1000000000.0 <<" seconds" << std::endl;
	os << "\tStd: " << c.sigma()/1000000000.0 << " seconds" << std::endl;

	return os;
}
