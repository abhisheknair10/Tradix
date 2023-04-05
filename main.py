import os
import time

class TextRender:
    def black_red(text):
        return '\033[30;41m ' + str(text) + ' \033[0m'
    
    def black_green(text):
        return '\033[30;42m ' + str(text) + ' \033[0m'


def get_terminal_size():
    rows, columns = os.popen('stty size', 'r').read().split()
    return int(rows), int(columns)

def print_row_col(row, col, text):
    return '\033[' + str(row) + ';' + str(col) + 'H' + text


def __main__():
    
    start = time.time()

    USER_CMD = ""
    USER_CMD_HIST = []
    while True:

        rows, columns = get_terminal_size()

        if time.time() - start > 0.5:
            start = time.time()
            row, col = get_terminal_size()
            
            os.system('clear')

            print(print_row_col(0, 0, TextRender.black_red(time.strftime('%H:%M:%S'))))

            print(print_row_col(row, 0, ":" + USER_CMD), end="", flush=True)



if __name__ == '__main__':
    __main__()