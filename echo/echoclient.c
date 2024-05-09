#include "csapp.h"

int main(int argc, char **argv) // argc : 입력받은 인자의 수, argv : 입력받은 인자들의 배열
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != 3) { // 파일 실행 시 인자를 제대로 넘겨주지 않았을 경우
        fprintf(stderr, "usage : %s <host> <port>\n", argv[0]); // 안내 메세지 출력
        exit(0); // 앱 종료
    }
    host = argv[1]; // 1번 인자를 host에 저장
    port = argv[2]; // 2번 인자를 port에 저장

    clientfd = Open_clientfd(host, port); // Open_clientfd로 서버와 연결, 리턴받은 소켓 식별자를 clientfd에 저장
    Rio_readinitb(&rio, clientfd); // rio 구조체를 초기화하고, rio를 통해 clientfd에 대한 읽기 작업을 수행할 수 있도록 설정

    while (Fgets(buf, MAXLINE, stdin) != NULL) { // 반복하여 유저에게서 받은 입력을 buf에 저장하는데, 입력이 끊기거나 오류가 발생한다면 반복문을 종료
        Rio_writen(clientfd, buf, strlen(buf)); // clientfd를 통해 buf에 저장된 데이터를 서버로 전송
        Rio_readlineb(&rio, buf, MAXLINE); // rio 구조체를 통해 clientfd에서 문자열 한 줄을 읽어와 buf에 저장, MAXLINE은 buf의 최대 크기
        Fputs(buf, stdout); // buf에 저장된 문자열을 표준 출력 stdout에 출력
    }
    Close(clientfd); // clientfd를 닫아서 클라이언트의 연결을 종료하고 사용한 리소스를 반환
    exit(0);
}