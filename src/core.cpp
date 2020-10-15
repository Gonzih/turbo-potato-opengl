#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <ncurses.h>

#include "spdlog/spdlog.h"

#include "random.hpp"
#include "game.hpp"
#include "logging.hpp"


using namespace std;

void curses_init() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
}

void sigint_handler_init() {
   struct sigaction cleanup;

   cleanup.sa_handler = [](int sig) {
       if (endwin() == ERR) {
           logger->info("Error releasing ncurse allocations");
           exit(1);
       }
       if (sig == SIGINT) {
           logger->info("Exited normally");
           exit(0);
       } else {
           logger->critical("Error: got signal {} {}", sig, strerror(sig));
           std::cout << strerror(sig) << endl;
           exit(sig);
       }
   };
   sigemptyset(&cleanup.sa_mask);
   cleanup.sa_flags = 0;

   sigaction(SIGINT, &cleanup, NULL);
   sigaction(SIGSEGV, &cleanup, NULL);
}

int main() {
    logging_init(spdlog::level::info);
    curses_init();
    sigint_handler_init();
    rand_init();

    int nh, nw;
    getmaxyx(stdscr, nh, nw);
    logger->info("Starting game with width={} height={}", nw, nh);
    auto game = Game(nw, nh);

    game.loop();

    return 0;
}
