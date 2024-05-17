from flask import Flask, request, jsonify
from flask_cors import CORS
import socket
import atexit
import redis

r = redis.Redis(host='localhost', port=6379)

app = Flask(__name__)
CORS(app)

CPP_SERVER_ADDRESS = ('localhost', 9999)


@app.route('/send_request/<msg>')
def send_request(msg):
    # Send a request to the C++ program

    print(msg)

    str = msg[0] + msg[2]
    if msg != "kill":
        if msg[1] == msg[3]:
            str += 'S'
        else:
            str += 'NS'

    print(str)

    if r.exists(str):
        return r.get(str)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(CPP_SERVER_ADDRESS)
        s.sendall(msg.encode())
        response = s.recv(1024).decode()
        
        r.set(str, response)
    return jsonify(response)

def handle_exit():
    send_request("kill")
    r.flushall()

if __name__ == '__main__':
    atexit.register(handle_exit)
    app.run(debug=True)

