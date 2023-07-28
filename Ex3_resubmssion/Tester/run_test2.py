import sys
from socket import *
import random
import time

def get_split_point(request):
    if random.choice([True, False]):
        return random.randint(-4, -1)
    return random.randint(0, len(request))

port = int(sys.argv[1])

request = \
"""GET /nope HTTP/1.1\r
Host: nova.cs.tau.ac.il\r
Connection: keep-alive\r
Cache-Control: max-age=0\r
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r
User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.57 Safari/537.36\r
Accept-Encoding: gzip,deflate,sdch\r
Accept-Language: en-US,en;q=0.8,he;q=0.6\r\n\r\n"""

s = socket()
s.connect(('localhost', port))
first_part_len = get_split_point(request)
s.send(request[:first_part_len])
time.sleep(0.1)
s.send(request[first_part_len:])

response = ''
while 1:
    response += s.recv(1024)
    if response.count('\r\n\r\n') == 2:
        break

lines = response.split('\r\n')
assert len(lines) == 9
assert lines[0] == 'HTTP/1.1 404 Not Found'
assert lines[1] == 'Content-type: text/html'
assert lines[2] == 'Content-length: 113'
assert lines[3] == ''
assert lines[4] == "<html><head><title>Not Found</title></head><body>"
assert lines[5] == "Sorry, the object you requested was not found."
assert lines[6] == "</body></html>"
