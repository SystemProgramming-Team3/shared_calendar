#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

char* client_func(char* ip, char* port, char* input);
int main(void)
{
	char input[256];
	char *ret;
	FILE * fp = fopen("creat.txt", "r");
	fgets(input, sizeof(input), fp);

	//gets(input);
	//fgets(input, sizeof(input), stdin);
	ret = client_func("127.0.0.1", "3000", input);
	printf("%s\n", ret);
	return 0;
}




char* client_func(char* ip, char* port, char* input)
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	char send_msg[BUF_SIZE];
	char *temp;
	char *command = NULL;
	struct sockaddr_in serv_adr;
	int add = 0;
	int del_idx = 0;
	char comm[3][256];
	char ret[1024];
	//if(argc!=3) {
		//printf("Usage : %s <IP> <port>\n", argv[0]);
		//exit(1);
	//}

	//input = "Test Test2 Test3";
	//char test[] = "TEST1 2 3";
	//char *ptr = strtok(input, " ");
	//while(ptr!=NULL)
	//{
		//strcpy(comm[del_idx], ptr);
		//printf("%s\n", comm[del_idx]);
		//ptr = strtok(NULL, " ");
	//}
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(ip);
	serv_adr.sin_port=htons(atoi(port));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		{
		puts("Connected......");


		strcpy(message, input);
		//if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			//break;
		/*
		if(!strcmp(message, ":help\n"))
		{
			fprintf(stdout,"\033[0;31m======LIST OF COMMANDS======\n");
			fprintf(stdout,"\033[0;34m1. ADD SCHEDULE (a)\n");
			fprintf(stdout,"2. SHOW SCHEDULE (c)\n");
			fprintf(stdout, "3. SHOW SCHEDULE OF THE MONTH (cc)\n");
			fprintf(stdout, "%c[0m\n", 27);
			continue;
		}
		*/
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
			
		//printf("Added Schedule\n");
		//printf("The day that schedule exists that month: %s\n", message);
		add=  0;
		sprintf(ret, "Added Schedule\nThe day that schedule exists that month: %s\n", message);
		}
		else{
			//printf("The Schedule is: %s \n", message);
			
			sprintf(ret, "The Schedule is: %s \n", message);
		}
		//printf("%s\n", ret);
	}
	close(sock);
	return ret;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

