#!/usr/bin/env python3

'''
Original script written and copyright; The TurtleCoin Developers 2018
Adapted for AmityCoin and copyright; The CalecXore Developers 2018
-Thanks

It works fine on Ubuntu 18.04; use that or install Python 3.6.

Make two wallets and fill one or both with big bucks.
Open the wallets with amity-service like so:

./amity-service -w Wallet1.wallet -p walletpass1 --rpc-password test --bind-port 31019
./amity-service -w Wallet1.wallet -p walletpass1 --rpc-password test --bind-port 31020

Then run:
python3 makechange.py

'''

import requests
import json
import secrets
import time
import sys
from threading import Thread

# Forks adjust as needed
moveDecimal = 10000  # AMIT has 4 decimals so 10000 is the divide/multiply factor
minAmount = 100 * moveDecimal  # min number for amount to xfer
# maxAmount is not used in this script..  We just keep TX'ing larger amounts
# maxAmount = 5000 * moveDecimal  # max number for amount to xfer
anonymity = 3
fee = 10 # atomic units, TRTL would be 0.10 as the tx network fee

def getAddress(host, port, rpcPassword):
    payload = {
        'jsonrpc': '2.0',
        'method': "getAddresses",
        'password': rpcPassword,
        'id': 'test',
        'params': {}
    }

    url = 'http://' + host + ':' + port + '/json_rpc'

    response = requests.post(
        url, data=json.dumps(payload),
        headers={'content-type': 'application/json'}
    ).json()

    if 'error' in response:
        print(response['error'])
        print('Failed to get address, exiting')
        sys.exit()
    else:
        return response['result']['addresses'][0]


def sendTransaction(host, port, rpcPassword, **kwargs):
    payload = {
        'jsonrpc': '2.0',
        'method': "sendTransaction",
        'password': rpcPassword,
        'id': 'test',
        'params': kwargs
    }

    url = 'http://' + host + ':' + port + '/json_rpc'

    response = requests.post(
        url, data=json.dumps(payload),
        headers={'content-type': 'application/json'}
    ).json()
    if 'error' in response:
        response['error']['amount'] =  kwargs['transfers'][0]['amount']/moveDecimal
        print(response['error'])
        return False
    else:
        response['result']['amount'] =  kwargs['transfers'][0]['amount']/moveDecimal
        print(response['result'])
        return True


def sendTXs(host, port, rpcPassword, sender, receiver):
    sleepAmount = 10
    amount = minAmount
    while True:
        amount += minAmount

        params = {'transfers': [{'address': receiver, 'amount': amount}],
                  'fee': fee,
                  'anonymity': anonymity,
                  'changeAddress': sender}

        if not sendTransaction(host, port, rpcPassword, **params):
            time.sleep(sleepAmount)
            print("Sleeping for " + str(sleepAmount) + " seconds...")
            sleepAmount += 10
            amount -= minAmount
        else:
            sleepAmount = 10


walletdHostA = "127.0.0.1"
walletdPortA = "31020"

walletdHostB = "127.0.0.1"
walletdPortB = "31019"

rpcPasswordA = "test"
rpcPasswordB = "test"

addressA = getAddress(walletdHostA, walletdPortA, rpcPasswordA)
addressB = getAddress(walletdHostB, walletdPortB, rpcPasswordB)

Thread(target=sendTXs, args=(walletdHostA, walletdPortA, rpcPasswordA,
       addressA, addressB)).start()

Thread(target=sendTXs, args=(walletdHostB, walletdPortB, rpcPasswordB,
       addressB, addressA)).start()
