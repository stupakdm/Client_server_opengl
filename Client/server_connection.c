#include "libraries.h"

int connect_to_server(const char *address)
{
    int err;
    char *ip=NULL;
    char * ip1;
    char *port=NULL;
    char *port1;
    int portNum;
    int i,j, flag_port;
    int ip_length, port_length;
    struct sockaddr_in client_addr;
    int socketfd, size_socket;
    char buf[100];
    int buf_size = 100;

    printf("Address: %s\n", address);
    ip = (char*)malloc(sizeof(char));
    if (ip == NULL)
    {
        printf("Malloc error\n");
        return -1;
    }
    ip1 = ip;

    port = (char*)malloc(sizeof(char));
    if (port == NULL)
    {
        printf("Malloc error\n");
        return -1;
    }
    port1 = port;
    flag_port = 0;
    for (i=0;i<strlen(address);i++)
    {
        if (address[i] == ':')
        {
            flag_port = 1;
            ip[i] = '\0';
            ip_length = i;
            continue;
        }
        if (flag_port == 0)
        {
            ip[i] = address[i];
            if (address[i+1] != '\0' && address[i+1] != ':')
            {
                ip1 = (char*)realloc(ip, sizeof(char)*(i+2));
                if (ip1 == NULL)
                {
                    printf("Realloc error\n");
                    return -1;
                }
                ip = ip1;
            }
        }
        else
        {
            port[j] = address[i];
            if (address[i+1] != '\0' && address[i+1] != ':')
            {
                port1 = (char*)realloc(port, sizeof(char)*(j+2));
                if (port1 == NULL)
                {
                    printf("Realloc error\n");
                    return -1;
                }
                port = port1;
            }
            j++;
        }

    }

    printf("IP-address: %s\n", ip);
    printf("Port: %s\n", port);

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        printf("Execv error\n");
        return -1;
    }
    client_addr.sin_addr.s_addr = inet_addr(ip);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(atoi(port));

    free(ip);
    free(port);

    size_socket = sizeof(client_addr);
    printf("PreConnecting..\n");
    if (connect(socketfd, (struct sockaddr*)&client_addr, size_socket) == 0) {
        printf("Connecting to server...\n");
    }
    bzero(buf, buf_size);
    printf("AfterConnecting..\n");
    while (1 == 1)
    {
        printf("write number(1- red)(2-green)(0-exit): ");
        scanf("%s", buf);
        //buf[buf_size-1] = 0;
        printf("buf %s\n", buf);
        send(socketfd, buf, buf_size, 0);
        if (strcmp(buf, "0") == 0)
          break;
    }
    //recv(socketfd, buf, buf_size, 0);

    //printf("%s\n", buf);

    close(socketfd);

    return 0;


}
