#include "libraries.h"

#define SOCKET_NAME "/tmp/S0cketGamePlAy4Life.socket"
#define LINUX_DATA 100

int create_server()
{
    int err;
    const char *ip = "10.0.2.15";
    int server_socket, client_socket;
    int linux_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_un name;
    int portNum = 51000;
    int size_of_clients = 10;
    const char* msg="Hello client";
    socklen_t size;
    int pid;
    char linux_data[100];
    char socket_data[100];
    //char *my_argv[2] = {"./Game/init_game", NULL};
    /* Создание межпроцессорного взаимодействия */
    printf("PreSocket\n");
    linux_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (linux_socket == -1)

    {
        printf("Execv error\n");
        return -1;
    }
    memset(&name, 0, sizeof(struct sockaddr_un));

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path)-1);

    /*pid = fork();
    if (pid < 0)
    {
        printf("Error forking\n");
        return -1;
    }
    else if (pid == 0)
    {

        err = execve(my_argv[0], (char**)my_argv, NULL);
        if (err == -1)
        {
            printf("Exec error\n");
            return -1;
        }
        exit(EXIT_SUCCESS);
    }*/

    printf("PreLinuxSocket\n");
    while (connect(linux_socket, (const struct sockaddr*)&name, sizeof(struct sockaddr_un)) == -1);

    memset(&linux_data, 0, LINUX_DATA);
    /*while (1==1)
    {
        memset(&linux_data, 0, LINUX_DATA);

        err = read(linux_socket, linux_data, LINUX_DATA);
        if (err == -1)
        {
            printf("Connect error\n");
            return -1;
        }
        linux_data[LINUX_DATA-1] = 0;

        if (strcmp(linux_data, "exit") == 0)
            break;
        printf("Data from another process: %s", linux_data);
    }
    */

    //Клиент-сервер сокет
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0)
    {
        printf("Error establishing connection\n");
        exit(1);
    }

    printf("Server Socket connection created...\n");

    server_addr.sin_family = AF_INET;

    server_addr.sin_addr.s_addr = inet_addr(ip);

    server_addr.sin_port = htons(portNum);
    size = sizeof(server_addr);
    bind(server_socket, (struct sockaddr*)&server_addr, size);

    listen(server_socket, size_of_clients);

    client_socket = accept(server_socket, (struct sockaddr*)&server_addr, &size);
    printf("Accepting sockets\n");
    int c;
    while (1 == 1)
    {
        printf("Recv socket\n");
        recv(client_socket, socket_data, LINUX_DATA, 0);
        printf("buffer1: %s\n", socket_data);
        if (strcmp(socket_data, "0") == 0)
            break;
        //send(linux_socket, socket_data, strlen(socket_data), 0);
        socket_data[LINUX_DATA-1] = 0;
        //getchar();
        err = send(linux_socket, socket_data, strlen(socket_data), 0);
        //err = write(linux_socket, socket_data, sizeof(char));
        if (err == -1)
        {
            printf("Connect error\n");
            return -1;
        }
        //linux_data[LINUX_DATA-1] = 0;

        //printf("Data from another process: %s", socket_data);
    }
    close(client_socket);
    close(linux_socket);

    //Ждем, пока все порожденные процессы авершат работу
   // while (wait(NULL) > 0);

    return 0;

}
