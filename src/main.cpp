// COMPILE AND RUN: clang++ main.cpp -o main; ./main
// compile to target: clang++ main.cpp -lncurses -o ../TradixApp/main

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
#include <curses.h>

using namespace std;

// =========================== GLOBAL VARIABLE ===========================

struct termios ORIG_TERMIOS, NEW_TERMIOS;
int ROWS, COLS;
string USER_CMD = "";

// =========================== GLOBAL VARIABLE ===========================

#define NEWLINE addstr("\n");

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
string TDY_PRICE = "0.00";
string VOLUME = "0.00";
string MKT_CAP = "0.00";
string PE_RATIO = "0.00";
string DIV_YIELD = "0.00%";

// list of available tickers
string TICKERS_AVAIL[] = {
    "AAPL", "TSLA", "SPY", "MSFT", "AMZN", "META", "GME", "BB", "AMD", 
    "NVDA", "GOOGL", "NFLX", "UBER", "PLTR", "SQ", "PYPL", "TWTR", "SNAP",
    "DIS", "BABA", "BIDU", "NIO", "XPEV", "LI", "F", "GM", "T", "NOK",
};

string TICKERS_AVAIL_LA[] ={
    "    |  AAPL  |   Apple Inc.                      |  Technology              |  ",
    "    |  TSLA  |   Tesla Inc.                      |  Consumer Cyclical       |  ",
    "    |  SPY   |   SPDR S&P 500 ETF Trust          |  Index Fund              |  ",
    "    |  MSFT  |   Microsoft Corporation           |  Technology              |  ",
    "    |  AMZN  |   Amazon.com, Inc.                |  Consumer Cyclical       |  ",
    "    |  META  |   Roundhill Ball Metaverse ETF    |  Index Fund              |  ",
    "    |  GME   |   GameStop Corp.                  |  Consumer Cyclical       |  ",
    "    |  BB    |   BlackBerry Limited              |  Technology              |  ",
    "    |  AMD   |   Advanced Micro Devices, Inc.    |  Technology              |  ",
    "    |  NVDA  |   NVIDIA Corporation              |  Technology              |  ",
    "    |  GOOGL |   Alphabet Inc.                   |  Communication Services  |  ",
    "    |  NFLX  |   Netflix, Inc.                   |  Communication Services  |  ",
    "    |  UBER  |   Uber Technologies, Inc.         |  Technology              |  ",
    "    |  PLTR  |   Palantir Technologies Inc.      |  Technology              |  ",
    "    |  SQ    |   Square, Inc.                    |  Technology              |  ",
    "    |  PYPL  |   PayPal Holdings, Inc.           |  Technology              |  ",
    "    |  TWTR  |   Twitter, Inc.                   |  Communication Services  |  ",
    "    |  SNAP  |   Snap Inc.                       |  Communication Services  |  ",
    "    |  DIS   |   The Walt Disney Company         |  Communication Services  |  ",
    "    |  BABA  |   Alibaba Group Holding Limited   |  Consumer Cyclical       |  ",
    "    |  BIDU  |   Baidu, Inc.                     |  Communication Services  |  ",
    "    |  NIO   |   NIO Inc.                        |  Consumer Cyclical       |  ",
    "    |  XPEV  |   XPeng Inc.                      |  Consumer Cyclical       |  ",
    "    |  LI    |   Li Auto Inc.                    |  Consumer Cyclical       |  ",
    "    |  F     |   Ford Motor Company              |  Consumer Cyclical       |  ",
    "    |  GM    |   General Motors Company          |  Consumer Cyclical       |  ",
    "    |  T     |   AT&T Inc.                       |  Communication Services  |  ",
    "    |  NOK   |   Nokia Corporation               |  Technology              |  ",
};

void get_data_stock() {
    // write ticker to request.json
    ofstream request_json;
    request_json.open("request.json");
    request_json << "{\n\t\"ticker\": \"" + TICKER + "\"\n}";
    request_json.close();
}

