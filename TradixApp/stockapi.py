import requests
import random
import time
import json

def stock_data():
    # get the ticler from the file request.json
    with open('request.json') as f:
        data = json.load(f)
        ticker = data['ticker']
    
    # get the data from the api
    url = ''
    r = requests.get(url)
    data = r.json()
    return data


def random_data():
    # get the ticler from the file request.json
    with open('request.json') as f:
        data = json.load(f)
        ticker = data['ticker']

    """
    following data needs to be generated randomly
    {
        "Market_Status": "STOCK MARKET CLOSED",
        "Ticker": "SPY",
        "PREV_OPEN": "0.00",
        "PREV_HIGH": "0.00",
        "PREV_LOW": "0.00",
        "PREV_CLOSE": "0.00",
        "TODAY_OPEN": "0.00",
        "TODAY_HIGH": "0.00",
        "TODAY_LOW": "0.00",
        "TODAY_CLOSE": "0.00",
        "CURR_PRICE": "0.00",
        "VOLUME": "0.00",
        "MKT_CAP": "0.00",
        "PE_RATIO": "0.00",
        "DIV_YIELD": "0.00%"
    }
    """

    Market_Status = "STOCK MARKET CLOSED"
    Ticker = ticker
    PREV_OPEN = str(random.randint(0, 1000))
    PREV_HIGH = str(random.randint(0, 1000))
    PREV_LOW = str(random.randint(0, 1000))
    PREV_CLOSE = str(random.randint(0, 1000))
    TODAY_OPEN = str(random.randint(0, 1000))
    TODAY_HIGH = str(random.randint(0, 1000))
    TODAY_LOW = str(random.randint(0, 1000))
    TODAY_CLOSE = str(random.randint(0, 1000))
    CURR_PRICE = str(random.randint(0, 1000))
    VOLUME = str(random.randint(0, 1000))
    MKT_CAP = str(random.randint(0, 1000))
    PE_RATIO = str(random.randint(0, 1000))
    DIV_YIELD = str(random.randint(0, 1000)) + "%"

    data = {
        "Market_Status": Market_Status,
        "Ticker": Ticker,
        "PREV_OPEN": PREV_OPEN,
        "PREV_HIGH": PREV_HIGH,
        "PREV_LOW": PREV_LOW,
        "PREV_CLOSE": PREV_CLOSE,
        "TODAY_OPEN": TODAY_OPEN,
        "TODAY_HIGH": TODAY_HIGH,
        "TODAY_LOW": TODAY_LOW,
        "TODAY_CLOSE": TODAY_CLOSE,
        "CURR_PRICE": CURR_PRICE,
        "VOLUME": VOLUME,
        "MKT_CAP": MKT_CAP,
        "PE_RATIO": PE_RATIO,
        "DIV_YIELD": DIV_YIELD
    }

    # write the data to the file
    with open('stockdata.json', 'w') as f:
        json.dump(data, f, indent=4)


if __name__ == '__main__':
    
    while True:
        
        random_data()
        time.sleep(1)