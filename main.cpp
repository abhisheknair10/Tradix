#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "color.cpp"

// =========================== GLOBAL VARIABLE ===========================

struct termios ORIG_TERMIOS, NEW_TERMIOS;
int ROWS, COLS;
string USER_CMD = "";

// =========================== GLOBAL VARIABLE ===========================

#define LINESPACE cout << endl;

using namespace std;

// ==================== STOCK MARKET GLOBAL VARIABLES ====================

string TICKER = " SPY ";

// list of available tickers
string TICKERS_AVAIL[] = {
    "AAPL",
    "TSLA",
    "SPY"
};

// ==================== STOCK MARKET GLOBAL VARIABLES ====================

// save the original terminal settings
void init_termios() {
    tcgetattr(STDIN_FILENO, &ORIG_TERMIOS);

    // Disable canonical mode and echo
    NEW_TERMIOS = ORIG_TERMIOS;
    NEW_TERMIOS.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &NEW_TERMIOS);
}

// restore the original terminal settings
void reset_termios() {
    tcsetattr(STDIN_FILENO, TCSANOW, &ORIG_TERMIOS);
}

// get date and time using time.h
string get_time() {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    string time = to_string(1 + ltm->tm_hour) + ":";
    if (ltm->tm_min < 10) {
        time += "0";
    }
    time += to_string(ltm->tm_min) + ":";
    if (ltm->tm_sec < 10) {
        time += "0";
    }
    time += to_string(ltm->tm_sec);

    string date = to_string(1 + ltm->tm_mon) + "/";
    if (ltm->tm_mday < 10) {
        date += "0";
    }
    date += to_string(ltm->tm_mday) + "/";
    date += to_string(1900 + ltm->tm_year);

    return " " + date + " " + time + " ";
}

// get terminal size
void get_terminal_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    ROWS = w.ws_row;
    COLS = w.ws_col;
}

// function to check for keyboard input
void check_input() {
    char input;
    if (read(STDIN_FILENO, &input, 1) > 0) {
        USER_CMD += input;
    }

    // if the backspace is pressed then delete the last character
    if (input == 127) {
        if (USER_CMD.length() > 0) {
            USER_CMD.pop_back();
            USER_CMD.pop_back();
        }
    }

    // if enter button is pressed then print the command
    if (input == '\n') {
        if (USER_CMD == "exit\n") {
            reset_termios();
            exit(0);
        }

        for (int i = 0; i < USER_CMD.length(); i++) {
            USER_CMD[i] = toupper(USER_CMD[i]);
        }
        for (int i = 0; i < sizeof(TICKERS_AVAIL) / sizeof(TICKERS_AVAIL[0]); i++) {
            if (USER_CMD == TICKERS_AVAIL[i] + "\n") {
                TICKER = " " + TICKERS_AVAIL[i] + " ";
            }
        }

        cout << "Command: " << USER_CMD << endl;
        USER_CMD = "";
    }
}

int main() {
    init_termios();

    // set input to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    while (true) {
        system("clear");
        get_terminal_size();
        
        // check for keyboard input
        check_input();

        // print Tradix Finance Terminal
        cout << COLOR_BLACK_PURPLE(" Tradix Finance Terminal ");

        // print date and time
        int space = COLS - 25 - get_time().length();
        string space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_BLACK_YELLOW(get_time()) << endl;

        LINESPACE;
        
        // print the ticker of the stock
        cout << COLOR_BLACK_GREEN(" Ticker: ") << " " << COLOR_BLACK_CYAN(TICKER) << endl;

        LINESPACE;

        // print the price of the stock
        string price = " 420.69 ";
        cout << COLOR_BLACK_GREEN(" Price: ") << " " << COLOR_BLACK_CYAN(price) << endl;

        // delay of 50ms
        usleep(50000);
        flush(cout);
    }

    reset_termios();

    return 0;
}
