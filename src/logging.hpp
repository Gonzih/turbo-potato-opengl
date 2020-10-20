#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <mutex>

namespace log {
    namespace {
        class Writer {
            private:
                std::string fname;
                std::ofstream fstream;
                std::mutex write_mutex;

            public:
                Writer(std::string fname) : fname(fname) {
                    fstream.open(fname, std::ios::out | std::ios::app);
                };

                template<typename First>
                void print(First first){
                    std::lock_guard<std::mutex> guard(write_mutex);
                    fstream << first;
                }

                template<typename First, typename... Rest>
                void print(First first, Rest... rest){
                    print(first);
                    print(' ');
                    print(rest...);
                }

                void flush() {
                    std::lock_guard<std::mutex> guard(write_mutex);
                    fstream.flush();
                }

                ~Writer() {
                    fstream.close();
                }
        };

        std::shared_ptr<Writer> logger;
    }


    void init(std::string fname) {
        logger = std::make_shared<Writer>(fname);
    }

    template<typename... Rest>
    void info(Rest... rest){
        logger->print("<INFO> ");
        logger->print(rest...);
        logger->print('\n');
        logger->flush();
    }

    template<typename... Rest>
    void critical(Rest... rest){
        logger->print("<OMGPANIC> ");
        logger->print(rest...);
        logger->print('\n');
        logger->flush();
    }
}
