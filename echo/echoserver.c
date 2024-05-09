#include "csapp.h"

void echo(int connfd) { // 클라이언트와 통신하는 함수 echo를 선언
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %d bytes\n", (int)n);
        Rio_writen(connfd, buf, n);
    }
}

int main(int argc, char **argv)
{
    int listenfd, connfd; // listen file descriptor, connect file descriptor
    socklen_t clientlen; // client의 주소 길이
    struct sockaddr_storage clientaddr; // 클라이언트의 주소 정보를 저장하는 구조체
    char client_hostname[MAXLINE], client_port[MAXLINE]; // 클라이언트의 호스트 이름과 포트 번호를 저장하는 배열

    if (argc != 2) { // 프로그램 실행 시 포트 번호를 인자로 입력하지 않았을 경우
        fprintf(stderr, "usage: %s <port>\n", argv[0]); // 사용법을 출력하고
        exit(0); // 프로그램 종료
    } 

    listenfd = Open_listenfd(argv[1]); // 주어진 포트 번호로 listen 소켓을 열어서 listenfd 생성
    while (1) { // 무한 루프
        clientlen = sizeof(struct sockaddr_storage); // 클라이언트의 주소 길이를 초기화
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // 클라이언트의 연결을 받아들이고, 연결된 클라이언트 소켓의 fd를 얻음
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0); // 클라이언트의 호스트 이름과 포트 번호를 얻음
        printf("Connected to (%s, %s)\n", client_hostname, client_port); // 연결된 클라이언트의 정보를 출력
        echo(connfd); // 클라이언트와 통신하는 echo 함수 호출
        Close(connfd); // 클라이언트 소켓을 닫음
    }
    exit(0);
}