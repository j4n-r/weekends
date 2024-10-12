import sys 
import socket


def main(argv):
    ipaddress = argv[1]
    port_range = argv[2].split("-")
    min_port =int (port_range[0])
    max_port = int (port_range[1])
    curr_port = int(min_port)
    for curr_port in range(min_port, max_port):
        try:
            sock = socket.socket()
            sock.settimeout(1)
            sock.connect((ipaddress, curr_port))
            try:
                data = sock.recv(4096)  
                print(f"Port {curr_port} is open and received data: {data}")
            except socket.timeout:
                print(f"Port {curr_port} is open, but no data received.")
            sock.close()
        except socket.error:
            continue


if __name__ == "__main__":
    if len(sys.argv) !=3:
        print(f"usage: {sys.argv[0]} <IPv4Address> <Portrange e.g. 10-200>")
    else:
        main(sys.argv)
