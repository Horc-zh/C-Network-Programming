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

    int cfd = 0;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ];

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9527);           //要求与服务器端口号一致
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1)
    {
        sys_err("socket");
    }

    int ret = connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret != 0)
    {
        sys_err("connnet");
    }

    int count = 10;
    while (count--)
    {
        write(cfd, "hello", 5);
        ret = read(cfd, buf, sizeof(buf));
        printf("%s\n", buf);
        sleep(1);
    }
    close(cfd);

    return 0;
}
