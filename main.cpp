#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "color.cpp"

using namespace std;

// Global Variables
struct termios ORIG_TERMIOS, NEW_TERMIOS;
int ROWS, COLS;

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

// get time
string get_time() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return dt;
}

// get terminal size
void get_terminal_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    ROWS = w.ws_row;
    COLS = w.ws_col;
}

// function to check for keyboard input
void check_input(string& USER_CMD) {
    char input;
    if (read(STDIN_FILENO, &input, 1) > 0) {
        USER_CMD += input;
    }

    // if enter button is pressed then print the command
    if (input == '\n') {
        if (USER_CMD == "exit\n") {
            reset_termios();
            exit(0);
        }

        cout << "Command: " << USER_CMD << endl;
        USER_CMD = "";
    }
}

int main() {

    string USER_CMD = "";
    init_termios();

    // Set input to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    while (true) {
        system("clear");
        get_terminal_size();
        
        // Check for keyboard input
        check_input(USER_CMD);

        // ====================== Print Header ======================
        // Print Tradix Finance Terminal
        cout << COLOR_BLACK_PURPLE("Tradix Finance Terminal");

        // Print date and time
        int space = COLS - 22 - get_time().length();
        string space_str = "";
        for (int i = 0; i < space; i++) {
            space_str += " ";
        }
        cout << space_str << COLOR_BLACK_YELLOW(get_time()) << endl;
        // ====================== Print Header ======================

        // Delay of 50ms
        usleep(50000);
    }

    reset_termios();

    return 0;
}
