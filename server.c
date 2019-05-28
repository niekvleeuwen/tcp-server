#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<string.h>	//strlen
#include<unistd.h>	//write

int main(int argc , char *argv[])
{
	int sock , new_sock , c, read_size;;
	struct sockaddr_in server , client;
	char *message,  client_message[2000];
	
	//maak een sock aan
	sock = socket(AF_INET , SOCK_STREAM , 0);
	//als sock een negatief nummer bevat dan is het aanmaken mislukt
	if (sock < 0){
		printf("Er is een probleem opgetreden bij het aanmaken van een sock\n");
	}
	
	//vul de struct
	server.sin_family = AF_INET; //domain wat we net hebben gekozen bij het aanmaken van een sock
	server.sin_addr.s_addr = INADDR_ANY; //accepteer verbindingen vanaf alle adressen
	server.sin_port = htons( 8888 ); //gebruik poort 8888
	
    //koppel het sock aan de host, de bind functie geeft (net als de sock functie) een negatief nummer als de operatie mislukt
    if(bind(sock,(struct sockaddr *)&server , sizeof(server)) < 0){
        printf("Koppelen van sock mislukt\n");
		return -1;
    }
    printf("Koppelen van sock gelukt\n");
	
	//laat het sock luisteren naar wat binnenkomt en laat maximaal 3 clients in de wachtrij staan
	listen(sock , 3);
	
	//wachten op een binnenkomende connection
	printf("Wachten op een binnenkomende connectie...\n");
	c = sizeof(struct sockaddr_in);
	while( (new_sock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		printf("Client is een verbinding aangegaan\n");

		//reageer op de 
		message = "Verbinding geaccepteerd\n";
		write(new_sock , message , strlen(message));
		
		//wacht op een ontvangen bericht
		while( (read_size = recv(new_sock , client_message , 2000 , 0)) > 0 )
		{
			//dump de tekst op het beeldscherm
			printf("Bericht van de client: %s", client_message);
			printf("\n");
			//maak de message leeg
			memset(&client_message[0], 0, sizeof(client_message));
			
			//stuur een bevestiging
			write(new_sock,"\nIk heb je bericht ontvangen\n",29);
		}
		//checken of de gebruiker de verbinding gesloten heeft of dat er ee fout is opgetreden
		if(read_size == 0){
			printf("Client heeft de verbinding gesloten\n");
		}else if(read_size == -1){
			printf("Er is een fout opgetreden bij het ontvangen\n");
		}
	}
	//checken of er een fout is opgetreden bij het accepteren van een binnenkomend bericht
	if (new_sock < 0){
		printf("Er is een fout opgetreden bij het accepteren\n");
		return 1;
	}

	return 0;
}