#include "headers.h"

void fetchManPage(const char *command_name)
{
    // DNS resolution for man.he.net
    struct hostent *he;
    he = gethostbyname("man.he.net");

    if (he == NULL)
    {
        perror("DNS resolution failed");
        return;
    }

    // Open a TCP socket to the IP address
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        perror("Socket creation failed");
        return;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    server.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(server.sin_zero), '\0', 8);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("Socket connection failed");
        return;
    }

    // Send a GET request to the website's server
    char request[100];
    sprintf(request, "GET /%s HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command_name);

    if (send(sockfd, request, strlen(request), 0) == -1)
    {
        perror("Send failed");
        return;
    }

    // Read and display the body of the website
    char buffer[MAX_BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = recv(sockfd, buffer, sizeof(buffer), 0)) > 0)
    {
        fwrite(buffer, 1, bytes_read, stdout);
    }

    // Close the socket
    close(sockfd);
}


