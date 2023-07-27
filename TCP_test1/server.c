#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main(void)
{
    int lfd = 0, cfd = 0;
    int ret = 0;
    char buf[BUFSIZ], client_IP[1024];

    struct sockaddr_in serv_addr, clit_addr;
    socklen_t clit_addr_len;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9527);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        sys_err("socket");
    }

    bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(lfd, 128);

    clit_addr_len = sizeof(clit_addr);
    cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);
    if (cfd == -1)
    {
        sys_err("accept");
    }

    printf("client ip:%s  port:%d\n",
           inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)),
           ntohs(clit_addr.sin_port));

    while (1)
    {
        ret = read(cfd, buf, sizeof(buf));

        printf("%s\n", buf);

        for (int i = 0; i < ret; i++)
        {
            buf[i] = toupper(buf[i]);
        }
        write(cfd, buf, ret);
    }

    close(cfd);
    close(lfd);
}