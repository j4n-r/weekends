import sys 
import socket


def main(argv):
    ipaddress = argv[1]
    port_range = argv[2].split("-")
    min_port =int (port_range[0])
    max_port = int (port_range[1])
    curr_port = int(min_port)
    for curr_port in range(max_port):
        try :
            sock = socket.socket()
            sock.connect((ipaddress, curr_port))
            print(f"Port {curr_port} is open")
            sock.close()
        except socket.error:
            continue


if __name__ == "__main__":
    if len(sys.argv) !=3:
        print(f"usage: {sys.argv[0]} <IPv4Address> <Portrange e.g. 10-200>")
    else:
        main(sys.argv)
