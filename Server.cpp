/*************************************************************************
	> File Name: Server.cpp
	> Created Time: Sat 09 Jan 2021 05:40:45 PM CST
 ************************************************************************/

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define SOCKET_PORT    4000
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024


int main(int argc, char **argv)
{
	std::cout << "The program has started" << std::endl;
	
	struct sockaddr_in server_addr;
	int server_socket;
	int opt = 1;

    bzero(&server_addr,sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(SOCKET_PORT);

	/* create a socket */
    server_socket = socket(PF_INET,SOCK_STREAM,0);
    if( server_socket < 0)
    {
        printf("Create Socket Failed!");
        exit(1);
    }

    /* bind socket to a specified address*/
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", SOCKET_PORT);
        exit(1);
    }

    /* listen a socket */
    if(listen(server_socket, LENGTH_OF_LISTEN_QUEUE))
    {
        printf("Server Listen Failed!");
        exit(1);
    }

	/* run server */
    while (1)
    {
        struct sockaddr_in client_addr;
		int client_socket;
        socklen_t length;
		char buffer[BUFFER_SIZE];

		/* accept socket from client */
		length = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);
        if( client_socket < 0)
        {
            printf("Server Accept Failed!\n");
            break;
        }

		/* receive data from client */
		while(1)
		{
			bzero(buffer, BUFFER_SIZE);
			length = recv(client_socket, buffer, BUFFER_SIZE, 0);
			if (length < 0)
			{
				printf("Server Recieve Data Failed!\n");
				break;
			}

			if('q' == buffer[0])
			{
				printf("Quit from client!\n");
				break;
			}

			printf("%s\n", buffer);
		}

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
