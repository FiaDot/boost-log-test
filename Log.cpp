#include "stdafx.h"
#include "Log.h"

#include <boost/log/common.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/record_ordering.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

// BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(test_lg, src::logger_mt)

CLog::CLog(LogSetting& setting) 
	: setting_(setting)
{
	global_attribute();

	add_console_sink();
//	add_vs_debug_output_sink();
	add_text_file_sink();
}


CLog::~CLog(void)
{		
	logging::core::get()->flush();
	logging::core::get()->remove_all_sinks();
}


void CLog::global_attribute()
{
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());	
	logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());
}

void CLog::add_console_sink()
{
	boost::shared_ptr<sinks::text_ostream_backend> backend = boost::make_shared<sinks::text_ostream_backend>();
	backend->add_stream(boost::shared_ptr<std::ostream>(&std::clog, logging::empty_deleter()));
	backend->auto_flush(true);

	typedef sinks::synchronous_sink<sinks::text_ostream_backend> sink_t;
	boost::shared_ptr<sink_t> sink(new sink_t(backend));

	sink->set_formatter(
		expr::format("[%1%] [%2%] [%3%] %4%")
		% expr::attr< unsigned int >("RecordID")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%d %H:%M:%S.%f")
		% logging::trivial::severity
		% expr::message
		);

	logging::core::get()->add_sink(sink);
}

void CLog::add_vs_debug_output_sink()
{
	boost::shared_ptr<sinks::debug_output_backend> backend = boost::make_shared<sinks::debug_output_backend>();

	typedef sinks::synchronous_sink<sinks::debug_output_backend> sink_t;
	boost::shared_ptr<sink_t> sink(new sink_t(backend));

	sink->set_formatter(
		expr::format("[%1%] [%2%] [%3%] %4%\n")
		% expr::attr< unsigned int >("RecordID")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
		% logging::trivial::severity
		% expr::message
		);

	logging::core::get()->add_sink(sink);
}

void CLog::add_text_file_sink()
{
	boost::shared_ptr< sinks::text_file_backend > file_backend = boost::make_shared< sinks::text_file_backend >(
		keywords::file_name = "log_%Y-%m-%d_%H-%M-%S_%N.txt",
		keywords::rotation_size = setting_.max_file_size_,

		// 시점 (일 변경시)
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),

		// 주기 (1분마다)
		// keywords::time_based_rotation = sinks::file::rotation_at_time_interval(boost::posix_time::minutes(1)),

		keywords::format = "[%TimeStamp%] (%Severity%) : %Message%",
		keywords::min_free_space= setting_.max_storage_size_ + setting_.max_file_size_
		);

	file_backend->auto_flush(true);

	typedef sinks::asynchronous_sink<
		sinks::text_file_backend,
		sinks::unbounded_ordering_queue<                                 
		logging::attribute_value_ordering<                               
		unsigned int,                                                    
		std::less< unsigned int >                                        
		>
		>
	> sink_t;

	boost::shared_ptr< sink_t > sink(new sink_t(
		file_backend,
		keywords::order = logging::make_attr_ordering("RecordID", std::less< unsigned int >()), 
		keywords::ordering_window = boost::posix_time::seconds(setting_.file_ordering_window_sec_) 		
		));


	
	sink->set_formatter(
		expr::format("[%1%] [%2%] %3% %4%")
		% expr::attr< unsigned int >("RecordID")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
		% logging::trivial::severity
		% expr::message
		);

	sink->locked_backend()->set_file_collector(
		sinks::file::make_collector(
		keywords::target = "logs",
		keywords::max_size = setting_.max_storage_size_
		)
		);

	logging::core::get()->add_sink(sink);
}



void CLog::add_text_file_sink_unorder()
{
	boost::shared_ptr< sinks::text_file_backend > file_backend = boost::make_shared< sinks::text_file_backend >(
		keywords::file_name = "log_%Y-%m-%d_%H-%M-%S_%N.txt",
		keywords::rotation_size = setting_.max_file_size_,

		// 시점 (일 변경시)
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),

		// 주기 (1분마다)
		// keywords::time_based_rotation = sinks::file::rotation_at_time_interval(boost::posix_time::minutes(1)),

		keywords::format = "[%TimeStamp%] (%Severity%) : %Message%",
		keywords::min_free_space= setting_.max_storage_size_ + setting_.max_file_size_
		);

	file_backend->auto_flush(true);

	typedef sinks::asynchronous_sink< sinks::text_file_backend > sink_t;
	boost::shared_ptr< sink_t > sink(new sink_t(file_backend));

	sink->set_formatter(
		expr::format("[%1%] [%2%] %3% %4%")
		% expr::attr< unsigned int >("RecordID")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
		% logging::trivial::severity
		% expr::message
		);

	sink->locked_backend()->set_file_collector(
		sinks::file::make_collector(
		keywords::target = "logs",
		keywords::max_size = setting_.max_storage_size_
		)
		);

	logging::core::get()->add_sink(sink);

}