#pragma once

/*
	�α׿� ���� ȭ��� �������� 
	���ϴ� �뷮�� �ִ� 10MB -> �Ѿ�� ���ϸ� %N% ����
	log ������ �ִ� �뷮�� 100MB -> �Ѿ�� LRU�� ����	
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

	// �ش� ���� �ִ� ���� ũ��
	int max_storage_size_;

	// �α� ���Ϻ� �ִ� ũ��
	int max_file_size_;

	// ȭ�� ��� ����
	bool is_print_screen_;

	// ���� ���� ����
	bool is_save_text_file_;	

	// ��Ƽ������ ��� ���� ����� ���� ������ ���� ������ ũ��(�ʴ���)
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

	// ���� �̺��� CPU ���� ����
	void add_text_file_sink_unorder();

private:
	LogSetting setting_;
};

