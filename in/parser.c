#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>
#include <signal.h>

int main(int argc, char *argv[]) {

	int fd[2];
	pipe(fd);

	signal(SIGINT, SIG_IGN);

	if (fork() == 0) {
		close(fd[0]);
		dup2(fd[1], 1);
		execlp("find", "find", ".", "-name", "*.tsp", NULL);
		close(fd[1]);
	}
	else {
		close(fd[1]);
		dup2(fd[0], 0);
		wait(NULL);
		char buf[128], c;
		int n;
		int count = 0;
		while ((n=read(0,&c,1)) > 0) {
			if (c != '\n') {
				buf[count++] = c;
			}
			else {
				buf[count] = '\0';
				printf("%s\n", buf);
				count = 0;
				int fd2[2];
				pipe(fd2);
				if (fork() == 0) {
					close(fd[0]);
					dup2(fd2[1], 1);
					execlp("more", "more", buf, NULL);
					close(fd2[1]);
				}
				else {
				//	wait(NULL);
					close(fd2[1]);
					int fd3[2];
					pipe(fd3);
					if (fork() == 0) {
						dup2(fd2[0], 0);
						close(fd3[0]);
						dup2(fd3[1], 1);
						execl("../generator_tsplib", "../generator_tsplib", NULL);
						printf("jest dobrze");
						close(fd3[1]);
						close(fd2[0]);
					}
					else {
						wait(NULL);
						close(fd2[0]);
						close(fd3[1]);
						dup2(fd3[0], 0);
						char path[128];
						printf("buf: %s\n", buf);
						int i;
						path[0] = '.';
						path[1] = '/';
						for (i=2; buf[i] != '.'; i++) {
							path[i] = buf[i];
						}
						path[i] = '.';
						path[i+1] = 'i';
						path[i+2] = 'n';
						path[i+3] = '\0';
						printf("%s\n", path);
						int file = open(path, O_WRONLY|O_CREAT|O_TRUNC, 0664);
						int n1;
						int buf2[128];
						while ((n1 = read(0, buf2, 128)) > 0) {
							write(file, buf2, n1);
				//			write(1, buf2, n1);
						}
						close(file);
						close(fd3[0]);
						puts("ok");
					}
				}
			}
		}
	}

	return 0;
}
