class TextRender:

    def black_red(text):
        return '\033[30;41m ' + str(text) + ' \033[0m'
    
    def black_green(text):
        return '\033[30;42m ' + str(text) + ' \033[0m'