//多进程版
//2023.7.31  22:42

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

void catch_child(int signum)
{
    while (waitpid(0, NULL, WNOHANG) > 0)
        ;
    return;
}

int main(void)
{
    int lfd = 0;
    int cfd = 0;
    pid_t pid = 0;
    struct sockaddr_in srv_addr, clt_addr;
    socklen_t clt_addr_len;
    int ret, i;
    char buf[BUFSIZ], client_IP[1024];

    memset(&srv_addr, 0, sizeof(srv_addr));

    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(80);

    lfd = socket(AF_INET, SOCK_STREAM, 0);

    ret = bind(lfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));

    listen(lfd, 128);
    clt_addr_len = sizeof(clt_addr);
    while (1)
    {
        cfd = accept(lfd, (struct sockaddr *)&clt_addr, &clt_addr_len);

        // printf("asdasdasdasd\n");

        printf("client ip:%s  port:%d\n",
               inet_ntop(AF_INET, &clt_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)),
               ntohs(clt_addr.sin_port));

        pid = fork();
        if (pid < 0)
        {
            sys_err("fork");
        }
        else if (pid == 0)
        {
            close(lfd);
            break;
        }
        else
        {
            struct sigaction act_;
            act_.sa_handler = catch_child;
            sigemptyset(&act_.sa_mask);
            act_.sa_flags = 0;

            sigaction(SIGCHLD, &act_, NULL);

            close(cfd);
            continue;
        }
    }
    if (pid == 0)
    {
        while (1)
        {
            ret = read(cfd, buf, sizeof(buf));

            // printf("client: %S\n",buf);

            if (ret == 0)
            {
                close(cfd);
                exit(1);
            }

            for (i = 0; i < ret; i++)
            {
                buf[i] = toupper(buf[i]);
                write(cfd, buf, ret);
                write(STDOUT_FILENO, buf, ret);
            }
        }
    }
}
