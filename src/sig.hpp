#pragma once

#include <signal.h>
#include <string.h>
#include "logging.hpp"

void sigint_handler_init() {
   struct sigaction cleanup;

   cleanup.sa_handler = [](int sig) {
       if (endwin() == ERR) {
           logger::info("Error releasing ncurse allocations");
           exit(1);
       }
       if (sig == SIGINT) {
           logger::info("Exited normally");
           exit(0);
       } else {
           logger::critical("Error: got signal:", sig, strerror(sig));
           std::cout << strerror(sig) << std::endl;
           exit(sig);
       }
   };
   sigemptyset(&cleanup.sa_mask);
   cleanup.sa_flags = 0;

   sigaction(SIGINT, &cleanup, NULL);
   sigaction(SIGSEGV, &cleanup, NULL);
}
