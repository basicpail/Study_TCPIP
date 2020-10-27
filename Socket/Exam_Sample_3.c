#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>


char webpage[]="HTTP/1.1 200 OK\r\n"
	"Server:Linux Web Server\r\n"
	"Content-Type:text/html; charset=UTF-8\r\n\r\n"
	"<!DOCTYPE html>\r\n"
	"<html><head><title>SANG_GEUN, SONG </title>\r\n"
	"<style>body{background-color: #FFFF00}</style></head>\r\n"
	"<body><center><h1>Hello world!!</h1><br>\r\n"
	"<img src=\"images.jpg\"></center></body></html>\r\n";

int main(int argc, char *argv[])
{
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t sin_len = sizeof(clnt_addr);
	int serv_sock, clnt_sock;
	char buf[2048];
	int fdimg;
	int option = TRUE;
	char img_buf[7000000];

	serv_sock = socket(AF_INET, SOCK_STREAM,0);

	//setsockopt(serv_sock, SOL_SOCKET,SO_REUSEADDR,&option,sizeof(int));

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		perror("bind() error!");
	if(listen(serv_sock,5) == -1) perror("listen() error!");

	while(1){
		clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&sin_len);
		puts("New client connection..");

		if(!fork()){
			close(serv_sock);

			//memset(buf,0,2048);
			//read(clnt_sock,buf,2047);
            read(clnt_sock,buf,sizeof(buf)-1);
			//printf("%s\n",buf);

			if(!strncmp(buf,"GET/images.jpg",13)){
			    fdimg = open("images.jpg",O_RDONLY);

				read(fdimg,img_buf,sizeof(img_buf));
				write(clnt_sock,img_buf,sizeof(img_buf));
				close(fdimg);
			}
			else
				write(clnt_sock,webpage,sizeof(webpage));
			close(clnt_sock);
			puts("closing...");
			exit(1);
		}
		close(clnt_sock);
}