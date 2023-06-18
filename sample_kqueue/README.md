# Kqueue Sample Loop
A sample code of Kqueue event loop.


```
# start the server
$ c++ -o serve colors.cpp server_http.cpp
$ ./server
  
# start the client in a separate terminal
$ go run client_http.go

you can access to the server by browser (localhost:6868)  
and check the HTTP request sent by that browser.  
```

The original code is from "Nginx Event Driven Architecture — Demonstrated in Code(with epoll)"  
A detailed explanation of the code structure can be found at  
-> https://eileen-code4fun.medium.com/nginx-event-driven-architecture-demonstrated-in-code-51bf0061cad9.




Kqueue referenced code (Sorry, in Japanese... google translation may help...)  
"Let's use kqueue and kevent on *BSD"  
->  http://x68000.q-e-d.net/~68user/net/c-kqueue-1.html
