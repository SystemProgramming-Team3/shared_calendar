#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <dirent.h>
#include <errno.h>
#define BUF_SIZE 1024
#define MAX_FILE_CONTENTS_LENGTH 128

void error_handling(char*);
char* isMonDir(char*); // 2023.04 디렉토리 안에 디렉토리를 탐색해서 어느 날에 일정이 있는지 반환
char* isDayDir(char*); // 2023.04 -> 07 -> 텍스트 파일의 제목과 내용 반환 
void addSchedule(char*, char*);
void rmSchedule(char*, char*);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	char* d;

	char* days;
	char* txtfile;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while (1) {
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;

		if (fd_num == 0)
			continue;

		for (i = 0; i < fd_max + 1; i++) {
			if (FD_ISSET(i, &cpy_reads)) {
				if (i == serv_sock) {
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads);
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
					printf("connected client: %d \n", clnt_sock);
				}
				else {
					str_len = read(i, buf, BUF_SIZE);

					if (str_len == 0) { // close(client)
						FD_CLR(i, &reads);
						close(i);
						printf("closed client: %d \n", i);
					}

					buf[str_len - 1] = '\0';
					char buf2[BUF_SIZE] = "\0";
					strcpy(buf2, buf);

					char delimeter[] = " "; // 띄어쓰기로 parsing
					char parsing[4][50];
					char* ptr;
					int p = 0;

					ptr = strtok(buf, delimeter);

					while (ptr != NULL) {
						strcpy(parsing[p++], ptr);
						ptr = strtok(NULL, delimeter);
					}

					if (strcmp(parsing[0], "cc") == 0) {
						days = isMonDir(parsing[1]);  // "cc 2023.04" 입력 받으면 days에 "1,2,15,31," 저장. if 2023.04 디렉토리 없으면 생성하고 ","만 저장.
						write(i, days, strlen(days)); // ex) server->client ",1,2,15,31," 
					}
					else if (strcmp(parsing[0], "c") == 0) {
						txtfile = isDayDir(parsing[1]); // "c 2023.04.07" 입력 받으면 일정 제목(.txt)과 txt 내용 반환. if 07 디렉토리 없으면 ">" 만 저장.
						write(i, txtfile, strlen(txtfile)); // serve->client ">PT.txt 13:00~14:00 GYM >birthday.txt 09:00~10:00"
					}
					else if (strcmp(parsing[0], "a") == 0) {  // "a 2023.04.07 key>contents" 입력 받으면 일정 key(.txt)와 contents 저장.
						addSchedule(parsing[1], &buf2[13]);
						days = isMonDir(parsing[1]);
						write(i, days, strlen(days));
					}
					else if (strcmp(parsing[0], "rm") == 0) { // "rm 2023.04.07 birthday" birthday.txt 삭제, 삭제 후 7일에 일정이 하나도 없다면 07폴더도 삭제
						rmSchedule(parsing[1], parsing[2]);
						days = isMonDir(parsing[1]);
						write(i, days, strlen(days));
					}
				}
			}
		}
	}

	return 0;
}

void error_handling(char* buf) {
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}

char* isMonDir(char* tmpfile) {
	DIR* dir;
	DIR* dir2;
	struct dirent* direntp;
	struct dirent* direntp2;
	char dayname[1024] = ",";
	char filename[1024] = "\0";
	int i;
	char* ptr = ",";

	for (i = 0; i < 7; i++) filename[i] = tmpfile[i];

	dir = opendir("."); // 현재 디렉토리를 열기-[

	if (dir == NULL) {
		printf("디렉토리를 열 수 없습니다.\n");
		return NULL;
	}

	while ((direntp = readdir(dir)) != NULL) {
		if (strcmp(direntp->d_name, filename) == 0) { // 파일 이름이 일치하는 경우
			closedir(dir);
			dir2 = opendir(filename);

			while (direntp2 = readdir(dir2)) {
				if (strcmp(direntp2->d_name, ".") == 0 || strcmp(direntp2->d_name, "..") == 0) {
					continue;
				}
				ptr = strcat(dayname, direntp2->d_name);
				ptr = strcat(dayname, ",");
			}
			closedir(dir2);
			return ptr; // 2023.04 디렉토리 안에 1, 3, 10, 31 디렉토리 있다면 ",1,3,10,31" 반환 
		}
	}

	mkdir(filename, 0755); // 2023.04 디렉토리 없으면 만들기
	closedir(dir);
	return ptr; // 2023.04 디렉토리 안에 일정 없으니까 ","만 반환
}

