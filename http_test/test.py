from flask import Flask, request, Response
import time
import random

app = Flask(__name__)
app.debug = True

i = 0


@app.route("/test")
def test_get():
    # time.sleep(random.uniform(0.5,1.5))
    global i
    if i == 100:
        i = 0
    i += 1
    return F"TEST {i}", 200


@app.route("/post", methods=['POST'])
def test_post():
    req_json = request.data
    print(req_json)
    return Response(response=req_json, status=200, mimetype='application/json')


@app.route("/sleep", methods=['POST', 'GET'])
def test_sleep():
    print("SLEEP")
    time.sleep(20)
    return Response(response="{'Sleep': 'Done'}", status=200, mimetype='application/json')