// ==================== STOCK MARKET GLOBAL VARIABLES ====================

// ======================= COLORED PRINT FUNCTION =======================

void COLOR_BLACK_PURPLE(string text) {
    init_pair(1, COLOR_BLACK, COLOR_MAGENTA);
    attron(COLOR_PAIR(1));
    addstr(" ");
    addstr(text.c_str());
    addstr(" ");
    attroff(COLOR_PAIR(1));
}

void COLOR_BLACK_YELLOW(string text) {
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    attron(COLOR_PAIR(2));
    addstr(" ");
    addstr(text.c_str());
    addstr(" ");
    attroff(COLOR_PAIR(2));
}

void COLOR_BLACK_GREEN(string text) {
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    attron(COLOR_PAIR(3));
    addstr(" ");
    addstr(text.c_str());
    addstr(" ");
    attroff(COLOR_PAIR(3));
}

void COLOR_BLACK_RED(string text) {
    init_pair(4, COLOR_BLACK, COLOR_RED);
    attron(COLOR_PAIR(4));
    addstr(" ");
    addstr(text.c_str());
    addstr(" ");
    attroff(COLOR_PAIR(4));
}

void COLOR_BLACK_CYAN(string text) {
    init_pair(5, COLOR_BLACK, COLOR_CYAN);
    attron(COLOR_PAIR(5));
    addstr(" ");
    addstr(text.c_str());
    addstr(" ");
    attroff(COLOR_PAIR(5));
}

void COLOR_WHITE_BLACK(string text) {
    init_pair(6, 255, COLOR_BLACK);
    attron(COLOR_PAIR(6));
    addstr(" ");
    addstr(text.c_str());
    addstr(" ");
    attroff(COLOR_PAIR(6));
}

void COLOR_GREEN_BLUE(string text) {
    init_pair(7, COLOR_GREEN, COLOR_BLUE);
    attron(COLOR_PAIR(7));
    addstr(" ");
    addstr(text.c_str());
    addstr(" ");
    attroff(COLOR_PAIR(7));
}

// ======================= COLORED PRINT FUNCTION =======================

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
int check_input() {
    int input = getch(); // get keyboard input using curses library
    if (input != ERR) {
        USER_CMD += input;
    }

    // if the backspace is pressed then delete the last character
    if (input == KEY_BACKSPACE || input == 127) {
        if (USER_CMD.length() > 0) {
            USER_CMD.pop_back();
            USER_CMD.pop_back();
        }
    }

    // if enter button is pressed then print the command
    if (input == KEY_ENTER || input == '\n') {
        if (USER_CMD == "exit\n" || USER_CMD == "quit\n") {
            reset_termios();
            endwin();
            exit(0);
        }

        for (int i = 0; i < USER_CMD.length(); i++) {
            USER_CMD[i] = toupper(USER_CMD[i]);
        }
        for (int i = 0; i < sizeof(TICKERS_AVAIL) / sizeof(TICKERS_AVAIL[0]); i++) {
            if (USER_CMD == TICKERS_AVAIL[i] + "\n") {
                TICKER = TICKERS_AVAIL[i];
                USER_CMD = "";
                return 55739;
            }
        }

        if (USER_CMD == "LA\n") {
            USER_CMD = "";
            return 100;
        }

        if (USER_CMD == "\n") {
            USER_CMD = "";
            return 11232;
        }

        USER_CMD = "";
    }

    // if arrow key is pressed
    if (input == KEY_UP || input == KEY_DOWN || input == 27) {
        if (input == 27) {
            input = getch(); // get the next character to identify which arrow key was pressed
            if (input == '[') {
                input = getch(); // get the next character after the '[' character
            }
        }
        switch (input) {
            case KEY_UP:
                USER_CMD = "";
                return 1001;
            case KEY_DOWN:
                USER_CMD = "";
                return 1002;
        }
    }

    return 0;
}

