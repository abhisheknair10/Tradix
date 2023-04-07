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

// =========================== GLOBAL VARIABLE ===========================

struct termios ORIG_TERMIOS, NEW_TERMIOS;
int ROWS, COLS;
string USER_CMD = "";

// =========================== GLOBAL VARIABLE ===========================

#define LINESPACE printf("\n");

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
    "NVDA", "GOOGL", "NFLX", "UBER", "PLTR"
};

string TICKERS_AVAIL_LA[] ={
    "AAPL - Apple Inc.", "TSLA - Tesla Inc.", "SPY - SPDR S&P 500 ETF Trust",
    "MSFT - Microsoft Corporation", "AMZN - Amazon.com Inc.", "META - META Platforms, Inc.",
    "GME - GameStop Corp.", "BB - BlackBerry Limited", "AMD - Advanced Micro Devices Inc.",
    "NVDA - NVIDIA Corporation", "GOOGL - Alphabet Inc.", "NFLX - Netflix Inc.",
    "UBER - Uber Technologies Inc.", "PLTR - Palantir Technologies Inc."
};

/*
    "SQ", "PYPL", "TWTR", "SNAP", "ROKU", "F", "GM", "NOK", "BBBY", "WISH",
    "AMC", "CLOV", "WKHS", "RBLX", "BAC", "WFC", "JPM", "GS", "C", "MS",
    "T", "VZ", "TMUS", "CMCSA", "NFLX", "DIS", "KO", "PEP", "MCD", "SBUX",
    "NKE", "WMT", "TGT", "COST", "HD", "LOW", "TJX", "DG", "ROST", "M",
    "JNJ", "PFE", "MRNA", "BNTX", "AZN", "GILD", "ABT", "LLY", "ABBV", "BMY",
    "CVS", "UNH", "MDT", "TMO", "DHR", "AMGN", "PFE", "JNJ", "MRK", "BMY",
    "ABBV", "LLY", "ABT", "GILD", "CVS", "UNH", "TMO", "DHR", "AMGN", "MDT",
    "PFE", "JNJ", "MRK", "BMY", "ABBV", "LLY", "ABT", "GILD", "CVS", "UNH",
    "TMO", "DHR", "AMGN", "MDT", "PFE", "JNJ", "MRK", "BMY", "ABBV", "LLY",
    "ABT", "GILD", "CVS", "UNH", "TMO", "DHR", "AMGN", "MDT", "PFE", "JNJ",
    "MRK", "BMY", "ABBV", "LLY", "ABT", "GILD", "CVS", "UNH", "TMO", "DHR",
    "AMGN", "MDT", "PFE", "JNJ", "MRK", "BMY", "ABBV", "LLY", "ABT", "GILD",
    "CVS", "UNH", "TMO", "DHR", "AMGN", "MDT", "PFE", "JNJ", "MRK", "BMY",
    "ABBV", "LLY", "ABT", "GILD", "CVS", "UNH", "TMO", "DHR", "AMGN", "MDT",
    "PFE", "JNJ", "MRK", "BMY", "ABBV", "LLY", "ABT", "GILD", "CVS", "UNH"
*/

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
int check_input() {
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
    if (input == 27) {
        read(STDIN_FILENO, &input, 1);
        if (input == 91) {
            read(STDIN_FILENO, &input, 1);
            switch (input) {
                case 65: // up arrow
                    USER_CMD = "";
                    return 1001;
                case 66: // down arrow
                    USER_CMD = "";
                    return 1002;
            }
        }
    }

    return 0;
}

