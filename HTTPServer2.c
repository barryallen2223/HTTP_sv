#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER 262144
#define BUFFERHTML 262144

char* htmlFn(char response_body[BUFFERHTML], char *buffer, FILE *page){
    char *caseN = (char *)malloc(BUFFERHTML*2);
    //
    if (caseN == NULL) {
        perror("[-] malloc failed");
        exit(EXIT_FAILURE);
    }
    memset(caseN, 0, BUFFERHTML*2);
    fgets(response_body, BUFFERHTML, page);
    while (fgets(buffer, BUFFER, page)) {
        strcat(caseN, buffer);
    }
    return caseN;
}

int main(int argc, char * argv []) {
    if (argc < 4) {
        printf("Usage: %s <HTTP PORT> %s <Log File> %s <DocumentRootFolder>\n", argv[0], argv[1], argv[2]);
        return 1;
    }

    int PORTn = atoi(argv[1]);
    char *logFile = argv[2];
    char *docRootFolder = argv[3];

    FILE *fp = fopen(logFile, "w"); // open the log file for writing
    if (fp == NULL) {
        printf("Error opening log file %s\n", logFile);
        return 1;
    }

    FILE *html_home, *case_1, *case_2, *case_3, *case_4, *error404, *txt1MB;
    // HTML Files
    html_home = fopen("templates/index.html", "r");
    case_1 = fopen("templates/case1.html", "r");
    case_2 = fopen("templates/case2.html", "r");
    case_3 = fopen("templates/case3.html", "r");
    case_4 = fopen("templates/case4.html", "r");
    error404 = fopen("templates/error.html", "r");
    // TXT Files
    txt1MB = fopen("file1.txt", "r");

    int sockfd, newsockfd, opt = 1;
    struct sockaddr_in server_address, client_address;
    int addrlen = sizeof(server_address);
    char buffer[BUFFER] = {0};
    char response_body[BUFFERHTML] = {0};
    //char *home_page = "<html><head><title>Home Page</title></head><body><h1>Welcome to my home page!</h1><img src='image1.png'><img src='image2.png'></body></html>";
    char *home_page = htmlFn(response_body, buffer, html_home);
    char *about_page = "<html><head><title>About Page</title></head><body><h1>About Me</h1><p>I am a software developer.</p></body></html>";
    char *error_page = "<html><head><title>Error Page</title></head><body><h1>404 Not Found</h1><p>The requested resource was not found.</p></body></html>";

    char *case_1_page = htmlFn(response_body, buffer, case_1);
    char *case_2_page = htmlFn(response_body, buffer, case_2);
    char *case_3_page = htmlFn(response_body, buffer, case_3);
    char *case_4_page = htmlFn(response_body, buffer, case_4);

    char *file_1_page = htmlFn(response_body, buffer, txt1MB);
    

    FILE *image1 = fopen("image1.png", "rb");
    FILE *image2 = fopen("image2.png", "rb");
    FILE *image3 = fopen("image3.png", "rb");
    FILE *image4 = fopen("image4.png", "rb");
    FILE *image5 = fopen("image5.png", "rb");
    FILE *image6 = fopen("image6.png", "rb");

    // Create a TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("[-] Socket failed\n");
        fprintf(fp, "[-] Socket failed\n");
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp, "[+] Socket created successfully!\n");
    // Set socket options
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("[-] Setsockopt failed\n");
        fprintf(fp, "[-] Setsockopt failed\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "[+] Socket configured successfully!\n");
    
    // Bind the socket to a port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORTn);
    if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("[-] Bind failed\n");
        fprintf(fp, "[-] Bind failed\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "[+] Socket binded successfully!\n");

    // Listen for incoming connections
    if (listen(sockfd, 30) < 0) {
        perror("[-] Listen failed");
        fprintf(fp, "[-] Listen failed\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "[+] Socket listening for incoming connections...\n");

    // Loop to accept incoming connections
    while (1) {
        // Accept an incoming connection
        if ((newsockfd = accept(sockfd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen)) < 0) {
            perror("[-] Accept failed");
            exit(EXIT_FAILURE);
        }

        // Receive the HTTP request from the client
        memset(buffer, 0, sizeof(buffer));
        if (recv(newsockfd, buffer, 1024, 0) < 0) {
            perror("[-] recv failed");
            exit(EXIT_FAILURE);
        }
        // Parse the request to determine which resource the client is requesting
        char *resource = strtok(buffer, " ");
        if (strcmp(resource, "GET") != 0) {
            // Method not supported
            sprintf(response_body, "HTTP/1.1 501 Not Implemented\r\n\r\n");
            send(newsockfd, response_body, strlen(response_body), 0);
        } else {
            // Get the requested URL
            resource = strtok(NULL, " ");
            if (strcmp(resource, "/") == 0) {
                // Home page
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s", home_page);
                send(newsockfd, response_body, strlen(response_body), 0);
            } else if (strcmp(resource, "/case1") == 0) {
                // Case 1 page
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s", case_1_page);
                send(newsockfd, response_body, strlen(response_body), 0);
            } else if (strcmp(resource, "/case2") == 0) {
                // Case 2 page
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s", case_2_page);
                send(newsockfd, response_body, strlen(response_body), 0);
            } else if (strcmp(resource, "/case3") == 0) {
                // Case 3 page
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s", case_3_page);
                send(newsockfd, response_body, strlen(response_body), 0);
            } else if (strcmp(resource, "/case5") == 0) {
                // Case 4 page
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s", case_4_page);
                send(newsockfd, response_body, strlen(response_body), 0);
            } else if (strcmp(resource, "/about") == 0) {
                // About page
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s", about_page);
                send(newsockfd, response_body, strlen(response_body), 0);
            } else if (strcmp(resource, "/file1") == 0) {
                // File 1
                int file_fd = open("file1.txt", O_RDONLY);
                if (file_fd < 0) {
                    printf("\nCould not open file\n");
                    return -1;
                }
                ssize_t read_size = 0;
                char buffer_file[BUFFER*4];
                while ((read_size = read(file_fd, buffer_file, sizeof(buffer_file))) > 0) {
                    // Send the chunk over the socket
                    ssize_t send_size = send(newsockfd, buffer_file, read_size, 0);
                    if (send_size < 0) {
                        printf("\nFailed to send data over socket\n");
                        return -1;
                    }
                }
            } else if (strcmp(resource, "/file2") == 0) {
                // File 2
                int file_fd = open("file2.txt", O_RDONLY);
                if (file_fd < 0) {
                    printf("\nCould not open file\n");
                    return -1;
                }
                ssize_t read_size = 0;
                char buffer_file[BUFFER*4];
                while ((read_size = read(file_fd, buffer_file, sizeof(buffer_file))) > 0) {
                    // Send the chunk over the socket
                    ssize_t send_size = send(newsockfd, buffer_file, read_size, 0);
                    if (send_size < 0) {
                        printf("\nFailed to send data over socket\n");
                        return -1;
                    }
                }
            } else if (strcmp(resource, "/file3") == 0) {
                // File 3
                int file_fd = open("file3.txt", O_RDONLY);
                if (file_fd < 0) {
                    printf("\nCould not open file\n");
                    return -1;
                }
                ssize_t read_size = 0;
                char buffer_file[BUFFER*4];
                while ((read_size = read(file_fd, buffer_file, sizeof(buffer_file))) > 0) {
                    // Send the chunk over the socket
                    ssize_t send_size = send(newsockfd, buffer_file, read_size, 0);
                    if (send_size < 0) {
                        printf("\nFailed to send data over socket\n");
                        return -1;
                    }
                }
            } else if (strcmp(resource, "/image1.png") == 0) {
                // Image 1
                fseek(image1, 0, SEEK_END);
                int image_size = ftell(image1);
                fseek(image1, 0, SEEK_SET);
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: %d\r\n\r\n", image_size);
                send(newsockfd, response_body, strlen(response_body), 0);
                while (!feof(image1)) {
                    int bytes_read = fread(buffer, 1, 1024, image1);
                    send(newsockfd, buffer, bytes_read, 0);
                }
            } else if (strcmp(resource, "/image2.png") == 0) {
                // Image 2
                fseek(image2, 0, SEEK_END);
                int image_size = ftell(image2);
                fseek(image2, 0, SEEK_SET);
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: %d\r\n\r\n", image_size);
                send(newsockfd, response_body, strlen(response_body), 0);
                while (!feof(image2)) {
                    int bytes_read = fread(buffer, 1, 1024, image2);
                    send(newsockfd, buffer, bytes_read, 0);
                }
            } else if (strcmp(resource, "/image3.png") == 0) {
                // Image 3
                fseek(image3, 0, SEEK_END);
                int image_size = ftell(image3);
                fseek(image3, 0, SEEK_SET);
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: %d\r\n\r\n", image_size);
                send(newsockfd, response_body, strlen(response_body), 0);
                while (!feof(image3)) {
                    int bytes_read = fread(buffer, 1, 1024, image3);
                    send(newsockfd, buffer, bytes_read, 0);
                }
            } else if (strcmp(resource, "/image4.png") == 0) {
                // Image 4
                fseek(image4, 0, SEEK_END);
                int image_size = ftell(image4);
                fseek(image4, 0, SEEK_SET);
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: %d\r\n\r\n", image_size);
                send(newsockfd, response_body, strlen(response_body), 0);
                while (!feof(image4)) {
                    int bytes_read = fread(buffer, 1, 1024, image4);
                    send(newsockfd, buffer, bytes_read, 0);
                }
            } else if (strcmp(resource, "/image5.png") == 0) {
                // Image 5
                fseek(image5, 0, SEEK_END);
                int image_size = ftell(image5);
                fseek(image5, 0, SEEK_SET);
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: %d\r\n\r\n", image_size);
                send(newsockfd, response_body, strlen(response_body), 0);
                while (!feof(image5)) {
                    int bytes_read = fread(buffer, 1, 1024, image5);
                    send(newsockfd, buffer, bytes_read, 0);
                }
            } else if (strcmp(resource, "/image6.png") == 0) {
                // Image 6
                fseek(image6, 0, SEEK_END);
                int image_size = ftell(image6);
                fseek(image6, 0, SEEK_SET);
                sprintf(response_body, "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: %d\r\n\r\n", image_size);
                send(newsockfd, response_body, strlen(response_body), 0);
                while (!feof(image6)) {
                    int bytes_read = fread(buffer, 1, 1024, image6);
                    send(newsockfd, buffer, bytes_read, 0);
                }
            } else {
                // Resource not found
                sprintf(response_body, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n%s", error_page);
                send(newsockfd, response_body, strlen(response_body), 0);
            }
        }
        // Close the connection
        close(newsockfd);
    }
    fclose(fp);
    return 0;
}
