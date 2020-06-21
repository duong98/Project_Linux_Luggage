/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <string.h> */
/* #include <sys/types.h> */
/* #include <sys/socket.h> */
/* #include <netinet/in.h> */
/* #include <netdb.h> */
/* #include <unistd.h> */
/* #define SERVER_ADDR “localhost” */
/* #define SERVER_PORT 5678 */
/* int main(void) */
/* { */
/* int iSock = 0; */
/* int iClientSock = 0; */
/* int n,bytes_to_read=255,bytes_to_write=255; */
/* struct sockaddr_in serverData= {0,0,""}; */
/* struct sockaddr_in clientData= {0,0,""}; */
/* int iClientSize = 0; */
/* char buffer[256],*bp; */
/* if( (iSock=socket(AF_INET, SOCK_STREAM,0)) == -1){ */
/* perror("socket"); */
/* exit(1); */
/* } */
/*  // Identifier making */
/* serverData.sin_family = AF_INET; */
/* serverData.sin_addr.s_addr = INADDR_ANY; */
/* serverData.sin_port = SERVER_PORT; */
/* if ( bind(iSock, (struct sockaddr*)&serverData,sizeof(serverData)) == -1 ){ */
/*   perror("bind"); */
/*   shutdown(iSock,SHUT_RDWR); */
/*   close(iSock); */
/*   exit(1); */
/* } */
/* // Connection setup */
/* if ( listen(iSock,1) == -1){ */
/*   perror ("listen"); */
/*   shutdown(iSock,SHUT_RDWR); */
/*   close(iSock); */
/*   exit(1); */
/* } */
/* iClientSize = sizeof(clientData); */
/* if ((iClientSock = accept(iSock,(struct sockaddr*)&clientData,(socklen_t *)&iClientSize)) == -1){ */
/*   perror ("listen"); */
/*   shutdown(iSock,SHUT_RDWR); */
/*   close(iSock); */
/*   exit(1); */
/* } */
/* bzero(buffer,256); */
/* bp=buffer; */
/* /\* while ((n=(read(iClientSock,bp,bytes_to_read)))>0){ *\/ */
/*   n=read(iClientSock,bp,bytes_to_read); */
/*   if(n == -1){ */
/*     perror("ERROR reading from socket"); */
/*     shutdown(iSock,SHUT_RDWR); */
/*     close(iSock); */
/*     exit(1); */
/*     }; */
/*   if (n == 0){ */
/*     perror("ERROR client disconnected"); */
/*     shutdown(iSock,SHUT_RDWR); */
/*     close(iSock); */
/*     exit(1); */
/*     } */
/*   printf("Here is the message: %s\n",buffer); */
/*   bp+=n; */
/*   bytes_to_read-=n; */
/*   //sleep(5); */
/*   if(write(iClientSock,"Message Received",bytes_to_write) == -1){ */
/*     perror("ERROR reading from socket"); */
/*     shutdown(iSock,SHUT_RDWR); */
/*     close(iSock); */
/*     exit(1); */
/*     }; */
/* return 0; */
/* } */
/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");
     bzero(buffer,256);
     for(;;){
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     if (n==0) {
       error("client disconnected");
       break;
     }
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     if (n==0) {
       error("client disconnected");
       break;
     }
     }
     close(newsockfd);
     close(sockfd);
     return 0;
}
