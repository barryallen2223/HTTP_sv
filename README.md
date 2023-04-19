
## HTTP Server - David Madrid Restrepo - Kevin Mauricio Loaiza Arango



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

The code was designed to be modular, which allows it to be easily extensible and maintainable. The main function is responsible for setting up the server and listening for incoming requests. Once a request is received, the request is beign processed and sends the appropriate response back to the client.

The server can serve different HTML, PNG and TXT files based on the URL requested by the client. The folder containing the HTML files can be specified as a command-line argument when the server is started. In addition, the server can log all requests to a specified log file.

But let's talk about sockets, what do they do, what are their function; In order to use sockets, we have to store the server's and client's struct information, and now we have to create the socket, by binding it to the indicated port in the specified parameter and an IP address given by INADDR_ANY, the family with AF_INET, and SOCK_STREAM would be the type of the socket and 0 is for the socket to decide waht type of protocol use by it's own.

For the next step we have to tell the system that the socket was created and associate to a port, using the funtion bind where we say it's attach to the strut where we defined the port.

Now the job for the socket is to listen, basically this means start the service without get any instructions and last the sockets wait to get any connections.

The process of connection waits for a client to connect to the server, and then, if the connection fails then we're gonna show the connection failed and log it onto the log file. But, in the other side if it's successful we're gonna show the message accepted connection sucessfully.

**Conclusions**

This HTTP server is a simple implementation of an HTTP/1.1 server using C. We made this program for the class of telematics, but also to understand pleany how the protocol HTTP works, it's a good base that allows us to extend this project with additional funcionalitys.

**Additional comments**

Overall, we think that the making of this project was challenging with the basic information and knowledge given during class, but nonetheless, it was worth all the time, effort and love we put in the project.


**References**

  

 - "HTTP/1.1 Semantics and Content" - https://tools.ietf.org/html/rfc7231
 - "C Standard Library" - https://en.cppreference.com/w/c/header
 - "Beej's Guide to Network Programming" - https://beej.us/guide/bgnet/html/multi/index.html
