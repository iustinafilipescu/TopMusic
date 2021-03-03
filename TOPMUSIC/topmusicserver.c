
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>

/* portul folosit */
#define PORT 2024



/* codul de eroare returnat de anumite apeluri */
extern int errno;



int login(int client)
{
	char raspuns[100] = "introduceti numele de utilizator:\n";
	if (write(client, raspuns, strlen(raspuns)) <= 0)
	{
		perror("[server]Eroare la write() catre client.\n");
	}

	char user[100];
	bzero(user, 100);
	printf("[server]Asteptam mesajul...\n");
	fflush(stdout);

	/* citirea userului trimis de client */
	if (read(client, user, 100) <= 0)
	{
		perror("[server]Eroare la read() de la client.\n");
	}
	user[strlen(user) - 1] = '\0';

	FILE* f = fopen("usernames.txt", "r");
	int wordExist = 0;
	int bufferLength = 255;
	char line[bufferLength];

	int count = 0; //daca clientul se autentifica cu iustina care e primul nume din fisier inseamna ca e admin
	int parola = 0; //pt verificare parola rand user=rand parola
	while (fgets(line, bufferLength, f))
	{
		++count;
		++parola;
		line[strlen(line) - 1] = '\0';


		int ptr = strncmp(line, user, strlen(line) - 1);

		if (ptr == 0) //s-a gasit userul e ok
		{
			wordExist = 1;
			break;
		}
	}

	fclose(f);

	if (wordExist == 1)
	{
		char raspuns[100] = "introduceti parola:\n";
		if (write(client, raspuns, strlen(raspuns)) <= 0)
		{
			perror("[server]Eroare la write() catre client.\n");
		}

		char parola1[100];
		bzero(parola1, 100);
		printf("[server]Asteptam mesajul...\n");
		fflush(stdout);

		/* citirea userului trimis de client */
		if (read(client, parola1, 100) <= 0)
		{
			perror("[server]Eroare la read() de la client.\n");
		}
		parola1[strlen(parola1) - 1] = '\0';

		FILE* f = fopen("passwords.txt", "r");
		int wordExist1 = 0;
		int bufferLength1 = 255;
		char line1[bufferLength];

		int count1 = 0;

		while (fgets(line1, bufferLength1, f))
		{
			++count1;
			line1[strlen(line1) - 1] = '\0';


			int ptr1 = strncmp(line1, parola1, strlen(line1) - 1);

			if (ptr1 == 0) //s-a gasit parola e ok
			{
				wordExist1 = 1;
				break;
			}
		}
		if (wordExist1 == 0)
			count1 = -1;
	

		if (parola == count1)
		{

			char raspuns[100] = "logare cu succes\n";

			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			if (count == 1) //count e 1 doar daca s-a folosit iustina pt logare
				return 1; //cont=1 - admin

			
			FILE* f2 = fopen("restrictionati.txt", "r");
			bufferLength = 255;
			char line2[bufferLength];

		
			while (fgets(line2, bufferLength, f2))
			{
				
				line2[strlen(line2) - 1] = '\0';


				int ptr = strncmp(line2, user, strlen(line2) - 1);

				if (ptr == 0) //s-a gasit userul 
				{
					fclose(f2);
					return 3; //userul este restrictionat
				}
			}
			fclose(f2);
			
			return 0; //cont=0 - utiliz
		}
		else //de la parola - parola introdusa nu este corecta
		{
			char raspuns[100] = "datele de autentificare sunt gresite\n";
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}
			return -1;
		}
	}
	else //de la username
	{
	char raspuns[100] = "datele de autentificare sunt gresite\n";
	if (write(client, raspuns, strlen(raspuns)) <= 0)
	{
		perror("[server]Eroare la write() catre client.\n");
	}
	return -1;
	}

 }


 int aparitie(char user[100]) //pentru functia de inregistrare
 {
	 FILE* f = fopen("usernames.txt", "r");
	 int wordExist = 0;
	 int bufferLength = 255;
	 char line[bufferLength];

	 while (fgets(line, bufferLength, f))
	 {
		 line[strlen(line) - 1] = '\0';

		 int ptr = strncmp(line, user, strlen(line) - 1);

		 if (ptr == 0)
		 {
			 fclose(f);
			 return 1; //s-a gasit userul deci userul nu poate fi folosit pt inregistrare
		 }
	 }
	 return 0;
 }

 int inregistrare(int client)
 {
	 char raspuns[100] = "introduceti numele de utilizator dorit:\n";
	 if (write(client, raspuns, strlen(raspuns)) <= 0)
	 {
		 perror("[server]Eroare la write() catre client.\n");
	 }

	 char user[100];
	 bzero(user, 100);
	 printf("[server]Asteptam mesajul...\n");
	 fflush(stdout);

	 /* citirea mesajului */
	 if (read(client, user, 100) <= 0)
	 {
		 perror("[server]Eroare la read() de la client.\n");
	 }
	 user[strlen(user) - 1] = '\0';

	 int ok;
	 while (ok = aparitie(user) == 1) //verificam daca nu cumva userul deja exista
	 {


		 char raspuns[100] = "acest nume de utilizator exista deja. va rugam introduceti alt username\n";
		 if (write(client, raspuns, strlen(raspuns)) <= 0)
		 {
			 perror("[server]Eroare la write() catre client.\n");

		 }

		 bzero(user, 100);

		 if (read(client, user, 100) <= 0) //citim din nou userul trimis de client
		 {
			 perror("[server]Eroare la read() de la client.\n");
		 }
		 user[strlen(user) - 1] = '\0';

	 }

	 if (ok == 0)
	 {//punem userul in fisier
		 FILE* f = fopen("usernames.txt", "a");
		 if (f)
		 {
			 fputs(user, f);
			 fputs("\n", f);

		 }
		 else
		 {
			 printf("Failed to open the file\n");
		 }
		 fclose(f);

		 bzero(raspuns, 100);
		 strcpy(raspuns, "introduceti parola dorita:\n");
		 if (write(client, raspuns, strlen(raspuns)) <= 0)
		 {
			 perror("[server]Eroare la write() catre client.\n");
		 }

		 char pass[100];
		 bzero(pass, 100);
		 printf("[server]Asteptam mesajul...\n");
		 fflush(stdout);

		 /* citirea mesajului */
		 if (read(client, pass, 100) <= 0)
		 {
			 perror("[server]Eroare la read() de la client.\n");
		 }
		 pass[strlen(pass) - 1] = '\0';

		 FILE* f1 = fopen("passwords.txt", "a");
		 if (f1)
		 {
			 fputs(pass, f1);
			 fputs("\n", f1);

		 }
		 else
		 {
			 printf("Failed to open the file\n");
		 }
		 fclose(f1);




		 char raspuns[100] = "v-ati inregistrat cu succes iar logarea s-a facut automat\n";
		 if (write(client, raspuns, strlen(raspuns)) <= 0)
		 {
			 perror("[server]Eroare la write() catre client.\n");

		 }
		 return 0; //returnam 0 -> cont=0 pt ca am logat automat clientul cu noul username
	 }


 }


	int adaugaMelodie(int client)
	{
		char raspuns[1000] = "introduceti numarul genului muzical:\n 1.pop\n 2.rock\n 3.RnB\n 4.dance\n 5.Hip-Hop\n";
		if (write(client, raspuns, strlen(raspuns)) <= 0)
		{
			perror("[server]Eroare la write() catre client.\n");
		}

		char top[100];
		bzero(top, 100);
		printf("[server]Asteptam mesajul...\n");
		fflush(stdout);

		/* citirea mesajului */
		if (read(client, top, 100) <= 0)
		{
			perror("[server]Eroare la read() de la client.\n");
		}
		top[strlen(top) - 1] = '\0';


		if (strcmp(top, "1") == 0)
		{
			FILE* f = fopen("pop.txt", "a");
			FILE* g = fopen("general.txt", "a");

			char informatii[1000]; 
			strcpy(informatii, "0");
			strcat(informatii, " Artist:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);

			char verificare[100];
			bzero(verificare, 100);
			strcat(verificare, "Artist:");
			strcat(verificare, artist);
			strcat(verificare, "-Piesa:");
			strcat(verificare, piesa);
			printf("%s", verificare);

			FILE* file = fopen("general.txt", "r"); //verificare daca exista deja piesa in fisierul general
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, file))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, verificare) != NULL) //daca exista o linie in general cu artistul si piesa
				{
					ok = 1;
					bzero(raspuns, 1000);
					strcpy(raspuns, "Piesa deja exista\n");
					if (write(client, raspuns, strlen(raspuns)) <= 0)
					{
						perror("[server]Eroare la write() catre client.\n");
					}


				}
				if (ok == 1)
				{
					fclose(file);
					return 0;
				}

			}
			fclose(file);

			//continuam cu introducerea datelor

			strcat(informatii, "-Link:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti linkul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char link[100];
			bzero(link, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, link, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			link[strlen(link) - 1] = '\0';


			strcat(informatii, link);
			strcat(informatii, "-Descriere:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti descrierea:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char descriere[100];
			bzero(descriere, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, descriere, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			descriere[strlen(descriere) - 1] = '\0';


			strcat(informatii, descriere);


			if (f)
			{
				fputs(informatii, f); //punem in fisierul genului muzical
				fputs("\n", f);

			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(f);
			if (g)
			{
				fputs(informatii, g); //punem in fisierul cu topul general
				fputs("\n", g);

			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(g);



		}
		else if (strcmp(top, "2") == 0)
		{
			FILE* f = fopen("rock.txt", "a");
			FILE* g = fopen("general.txt", "a");

			char informatii[1000];
			strcpy(informatii, "0");
			strcat(informatii, " Artist:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);

			char verificare[100];
			bzero(verificare, 100);
			strcat(verificare, "Artist:");
			strcat(verificare, artist);
			strcat(verificare, "-Piesa:");
			strcat(verificare, piesa);
			printf("%s", verificare);

			FILE* file = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, file))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, verificare) != NULL)
				{
					ok = 1;
					bzero(raspuns, 1000);
					strcpy(raspuns, "Piesa deja exista\n");
					if (write(client, raspuns, strlen(raspuns)) <= 0)
					{
						perror("[server]Eroare la write() catre client.\n");
					}


				}
				if (ok == 1)
				{
					fclose(file);
					return 0;
				}

			}
			fclose(file);

			strcat(informatii, "-Link:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti linkul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char link[100];
			bzero(link, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, link, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			link[strlen(link) - 1] = '\0';


			strcat(informatii, link);
			strcat(informatii, "-Descriere:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti descrierea:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char descriere[100];
			bzero(descriere, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, descriere, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			descriere[strlen(descriere) - 1] = '\0';


			strcat(informatii, descriere);

			if (f)
			{
				fputs(informatii, f);
				fputs("\n", f);

			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(f);
			if (g)
			{
				fputs(informatii, g);
				fputs("\n", g);

			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(g);
		}
		else if (strcmp(top, "3") == 0)
		{
			FILE* f = fopen("RnB.txt", "a");
			FILE* g = fopen("general.txt", "a");

			char informatii[1000];
			strcpy(informatii, "0");
			strcat(informatii, " Artist:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);

			char verificare[100];
			bzero(verificare, 100);
			strcat(verificare, "Artist:");
			strcat(verificare, artist);
			strcat(verificare, "-Piesa:");
			strcat(verificare, piesa);
			printf("%s", verificare);

			FILE* file = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, file))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, verificare) != NULL)
				{
					ok = 1;
					bzero(raspuns, 1000);
					strcpy(raspuns, "Piesa deja exista\n");
					if (write(client, raspuns, strlen(raspuns)) <= 0)
					{
						perror("[server]Eroare la write() catre client.\n");
					}


				}
				if (ok == 1)
				{
					fclose(file);
					return 0;
				}

			}
			fclose(file);

			strcat(informatii, "-Link:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti linkul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char link[100];
			bzero(link, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, link, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			link[strlen(link) - 1] = '\0';


			strcat(informatii, link);
			strcat(informatii, "-Descriere:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti descriere:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char descriere[100];
			bzero(descriere, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, descriere, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			descriere[strlen(descriere) - 1] = '\0';


			strcat(informatii, descriere);

			if (f)
			{
				fputs(informatii, f);
				fputs("\n", f);

			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(f);
			if (g)
			{
				fputs(informatii, g);
				fputs("\n", g);

			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(g);
		}
		else if (strcmp(top, "4") == 0)
		{
			FILE* f = fopen("dance.txt", "a");
			FILE* g = fopen("general.txt", "a");

			char informatii[1000];
			strcpy(informatii, "0");
			strcat(informatii, " Artist:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);

			char verificare[100];
			bzero(verificare, 100);
			strcat(verificare, "Artist:");
			strcat(verificare, artist);
			strcat(verificare, "-Piesa:");
			strcat(verificare, piesa);
			printf("%s", verificare);

			FILE* file = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, file))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, verificare) != NULL)
				{
					ok = 1;
					bzero(raspuns, 1000);
					strcpy(raspuns, "Piesa deja exista\n");
					if (write(client, raspuns, strlen(raspuns)) <= 0)
					{
						perror("[server]Eroare la write() catre client.\n");
					}


				}
				if (ok == 1)
				{
					fclose(file);
					return 0;
				}

			}
			fclose(file);


			strcat(informatii, "-Link:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti linkul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char link[100];
			bzero(link, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, link, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			link[strlen(link) - 1] = '\0';


			strcat(informatii, link);
			strcat(informatii, "-Descriere:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti descriere:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char descriere[100];
			bzero(descriere, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, descriere, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			descriere[strlen(descriere) - 1] = '\0';


			strcat(informatii, descriere);

			if (f)
			{
				fputs(informatii, f);
				fputs("\n", f);

			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(f);
			if (g)
			{
				fputs(informatii, g);
				fputs("\n", g);

			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(g);
		}
		else if (strcmp(top, "5") == 0)
		{
			FILE* f = fopen("Hip-Hop.txt", "a");
			FILE* g = fopen("general.txt", "a");

			char informatii[1000];
			strcpy(informatii, "0");
			strcat(informatii, " Artist:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);

			char verificare[100];
			bzero(verificare, 100);
			strcat(verificare, "Artist:");
			strcat(verificare, artist);
			strcat(verificare, "-Piesa:");
			strcat(verificare, piesa);
			printf("%s", verificare);

			FILE* file = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, file))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, verificare) != NULL)
				{
					ok = 1;
					bzero(raspuns, 1000);
					strcpy(raspuns, "Piesa deja exista\n");
					if (write(client, raspuns, strlen(raspuns)) <= 0)
					{
						perror("[server]Eroare la write() catre client.\n");
					}


				}
				if (ok == 1)
				{
					fclose(file);
					return 0;
				}

			}
			fclose(file);

			strcat(informatii, "-Link:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti linkul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char link[100];
			bzero(link, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, link, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			link[strlen(link) - 1] = '\0';


			strcat(informatii, link);
			strcat(informatii, "-Descriere:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti descrierea:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char descriere[100];
			bzero(descriere, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, descriere, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			descriere[strlen(descriere) - 1] = '\0';


			strcat(informatii, descriere);

			if (f)
			{
				fputs(informatii, f);
				fputs("\n", f);

			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(f);
			if (g)
			{
				fputs(informatii, g);
				fputs("\n", g);

			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(g);
		}
		return 1;
	}

	void sortare(char fisier[100]) //pentru afisarea topului 
	{
		FILE* file;
		file = fopen(fisier, "r");

		char array[1000][1000];

		int i = 0;
		while (fgets(array[i], sizeof(array[i]), file)) //punem in matrice fisierul 
		{
			array[i][strlen(array[i]) - 1] = '\0';
			i++;
		}


		int k = 0;
		char vot[100][100];

		int l = i;
		int j = 0;

		for (int i = 0; i < l; i++) //punem in matricea vot nr de voturi al fiecarei piese (adica nr din fata fiecarei linii pt  ca o linie = o piesa)
		{
			k = 0;

			
				while (array[i][k] != ' ')
				{
					vot[j][k] = array[i][k]; //fiecare rand din matrice va reprezenta nr de voturi pt piesa de pe randul corespunzator
					k++;
				}
			
			
			vot[j][k] = '\0';
			j++;

		}


		file = fopen(fisier, "w");

		char aux[500];
		char aux1[5];

		for (int i = 0; i < l - 1; i++) //daca vot[i]<vot[j] inseamna ca trebuie sa schimbam linia i cu linia j din matricea care reprez fisierul
		{

			for (int j = i + 1; j < l; j++)
			{

				if (atoi(vot[i]) < atoi(vot[j]))
				{
					strcpy(aux, array[i]);
					strcpy(array[i], array[j]);
					strcpy(array[j], aux);
					
					strcpy(aux1, vot[i]);
					strcpy(vot[i], vot[j]);
					strcpy(vot[j], aux1);

				}
				bzero(aux, 500);
				bzero(aux1, 5);
			}
		}

		file = fopen(fisier, "a"); // punem matricea modificata in fisier
		for (i = 0; i < l; i++)
		{
			
			fputs(array[i], file);
			fputc('\n', file);
		}

		fclose(file);
	}

	void vizualizeazaTop(int client)
	{
		char raspuns[1000] = "introduceti numarul topul pe care il doriti:\n 1.pop\n 2.rock\n 3.dance\n 4.RnB\n 5.Hip-Hop\n 6.Topul general\n ";
		if (write(client, raspuns, strlen(raspuns)) <= 0)
		{
			perror("[server]Eroare la write() catre client.\n");
		}

		char top[100];
		bzero(top, 100);
		printf("[server]Asteptam mesajul...\n");
		fflush(stdout);

		/* citirea mesajului */
		if (read(client, top, 100) <= 0)
		{
			perror("[server]Eroare la read() de la client.\n");
		}
		top[strlen(top) - 1] = '\0';


		if (strcmp(top, "1") == 0)
		{
			FILE* file = fopen("pop.txt", "r");

			sortare("pop.txt");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Topul pop este:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), file))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}

			fclose(file);

		}
		else if (strcmp(top, "2") == 0)
		{
			FILE* file = fopen("rock.txt", "r");

			sortare("rock.txt");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Topul rock este:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), file))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}

			fclose(file);

		}

		else if (strcmp(top, "3") == 0)
		{
			FILE* file = fopen("dance.txt", "r");

			sortare("dance.txt");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Topul dance este:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), file))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}

			fclose(file);

		}

		else if (strcmp(top, "4") == 0)
		{
			FILE* file = fopen("RnB.txt", "r");

			sortare("RnB.txt");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Topul RnB este:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), file))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}

			fclose(file);

		}

		else if (strcmp(top, "5") == 0)
		{
			FILE* file = fopen("Hip-Hop.txt", "r");

			sortare("Hip-Hop.txt");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Topul Hip-Hop este:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), file))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}

			fclose(file);

		}

		else if (strcmp(top, "6") == 0)
		{
			FILE* file = fopen("general.txt", "r");

			sortare("general.txt");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Topul general este:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char topul[20000];
			bzero(topul, 20000);
			while (fgets(topul, sizeof(topul), file))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 20000);
			}

			fclose(file);

		}


	}

	int adaugaComentariu(int client)
	{
		FILE* file;
		file = fopen("general.txt", "r");

		if (file == NULL)
		{
			printf("Cannot open file \n");
			exit(0);
		}

		char nume[1000];
		bzero(nume, 1000);
		while (fgets(nume, sizeof(nume), file))
		{

			if (write(client, nume, strlen(nume)) <= 0)
			{
				perror("[server]Eroare la write() catre client din afiseazaTop.\n");
			}
			bzero(nume, 1000);
		}

		fclose(file);


		char informatii[1000];
		bzero(informatii, 1000);

		strcat(informatii, "Artist:");

		char raspuns[1000];
		bzero(raspuns, 1000);
		strcpy(raspuns, "Introduceti artistul:\n");
		if (write(client, raspuns, strlen(raspuns)) <= 0)
		{
			perror("[server]Eroare la write() catre client.\n");
		}

		char artist[100];
		bzero(artist, 100);
		printf("[server]Asteptam mesajul...\n");
		fflush(stdout);

		/* citirea mesajului */
		if (read(client, artist, 100) <= 0)
		{
			perror("[server]Eroare la read() de la client.\n");
		}
		artist[strlen(artist) - 1] = '\0';


		strcat(informatii, artist);
		strcat(informatii, "-Piesa:");

		bzero(raspuns, 1000);
		strcpy(raspuns, "Introduceti piesa:\n");
		if (write(client, raspuns, strlen(raspuns)) <= 0)
		{
			perror("[server]Eroare la write() catre client.\n");
		}

		char piesa[100];
		bzero(piesa, 100);
		printf("[server]Asteptam mesajul...\n");
		fflush(stdout);

		/* citirea mesajului */
		if (read(client, piesa, 100) <= 0)
		{
			perror("[server]Eroare la read() de la client.\n");
		}
		piesa[strlen(piesa) - 1] = '\0';

		strcat(informatii, piesa);
		printf("%s\n", informatii);
		FILE* g = fopen("general.txt", "r"); //verificam daca exista piesa in general
		int wordExist = 0;
		int bufferLength = 255;
		char line[bufferLength];

		int ok = 0;
		while (fgets(line, bufferLength, g))
		{
			line[strlen(line) - 1] = '\0';


			if (strstr(line, informatii) != NULL)
			{
				ok = 1;

			}

		}

		if (ok == 0)
		{
			bzero(raspuns, 1000);
			strcpy(raspuns, "Piesa nu exista\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}
			fclose(g);
			return 0;
		}
		fclose(g);

		bzero(raspuns, 1000);
		strcpy(raspuns, "Introduceti comentariul:\n");
		if (write(client, raspuns, strlen(raspuns)) <= 0)
		{
			perror("[server]Eroare la write() catre client.\n");
		}

		char comm[100];
		bzero(comm, 100);
		printf("[server]Asteptam mesajul...\n");
		fflush(stdout);

		/* citirea mesajului */
		if (read(client, comm, 100) <= 0)
		{
			perror("[server]Eroare la read() de la client.\n");
		}
		comm[strlen(comm) - 1] = '\0';

		strcat(informatii, "-Comentarii:");
		strcat(informatii, comm);
		//strcat(informatii, " ~ ");

		FILE* com = fopen("comentarii.txt", "a");

		if (com)
		{
			fputs(informatii, com);
			fputs("\n", com);

		}
		else
		{
			printf("Failed to open the file\n");
		}
		fclose(com);


		return 0;

	}

	void vizualizeazaComm(int client)
	{


		FILE* file;
		file = fopen("comentarii.txt", "r");

		char array[1000][1000];

		int i = 0;
		while (fgets(array[i], sizeof(array[i]), file)) //punem in matrice fisierul 
		{
			array[i][strlen(array[i]) - 1] = '\0';
			i++;
		}


		char aux[500];
		int l = i;
		for (int i = 0; i < l - 1; i++)
		{
			for (int j = i + 1; j < l; j++)
			{
				if (array[i][7] > array[j][7]) //ordonam comentariile pentru a avea toate commurile de la o piesa unul dupa celalalt
				{
					strcpy(aux, array[i]);
					strcpy(array[i], array[j]);
					strcpy(array[j], aux);
				}
				bzero(aux, 500);
			}


		}

		file = fopen("comentarii.txt", "w");
		for (i = 0; i < l; i++)
		{
		
			fputs(array[i], file);
			fputc('\n', file);
		}

		fclose(file);


		FILE* f = fopen("comentarii.txt", "r");

		char comms[1000];
		bzero(comms, 1000);
		while (fgets(comms, sizeof(comms), f))
		{

			if (write(client, comms, strlen(comms)) <= 0)
			{
				perror("[server]Eroare la write() catre client din afiseazaTop.\n");
			}
			bzero(comms, 1000);
		}
		fclose(f);
	}

	int voteaza(int client)
	{

		char raspuns[1000] = "din motive de securitate, pentru a putea vota este nevoie sa va reintroduceti datele de autentificare\n"; 
		if (write(client, raspuns, strlen(raspuns)) <= 0)
		{
			perror("[server]Eroare la write() catre client.\n");
		}
		bzero(raspuns, 1000);

		int restrictionat;
		restrictionat=login(client); //pentru a putea vedea daca e restrictionat
		if (restrictionat == -1) //date incorecte
			return 0;
		if (restrictionat == 3)
		{
			strcpy(raspuns, "sunteti restrictionat, nu puteti vota ");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}
			bzero(raspuns, 1000);
			return 0;

		}




		strcpy(raspuns, "introduceti numarul genului muzical melodiei pe care vreti sa o votati:\n 1.pop\n 2.rock\n 3.dance\n 4.RnB\n 5.Hip-Hop\n ");
		if (write(client, raspuns, strlen(raspuns)) <= 0)
		{
			perror("[server]Eroare la write() catre client.\n");
		}
		bzero(raspuns, 1000);

		char top[100];
		bzero(top, 100);
		printf("[server]Asteptam mesajul...\n");
		fflush(stdout);

		/* citirea mesajului */
		if (read(client, top, 100) <= 0)
		{
			perror("[server]Eroare la read() de la client.\n");
		}

		top[strlen(top) - 1] = '\0';


		if (strcmp(top, "1") == 0)
		{
			FILE* f = fopen("pop.txt", "r");

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), f))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}
			fclose(f);


			char informatii[1000];
			bzero(informatii, 1000);

			strcat(informatii, "Artist:");

			char raspuns[1000];
			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);
			printf("%s\n", informatii);
			FILE* g = fopen("general.txt", "r"); //verificam daca exista piesa
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, g))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, informatii) != NULL)
				{
					ok = 1;

				}

			}

			if (ok == 0)
			{
				bzero(raspuns, 1000);
				strcpy(raspuns, "Piesa nu exista\n");
				if (write(client, raspuns, strlen(raspuns)) <= 0)
				{
					perror("[server]Eroare la write() catre client.\n");
				}
				fclose(g);
				return 0;
			}
			fclose(g);


			char array[1000][1000];
			FILE* general = fopen("general.txt", "r");



			int i = 0;
			while (fgets(array[i], sizeof(array[i]), general)) //punem in matrice fisierul 
			{
				array[i][strlen(array[i]) - 1] = '\0';
				i++;

			}

			int l = i;

			char nrvot[5]; //punem in acest vector numarul de voturi pentru piesa aleasa
			bzero(nrvot, 5);
			int y;
			int k = 0;
			for (int i = 0; i < l; i++)
			{
				
				if (strstr(array[i], informatii) != NULL) //randul cu piesa aleasa
				{
					while (array[i][k] != 'A')
					{
						 y = i;
						nrvot[k] = array[i][k];
						k++;
					}
					nrvot[k] = '\0';

				}
			}
			//nr vot =[0,99]
			int x = atoi(nrvot); //transformam in int
			x = x + 1; //adaugam 1 la nr de voturi
			if (x % 10==0) // in urma operatiei de inversat se pierde 0 
				array[y][1] = '0';
			int rev;
			while (x != 0)
			{
				int remainder = x % 10;
				rev = rev * 10 + remainder;
				x /= 10;
			}

			for (int i = 0; i < k; i++) //actualizam randul cu nr de voturi
			{
				if (rev)
				{
					int c = rev % 10;
					array[y][i] = c + '0';
					rev = rev / 10;
				}
				else
				{
					break;
				}
			}
			




			fclose(general);
			FILE* fisier;
			fisier = fopen("general.txt", "w"); //asctualizam fisierul general cu matricea modificata
			for (i = 0; i < l; i++)
			{
			
				fputs(array[i], fisier);
				fputc('\n', fisier);
			}

			fclose(fisier);


			char array1[1000][1000];
			FILE* general1 = fopen("pop.txt", "r"); //procedam la fel si in fisierul pop



			i = 0;
			while (fgets(array1[i], sizeof(array1[i]), general1)) //punem in matrice fisierul 
			{
				array1[i][strlen(array1[i]) - 1] = '\0';
				i++;

			}

			l = i;



			
			bzero(nrvot, 5);
			
			 k = 0;
			for (int i = 0; i < l; i++)
			{

				if (strstr(array1[i], informatii) != NULL)
				{
					while (array1[i][k] != 'A')
					{
						y = i;
						nrvot[k] = array1[i][k];
						k++;
					}
					nrvot[k] = '\0';

				}
			}

			 x = atoi(nrvot);
			x = x + 1;
			if (x %10==0)
				array1[y][1] = '0';
			rev = 0;
			while (x != 0)
			{
				int remainder = x % 10;
				rev = rev * 10 + remainder;
				x /= 10;
			}
			printf("%d\n", rev);
			for (int i = 0; i < k; i++)
			{
				if (rev)
				{
					int c =rev % 10;
					array1[y][i] = c + '0';
					rev = rev / 10;
				}
				else
				{
					break;
				}
			}
				

			



			fclose(general1);
			FILE* fisier1;
			fisier1 = fopen("pop.txt", "w");
			for (i = 0; i < l; i++)
			{
				fputs(array1[i], fisier1);
				fputc('\n', fisier1);
			}

			fclose(fisier1);


		}


		if (strcmp(top, "2") == 0)
		{
			FILE* f = fopen("rock.txt", "r");

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), f))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}
			fclose(f);


			char informatii[1000];
			bzero(informatii, 1000);

			strcat(informatii, "Artist:");

			char raspuns[1000];
			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);
			printf("%s\n", informatii);
			FILE* g = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, g))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, informatii) != NULL)
				{
					ok = 1;

				}

			}

			if (ok == 0)
			{
				bzero(raspuns, 1000);
				strcpy(raspuns, "Piesa nu exista\n");
				if (write(client, raspuns, strlen(raspuns)) <= 0)
				{
					perror("[server]Eroare la write() catre client.\n");
				}
				fclose(g);
				return 0;
			}
			fclose(g);


			char array[1000][1000];
			FILE* general = fopen("general.txt", "r");



			int i = 0;
			while (fgets(array[i], sizeof(array[i]), general)) //punem in matrice fisierul 
			{
				array[i][strlen(array[i]) - 1] = '\0';
				i++;

			}

			int l = i;


			char nrvot[5];
			bzero(nrvot, 5);
			int y;
			int k = 0;
			for (int i = 0; i < l; i++)
			{

				if (strstr(array[i], informatii) != NULL)
				{
					while (array[i][k] != 'A')
					{
						y = i;
						nrvot[k] = array[i][k];
						k++;
					}
					nrvot[k] = '\0';

				}
			}

			int x = atoi(nrvot);
			x = x + 1;
			if (x % 10==0)
				array[y][1] = '0';
			int rev;
			while (x != 0)
			{
				int remainder = x % 10;
				rev = rev * 10 + remainder;
				x /= 10;
			}

			for (int i = 0; i < k; i++)
			{
				if (rev)
				{
					int c = rev % 10;
					array[y][i] = c + '0';
					rev = rev / 10;
				}
				else
				{
					break;
				}
			}


			fclose(general);
			FILE* fisier;
			fisier = fopen("general.txt", "w");
			for (i = 0; i < l; i++)
			{
				
				fputs(array[i], fisier);
				fputc('\n', fisier);
			}

			fclose(fisier);


			char array1[1000][1000];
			FILE* general1 = fopen("rock.txt", "r");



			i = 0;
			while (fgets(array1[i], sizeof(array1[i]), general1)) //punem in matrice fisierul 
			{
				array1[i][strlen(array1[i]) - 1] = '\0';
				i++;

			}

			l = i;


			bzero(nrvot, 5);

			k = 0;
			for (int i = 0; i < l; i++)
			{

				if (strstr(array1[i], informatii) != NULL)
				{
					while (array1[i][k] != 'A')
					{
						y = i;
						nrvot[k] = array1[i][k];
						k++;
					}
					nrvot[k] = '\0';

				}
			}

			x = atoi(nrvot);
			x = x + 1;
			if (x % 10==0)
				array1[y][1] = '0';
			rev = 0;
			while (x != 0)
			{
				int remainder = x % 10;
				rev = rev * 10 + remainder;
				x /= 10;
			}
			printf("%d\n", rev);
			for (int i = 0; i < k; i++)
			{
				if (rev)
				{
					int c = rev % 10;
					array1[y][i] = c + '0';
					rev = rev / 10;
				}
				else
				{
					break;
				}
			}



			fclose(general1);
			FILE* fisier1;
			fisier1 = fopen("rock.txt", "w");
			for (i = 0; i < l; i++)
			{
				
				fputs(array1[i], fisier1);
				fputc('\n', fisier1);
			}

			fclose(fisier1);


		}



		if (strcmp(top, "3") == 0)
		{
			FILE* f = fopen("dance.txt", "r");

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), f))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}
			fclose(f);


			char informatii[1000];
			bzero(informatii, 1000);

			strcat(informatii, "Artist:");

			char raspuns[1000];
			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);
			printf("%s\n", informatii);
			FILE* g = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, g))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, informatii) != NULL)
				{
					ok = 1;

				}

			}

			if (ok == 0)
			{
				bzero(raspuns, 1000);
				strcpy(raspuns, "Piesa nu exista\n");
				if (write(client, raspuns, strlen(raspuns)) <= 0)
				{
					perror("[server]Eroare la write() catre client.\n");
				}
				fclose(g);
				return 0;
			}
			fclose(g);


			char array[1000][1000];
			FILE* general = fopen("general.txt", "r");



			int i = 0;
			while (fgets(array[i], sizeof(array[i]), general)) //punem in matrice fisierul 
			{
				array[i][strlen(array[i]) - 1] = '\0';
				i++;

			}

			int l = i;


			char nrvot[5];
			bzero(nrvot, 5);
			int y;
			int k = 0;
			for (int i = 0; i < l; i++)
			{

				if (strstr(array[i], informatii) != NULL)
				{
					while (array[i][k] != 'A')
					{
						y = i;
						nrvot[k] = array[i][k];
						k++;
					}
					nrvot[k] = '\0';

				}
			}

			int x = atoi(nrvot);
			x = x + 1;
			if (x % 10==0)
				array[y][1] = '0';
			int rev;
			while (x != 0)
			{
				int remainder = x % 10;
				rev = rev * 10 + remainder;
				x /= 10;
			}

			for (int i = 0; i < k; i++)
			{
				if (rev)
				{
					int c = rev % 10;
					array[y][i] = c + '0';
					rev = rev / 10;
				}
				else
				{
					break;
				}
			}



			fclose(general);
			FILE* fisier;
			fisier = fopen("general.txt", "w");
			for (i = 0; i < l; i++)
			{
				
				fputs(array[i], fisier);
				fputc('\n', fisier);
			}

			fclose(fisier);


			char array1[1000][1000];
			FILE* general1 = fopen("dance.txt", "r");



			i = 0;
			while (fgets(array1[i], sizeof(array1[i]), general1)) //punem in matrice fisierul 
			{
				array1[i][strlen(array1[i]) - 1] = '\0';
				i++;

			}

			l = i;


			bzero(nrvot, 5);

			k = 0;
			for (int i = 0; i < l; i++)
			{

				if (strstr(array1[i], informatii) != NULL)
				{
					while (array1[i][k] != 'A')
					{
						y = i;
						nrvot[k] = array1[i][k];
						k++;
					}
					nrvot[k] = '\0';

				}
			}

			x = atoi(nrvot);
			x = x + 1;
			if (x % 10==0)
				array1[y][1] = '0';
			rev = 0;
			while (x != 0)
			{
				int remainder = x % 10;
				rev = rev * 10 + remainder;
				x /= 10;
			}
			printf("%d\n", rev);
			for (int i = 0; i < k; i++)
			{
				if (rev)
				{
					int c = rev % 10;
					array1[y][i] = c + '0';
					rev = rev / 10;
				}
				else
				{
					break;
				}
			}



			fclose(general1);
			FILE* fisier1;
			fisier1 = fopen("dance.txt", "w");
			for (i = 0; i < l; i++)
			{
		
				fputs(array1[i], fisier1);
				fputc('\n', fisier1);
			}

			fclose(fisier1);


		}


		if (strcmp(top, "4") == 0)
		{
			FILE* f = fopen("RnB.txt", "r");

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), f))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}
			fclose(f);


			char informatii[1000];
			bzero(informatii, 1000);

			strcat(informatii, "Artist:");

			char raspuns[1000];
			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);
			printf("%s\n", informatii);
			FILE* g = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, g))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, informatii) != NULL)
				{
					ok = 1;

				}

			}

			if (ok == 0)
			{
				bzero(raspuns, 1000);
				strcpy(raspuns, "Piesa nu exista\n");
				if (write(client, raspuns, strlen(raspuns)) <= 0)
				{
					perror("[server]Eroare la write() catre client.\n");
				}
				fclose(g);
				return 0;
			}
			fclose(g);


			char array[1000][1000];
			FILE* general = fopen("general.txt", "r");



			int i = 0;
			while (fgets(array[i], sizeof(array[i]), general)) //punem in matrice fisierul 
			{
				array[i][strlen(array[i]) - 1] = '\0';
				i++;

			}

			int l = i;


			char nrvot[5];
			bzero(nrvot, 5);
			int y;
			int k = 0;
			for (int i = 0; i < l; i++)
			{

				if (strstr(array[i], informatii) != NULL)
				{
					while (array[i][k] != 'A')
					{
						y = i;
						nrvot[k] = array[i][k];
						k++;
					}
					nrvot[k] = '\0';

				}
			}

			int x = atoi(nrvot);
			x = x + 1;
			if (x% 10==0)
				array[y][1] = '0';
			int rev;
			while (x != 0)
			{
				int remainder = x % 10;
				rev = rev * 10 + remainder;
				x /= 10;
			}

			for (int i = 0; i < k; i++)
			{
				if (rev)
				{
					int c = rev % 10;
					array[y][i] = c + '0';
					rev = rev / 10;
				}
				else
				{
					break;
				}
			}



			fclose(general);
			FILE* fisier;
			fisier = fopen("general.txt", "w");
			for (i = 0; i < l; i++)
			{
				
				fputs(array[i], fisier);
				fputc('\n', fisier);
			}

			fclose(fisier);


			char array1[1000][1000];
			FILE* general1 = fopen("RnB.txt", "r");



			i = 0;
			while (fgets(array1[i], sizeof(array1[i]), general1)) //punem in matrice fisierul 
			{
				array1[i][strlen(array1[i]) - 1] = '\0';
				i++;

			}

			l = i;


			bzero(nrvot, 5);

			k = 0;
			for (int i = 0; i < l; i++)
			{

				if (strstr(array1[i], informatii) != NULL)
				{
					while (array1[i][k] != 'A')
					{
						y = i;
						nrvot[k] = array1[i][k];
						k++;
					}
					nrvot[k] = '\0';

				}
			}

			x = atoi(nrvot);
			x = x + 1;
			if (x % 10==0)
				array1[y][1] = '0';
			rev = 0;
			while (x != 0)
			{
				int remainder = x % 10;
				rev = rev * 10 + remainder;
				x /= 10;
			}
			printf("%d\n", rev);
			for (int i = 0; i < k; i++)
			{
				if (rev)
				{
					int c = rev % 10;
					array1[y][i] = c + '0';
					rev = rev / 10;
				}
				else
				{
					break;
				}
			}



			fclose(general1);
			FILE* fisier1;
			fisier1 = fopen("RnB.txt", "w");
			for (i = 0; i < l; i++)
			{
				
				fputs(array1[i], fisier1);
				fputc('\n', fisier1);
			}

			fclose(fisier1);


		}


		if (strcmp(top, "5") == 0)
		{
			FILE* f = fopen("Hip-Hop.txt", "r");

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), f))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}
			fclose(f);


			char informatii[1000];
			bzero(informatii, 1000);

			strcat(informatii, "Artist:");

			char raspuns[1000];
			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);
			printf("%s\n", informatii);
			FILE* g = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, g))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, informatii) != NULL)
				{
					ok = 1;

				}

			}

			if (ok == 0)
			{
				bzero(raspuns, 1000);
				strcpy(raspuns, "Piesa nu exista\n");
				if (write(client, raspuns, strlen(raspuns)) <= 0)
				{
					perror("[server]Eroare la write() catre client.\n");
				}
				fclose(g);
				return 0;
			}
			fclose(g);


			char array[1000][1000];
			FILE* general = fopen("general.txt", "r");



			int i = 0;
			while (fgets(array[i], sizeof(array[i]), general)) //punem in matrice fisierul 
			{
				array[i][strlen(array[i]) - 1] = '\0';
				i++;

			}

			int l = i;


			char nrvot[5];
			bzero(nrvot, 5);
			int y;
			int k = 0;
			for (int i = 0; i < l; i++)
			{

				if (strstr(array[i], informatii) != NULL)
				{
					while (array[i][k] != 'A')
					{
						y = i;
						nrvot[k] = array[i][k];
						k++;
					}
					nrvot[k] = '\0';

				}
			}

			int x = atoi(nrvot);
			x = x + 1;
			if (x %10==0)
				array[y][1] = '0';
			int rev;
			while (x != 0)
			{
				int remainder = x % 10;
				rev = rev * 10 + remainder;
				x /= 10;
			}

			for (int i = 0; i < k; i++)
			{
				if (rev)
				{
					int c = rev % 10;
					array[y][i] = c + '0';
					rev = rev / 10;
				}
				else
				{
					break;
				}
			}



			fclose(general);
			FILE* fisier;
			fisier = fopen("general.txt", "w");
			for (i = 0; i < l; i++)
			{
				
				fputs(array[i], fisier);
				fputc('\n', fisier);
			}

			fclose(fisier);


			char array1[1000][1000];
			FILE* general1 = fopen("Hip-Hop.txt", "r");



			i = 0;
			while (fgets(array1[i], sizeof(array1[i]), general1)) //punem in matrice fisierul 
			{
				array1[i][strlen(array1[i]) - 1] = '\0';
				i++;

			}

			l = i;


			bzero(nrvot, 5);

			k = 0;
			for (int i = 0; i < l; i++)
			{

				if (strstr(array1[i], informatii) != NULL)
				{
					while (array1[i][k] != 'A')
					{
						y = i;
						nrvot[k] = array1[i][k];
						k++;
					}
					nrvot[k] = '\0';

				}
			}

			x = atoi(nrvot);
			x = x + 1;
			if (x % 10==0)
				array1[y][1] = '0';
			rev = 0;
			while (x != 0)
			{
				int remainder = x % 10;
				rev = rev * 10 + remainder;
				x /= 10;
			}
			printf("%d\n", rev);
			for (int i = 0; i < k; i++)
			{
				if (rev)
				{
					int c = rev % 10;
					array1[y][i] = c + '0';
					rev = rev / 10;
				}
				else
				{
					break;
				}
			}


			fclose(general1);
			FILE* fisier1;
			fisier1 = fopen("Hip-Hop.txt", "w");
			for (i = 0; i < l; i++)
			{
				
				fputs(array1[i], fisier1);
				fputc('\n', fisier1);
			}

			fclose(fisier1);


		}




		return 1;

	}

	int restrictioneaza(int client, int cont)
	{
		if (cont != 1)
		{
			char raspuns[1000] = "Nu puteti folosi aceasta comanda intrucat nu sunteti administrator\n ";
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}
			return 0;
		}

		FILE* file;
		file = fopen("usernames.txt", "r");

		if (file == NULL)
		{
			printf("Cannot open file \n");
			exit(0);
		}

		char nume[100];
		bzero(nume, 100);
		while (fgets(nume, sizeof(nume), file)) //afisam usernameurile pentru restrictionare
		{

			if (write(client, nume, strlen(nume)) <= 0)
			{
				perror("[server]Eroare la write() catre client din afiseazaTop.\n");
			}
			bzero(nume, 100);
		}

		fclose(file);


		char raspuns[1000] = "introduceti numele utilizatorului pe care doriti sa il restrictionati:\n ";
		if (write(client, raspuns, strlen(raspuns)) <= 0)
		{
			perror("[server]Eroare la write() catre client.\n");
		}

		char nume1[100];
		bzero(nume1, 100);
		printf("[server]Asteptam mesajul...\n");
		fflush(stdout);

		/* citirea mesajului */
		if (read(client, nume1, 100) <= 0)
		{
			perror("[server]Eroare la read() de la client.\n");
		}


		FILE* f = fopen("usernames.txt", "r");
		int wordExist = 0;
		int bufferLength = 255;
		char line[bufferLength];


		//verificam daca usernameul introdus exista in fisierul usernames
		while (fgets(line, bufferLength, f))
		{

			line[strlen(line) - 1] = '\0';


			int ptr = strncmp(line, nume1, strlen(line) - 1);

			if (ptr == 0)
			{
				wordExist = 1;
				break;
			}
		}
		fclose(f);
		if (wordExist == 1) //daca exista il punem in restrictionati
		{
			FILE* f = fopen("restrictionati.txt", "a");
			if (f)
			{
				fputs(nume1, f);


			}
			else
			{
				printf("Failed to open the file\n");
			}
			fclose(f);


			char raspuns[100] = "restrictionare reusita\n";
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}


		}

		else
		{
			char raspuns[100] = "username not found\n";
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}
			return -1;
		}


	}

	int sterge(int client, int cont)
	{
		if (cont != 1)
		{
			char raspuns[1000] = "Nu puteti folosi aceasta comanda intrucat nu sunteti administrator\n ";
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}
			return 0;
		}

		char raspuns[1000] = "introduceti numarul genului muzical melodiei pe care vreti sa o stergeti:\n 1.pop\n 2.rock\n 3.dance\n 4.RnB\n 5.Hip-Hop\n ";
		if (write(client, raspuns, strlen(raspuns)) <= 0)
		{
			perror("[server]Eroare la write() catre client.\n");
		}
		bzero(raspuns, 1000);

		char top[100];
		bzero(top, 100);
		printf("[server]Asteptam mesajul...\n");
		fflush(stdout);

		/* citirea mesajului */
		if (read(client, top, 100) <= 0)
		{
			perror("[server]Eroare la read() de la client.\n");
		}

		top[strlen(top) - 1] = '\0';


		if (strcmp(top, "1") == 0)
		{
			FILE* f = fopen("pop.txt", "r");

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), f)) //ni se afiseazA melodiile disponibile pentru stergere din fisierul corespunzator genului
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}
			fclose(f);

			char informatii[1000];
			bzero(informatii, 1000);

			strcat(informatii, "Artist:");

			char raspuns[1000];
			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);
			printf("%s\n", informatii);
			FILE* g = fopen("general.txt", "r"); //verificam daca s-a introdus o piesa existenta
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, g))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, informatii) != NULL)
				{
					ok = 1;

				}

			}

			if (ok == 0)
			{
				bzero(raspuns, 1000);
				strcpy(raspuns, "Piesa nu exista\n");
				if (write(client, raspuns, strlen(raspuns)) <= 0)
				{
					perror("[server]Eroare la write() catre client.\n");
				}
				fclose(g);
				return 0;
			}
			fclose(g);

			char array[1000][1000];
			FILE* general;
			general = fopen("general.txt", "r"); 



			int i = 0;
			while (fgets(array[i], sizeof(array[i]), general)) //punem in matrice fisierul 
			{
				array[i][strlen(array[i]) - 1] = '\0';
				i++;

			}

			int l = i;
			fclose(general);

			general = fopen("general.txt", "w");

			for (int i = 0; i < l; i++)
			{
				if (strstr(array[i], informatii) != NULL)//stergem randul corespunzator melodiei
				{
					for (int j = i; j < l - 1; j++)
						strcpy(array[j], array[j + 1]);

				}
			}

			fclose(general);

			FILE* fisier;
			fisier = fopen("general.txt", "w"); //actualizam in general
			for (i = 0; i < l - 1; i++)
			{
				
				fputs(array[i], fisier);
				fputc('\n', fisier);
			}

			fclose(fisier);


			char array1[1000][1000];
			FILE* general1;
			general1 = fopen("pop.txt", "r");



			i = 0;
			while (fgets(array1[i], sizeof(array1[i]), general1)) //punem in matrice fisierul 
			{
				array1[i][strlen(array1[i]) - 1] = '\0';
				i++;

			}

			l = i;
			fclose(general1);

			general1 = fopen("pop.txt", "w"); //procedam la fel si in pop-fisierul corespunzator genului, stergem randul piesei si actualizam

			for (int i = 0; i < l; i++)
			{
				if (strstr(array1[i], informatii) != NULL)
				{
					for (int j = i; j < l - 1; j++)
						strcpy(array1[j], array1[j + 1]);

				}
			}

			fclose(general1);

			FILE* fisier1;
			fisier1 = fopen("pop.txt", "w");
			for (i = 0; i < l - 1; i++)
			{
			
				fputs(array1[i], fisier1);
				fputc('\n', fisier1);
			}

			fclose(fisier1);

		}

		if (strcmp(top, "2") == 0)
		{
			FILE* f = fopen("rock.txt", "r");

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), f))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}
			fclose(f);

			char informatii[1000];
			bzero(informatii, 1000);

			strcat(informatii, "Artist:");

			char raspuns[1000];
			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);
			printf("%s\n", informatii);
			FILE* g = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, g))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, informatii) != NULL)
				{
					ok = 1;

				}

			}

			if (ok == 0)
			{
				bzero(raspuns, 1000);
				strcpy(raspuns, "Piesa nu exista\n");
				if (write(client, raspuns, strlen(raspuns)) <= 0)
				{
					perror("[server]Eroare la write() catre client.\n");
				}
				fclose(g);
				return 0;
			}
			fclose(g);

			char array[1000][1000];
			FILE* general;
			general = fopen("general.txt", "r");



			int i = 0;
			while (fgets(array[i], sizeof(array[i]), general)) //punem in matrice fisierul 
			{
				array[i][strlen(array[i]) - 1] = '\0';
				i++;

			}

			int l = i;
			fclose(general);

			general = fopen("general.txt", "w");

			for (int i = 0; i < l; i++)
			{
				if (strstr(array[i], informatii) != NULL)
				{
					for (int j = i; j < l - 1; j++)
						strcpy(array[j], array[j + 1]);

				}
			}

			fclose(general);

			FILE* fisier;
			fisier = fopen("general.txt", "w");
			for (i = 0; i < l - 1; i++)
			{
				
				fputs(array[i], fisier);
				fputc('\n', fisier);
			}

			fclose(fisier);


			char array1[1000][1000];
			FILE* general1;
			general1 = fopen("rock.txt", "r");



			i = 0;
			while (fgets(array1[i], sizeof(array1[i]), general1)) //punem in matrice fisierul 
			{
				array1[i][strlen(array1[i]) - 1] = '\0';
				i++;

			}

			l = i;
			fclose(general1);

			general1 = fopen("rock.txt", "w");

			for (int i = 0; i < l; i++)
			{
				if (strstr(array1[i], informatii) != NULL)
				{
					for (int j = i; j < l - 1; j++)
						strcpy(array1[j], array1[j + 1]);

				}
			}

			fclose(general1);

			FILE* fisier1;
			fisier1 = fopen("rock.txt", "w");
			for (i = 0; i < l - 1; i++)
			{
				
				fputs(array1[i], fisier1);
				fputc('\n', fisier1);
			}

			fclose(fisier1);

		}

		if (strcmp(top, "3") == 0)
		{
			FILE* f = fopen("dance.txt", "r");

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), f))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}
			fclose(f);

			char informatii[1000];
			bzero(informatii, 1000);

			strcat(informatii, "Artist:");

			char raspuns[1000];
			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);
			printf("%s\n", informatii);
			FILE* g = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, g))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, informatii) != NULL)
				{
					ok = 1;

				}

			}

			if (ok == 0)
			{
				bzero(raspuns, 1000);
				strcpy(raspuns, "Piesa nu exista\n");
				if (write(client, raspuns, strlen(raspuns)) <= 0)
				{
					perror("[server]Eroare la write() catre client.\n");
				}
				fclose(g);
				return 0;
			}
			fclose(g);

			char array[1000][1000];
			FILE* general;
			general = fopen("general.txt", "r");



			int i = 0;
			while (fgets(array[i], sizeof(array[i]), general)) //punem in matrice fisierul 
			{
				array[i][strlen(array[i]) - 1] = '\0';
				i++;

			}

			int l = i;
			fclose(general);

			general = fopen("general.txt", "w");

			for (int i = 0; i < l; i++)
			{
				if (strstr(array[i], informatii) != NULL)
				{
					for (int j = i; j < l - 1; j++)
						strcpy(array[j], array[j + 1]);

				}
			}

			fclose(general);

			FILE* fisier;
			fisier = fopen("general.txt", "w");
			for (i = 0; i < l - 1; i++)
			{
				
				fputs(array[i], fisier);
				fputc('\n', fisier);
			}

			fclose(fisier);


			char array1[1000][1000];
			FILE* general1;
			general1 = fopen("dance.txt", "r");



			i = 0;
			while (fgets(array1[i], sizeof(array1[i]), general1)) //punem in matrice fisierul 
			{
				array1[i][strlen(array1[i]) - 1] = '\0';
				i++;

			}

			l = i;
			fclose(general1);

			general1 = fopen("dance.txt", "w");

			for (int i = 0; i < l; i++)
			{
				if (strstr(array1[i], informatii) != NULL)
				{
					for (int j = i; j < l - 1; j++)
						strcpy(array1[j], array1[j + 1]);

				}
			}

			fclose(general1);

			FILE* fisier1;
			fisier1 = fopen("dance.txt", "w");
			for (i = 0; i < l - 1; i++)
			{
			
				fputs(array1[i], fisier1);
				fputc('\n', fisier1);
			}

			fclose(fisier1);

		}

		if (strcmp(top, "4") == 0)
		{
			FILE* f = fopen("RnB.txt", "r");

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), f))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}
			fclose(f);

			char informatii[1000];
			bzero(informatii, 1000);

			strcat(informatii, "Artist:");

			char raspuns[1000];
			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);
			printf("%s\n", informatii);
			FILE* g = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, g))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, informatii) != NULL)
				{
					ok = 1;

				}

			}

			if (ok == 0)
			{
				bzero(raspuns, 1000);
				strcpy(raspuns, "Piesa nu exista\n");
				if (write(client, raspuns, strlen(raspuns)) <= 0)
				{
					perror("[server]Eroare la write() catre client.\n");
				}
				fclose(g);
				return 0;
			}
			fclose(g);

			char array[1000][1000];
			FILE* general;
			general = fopen("general.txt", "r");



			int i = 0;
			while (fgets(array[i], sizeof(array[i]), general)) //punem in matrice fisierul 
			{
				array[i][strlen(array[i]) - 1] = '\0';
				i++;

			}

			int l = i;
			fclose(general);

			general = fopen("general.txt", "w");

			for (int i = 0; i < l; i++)
			{
				if (strstr(array[i], informatii) != NULL)
				{
					for (int j = i; j < l - 1; j++)
						strcpy(array[j], array[j + 1]);

				}
			}

			fclose(general);

			FILE* fisier;
			fisier = fopen("general.txt", "w");
			for (i = 0; i < l - 1; i++)
			{
				
				fputs(array[i], fisier);
				fputc('\n', fisier);
			}

			fclose(fisier);


			char array1[1000][1000];
			FILE* general1;
			general1 = fopen("RnB.txt", "r");



			i = 0;
			while (fgets(array1[i], sizeof(array1[i]), general1)) //punem in matrice fisierul 
			{
				array1[i][strlen(array1[i]) - 1] = '\0';
				i++;

			}

			l = i;
			fclose(general1);

			general1 = fopen("RnB.txt", "w");

			for (int i = 0; i < l; i++)
			{
				if (strstr(array1[i], informatii) != NULL)
				{
					for (int j = i; j < l - 1; j++)
						strcpy(array1[j], array1[j + 1]);

				}
			}

			fclose(general1);

			FILE* fisier1;
			fisier1 = fopen("RnB.txt", "w");
			for (i = 0; i < l - 1; i++)
			{
				
				fputs(array1[i], fisier1);
				fputc('\n', fisier1);
			}

			fclose(fisier1);

		}

		if (strcmp(top, "5") == 0)
		{
			FILE* f = fopen("Hip-Hop.txt", "r");

			char topul[1000];
			bzero(topul, 1000);
			while (fgets(topul, sizeof(topul), f))
			{

				if (write(client, topul, strlen(topul)) <= 0)
				{
					perror("[server]Eroare la write() catre client din afiseazaTop.\n");
				}
				bzero(topul, 1000);
			}
			fclose(f);

			char informatii[1000];
			bzero(informatii, 1000);

			strcat(informatii, "Artist:");

			char raspuns[1000];
			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti artistul:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char artist[100];
			bzero(artist, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, artist, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			artist[strlen(artist) - 1] = '\0';


			strcat(informatii, artist);
			strcat(informatii, "-Piesa:");

			bzero(raspuns, 1000);
			strcpy(raspuns, "Introduceti piesa:\n");
			if (write(client, raspuns, strlen(raspuns)) <= 0)
			{
				perror("[server]Eroare la write() catre client.\n");
			}

			char piesa[100];
			bzero(piesa, 100);
			printf("[server]Asteptam mesajul...\n");
			fflush(stdout);

			/* citirea mesajului */
			if (read(client, piesa, 100) <= 0)
			{
				perror("[server]Eroare la read() de la client.\n");
			}
			piesa[strlen(piesa) - 1] = '\0';

			strcat(informatii, piesa);
			printf("%s\n", informatii);
			FILE* g = fopen("general.txt", "r");
			int wordExist = 0;
			int bufferLength = 255;
			char line[bufferLength];

			int ok = 0;
			while (fgets(line, bufferLength, g))
			{
				line[strlen(line) - 1] = '\0';


				if (strstr(line, informatii) != NULL)
				{
					ok = 1;

				}

			}

			if (ok == 0)
			{
				bzero(raspuns, 1000);
				strcpy(raspuns, "Piesa nu exista\n");
				if (write(client, raspuns, strlen(raspuns)) <= 0)
				{
					perror("[server]Eroare la write() catre client.\n");
				}
				fclose(g);
				return 0;
			}
			fclose(g);

			char array[1000][1000];
			FILE* general;
			general = fopen("general.txt", "r");



			int i = 0;
			while (fgets(array[i], sizeof(array[i]), general)) //punem in matrice fisierul 
			{
				array[i][strlen(array[i]) - 1] = '\0';
				i++;

			}

			int l = i;
			fclose(general);

			general = fopen("general.txt", "w");

			for (int i = 0; i < l; i++)
			{
				if (strstr(array[i], informatii) != NULL)
				{
					for (int j = i; j < l - 1; j++)
						strcpy(array[j], array[j + 1]);

				}
			}

			fclose(general);

			FILE* fisier;
			fisier = fopen("general.txt", "w");
			for (i = 0; i < l - 1; i++)
			{
			
				fputs(array[i], fisier);
				fputc('\n', fisier);
			}

			fclose(fisier);


			char array1[1000][1000];
			FILE* general1;
			general1 = fopen("Hip-Hop.txt", "r");



			i = 0;
			while (fgets(array1[i], sizeof(array1[i]), general1)) //punem in matrice fisierul 
			{
				array1[i][strlen(array1[i]) - 1] = '\0';
				i++;

			}

			l = i;
			fclose(general1);

			general1 = fopen("Hip-Hop.txt", "w");

			for (int i = 0; i < l; i++)
			{
				if (strstr(array1[i], informatii) != NULL)
				{
					for (int j = i; j < l - 1; j++)
						strcpy(array1[j], array1[j + 1]);

				}
			}

			fclose(general1);

			FILE* fisier1;
			fisier1 = fopen("Hip-Hop.txt", "w");
			for (i = 0; i < l - 1; i++)
			{
				
				fputs(array1[i], fisier1);
				fputc('\n', fisier1);
			}

			fclose(fisier1);

		}

		return 1;



	}



	int main()
	{
		struct sockaddr_in server;	// structura folosita de server
		struct sockaddr_in from;
		char msg[100];		//mesajul primit de la client 
		char display[1000];        //mesaj de raspuns pentru client
		int sd;			//descriptorul de socket 

		/* crearea unui socket */
		if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("[server]Eroare la socket().\n");
			return errno;
		}

		/* pregatirea structurilor de date */
		bzero(&server, sizeof(server));
		bzero(&from, sizeof(from));

		/* umplem structura folosita de server */
		/* stabilirea familiei de socket-uri */
		server.sin_family = AF_INET;
		/* acceptam orice adresa */
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		/* utilizam un port utilizator */
		server.sin_port = htons(PORT);

		/* atasam socketul */
		if (bind(sd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1)
		{
			perror("[server]Eroare la bind().\n");
			return errno;
		}

		/* punem serverul sa asculte daca vin clienti sa se conecteze */
		if (listen(sd, 5) == -1)
		{
			perror("[server]Eroare la listen().\n");
			return errno;
		}

		/* servim in mod concurent clientii... */
		while (1)
		{
			int client;
			int length = sizeof(from);

			printf("[server]Asteptam la portul %d...\n", PORT);
			fflush(stdout);

			/* acceptam un client (stare blocanta pina la realizarea conexiunii) */
			client = accept(sd, (struct sockaddr*)&from, &length);

			/* eroare la acceptarea conexiunii de la un client */
			if (client < 0)
			{
				perror("[server]Eroare la accept().\n");
				continue;
			}

			int pid;
			pid = fork();
			if (pid == -1) //eroare la fork
			{
				close(client);
				continue;
			}
			else if (pid > 0) //parinte
			{
				close(client);
				while (waitpid(-1, NULL, WNOHANG));  //daca mai sunt copii continua
				continue;
			}
			else if (pid == 0) //copil, procesam cererile clientului
			{

				close(sd);
				int cont = -1; //0 - utilizator , 1-admin
				while (cont == -1)
				{


					bzero(display, 1000);
					strcpy(display, "Introduceti numarul comenzii dorite:\n 1.inregistrare \n 2.login");

					if (write(client, display, strlen(display)) <= 0)
					{
						perror("[server]Eroare la write() catre client.\n");
						continue;           /* continuam sa ascultam */
					}
					else
					{
						printf("[server]Mesajul a fost trasmis cu succes.\n");
					}

					bzero(msg, 100);
					if (read(client, msg, 100) <= 0)
					{
						perror("[server]Eroare la read() de la client.\n");
						close(client);	/* inchidem conexiunea cu clientul */
						continue;		/* continuam sa ascultam */
					}
					else
					{
						msg[strlen(msg) - 1] = '\0';
						if (strcmp(msg, "2") == 0)
						{
							cont = login(client); //logheaza
						}
						else if (strcmp(msg, "1") == 0)
						{
							cont = inregistrare(client); //inregistreaza si logheaza automat
						}
						else
						{
							bzero(display, 1000);
							strcpy(display, "nu s-a gasit comanda");
							if (write(client, display, strlen(display)) <= 0)
							{
								perror("[server]Eroare la write() catre client.\n");
								continue;           /* continuam sa ascultam */
							}
							else
							{
								printf("[server]Mesajul a fost trasmis cu succes.\n");
							}
						}
					}
				}



				while (cont == 0 || cont == 1 || cont ==3)
				{

					bzero(display, 1000);
					strcpy(display, "\nIntroduceti numarul comenzii dorite:\n1.adauga melodie\n2.vizualizeaza top\n3.adauga comentariu\n4.voteaza\n5.restrictioneaza\n6.sterge melodie\n7.Vizualizeaza comentarii\n8.quit");
					if (write(client, display, strlen(display)) <= 0)
					{
						perror("[server]Eroare la write() catre client.\n");
						continue;           /* continuam sa ascultam */
					}
					else
					{
						printf("[server]Mesajul a fost trasmis cu succes.\n");
					}

					bzero(msg, 100);
					printf("[server]Asteptam mesajul...\n");
					fflush(stdout);

					// citirea mesajului
					if (read(client, msg, 100) <= 0)
					{
						perror("[server]Eroare la read() de la client.\n");
						close(client);	// inchidem conexiunea cu clientul
						continue;		// continuam sa ascultam
					}
					msg[strlen(msg) - 1] = '\0';
					if (strcmp(msg, "1") == 0)
						adaugaMelodie(client);
					else if (strcmp(msg, "2") == 0)
						vizualizeazaTop(client);
					else if (strcmp(msg, "3") == 0)
						adaugaComentariu(client);
					else if (strcmp(msg, "4") == 0)
						voteaza(client);
					else if (strcmp(msg, "5") == 0)
					{

						restrictioneaza(client, cont);
					}
					else if (strcmp(msg, "6") == 0)
						sterge(client, cont);
					else if (strcmp(msg, "7") == 0)
						vizualizeazaComm(client);

					else if (strcmp(msg, "8") == 0)
					{

						close(client);
						exit(1);  
					}
					else
					{
						bzero(display, 1000);
						strcpy(display, "comanda gresita\n");
						if (write(client, display, strlen(display)) <= 0)
						{
							perror("[server]Eroare la write() catre client.\n");
							continue;           // continuam sa ascultam
						}
						else
							printf("[server]Mesajul a fost trasmis cu succes.\n");

					}
				}

			}


		}


	}				/* while */

		/* main */