import socket
import time

HOST = '127.0.0.1'
PORT = 3555

# Incomplete HTTP POST request
request_lines = [
    "POST / HTTP/1.1\r\n",
    "Host: localhost\r\n",
    "Content-Length: 20\r\n",
    "Content-Type: text/plain\r\n",
    "\r\n",
    "This is 20 bytes"
]

def slow_send(sock, data, delay=0.5):
    for char in data:
        sock.send(char.encode())
        time.sleep(delay)

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        print(f"Connected to {HOST}:{PORT}")
        
        full_request = ''.join(request_lines)
        print("Sending request very slowly...")
        slow_send(s, full_request, delay=0.2)  # 0.2 seconds between bytes

        print("Done sending. Waiting for response...")
        try:
            s.settimeout(5)
            response = s.recv(4096)
            print("Server responded:\n", response.decode(errors='ignore'))
        except socket.timeout:
            print("No response received (timeout).")

if __name__ == "__main__":
    main()

    