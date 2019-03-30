#include "pch.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <algorithm>

class StopWatch
{
private:
	const std::chrono::steady_clock::time_point mStart;
	const std::string mName;
	bool mExpired;
	long long mDuration;

public:
	StopWatch() = delete;
	StopWatch(const StopWatch&) = delete;
	StopWatch(const StopWatch&&) = delete;
	void operator=(const StopWatch&) = delete;
	void operator=(const StopWatch&&) = delete;

	StopWatch(const std::string& pName)
		:
		mStart(std::chrono::steady_clock::now()),
		mName(std::move(pName)),
		mExpired(false),
		mDuration(0)
	{
	}

	~StopWatch()
	{
		Expire<std::chrono::microseconds>();		
	}

	template <typename T>
	void Expire()
	{
		if (!mExpired)
		{
			auto timeTaken = std::chrono::steady_clock::now() - mStart;
			auto duration = std::chrono::duration_cast<T>(timeTaken);			
			mDuration = duration.count();
			mExpired = true;
			std::cout << mName.c_str() << " => " << mDuration << "\r\n";
		}
	}

};

void Worker1(const int pStart, const int pCount)
{
	std::ofstream resultFile("primelist.txt", std::ios_base::out | std::ios_base::trunc);
	const unsigned int max(pStart + pCount);
	unsigned long local(0);
	for (unsigned int i(pStart); i < max; ++i)
	{		
		if (i > 1)
		{			
			bool isPrime(true);			
			local = i - 1;
			while (local > 1)
			{
				if ( (i % local) == 0 )
				{
					isPrime = false;
					break;
				}
				--local;
			}
			if (isPrime)
			{
				std::cout << i << " is a Prime Number" << std::endl;
				if (resultFile && resultFile.is_open())
				{
					resultFile << i << std::endl;
				}
			}
		}
	}
	if (resultFile && resultFile.is_open())
	{
		resultFile.close();
	}
}

void Usage();

int main(int argc, char** argv)
{
	bool inValidParams(true);
	if (argc > 1)
	{
		int ourStart = atoi(argv[1]);
		if (argc > 2)
		{			
			int ourCount = atoi(argv[2]);
			inValidParams = false;
			StopWatch sw1("First Prime List");
			Worker1(ourStart, ourCount);
			sw1.Expire<std::chrono::milliseconds>();
		}
	}

	if (inValidParams)
	{
		Usage();
	}
}


void Usage()
{
	std::cout << "Usage:   CPPT  <Start>  <Count>\r\n";
	std::cout << "         Where Start is an integer\r\n";
	std::cout << "         and Count is also an integer\r\n";
	std::cout << "\r\n";
}