#include <stdio.h>
#include <signal.h>

FILE *popen2(const char*, const char*);

#define BUFF_SIZE 100
#define PIPE_ENDS 2

int main(){
	FILE *pipein_fp, *pipeout_fp;
	char readbuf[BUFF_SIZE];
	
	if( (pipein_fp = popen2("ls", "r")) == NULL){
		perror("popen2");
		exit(1);
	}

	if( (pipeout_fp = popen2("sort -r", "w")) == NULL){
		perror("popen2");
		exit(1);
	}

	while( fgets(readbuf, BUFF_SIZE, pipein_fp))
		fputs(readbuf, pipeout_fp);

	pclose(pipein_fp);
	pclose(pipeout_fp);
	sleep(1);

	return 0;
}

FILE *popen2(const char *command, const char *mode){

	int apipe[PIPE_ENDS];
	int parent = 0;
	int child = 1;
	
	if(mode=='r')
	{
		parent = 0;
		child = 1;
	}
	else if(mode=='w')
	{
		child = 0;
		parent =1 ;
	}
	
	
	pipe(apipe);
	
	pid = fork();
	if(pid>0)
	{
		close(child);
		return fdopen(apipe[parent], mode);
	}
	else if(pid==0)
	{
		close(parent);
		return fdopen(apipe[child], mode);







}
