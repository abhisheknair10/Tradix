// COMPILE AND RUN: clang++ stockdata.cpp -o stockdata; ./stockdata
// compile to target: clang++ stockdata.cpp -o ../TradixApp/stockdata

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "color.cpp"

string TICKER = "AAPL";
string PRICE = "0.00";
string TOKEN = "cgmv0upr01qhveustqb0cgmv0upr01qhveustqbg";

// function to connect to the API and get the stock data
// https://finnhub.io/api/v1/quote?symbol=SPY&token=cgmv0upr01qhveustqb0cgmv0upr01qhveustqbg
void get_stock_data() {
    string command = "curl -s \"https://finnhub.io/api/v1/quote?symbol=" + TICKER + "&token=" + TOKEN + "\"";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        cout << "ERROR: Failed to run command" << endl;
        exit(1);
    }
    char buffer[128];
    string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            result += buffer;
        }
    }
    pclose(pipe);

    // parse the result
    int start = result.find("c\":") + 3;
    int end = result.find(",", start);
    PRICE = result.substr(start, end - start);
}

int main() {

    char buffer[128];
    while (1) {

        // read from file request.txt
        FILE* file = fopen("request.txt", "r");
        if (!file) {
            cout << "ERROR: Failed to open file" << endl;
            exit(1);
        }
        fgets(buffer, 128, file);
        fclose(file);
        TICKER = buffer;

        get_stock_data();
        
        // write the data to a file stockdata.txt
        FILE* file2 = fopen("stockdata.txt", "w");
        if (!file2) {
            cout << "ERROR: Failed to open file2" << endl;
            exit(1);
        }
        string inter_ticker = TICKER.substr(1, TICKER.length() - 2);
        fprintf(file2, "%s: %s", inter_ticker.c_str(), PRICE.c_str());
        fclose(file2);

        sleep(1);

        system("clear");
        
        // display at the centre of the terminal screen
        //cout << "\033[42;30m Stock Data Network Established... \033[0m" << endl;
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int COLS = w.ws_col;
        int ROWS = w.ws_row;
        int mid_row = ROWS / 2;
        int mid_col = COLS / 2;
        
        string message = " Network Established with API and Streaming Data... ";
        int length = message.length();
        
        int start_col = mid_col - length / 2;
        
        for (int i = 0; i < mid_row; i++) {
            cout << endl;
        }

        for (int i = 0; i < start_col; i++) {
            cout << " ";
        }

        cout << "\033[42;30m" << message << "\033[0m" << endl;

        flush(cout);
    }
}