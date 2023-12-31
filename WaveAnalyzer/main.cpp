#include <iostream>
#include <math.h>
#include "dfft.h"
#include "Render.h"
#include "wave.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <openssl/sha.h>

#define PORT 38919
#define MAXLENGTH 1024+1

void parsestr(char *request,char *data)
{
    int needle;
    strcat(request,"HTTP/1.1 101 WebSocket Protocol Handshake\r\n");
    strcat(request,"Upgrade:WebSocket\r\n");
    strcat(request,"Connection:Upgrade\r\n");
    //这个值等于 base64_encode(sha1(key+258EAFA5-E914-47DA-95CA-C5AB0DC85B11)) 由于我这里没有合适的base64算法和sha1算法，所以就不写了。
    strcat(request,"Sec-WebSocket-Accept:ZmQ5OWUxMjgwMTViNTEyM2FmZTRlOGViODZkNTk3OTBjMWRiYjBiYg==\r\n");
}

int websocket(int argc,char ** argv)
{
    int sockfd, maxfd, ret, retval, newfd;
    socklen_t len;
    int reuse = 1;
    fd_set rwfd;
    struct sockaddr_in l_addr;
    struct sockaddr_in c_addr;
    struct timeval tv;
    char buf[MAXLENGTH];
    char request[MAXLENGTH];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    if (sockfd < 0)
        perror("socket");
    l_addr.sin_port = htons(PORT);
    l_addr.sin_family = AF_INET;
    l_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *) &l_addr, sizeof(struct sockaddr)) < 0)
        perror("bind");
    listen(sockfd, 5);
    len = sizeof(struct sockaddr);
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    bzero(request, MAXLENGTH);
    while (1) {
        newfd = accept(sockfd, (struct sockaddr *) &c_addr, &len);
        if (newfd == -1) {
            perror("accept");
            exit(1);
        } else {
            printf("%s is comming\n", inet_ntoa(c_addr.sin_addr));
        }
        while (1) {
            FD_ZERO(&rwfd);
            FD_SET(0, &rwfd);
            FD_SET(newfd, &rwfd);
            maxfd = 0;
            if (newfd > maxfd)
                maxfd = newfd;
            retval = select(maxfd + 1, &rwfd, NULL, NULL, &tv);
            if (retval == -1) {
                perror("select");
                break;
                /*  }else if(retval==0){
                        printf("no data\n");
                        continue;
*/
            } else {
                if (FD_ISSET(0, &rwfd)) {
                    bzero(buf, MAXLENGTH);
                    fgets(buf, MAXLENGTH, stdin);
                    parsestr(request, buf);
                    len = send(newfd, request, MAXLENGTH, 0);
                    if (len > 0)
                        printf("i sayed:%s\n", buf);
                }
                if (FD_ISSET(newfd, &rwfd)) {
                    len = recv(newfd, buf, MAXLENGTH, 0);
                    if (len > 0) {
                        if (strncmp(buf, "quit", 4) == 0) {
                            close(newfd);
                        }
                    } else {
                        printf("client says:%s\n", buf);
                    }
                }
            }
        }
    }
}

int main() {

    int nn = (int)pow(2, 10);

    dfft* _dfft = new dfft(nn);
    wave* _wave = new wave();

    _wave->load("/Users/Sonic/Desktop/Sqr_1000Hz_wavTones.wav");

    printf("%d,%d\r\n", _wave->m_record_file.bytes_per_sample, _wave->m_record_file.sample_rate);


    for(int i = 0; i < nn; i++)
    {
        printf("%4.2f,", _wave->m_record_file.channels[0][10000+i]);
    }

    printf("\r\n");

    for(int i = 0; i < nn; i++)
    {
        _dfft->add_point(i, _wave->m_record_file.channels[0][10000+i], 0);
    }

    _dfft->perform(1);

    render::perfom(nn, _dfft->get_data());



    return 0;
}

