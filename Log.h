#pragma once

/*
	로그에 대해 화면과 파일저장 
	파일당 용량은 최대 10MB -> 넘어가면 파일명에 %N% 증가
	log 폴더에 최대 용량은 100MB -> 넘어가면 LRU로 삭제	
*/


#include <boost/log/trivial.hpp>


enum ELoggerSinker
{
	CONSOLE_SINK = 1,
	VS_DEBUG_SINK,
	TEXT_FILE_SINK
};


class LogSetting
{
public:
	LogSetting() 
		: max_storage_size_(1024 * 1024 * 100)	// 100MB
		, max_file_size_(1024 * 1024 * 10)		// 10MB
		, file_ordering_window_sec_(1)
		, is_print_screen_(true)
		, is_save_text_file_(true)
	{
	}

	// 해당 폴더 최대 저장 크기
	int max_storage_size_;

	// 로그 파일별 최대 크기
	int max_file_size_;

	// 화면 출력 여부
	bool is_print_screen_;

	// 파일 저장 여부
	bool is_save_text_file_;	

	// 멀티스레드 기반 파일 저장시 순서 보정을 위한 윈도우 크기(초단위)
	int file_ordering_window_sec_;
};


class CLog
{
public:	
	CLog(LogSetting& setting);
	~CLog(void);
		

private:
	void global_attribute();

	void add_console_sink();
	void add_vs_debug_output_sink();
	
	void add_text_file_sink();

	// 순서 미보장 CPU 사용률 낮음
	void add_text_file_sink_unorder();

private:
	LogSetting setting_;
};

