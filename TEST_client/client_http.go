// open new terminal, and run the code below
// go run client_http.go
// (you can change the line 16 to attack the server)

package main

import (
  "fmt"
  "log"
  "net"
  "sync"
  "io/ioutil"
)

const (
  addr = "localhost:6868"
  connCount = 2000
   // change the number and try with big number!
)

func connect(data string) {
  tcpAddr, err := net.ResolveTCPAddr("tcp", addr)
  if err != nil {
    log.Fatalf("ResolveTCPAddr failed; %v", err)
  }

  conn, err := net.DialTCP("tcp", nil, tcpAddr)
  if err != nil {
    log.Fatalf("Dial failed; %v", err)
  }

  _, err = conn.Write([]byte(data))
  if err != nil {
    log.Fatalf("Write to server failed; %v", err)
  }


  fmt.Printf("<----------\n")
  fmt.Printf("Data send Done! : %s\n", data)
  data_accept, err := ioutil.ReadAll(conn)
  fmt.Println("Response:", string(data_accept))
  fmt.Println("Total response size:", len(data_accept))
  fmt.Printf("---------->\n")
}

func main() {
  var wg sync.WaitGroup
  log.Printf("Data(%d) sending start.\n", connCount)
  for i := 0; i < connCount; i ++ {
    wg.Add(1)
    data := fmt.Sprintf("'Hello from %d'", i)
    go func() {
      defer wg.Done()
      connect(data)
    } ()
  }
  wg.Wait()
}