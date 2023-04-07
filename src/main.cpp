// COMPILE AND RUN: clang++ main.cpp -o main; ./main
// compile to target: clang++ main.cpp -o ../TradixApp/main

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <fstream>

#include "color.cpp"

using namespace std;

// =========================== GLOBAL VARIABLE ===========================

struct termios ORIG_TERMIOS, NEW_TERMIOS;
int ROWS, COLS;
string USER_CMD = "";

// =========================== GLOBAL VARIABLE ===========================

#define LINESPACE cout << endl;

// ==================== STOCK MARKET GLOBAL VARIABLES ====================

string TOKEN = "cgmv0upr01qhveustqb0cgmv0upr01qhveustqbg";

// available data points
string MARKET_STATUS = "STOCK MARKETS CLOSED";
string TICKER = "SPY";

string PREV_OPEN = "0.00";
string PREV_HIGH = "0.00";
string PREV_LOW = "0.00";
string PREV_CLOSE = "0.00";

string TODAY_OPEN = "0.00";
string TODAY_HIGH = "0.00";
string TODAY_LOW = "0.00";
string TODAY_CLOSE = "0.00";
string CURR_PRICE = "0.00";
string VOLUME = "0.00";
string MKT_CAP = "0.00";
string PE_RATIO = "0.00";
string DIV_YIELD = "0.00%";

// list of available tickers
string TICKERS_AVAIL[] = {
    "AAPL", "TSLA", "SPY", "MSFT", "AMZN", "META", "GME", "BB", "AMD", 
    "NVDA", "GOOGL", "NFLX", "UBER", "PLTR"
};

void get_data_stock() {
    // write ticker to request.json
    ofstream request_json;
    request_json.open("request.json");
    request_json << "{\n\t\"ticker\": \"" + TICKER + "\"\n}";
    request_json.close();
}

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

    return date + " " + time;
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
        if (USER_CMD == "exit\n" || USER_CMD == "quit\n") {
            reset_termios();
            exit(0);
        }

        for (int i = 0; i < USER_CMD.length(); i++) {
            USER_CMD[i] = toupper(USER_CMD[i]);
        }
        for (int i = 0; i < sizeof(TICKERS_AVAIL) / sizeof(TICKERS_AVAIL[0]); i++) {
            if (USER_CMD == TICKERS_AVAIL[i] + "\n") {
                TICKER = TICKERS_AVAIL[i];
            }
        }
        USER_CMD = "";
    }
}

int main() {
    init_termios();

    // set input to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    int space;
    string space_str;

    int adjustspace = 9;

    while (true) {
        system("clear");
        get_terminal_size();

        get_data_stock();
        
        // check for keyboard input
        check_input();

        // print Tradix Finance Terminal
        cout << COLOR_BLACK_PURPLE("Tradix Finance Terminal");

        // display market status
        space = (COLS / 2) - (MARKET_STATUS.length() / 2) - 23;
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        if (MARKET_STATUS == "STOCK MARKETS CLOSED") {
            cout << space_str << COLOR_BLACK_RED(MARKET_STATUS);
        } else {
            cout << space_str << COLOR_BLACK_GREEN(MARKET_STATUS);
        }

        // display time and date
        space = (COLS / 2) - ((MARKET_STATUS.length() + 2) / 2) - get_time().length() - 5;
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_GREEN_BLUE(get_time());

        cout << endl;
        LINESPACE;

        // Display Ticker
        cout << "    " << COLOR_BLACK_YELLOW("TICKER:") << " " << COLOR_GREEN_BLUE(TICKER);
        // display previous open
        space = adjustspace - TICKER.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_BLACK_WHITE("PREV OPEN:") << " " << COLOR_BLACK_WHITE(PREV_OPEN);
        // display previous high
        space = adjustspace - PREV_OPEN.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_BLACK_WHITE("PREV HIGH:") << " " << COLOR_BLACK_WHITE(PREV_HIGH);
        // display previous low
        space = adjustspace - PREV_HIGH.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_BLACK_WHITE("PREV LOW:") << " " << COLOR_BLACK_WHITE(PREV_LOW);
        // display previous close
        space = adjustspace - PREV_LOW.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_BLACK_WHITE("PREV CLOSE:") << " " << COLOR_BLACK_WHITE(PREV_CLOSE);

        cout << endl;
        LINESPACE;

        // display current price
        cout << "    " << COLOR_BLACK_YELLOW("PRICE: ") << " " << COLOR_BLACK_WHITE(CURR_PRICE);
        // display current open
        space = adjustspace - CURR_PRICE.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_WHITE_BLACK("CURR OPEN:") << " " << COLOR_BLACK_WHITE(TODAY_OPEN);
        // display current high
        space = adjustspace - CURR_PRICE.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_WHITE_BLACK("CURR HIGH:") << " " << COLOR_BLACK_WHITE(TODAY_HIGH);
        // display current low
        space = adjustspace - TODAY_HIGH.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_WHITE_BLACK("CURR LOW:") << " " << COLOR_BLACK_WHITE(TODAY_LOW);
        // display current close
        space = adjustspace - TODAY_LOW.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_WHITE_BLACK("CURR CLOSE:") << " " << COLOR_BLACK_WHITE(TODAY_CLOSE);

        cout << endl;
        LINESPACE;

        // display mktd cap
        cout << "    " << COLOR_BLACK_YELLOW("MT CAP:") << " " << COLOR_BLACK_WHITE(MKT_CAP);

        // display volume
        space = adjustspace - MKT_CAP.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_WHITE_BLACK(" VOLUME:  ") << " " << COLOR_BLACK_WHITE(VOLUME);

        // display pe ratio
        space = adjustspace - VOLUME.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_BLACK_PURPLE("PE RATIO: ") << " " << COLOR_BLACK_WHITE(PE_RATIO);

        // display div yield
        space = adjustspace - PE_RATIO.length();
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_BLACK_PURPLE("DIV YLD: ") << " " << COLOR_BLACK_WHITE(DIV_YIELD);

        cout << endl;
        LINESPACE;

        // sleep for 50ms
        usleep(50000);
        flush(cout);
    }

    reset_termios();

    return 0;
}
