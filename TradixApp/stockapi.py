import requests
import random
import time

def stock_data():
    url = ''
    r = requests.get(url)
    data = r.json()
    return data


def random_data():
    pass

if __name__ == '__main__':
    
    while True:
        
        random_data()
        time.sleep(1)