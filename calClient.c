#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	char send_msg[BUF_SIZE];
	char *temp;
	char *command = NULL;
	struct sockaddr_in serv_adr;
	int add = 0;


	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected......");

	while(1)
	{
		fputs("Input Command (Input :help to show help): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		if(!strcmp(message, ":help\n"))
		{
			fprintf(stdout,"\033[0;31m======LIST OF COMMANDS======\n");
			fprintf(stdout,"\033[0;34m1. ADD SCHEDULE (a)\n");
			fprintf(stdout,"2. SHOW SCHEDULE (c)\n");
			fprintf(stdout, "3. SHOW SCHEDULE OF THE MONTH (cc)\n");
			fprintf(stdout, "%c[0m\n", 27);
			continue;
		}
		if(message[0]=='a')
		{
			add=1;
		}

		//command = strtok(message, " ");
		//strcpy(temp, command);
		//printf("%s\n", temp);
		write(sock, message, strlen(message));
		str_len=read(sock, message, BUF_SIZE-1);
		message[str_len]=0;
		
		if(add==1)
		{
			
		printf("Added Schedule\n");
		printf("The day that schedule exists that month: %s\n", message);
		add=  0;
		}
		else{
			printf("The Schedule is: %s \n", message);
		}
	}
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

