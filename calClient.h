#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);
void read_command_and_send();
int *cc_client_func(char *ip, char *port, char *input);
int *a_client_func(char *ip, char *port, char *input);
char *c_client_func(char *ip, char *port, char *input);
int* rm_client_func(char* ip, char* port, char* input);
int* mv_client_func(char* ip, char* port, char* input);



// client_func Function Test용 Stub Code (main function)

/*
int main(void)
{
	char input[256];
	int *ret;
	int * cret;
	char* cdret;
	int i = 0;
	//FILE * fp = fopen("creat.txt", "r");
	//fgets(input, sizeof(input), fp);


	// ret = cc_client_func("127.0.0.1", "3001", "a 2023.05 ");

	
	// for(int i=0;i<31;i++)
	// {
	// 	printf("%d\n", ret[i]);
	// }
	//cret = mv_client_func("127.0.0.1", "3001", "mv 2023.05.25 2023.06.27 you ");
	//cret = rm_client_func("127.0.0.1", "3001", "rm 2023.05.26");

	// for(i=0;i<31;i++)
	// {
	//  	printf("cret: %d\n", cret[i]);
	// }
	cdret = c_client_func("127.0.0.1", "3000", "c 2023.05.29 ");
	puts(cdret);
	//printf("final:%s", cdret);
	// char k = 'a';
	// char *juso = cdret;
	// printf("final: %s\n", cdret);
	// cdret = cdret+BUF_SIZE;
	// printf("final: %s\n", cdret);

	//printf("final: %s\n", cdret[1]);

	return 0;
}
*/


void read_command_and_send()
{
	


}

char* trim(char* origin) {
	
	char* res_array = (char*)malloc(sizeof(char)*BUF_SIZE);
	memset(res_array, 0, sizeof(char)*BUF_SIZE);
	int idx = 0;
	
	int iter = 0;
	int empty = 1;  
	int newline = 0; 

	for(iter = 0;origin[iter]!='\0';iter++) {
		if(origin[iter]=='\n') {
			
			while (idx > 0 && (res_array[idx-1] == ' ' || res_array[idx-1] == '\t')) {
				idx--;
			}
			if (!newline) { 
				res_array[idx++] = '\n';
			}
			empty = 1;  
			newline = 1; 
		} else if(origin[iter]!=' '&& origin[iter]!='\t') {
			res_array[idx++] = origin[iter];
			empty = 0;  
			newline = 0; 
		} else if (!empty) {  
			res_array[idx++] = origin[iter];
		}
	}
	
	
	while (idx > 0 && (res_array[idx-1] == ' ' || res_array[idx-1] == '\t' || res_array[idx-1] == '\n')) {
		idx--;
	}
	res_array[idx] = '\0';
	return res_array;
}


void stringfilter(char *str, char *findstr) {
	int length = strlen(findstr);
	char *ptr = strstr(str, findstr);
	while(ptr!=NULL)
	{
		memmove(ptr, ptr+length, strlen(ptr+length)+1);
		ptr = strstr(str, findstr);
	}
}


char *c_client_func(char *ip, char *port, char *input)
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
	char ret[BUF_SIZE];
	char *ptr = NULL;
	char *tit_ptr = NULL;
	char *res_array;
	int cal_array[31];
	char* findptr;



	res_array = (char*)malloc(sizeof(char)*BUF_SIZE);
	// if(argc!=3) {
	// printf("Usage : %s <IP> <port>\n", argv[0]);
	// exit(1);
	//}

	// input = "Test Test2 Test3";
	// char test[] = "TEST1 2 3";
	// char *ptr = strtok(input, " ");
	// while(ptr!=NULL)
	//{
	// strcpy(comm[del_idx], ptr);
	// printf("%s\n", comm[del_idx]);
	// ptr = strtok(NULL, " ");
	//}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(ip);
	serv_adr.sin_port = htons(atoi(port));

	if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
	{
		// error_handling("connect() error!");
		cal_array[0] = -1;
		return cal_array;
	}
	else
	{
		// puts("Connected......");

		strcpy(message, input);
		// if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		// break;
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
		if (message[0] == 'a')
		{
			add = 1;
		}

		// command = strtok(message, " ");
		// strcpy(temp, command);
		// printf("%s\n", temp);
		write(sock, message, strlen(message));
		str_len = read(sock, message, BUF_SIZE - 1);
		message[str_len] = 0;

		if (add == 1)
		{

			// printf("Added Schedule\n");
			// printf("The day that schedule exists that month: %s\n", message);
			add = 0;
			sprintf(ret, "Added Schedule\nThe day that schedule exists that month: %s\n", message);
		}
		else
		{
			// printf("The Schedule is: %s \n", message);

			sprintf(ret, "The Schedule is: %s \n", message);
		
			findptr = strstr(message, "txt");
			/*
			while(findptr !=EOF)
			{
				strncpy(findptr, "", 3);
				findptr++;
			}
			*/
			//printf("location: %s", message);
			
			stringfilter(message, "txt");
			// printf("\n출력:");
			// puts(message);
			// printf("출력 종료\n");
			ptr = strtok(message, ">.");
			while (ptr != NULL)
			{
				if(strcmp(ptr, ".txt"))
				{
					
				sprintf(res_array, "%s\n%s", res_array, ptr);
				//strcpy(res_array[del_idx], ptr);
				//printf("origin: %s\n", ptr);
				//printf("resar: %s\n",res_array);
				//del_idx++;
				}

				// printf("string: %s\n", res_array[del_idx]);
				//cal_array[del_idx] = atoi(ptr);
				// printf("d: %d\n", cal_array[del_idx]);
				ptr = strtok(NULL, ">.");
				
			}
		}
		// printf("%s\n", ret);
	}
	close(sock);
	
	res_array = trim(res_array);
	return (char*)res_array;
}

