#include "stdio.h"
#include "stdlib.h"

int main(int argc, char *argv[])
{
    int err, create;
    const char *ip = "127.0.0.1";
    int server_socket, client_socket;
    int count_koma=0;
    int count_port=-1;
    int i;
    //struct sockaddr_in servAddr;
    if (argc < 2)
    {
        printf("Not enough args\n");
        return -1;
    }
    if (argc == 2)
    {
        if (strcmp(argv[1], "create") == 0)
        {
            err = create_server();
            if (err == -1)
            {
                printf("Error during creating server\n");
                return -1;
            }
            else
            {
                printf("Goodbye\n");
                return 0;
            }
        }
        else if (strcmp(argv[1], "connect") == 0)
        {
            printf("Not enough args for connect\n");
            return -1;
        }
        else {
            printf("Not right args\n");
            return -1;
        }
    }
    if (argc == 3)
    {
        if (strcmp(argv[1], "connect") == 0)
        {
            for (i=0;i<strlen(argv[2]);i++)
            {
                if (argv[2][i] == '.')
                    count_koma++;
                if (argv[2][i] == ':')
                    count_port = 1;
            }
            if ((count_koma == 3) && (count_port == 1))
            {
                err = connect_to_server(argv[2]);
                if (err == -1)
                {
                    printf("Error during connecting to server\n");
                    return -1;
                }
                else
                {
                    printf("Goodbye\n");
                    return 0;
                }
            }
        }
    }
    return 0;
    //server_socket = socket(AF_INET, SOCK_STREAM, 0);  // Создаем сервер с надежным передачами TCP/IP( Для UDP используем SOCK_DGRAM)





}
