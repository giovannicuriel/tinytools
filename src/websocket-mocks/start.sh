#!/bin/bash
ADMIN_TOKEN=$(curl -H "Content-Type: application/json; charset=utf-8" http://dojot_apigw_1:8000/auth --request POST --data '{"username": "admin", "passwd": "admin"}' | python3 -c 'import sys, json
print(json.load(sys.stdin)["jwt"])')
SOCKET_TOKEN=$(curl -H "Content-Type: application/json; charset=utf-8" -sH "Authorization: Bearer $ADMIN_TOKEN" http://dojot_apigw_1:8000/stream/socketio | python3 -c 'import sys, json
print(json.load(sys.stdin)["token"])')
npm run build
npm start -- -s "http://dojot_apigw_1:8000/" -c socketio -t $SOCKET_TOKEN