int main() {

    initscr();

    init_termios();

    cbreak(); // Line buffering disabled, pass on everything to me
    noecho(); // Don't echo while we do getch
    nodelay(stdscr, true); // Set input to non-blocking mode
    keypad(stdscr, true); // Enable interpretation of arrow keys
    start_color(); // Enable color

    int space;
    string space_str;

    int adjustspace = 9;

    string terminal_state = "narrow_stock";
    int check_input_state = 0;

    int active_cursor_ticker = 0;
    
    while (true) {
        // clear the screen
        clear();

        // get terminal size
        get_terminal_size();

        get_data_stock();
        
        // check for keyboard input
        check_input_state = check_input();
        
        if (check_input_state == 100) {
            terminal_state = "list_stock";
        }

        // print Tradix Finance Terminal
        //printf("%s", COLOR_BLACK_PURPLE("Tradix Finance Terminal").c_str());
        COLOR_BLACK_PURPLE("Tradix Finance Terminal");

        // display market status
        space = (COLS / 2) - ((MARKET_STATUS.length() + 2) / 2) - 25;
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        if (MARKET_STATUS == "STOCK MARKETS CLOSED") {
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_RED(MARKET_STATUS);
        } else {
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_GREEN(MARKET_STATUS);
        }

        // display time and date
        space = (COLS / 2) - ((get_time().length() + 2) / 2) - 25;
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        COLOR_WHITE_BLACK(space_str.c_str());
        COLOR_GREEN_BLUE(get_time());

        NEWLINE;
        NEWLINE;

        if (terminal_state == "narrow_stock"){
            // Display Ticker
            COLOR_WHITE_BLACK("    ");
            COLOR_BLACK_YELLOW("TICKER:");
            COLOR_GREEN_BLUE(TICKER);
            // display previous open
            space = adjustspace - TICKER.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            
            // display previous open
            space = adjustspace - TICKER.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            //printf("%s %s", space_str.c_str(), COLOR_WHITE_BLACK("PREV OPEN:").c_str());
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW("PREV OPEN:");
            COLOR_WHITE_BLACK(PREV_OPEN);

            // display previous low
            space = adjustspace - PREV_OPEN.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW("PREV LOW:");
            COLOR_WHITE_BLACK(PREV_LOW);

            // display previous high
            space = adjustspace - PREV_LOW.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW("PREV HIGH:");
            COLOR_WHITE_BLACK(PREV_HIGH);

            // display previous close
            space = adjustspace - PREV_HIGH.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW("PREV CLOSE:");
            COLOR_WHITE_BLACK(PREV_CLOSE);

            NEWLINE;
            NEWLINE;

            // display current price
            COLOR_WHITE_BLACK("    ");
            COLOR_BLACK_YELLOW("PRICE: ");
            COLOR_WHITE_BLACK(TDY_PRICE);

            // display current open
            space = adjustspace - TDY_PRICE.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW("CURR OPEN:");
            COLOR_WHITE_BLACK(TODAY_OPEN);

            // display current low
            space = adjustspace - TDY_PRICE.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW("CURR LOW:");
            COLOR_WHITE_BLACK(TODAY_LOW);

            // display current high
            space = adjustspace - TODAY_LOW.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW("CURR HIGH:");
            COLOR_WHITE_BLACK(TODAY_HIGH);

            // display current close
            space = adjustspace - TODAY_HIGH.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW("CURR CLOSE:");
            COLOR_WHITE_BLACK(TODAY_CLOSE);

            NEWLINE;
            NEWLINE;
            
            // display volume
            COLOR_WHITE_BLACK("    ");
            COLOR_BLACK_YELLOW("VOLUME:");
            COLOR_WHITE_BLACK(VOLUME);

            // display mkt cap
            space = adjustspace - VOLUME.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW(" MKT CAP: ");
            COLOR_WHITE_BLACK(MKT_CAP);

            // display PE ratio
            space = adjustspace - MKT_CAP.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW("PE RATIO:");
            COLOR_WHITE_BLACK(PE_RATIO);

            // display DIV_YLD
            space = adjustspace - PE_RATIO.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            COLOR_WHITE_BLACK(space_str.c_str());
            COLOR_BLACK_YELLOW(" DIV YLD: ");
            COLOR_WHITE_BLACK(DIV_YIELD);

            NEWLINE;
            NEWLINE;
            
        }
        else if (terminal_state == "list_stock") {

            // if USER_CMD is there in the list of tickers, then set active_cursor_ticker to that
            for (int i = 0; i < sizeof(TICKERS_AVAIL) / sizeof(TICKERS_AVAIL[0]); i++) {
                // capitalize USER_CMD
                string USER_CMD_CAP = USER_CMD;
                transform(USER_CMD_CAP.begin(), USER_CMD_CAP.end(), USER_CMD_CAP.begin(), ::toupper);
                if (TICKERS_AVAIL[i] == USER_CMD_CAP) {
                    active_cursor_ticker = i;
                }
            }

            if (check_input_state == 55739){
                terminal_state = "narrow_stock";
            }

            if (check_input_state == 11232) {
                terminal_state = "narrow_stock";
                TICKER = TICKERS_AVAIL[active_cursor_ticker];
                USER_CMD = "";
            }

            // if up arrow is clicked, change active_cursor_ticker to the previous ticker
            if (check_input_state == 1001) {
                if (active_cursor_ticker == 0) {
                    active_cursor_ticker = sizeof(TICKERS_AVAIL) / sizeof(TICKERS_AVAIL[0]) - 1;
                } else {
                    active_cursor_ticker--;
                }
            }

            // if down arrow is clicked, change active_cursor_ticker to the next ticker
            if (check_input_state == 1002) {
                if (active_cursor_ticker == sizeof(TICKERS_AVAIL) / sizeof(TICKERS_AVAIL[0]) - 1) {
                    active_cursor_ticker = 0;
                } else {
                    active_cursor_ticker++;
                }
            }

            // print out the list of tickers along with whitespace after so that
            for (int i = 0; i < sizeof(TICKERS_AVAIL_LA) / sizeof(TICKERS_AVAIL_LA[0]); i++) {
                // assume the max length of a ticker is 6
                space = 70 - TICKERS_AVAIL_LA[i].length();
                space_str = "";
                for (int j = 0; j < space; j++) {
                    space_str += " ";
                }
                if (i == active_cursor_ticker) {
                    COLOR_WHITE_BLACK("    ");
                    COLOR_BLACK_YELLOW(TICKERS_AVAIL_LA[i]);
                    COLOR_BLACK_YELLOW(space_str.c_str());
                    NEWLINE;
                    
                } else {
                    COLOR_WHITE_BLACK("    ");
                    COLOR_WHITE_BLACK(TICKERS_AVAIL_LA[i]);
                    COLOR_WHITE_BLACK(space_str.c_str());
                    NEWLINE;
                }
            }

            NEWLINE;
            NEWLINE;
            
        }

        // move cursor to the bottom left
        move(ROWS - 1, 0);
        // print user command
        init_pair(6, 255, COLOR_BLACK);
        attron(COLOR_PAIR(6));
        addstr(":");
        // convert to uppercase
        string USER_CMD_CAP = USER_CMD;
        transform(USER_CMD_CAP.begin(), USER_CMD_CAP.end(), USER_CMD_CAP.begin(), ::toupper);
        addstr(USER_CMD_CAP.c_str());
        attroff(COLOR_PAIR(6));

        refresh();

        // sleep for 50ms
        usleep(50000);
        // flush everything
        fflush(stdout);
        fflush(stdin);
    }

    reset_termios();

    endwin();

    return 0;
}