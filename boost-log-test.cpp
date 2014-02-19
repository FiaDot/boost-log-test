// boost-log-test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <thread>
#include <chrono>

#include "Log.h"

const int MAX_COUNT = 1000000;


int _tmain(int argc, _TCHAR* argv[])
{
	LogSetting setting;
	CLog log(setting);
	
	

	std::thread  Thread1([] () 
	{
		for(int n=0;n<MAX_COUNT;n++)
		{
			BOOST_LOG_TRIVIAL(fatal) << "thread 1";		
//			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});

	std::thread  Thread2([] () 
	{
		for(int n=0;n<MAX_COUNT;n++)
		{
			BOOST_LOG_TRIVIAL(error) << "thread 2";
//			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});

	Thread1.join();
	Thread2.join();

	return 0;
}

