#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

void init_logging(const std::string &log_file)
{
    logging::register_simple_formatter_factory<logging::trivial::severity_level,
    char>("Severity");

    logging::add_file_log(
        keywords::file_name = log_file,
        keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%]
        %Message%");

    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info);

    logging::add_common_attributes();
}

#endif