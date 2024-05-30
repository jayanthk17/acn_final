#!/usr/bin/env python3
import socket

def Main():
    host = '127.0.0.1'
    port = 50001

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server = (host, port)

    message = input("Type your message: ")
    s.sendto(message.encode('utf-8'), server)

    data, server = s.recvfrom(1024)
    print("Received from server: " + data.decode('utf-8'))

    s.close()

if __name__ == '__main__':
    Main()
