// open new terminal, and run the code below
// go run client_httpcontent.go
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
  addr = "localhost:5555"
  connCount = 1
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
    
    // data = fmt.Sprintf("GET /favicon.ico HTTP/1.1")
    // data += fmt.Sprintf("Host: localhost:8080")
    // data += fmt.Sprintf("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:109.0) Gecko/20100101 Firefox/114.0")
    // data += fmt.Sprintf("Accept: image/avif,image/webp,*/*")
    // data += fmt.Sprintf("Content-Length: 258")
    // data += fmt.Sprintf("\n")
    // data += fmt.Sprintf("body data data data")
    // data += fmt.Sprintf("hello hello 123456")
    go func() {
      defer wg.Done()
      data := "GET /ico.png HTTP/1.1\n" + "Host: localhost:5555\r\n" + "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:109.0) Gecko/20100101 Firefox/114.0\r\n"
      data += "Accept: image/avif,image/webp,*/*\r\n"
      data +=  "Content-Type: multipart/form-data\r\n"
      data +=  "Content-Length: 258\r\n"
      data +=  "\r\n"
      data += "body data data data\r\n"
      data += "hello hello 12345"
  

      connect(data)
    } ()
  }
  wg.Wait()
}