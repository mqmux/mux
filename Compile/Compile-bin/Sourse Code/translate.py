# -*- coding: utf-8 -*-
import os
import sys
import uuid
import requests
import hashlib
import time
import json
from importlib import reload

import time

reload(sys)

YOUDAO_URL = 'https://openapi.youdao.com/api'
APP_KEY = '2f410289bc8a4f6f'
APP_SECRET = 'ZKQ9H3ayT1rTKxaQj9FUpjL87YsQcsWM'


def encrypt(signStr):
    hash_algorithm = hashlib.sha256()
    hash_algorithm.update(signStr.encode('utf-8'))
    return hash_algorithm.hexdigest()


def truncate(q):
    if q is None:
        return None
    size = len(q)
    return q if size <= 20 else q[0:10] + str(size) + q[size - 10:size]


def do_request(data):
    headers = {'Content-Type': 'application/x-www-form-urlencoded'}
    return requests.post(YOUDAO_URL, data=data, headers=headers)


def connect():
    q = encrypt_file
    # q = "文化复兴"

    data = {}
    data['from'] = 'auto'
    data['to'] = 'auto' # zh-CHS en auto
    data['signType'] = 'v3'
    curtime = str(int(time.time()))
    data['curtime'] = curtime
    salt = str(uuid.uuid1())
    signStr = APP_KEY + truncate(q) + salt + curtime + APP_SECRET
    sign = encrypt(signStr)
    data['appKey'] = APP_KEY
    data['q'] = q
    data['salt'] = salt
    data['sign'] = sign
    data['vocabId'] = "general"
    
    # print("curtime="+curtime)
    # print("salt="+salt)
    # print("signStr="+signStr)
    # print("sign="+sign)
    # print("---------------------")
    
    response = do_request(data)
    contentType = response.headers['Content-Type']
    
    decoded_data = response.content.decode('utf-8')
    # print(decoded_data)
    # print("---------------------")
    
    parsed_data = json.loads(response.content)
    # print(parsed_data["translation"])
    youdao_result = str(parsed_data["translation"])
    youdao_result = youdao_result.replace("[", "").replace("]", "").replace("\'", "")
    print(youdao_result)
    
    try:
        print(parsed_data["basic"])
        print(parsed_data["web"])
    except Exception as err:
        exit


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: python '+sys.argv[0]+' words')
        exit()
    result = ""
    for arg in sys.argv[1:]:
        result += arg + " "
    encrypt_file = result
    connect()