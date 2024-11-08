package main

import (
	"bufio"
	"fmt"
	"github.com/gorilla/websocket"
	"net/http"
	"os"
)

var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool { return true },
}

type Server struct {
	c    *websocket.Conn
	recv chan []byte
	send chan []byte
}

func (s *Server) serveWS(w http.ResponseWriter, r *http.Request) {
	ws, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
    fmt.Println("Connected to: ",ws.RemoteAddr())
	s.c = ws
	go func() {
		for {
			_, msg, err := ws.ReadMessage()
			if err != nil {
				fmt.Println("read message error")
				return
			}
            fmt.Printf("Client: %s\n", msg)
		}
	}()

	go func() {
		for msg := range s.send {
			err := s.c.WriteMessage(websocket.TextMessage, msg)
			if err != nil {
				fmt.Println(err)
			}
		}
	}()
	select {}
}

func main() {
	s := Server{
		recv: make(chan []byte),
		send: make(chan []byte),
	}
	go s.initServer()
	scanner := bufio.NewScanner(os.Stdin)

	for {
		if scanner.Scan() {
			input := scanner.Bytes()
			s.send <- input
		}

		if err := scanner.Err(); err != nil {
			fmt.Println("Error reading from stdin:", err)
			break
		}
	}

}
func (s *Server) initServer() {
	http.HandleFunc("/", s.serveWS)
	http.ListenAndServe(":8080", nil)
}