char* isDayDir(char* tmpfile) {
	DIR* dir;

	struct dirent* direntp;
	struct dirent* direntp2;
	char schedule[1024] = ">"; // scheule '>'로 구분
	char filename[1024] = "\0";
	char dayname[1024] = "\0";
	char txtname[1024] = "\0";
	char contents[1024] = "\0";

	int i;
	char* ptr = "";

	for (i = 0; i < 7; i++) filename[i] = tmpfile[i];
	filename[7] = '/';
	for (i = 8; i < 10; i++) dayname[i - 8] = tmpfile[i];

	strcat(filename, dayname);
	dir = opendir(filename); // 현재 디렉토리를 열기

	if (dir == NULL) {
		//printf("해당하는 날짜에 일정이 없습니다.\n");
		return ">";
	}

	while ((direntp = readdir(dir)) != NULL) { // 폴더 안의 파일들 검색
		if (direntp->d_type == DT_REG && strstr(direntp->d_name, ".txt") != NULL) { // 텍스트 파일인 경우
			strcpy(txtname, filename);
			strcat(txtname, "/");
			strcat(txtname, direntp->d_name);
			FILE* fp = fopen(txtname, "r"); // 파일 열기

			if (fp == NULL) {
				perror("fopen error");
				return "error";
			}
			strcat(schedule, direntp->d_name);
			strcat(schedule, " ");

			while (fgets(contents, MAX_FILE_CONTENTS_LENGTH, fp) != NULL) { // 파일 내용 출력
				strcat(schedule, contents);
			}
			ptr = strcat(schedule, ">");
			fclose(fp); // 파일 닫기
		}
	}

	closedir(dir);

	return ptr; // 반환
}

void addSchedule(char* tmpfile, char* contents){
	DIR* dir;
	FILE* fp;

	char filename[1024] = "\0";
	char dayname[1024] = "\0";
	char txtname[1024] = "\0";

	char delimeter[] = ">"; //  parsing
	char parsing[2][50];
	char* ptr;
	int p = 0;
	int i;

	for (i = 0; i < 7; i++) {
		filename[i] = tmpfile[i];
	}
	filename[7] = '/';
	for (i = 8; i < 10; i++) {
		dayname[i - 8] = tmpfile[i];
	}


	strcat(filename, dayname);

	dir = opendir(filename); // 일정을 넣을 날짜 디렉토리 열기

	if (dir == NULL) { 
		mkdir(filename, 0755);
		dir = opendir(filename);
	}

	ptr = strtok(contents, delimeter);

	while (ptr != NULL) {
		strcpy(parsing[p++], ptr);
		ptr = strtok(NULL, delimeter);
	}

	strcat(txtname, filename);
	strcat(txtname, "/");
	strcat(txtname, parsing[0]);
	strcat(txtname, ".txt");
	creat(txtname, 0755);

	puts(parsing[1]);
	fp = fopen(txtname, "w+");
	fputs(parsing[1], fp);
	
	closedir(dir);
	close(fp);
	return;
}

void rmSchedule(char* tmpfile, char* contents) {
	DIR* dir;

	struct dirent* direntp;

	char filename[1024] = "\0";
	char filename2[1024] = "\0";
	char dayname[1024] = "\0";
	char txtname[1024] = "\0";

	int i;
	int file_count = 0;
	char* ptr = "";

	for (i = 0; i < 7; i++) filename[i] = tmpfile[i];
	filename[7] = '/';
	for (i = 8; i < 10; i++) dayname[i - 8] = tmpfile[i];
	strcat(filename, dayname);

	dir = opendir(filename); // 해당하는 날짜의 디렉토리 열기

	if (dir == NULL) {
		//printf("Failed to open directory\n"\n");
		return;
	}

	strcat(txtname, contents);
	strcat(txtname, ".txt");

	strcat(filename2, filename);
	strcat(filename2, "/");
	strcat(filename2, txtname);
	remove(filename2);

	while ((direntp = readdir(dir)) != NULL) {
		if (direntp->d_type == DT_REG) file_count++;	
	}

	if (file_count == 0) rmdir(filename);

	return;
}
