# Written by Nicholas Gibson on 1/24/18
# Written by Nicholas Gibson on 1/24/18
# https://wiki.python.org/moin/UdpCommunication

import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 3000

print "UDP target IP: ", UDP_IP
print "UDP target port: ", UDP_PORT

sock = socket.socket(socket.AF_INET,  # Internet
                     socket.SOCK_DGRAM)  # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(10240) # buffer size is 1024 bytes
    print "received message: ", data
