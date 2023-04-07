#include <string>

using namespace std;

// Define Colors
string COLOR_BLACK_RED(string text) { 
    return "\033[41;30m " + text + " \033[0m"; 
}

string COLOR_BLACK_GREEN(string text) { 
    return "\033[42;30m " + text + " \033[0m"; 
}

string COLOR_BLACK_YELLOW(string text) { 
    return "\033[43;30m " + text + " \033[0m"; 
}

string COLOR_BLACK_PURPLE(string text) { 
    return "\033[45;30m " + text + " \033[0m"; 
}

string COLOR_BLACK_CYAN(string text) { 
    return "\033[46;30m " + text + " \033[0m"; 
}

string COLOR_BLACK_WHITE(string text) { 
    return "\033[0m " + text + " \033[0m"; 
}

string COLOR_GREEN_BLUE(string text) { 
    return "\033[44;32m " + text + " \033[0m"; 
}

string COLOR_WHITE_BLACK(string text) { 
    return "\033[30;47m " + text + " \033[0m"; 
}