#ifndef TP_LOGGING_H
#define TP_LOGGING_H
#include <iostream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

std::shared_ptr<spdlog::logger> logger;

void logging_init(spdlog::level::level_enum log_level) {
    logger = spdlog::basic_logger_mt("basic_logger", "turbo-potato.log");
    logger->set_level(log_level);
    logger->flush_on(log_level);
}

#endif
