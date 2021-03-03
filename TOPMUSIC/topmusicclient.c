#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include<fcntl.h>

extern int errno;


int port;


/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main(int argc, char* argv[])
{
	int sd;			// descriptorul de socket
	struct sockaddr_in server;	// structura folosita pentru conectare 
	char clientmsg[100];
	char servermsg[5000];

	/* exista toate argumentele in linia de comanda? */
	if (argc != 3)
	{
		printf("[client] Sintaxa: %s <adresa_server> <port>\n", argv[0]);
		return -1;
	}

	/* stabilim portul */
	port = atoi(argv[2]);

	/* cream socketul */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("[client] Eroare la socket().\n");
		return errno;
	}


	/* umplem structura folosita pentru realizarea conexiunii cu serverul */
	/* familia socket-ului */
	server.sin_family = AF_INET;
	/* adresa IP a serverului */
	server.sin_addr.s_addr = inet_addr(argv[1]);
	/* portul de conectare */
	server.sin_port = htons(port);

	/* ne conectam la server */
	if (connect(sd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("[client]Eroare la connect().\n");
		return errno;
	}
	int ok = 1;
	int pid = fork();
	if (pid > 0)
	{
		while (ok)
		{
			bzero(servermsg, 5000); //citire si afisare de la server
			read(sd, servermsg, 5000);
			printf("%s\n", servermsg);
		}
	}
	else if (pid == 0) //copil
	{
		while (ok)
		{
			bzero(clientmsg, 100);
			read(0, clientmsg, 100); //citire de la tastatura

			/* trimiterea mesajului la server */
			write(sd, clientmsg, strlen(clientmsg)); //transmite catre server
			

		}
	}



	/* inchidem conexiunea, am terminat */
	close(sd);
}


