#include "headers.h"

int man_function(char* command_name)
{
    int sfd;    // https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
    char* host = (char*)calloc(1024,sizeof(char));
    char* address = (char*)calloc(1024,sizeof(char));
    sprintf(host, "man.he.net");
    struct addrinfo  hints;         // https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
    struct addrinfo  *result;

    char* PORT = "80"; // https://www.infoworld.com/article/2853780/socket-programming-for-scalable-systems.html

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int s = getaddrinfo("man.he.net", PORT , &hints, &result);
    if (s == (-1)) 
    {
        printf("\033[1;31mERROR: Couldn't get information about Host\n");
        printf("\033[0m");
        return 0;
    }

    sfd = socket(result->ai_family, result->ai_socktype,result->ai_protocol);
    if (sfd == (-1)) 
    {
        printf("\033[1;31mERROR: Couldn't establish TCP Socket\n");
        printf("\033[0m");
        return 0;
    }

    int con = connect(sfd, result->ai_addr, result->ai_addrlen);
    if(con == (-1))
    {
        printf("\033[1;31mERROR: Couldn't connect with server\n");
        printf("\033[0m");
        return 0;        
    }


    char* recvline = (char*)calloc(1024,sizeof(char));
    int send_sig;
    int recv_data;
    sprintf(address,"GET /?topic=%s&section=all HTTP/1.1\r\nHost: %s\r\n\r\n",command_name,host);   // https://cboard.cprogramming.com/c-programming/142841-sending-http-get-request-c.html
    send_sig = send(sfd,address,strlen(address),0);     // https://man7.org/linux/man-pages/man2/send.2.html
    if( send_sig == (-1))
    {
        printf("\033[1;31mERROR: Couldn't send API request to the server\n");
        printf("\033[0m");
        return 0;
    }
    recv_data = recv(sfd,recvline,1024,0);          // https://man7.org/linux/man-pages/man2/recv.2.html
    if(recv_data <= 0)
    {
        printf("\033[1;31mERROR: Page has nothing on it\n");
        printf("\033[0m");
        return 0;
    }
    int http_response_code;
    if(strstr(recvline,"<h2 align=center>No matches for") != NULL)
    {
        printf("\033[1;31mERROR: Command does not exist\n");
        printf("\033[0m");
        return 0;        
    }
    sscanf(recvline, "HTTP/1.0 %d", &http_response_code);
    if (http_response_code == 404)
    {
        printf("\033[1;31mERROR: Page does not exist\n");
        printf("\033[0m");
        return 0;        
    }
    while(recv_data > 0)
    {
        char* start = strstr(recvline,"NAME\n");
        char* end = strstr(recvline,"</PRE>");
        if( start != NULL && end == NULL)
        {
            // start += 6;
            fwrite(start,sizeof(char),strlen(start),stdout);
        }
        else if(start == NULL && end != NULL)
        {
            int length = strlen(recvline) - strlen(end);
            fwrite(recvline,sizeof(char),length,stdout);
            break;
        }
        else if(start != NULL && end != NULL)
        {
            // start += 6;
            int length = strlen(start) - strlen(end);
            fwrite(start,sizeof(char),length,stdout); 
            break;        
        }
        else
        {
            fwrite(recvline,sizeof(char),strlen(recvline),stdout);
        }
        recv_data= recv(sfd,recvline,1024,0);
    }
    printf("\n");
    close(sfd);
    freeaddrinfo(result);
    free(host);
    free(address);
    free(recvline);
    return 1;
}
