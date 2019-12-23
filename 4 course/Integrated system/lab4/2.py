import os, sys, threading, time
from http.server import HTTPServer, CGIHTTPRequestHandler
from requests.exceptions import HTTPError
import json
import requests

ID = []

def DoRequest(method, st="", cmd="", data=""):
    try:
        url = 'http://localhost:8080/cgi-bin/book.py'+st
        header = {"Content-Type": "application/json"}
        res = method(url+cmd, headers=header, data=json.dumps(data))

        if res.status_code == 200:
            return json.loads(res.content)
    except Exception as e:
        pass

def GetMessages():
    messages = DoRequest(requests.get,"?type=Getmsg") # запрос
    # парсинг
    for item in messages:
        ID.append(messages[item]['id'])
        print('id = ' + messages[item]['id'] + ' to = ' + messages[item]['to'] + ' msg = ' + messages[item]['msg'])

def AddMessage():
    print('To?')
    to = input()
    print('Message?')
    msg = input()

    res = DoRequest(requests.get,f'?id=0&type=add&to={to}&message={msg}')
    if(res['res'] == 'OK'):
        print('\n\n----------- OK -----------\n\n')


def Findelem(a,id):
    for x in [a]:
        if id in x:
            return True
    return False


def DeleteMessage():
    GetMessages();
    print('Id?')
    id = input()
    if(Findelem(ID,id)):
        res = DoRequest(requests.get,f'?type=DeleteItem&id={id}')
        if(res['res'] == 'OK'):
            print('\n\n----------- OK -----------\n\n')
    else:
        print('Repeat')

def main():
    while True:
        print('\n\n')
        print('1: Get all messages')
        print('2: Add message')
        print('3: Delete message')
        print('0: Exit')

        param = input()
        if(param == '1'):
            GetMessages()
        else:
            if(param == '2'):
                AddMessage()
            else:
                if(param == '3'):
                    DeleteMessage()
                else:
                    if(param == '0'):
                        return
                    else:
                        print('Repeat, please')

        


main()