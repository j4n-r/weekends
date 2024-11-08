package main

import (
	"fmt"
	"os"

	"github.com/google/gopacket"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Printf("Usage: %s <IPv4 Address> <Port | Port-Port> <TCP || UDP>", os.Args[0])

	}
}
