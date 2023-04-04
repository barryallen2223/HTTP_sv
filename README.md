
## HTTP Server



**Introduction**

  

This HTTP Server was created using the C language, and it is capable of responding to HTTP/1.1 GET, POST and HEAD requests made to the server. It serves HTML files that are located in the specified folder. In addition, it is also capable of logging all requests that it receives in the specified file.

**Development**

  

The HTTP server was developed using the C programming language with sockets, using the following libraries:
 - stdio.h
 - stdlib.h
 - string.h
 - unistd.h
 - stdbool.h
 - netinet/in.h
 - sys/socket.h
 - sys/stat.h
 - fcntl.h

The code was designed to be modular, which allows it to be easily extensible and maintainable. The main function is responsible for setting up the server and listening for incoming requests. Once a request is received the request is beign processed and sends the appropriate response back to the client.

The server can serve different HTML, PNG and TXT files based on the URL requested by the client. The folder containing the HTML files can be specified as a command-line argument when the server is started. In addition, the server can log all requests to a specified log file.

**Conclusions**

This HTTP server is a simple implementation of an HTTP/1.1 server using C. It is a good starting point for learning how an HTTP server works and can be easily extended to add additional functionality.

**References**

  

 - "HTTP/1.1 Semantics and Content" - https://tools.ietf.org/html/rfc7231
 - "C Standard Library" - https://en.cppreference.com/w/c/header
 - "Beej's Guide to Network Programming" - https://beej.us/guide/bgnet/html/multi/index.html

