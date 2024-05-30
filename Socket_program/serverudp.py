#!/usr/bin/env python3
import socket

def Main():
    host = '127.0.0.1'
    port = 50001

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind((host, port))
    print("Server Started")

    while True:
        data, addr = s.recvfrom(1024)
        message = data.decode('utf-8')
        print("Message from: " + str(addr))
        print("From connected user: " + message)

        # Echo the message back to the client
        s.sendto(data, addr)

if __name__ == '__main__':
    Main()
