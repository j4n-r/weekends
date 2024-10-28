package main

import (
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
)

func main() {
	if len(os.Args) < 3 {
		fmt.Printf("Usage: %s <IP Address> <Portrange: e.g. 20-100>", os.Args[0])
		os.Exit(1)
	}
	ipaddr := os.Args[1]
	port_range := strings.Split(os.Args[2], "-")
	port_start, err := strconv.Atoi(port_range[0])
	if err != nil {
		fmt.Printf("Usage: %s <IP Address> <Portrange: e.g. 20-100>", os.Args[0])
		os.Exit(1)
	}
	port_end, err := strconv.Atoi(port_range[1])
	if err != nil {
		fmt.Printf("Usage: %s <IP Address> <Portrange: e.g. 20-100>", os.Args[0])
		os.Exit(1)
	}

	for port := port_start; port < port_end; port++ {
		_, err := net.Dial("tcp", fmt.Sprintf("%s:%d", ipaddr, port))
		if err != nil {
		} else {
			fmt.Printf("Port %d is open\n", port)
		}
	}

}
