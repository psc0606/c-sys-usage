/*----------------------------------------
2. 创建两个进程：
a) 父进程执行文件拷贝操作。（为了更好验证，请选择大小在M级以上文件）。如果接受到SIGUSR1信号，将打印出当前拷贝进度
b) 子进程每隔一个固定时间向父进程发生SIGUSR1信号。
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

long int size = 0;
long int asize = 0;

void sig_alarm(int sigo)
{
	printf("%ld/%ld,%4.2f%\n",asize,size,1.0*asize/size*100);
}

void cp_file(char **argv)
{
	struct stat buf;
	int fd1, fd2, n;
	char data[100];
	if (stat(argv[1],&buf) < 0)
	{
		perror("stat err");
		exit(0);
	}
	if ((fd1 = open(argv[1],O_RDONLY)) < 0)
	{
		perror("open 1 err");
		exit(0);
	}
	umask(0000);
	if ((fd2 = open(argv[2],O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
	{
		perror("open 2 err");
		exit(0);
	}
	size = buf.st_size;
	sleep(2);
	while (1)
	{
		if ((n = read(fd1,data,sizeof(data))) < 0)
		{
			perror("read err");
			exit(0);
		}
		if (n == 0)
		{
			break;
		}
		if (write(fd2,data,n) != n)
		{
			perror("write err");
			exit(0);
		}
		asize += n;
		sleep(1);
	}
	close(fd1);
	close(fd2);
}

int main(int argc,char **argv)
{
	if (argc != 3)
	{
		perror("format err");
		exit(0);
	}
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		perror("fork err");
		exit(0);
	}
	else if (pid == 0)
	{
		pid_t ppid = getppid();
		sleep(1);
		while (1)
		{
			if (kill(ppid,SIGUSR1) < 0)
			{
				perror("kill err");
				exit(0);
			}
			sleep(1);
		}
	}
	else
	{
		struct sigaction sig_act;
		sig_act.sa_handler = sig_alarm;
		if (sigaction(SIGUSR1,&sig_act,NULL) < 0)
		{
			perror("sigaction err");
			exit(0);
		}
		
		cp_file(argv);
		
		if (kill(pid,SIGKILL) < 0)
		{
			perror("kill err");
			exit(0);
		}
		wait(NULL);
	}
}