int main() {

    init_termios();

    // set input to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    int space;
    string space_str;

    int adjustspace = 9;

    string terminal_state = "list_stock";
    int check_input_state = 0;

    int active_cursor_ticker = 0;
    
    while (true) {
        system("clear");
        get_terminal_size();

        get_data_stock();
        
        // check for keyboard input
        check_input_state = check_input();
        
        if (check_input_state == 100) {
            terminal_state = "list_stock";
        }

        // print Tradix Finance Terminal
        printf("%s", COLOR_BLACK_PURPLE("Tradix Finance Terminal").c_str());

        // display market status
        space = (COLS / 2) - (MARKET_STATUS.length() / 2) - 23;
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        if (MARKET_STATUS == "STOCK MARKETS CLOSED") {
            printf("%s%s", space_str.c_str(), COLOR_BLACK_RED(MARKET_STATUS).c_str());
        } else {
            printf("%s%s", space_str.c_str(), COLOR_BLACK_GREEN(MARKET_STATUS).c_str());
        }

        // display time and date
        space = (COLS / 2) - ((MARKET_STATUS.length() + 2) / 2) - get_time().length() - 5;
        space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        printf("%s%s", space_str.c_str(), COLOR_GREEN_BLUE(get_time()).c_str());

        printf("\n");
        LINESPACE;

        if (terminal_state == "narrow_stock"){
            // Display Ticker
            printf("    %s %s", COLOR_BLACK_YELLOW("TICKER:").c_str(), COLOR_GREEN_BLUE(TICKER).c_str());
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
            printf("%s %s", space_str.c_str(), COLOR_BLACK_WHITE("PREV OPEN:").c_str());
            printf("%s", COLOR_BLACK_WHITE(PREV_OPEN).c_str());

            // display previous low
            space = adjustspace - PREV_OPEN.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            printf("%s %s", space_str.c_str(), COLOR_BLACK_WHITE("PREV LOW:").c_str());
            printf("%s", COLOR_BLACK_WHITE(PREV_LOW).c_str());

            // display previous high
            space = adjustspace - PREV_LOW.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            printf("%s %s", space_str.c_str(), COLOR_BLACK_WHITE("PREV HIGH:").c_str());
            printf("%s", COLOR_BLACK_WHITE(PREV_HIGH).c_str());

            // display previous close
            space = adjustspace - PREV_HIGH.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            printf("%s %s", space_str.c_str(), COLOR_BLACK_WHITE("PREV CLOSE:").c_str());
            printf("%s", COLOR_BLACK_WHITE(PREV_CLOSE).c_str());

            printf("\n");
            LINESPACE;

            // display current price
            printf("    %s %s", COLOR_BLACK_YELLOW("PRICE: ").c_str(), COLOR_BLACK_WHITE(TDY_PRICE).c_str());

            // display current open
            space = adjustspace - TDY_PRICE.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            printf("%s %s", space_str.c_str(), COLOR_BLACK_YELLOW("CURR OPEN:").c_str());
            printf("%s", COLOR_BLACK_WHITE(TODAY_OPEN).c_str());

            // display current low
            space = adjustspace - TDY_PRICE.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            printf("%s %s", space_str.c_str(), COLOR_BLACK_YELLOW("CURR LOW:").c_str());
            printf("%s", COLOR_BLACK_WHITE(TODAY_LOW).c_str());

            // display current high
            space = adjustspace - TODAY_LOW.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            printf("%s %s", space_str.c_str(), COLOR_BLACK_YELLOW("CURR HIGH:").c_str());
            printf("%s", COLOR_BLACK_WHITE(TODAY_HIGH).c_str());

            // display current close
            space = adjustspace - TODAY_HIGH.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            printf("%s %s", space_str.c_str(), COLOR_BLACK_YELLOW("CURR CLOSE:").c_str());
            printf("%s", COLOR_BLACK_WHITE(TODAY_CLOSE).c_str());

            printf("\n");
            LINESPACE;

            // display volume
            printf("    %s %s", COLOR_BLACK_YELLOW("VOLUME:").c_str(), COLOR_BLACK_WHITE(VOLUME).c_str());

            // display mkt cap
            space = adjustspace - VOLUME.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            printf("%s %s", space_str.c_str(), COLOR_BLACK_YELLOW(" MKT CAP: ").c_str());
            printf("%s", COLOR_BLACK_WHITE(MKT_CAP).c_str());

            // display PE ratio
            space = adjustspace - MKT_CAP.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            printf("%s %s", space_str.c_str(), COLOR_BLACK_YELLOW("PE RATIO:").c_str());
            printf("%s", COLOR_BLACK_WHITE(PE_RATIO).c_str());

            // display DIV_YLD
            space = adjustspace - PE_RATIO.length();
            space_str = "";
            for (int i = 0; i < space; i++) {
                space_str += " ";
            }
            printf("%s %s", space_str.c_str(), COLOR_BLACK_YELLOW(" DIV YLD: ").c_str());
            printf("%s", COLOR_BLACK_WHITE(DIV_YIELD).c_str());

            printf("\n");
            LINESPACE;

        }
        else if (terminal_state == "list_stock") {

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
                space = 32 - TICKERS_AVAIL_LA[i].length();
                space_str = "";
                for (int j = 0; j < space; j++) {
                    space_str += " ";
                }
                if (i == active_cursor_ticker) {
                    printf(
                        "    %s%s", 
                        COLOR_BLACK_YELLOW(TICKERS_AVAIL_LA[i]).c_str(), 
                        COLOR_BLACK_YELLOW(space_str).c_str()
                    );
                    printf("\n");
                    
                } else {
                    printf(
                        "    %s%s", 
                        COLOR_BLACK_WHITE(TICKERS_AVAIL_LA[i]).c_str(), 
                        COLOR_BLACK_WHITE(space_str).c_str()
                    );
                    printf("\n");
                }
            }

            printf("\n");
            LINESPACE;
        }

        // sleep for 30ms
        usleep(30000);
        // flush everything
        fflush(stdout);
        fflush(stdin);
    }

    reset_termios();

    return 0;
}