int *a_client_func(char *ip, char *port, char *input)
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
	char ret[BUF_SIZE];
	char *ptr = NULL;
	char res_array[33][BUF_SIZE];
	int * cal_array = malloc(sizeof(int)*31);
	// if(argc!=3) {
	// printf("Usage : %s <IP> <port>\n", argv[0]);
	// exit(1);
	//}

	// input = "Test Test2 Test3";
	// char test[] = "TEST1 2 3";
	// char *ptr = strtok(input, " ");
	// while(ptr!=NULL)
	//{
	// strcpy(comm[del_idx], ptr);
	// printf("%s\n", comm[del_idx]);
	// ptr = strtok(NULL, " ");
	//}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(ip);
	serv_adr.sin_port = htons(atoi(port));

	if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
	{
		// error_handling("connect() error!");
		cal_array[0] = -1;
		return cal_array;
	}
	else
	{
		// puts("Connected......");

		strcpy(message, input);
		// if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		// break;
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
		if (message[0] == 'a')
		{
			add = 1;
		}

		// command = strtok(message, " ");
		// strcpy(temp, command);
		// printf("%s\n", temp);
		sprintf(message, "%s \0", message);
		write(sock, message, strlen(message));
		str_len = read(sock, message, BUF_SIZE - 1);
		message[str_len] = 0;

		if (add == 1)
		{

			// printf("Added Schedule\n");
			// printf("The day that schedule exists that month: %s\n", message);
			add = 0;
			sprintf(ret, "Added Schedule\nThe day that schedule exists that month: %s\n", message);
			ptr = strtok(message, ",");
			while (ptr != NULL)
			{
				strcpy(res_array, ptr);
				// printf("origin: %s\n", ptr);
				// printf("string: %s\n", res_array[del_idx]);
				cal_array[del_idx] = atoi(ptr);
				// printf("d: %d\n", cal_array[del_idx]);
				ptr = strtok(NULL, ",");
				del_idx++;
			}
		}
		else
		{
			// printf("The Schedule is: %s \n", message);

			sprintf(ret, "The Schedule is: %s \n", message);
			ptr = strtok(message, ",");
			while (ptr != NULL)
			{
				strcpy(res_array, ptr);
				// printf("origin: %s\n", ptr);
				// printf("string: %s\n", res_array[del_idx]);
				cal_array[del_idx] = atoi(ptr);
				// printf("d: %d\n", cal_array[del_idx]);
				ptr = strtok(NULL, ",");
				del_idx++;
			}
		}
		// printf("%s\n", ret);
	}
	close(sock);
	return cal_array;
}

