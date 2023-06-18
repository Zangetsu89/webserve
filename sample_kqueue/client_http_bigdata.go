// go run client_http_bigdata.go
// send big file to the server

package main

import (
  "fmt"
  "log"
  "net"
  "io/ioutil"
  "os"
)

const (
  addr = "localhost:6868"
//   connCount = 20  // change the number and try with big number!
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
  data, err := os.ReadFile("bigdata.txt")
  if err != nil {
  	log.Fatalf("Read big data failed; %v", err)
  }

  fmt.Printf("Big data sending start.\n")
  connect(string(data))
}