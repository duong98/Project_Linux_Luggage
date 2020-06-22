/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <string.h> */
/* #include <sys/types.h> */
/* #include <sys/socket.h> */
/* #include <netinet/in.h> */
/* #include <netdb.h> */
/* #include <unistd.h> */
/* #define SERVER_ADDR "localhost" */
/* #define SERVER_PORT 5678 */
/* int main() */
/* { */
/* int iSock = 0; */
/* char buffer[256]="test",*bp; */
/* int n,bytes_to_read=255,bytes_to_write=255; */
/* struct sockaddr_in serverData = {0,0,""}; */
/* struct hostent *hp; */
/* // Creating socket */
/* if ( (iSock=socket(AF_INET,SOCK_STREAM,0)) == -1 ){ */
/*   perror("socket"); */
/*   exit(1); */
/* } */
/* if( (hp=gethostbyname(SERVER_ADDR)) == NULL ){ */
/*   perror ("Cannot get host\n"); */
/*   shutdown(iSock,SHUT_RDWR); */
/*   close(iSock); */
/*   exit(1); */
/* } */
/* memcpy( (char*)&serverData.sin_addr, (char*)hp->h_addr, hp->h_length ); */
/* serverData.sin_family=AF_INET; */
/* serverData.sin_port=SERVER_PORT; */
/* // Begin the connection */
/* if ( connect(iSock, (struct sockaddr*)&serverData,sizeof(serverData)) == -1 ){ */
/*   perror("connect"); */
/*   shutdown(iSock,SHUT_RDWR); */
/*   close(iSock); */
/*   exit(1); */
/* } */
/* bp=buffer; */
/* bzero(buffer,256); */
/* /\* while ((n=(read(iSock,bp,bytes_to_read)))>0){ *\/ */
/* /\*   if(n == -1){ *\/ */
/* /\*     perror("ERROR reading from socket"); *\/ */
/* /\*     shutdown(iSock,SHUT_RDWR); *\/ */
/* /\*     close(iSock); *\/ */
/* /\*     exit(1); *\/ */
/* /\*     }; *\/ */
/* /\*   if (n == 0){ *\/ */
/* /\*     perror("ERROR client disconnected"); *\/ */
/* /\*     shutdown(iSock,SHUT_RDWR); *\/ */
/* /\*     close(iSock); *\/ */
/* /\*     exit(1); *\/ */
/* /\*     } *\/ */
/* /\*   printf("Here is the message: %s\n",buffer); *\/ */
/* /\*   bp+=n; *\/ */
/* /\*   bytes_to_read-=n; *\/ */
/* /\*   sleep(5); *\/ */
/* /\*   if(write(iSock,"test",bytes_to_write) == -1){ *\/ */
/* /\*     perror("ERROR reading from socket"); *\/ */
/* /\*     shutdown(iSock,SHUT_RDWR); *\/ */
/* /\*     close(iSock); *\/ */
/* /\*     exit(1); *\/ */
/* /\*     }; *\/ */
/* /\*  } *\/ */
/* if(write(iSock,"test",strlen(buffer)) == -1){ */
/*   perror("ERROR reading from socket"); */
/*   exit(1); */
/*  }; */

/* if(read(iSock,buffer,bytes_to_read) == -1){ */
/*   perror("ERROR reading from socket"); */
/*   exit(1); */
/*  }; */
/* printf("Here is the message: %s\n",buffer); */
/* // End of the connection */
/* /\* shutdown(iSock,SHUT_RDWR); *\/ */
/* /\* close(iSock); *\/ */
/* return 0; */
/* } */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define BUFFSIZE 1024
#define USERLEN 32
#define OPTLEN 16
struct  message{
  char option[OPTLEN];
  char user[USERLEN];
  char buff[BUFFSIZE];
  char target[USERLEN];
  int sockid;
};
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    struct message m;
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    for(;;){
      sleep(3);
      /* printf("Please enter the message: "); */
      bzero(buffer,256);
      /* fgets(buffer,255,stdin); */
      strcpy(m.option,"123");
      strcpy(m.buff,"12346");
      strcpy(m.user,"1234");
      strcpy(m.target,"12345");
      m.sockid = sockfd;
      n = write(sockfd,&m,sizeof(struct message));
      if (n==0) {
         error("server disconnected");
         break;
       }
      if (n < 0)
         error("ERROR writing to socket");
      bzero(buffer,256);
      n = read(sockfd,buffer,255);
      if (n < 0)
         error("ERROR reading from socket");
      printf("%s\n",buffer);
      if (n==0) {
         error("server disconnected");
         break;
       }
    }
    close(sockfd);
    return 0;
}