int* cc_client_func(char* ip, char* port, char* input)
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
	char ret[BUF_SIZE];
	char *ptr = NULL;
	char res_array[33][BUF_SIZE];
	int * cal_array = malloc(sizeof(int)*31);
		// if(argc!=3) {
		// printf("Usage : %s <IP> <port>\n", argv[0]);
		// exit(1);
		//}

		// input = "Test Test2 Test3";
		// char test[] = "TEST1 2 3";
		// char *ptr = strtok(input, " ");
		// while(ptr!=NULL)
		//{
		// strcpy(comm[del_idx], ptr);
		// printf("%s\n", comm[del_idx]);
		// ptr = strtok(NULL, " ");
		//}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(ip);
	serv_adr.sin_port=htons(atoi(port));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
	{
		//error_handling("connect() error!");
		cal_array[0] = -1;
		return cal_array;
	}
	else
		{
		//puts("Connected......");


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

		printf("%s",message);
		
		if(add==1)
		{
			
		//printf("Added Schedule\n");
		//printf("The day that schedule exists that month: %s\n", message);
		add=  0;
		sprintf(ret, "Added Schedule\nThe day that schedule exists that month: %s\n", message);
		}
		else{
			//printf("The Schedule is: %s \n", message);
			
			printf("The Schedule is: %s \n", message);
			ptr = strtok(message, ",");
			while(ptr!=NULL)
			{
			strcpy(res_array, ptr);
			//printf("origin: %s\n", ptr);
			//printf("string: %s\n", res_array[del_idx]);
			cal_array[del_idx] = atoi(ptr);
			//printf("d: %d\n", cal_array[del_idx]);
			ptr = strtok(NULL, ",");
			del_idx++;
			}
		}
		//printf("%s\n", ret);
	}
	close(sock);


	return cal_array;
}


int* rm_client_func(char* ip, char* port, char* input)
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
	char ret[BUF_SIZE];
	char *ptr = NULL;
	char res_array[33][BUF_SIZE];
	int * cal_array = malloc(sizeof(int)*31);
		// if(argc!=3) {
		// printf("Usage : %s <IP> <port>\n", argv[0]);
		// exit(1);
		//}

		// input = "Test Test2 Test3";
		// char test[] = "TEST1 2 3";
		// char *ptr = strtok(input, " ");
		// while(ptr!=NULL)
		//{
		// strcpy(comm[del_idx], ptr);
		// printf("%s\n", comm[del_idx]);
		// ptr = strtok(NULL, " ");
		//}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(ip);
	serv_adr.sin_port=htons(atoi(port));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
	{
		//error_handling("connect() error!");
		cal_array[0] = -1;
		return cal_array;
	}
	else
		{
		//puts("Connected......");


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

		printf("%s",message);
		
		if(add==1)
		{
			
		//printf("Added Schedule\n");
		//printf("The day that schedule exists that month: %s\n", message);
		add=  0;
		sprintf(ret, "Removed Schedule\nThe day that schedule exists that month: %s\n", message);
		}
		else{
			//printf("The Schedule is: %s \n", message);
			
			printf("The Schedule is: %s \n", message);
			ptr = strtok(message, ",");
			while(ptr!=NULL)
			{
			strcpy(res_array, ptr);
			//printf("origin: %s\n", ptr);
			//printf("string: %s\n", res_array[del_idx]);
			cal_array[del_idx] = atoi(ptr);
			//printf("d: %d\n", cal_array[del_idx]);
			ptr = strtok(NULL, ",");
			del_idx++;
			}
		}
		//printf("%s\n", ret);
	}
	close(sock);


	return cal_array;
}



int* mv_client_func(char* ip, char* port, char* input)
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
	char ret[BUF_SIZE];
	char *ptr = NULL;
	char res_array[33][BUF_SIZE];
	int * cal_array = malloc(sizeof(int)*31);
		// if(argc!=3) {
		// printf("Usage : %s <IP> <port>\n", argv[0]);
		// exit(1);
		//}

		// input = "Test Test2 Test3";
		// char test[] = "TEST1 2 3";
		// char *ptr = strtok(input, " ");
		// while(ptr!=NULL)
		//{
		// strcpy(comm[del_idx], ptr);
		// printf("%s\n", comm[del_idx]);
		// ptr = strtok(NULL, " ");
		//}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(ip);
	serv_adr.sin_port=htons(atoi(port));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
	{
		//error_handling("connect() error!");
		cal_array[0] = -1;
		return cal_array;
	}
	else
		{
		//puts("Connected......");


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

		printf("%s",message);
		
		if(add==1)
		{
			
		//printf("Added Schedule\n");
		//printf("The day that schedule exists that month: %s\n", message);
		add=  0;
		sprintf(ret, "Removed Schedule\nThe day that schedule exists that month: %s\n", message);
		}
		else{
			//printf("The Schedule is: %s \n", message);
			
			printf("The Schedule is: %s \n", message);
			ptr = strtok(message, ",");
			while(ptr!=NULL)
			{
			strcpy(res_array, ptr);
			//printf("origin: %s\n", ptr);
			//printf("string: %s\n", res_array[del_idx]);
			cal_array[del_idx] = atoi(ptr);
			//printf("d: %d\n", cal_array[del_idx]);
			ptr = strtok(NULL, ",");
			del_idx++;
			}
		}
		//printf("%s\n", ret);
	}
	close(sock);


	return cal_array;
}







void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

