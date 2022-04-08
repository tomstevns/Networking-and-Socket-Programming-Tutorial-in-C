#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <winsock.h>
struct WSAData wsaData;
void init_socket_lib() { WSAStartup(MAKEWORD(1, 1), &wsaData); }
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
typedef int SOCKET;
#define INVALID_SOCKET -1
#define closesocket(s) close(s)
void init_socket_lib() {}
#endif

int main(void)
{
  SOCKET listenfd = INVALID_SOCKET, connfd = INVALID_SOCKET;

  struct sockaddr_in serv_addr;

  char sendBuff[1500]; // to fit better within the Ethernet MTU
  int numrv;

  init_socket_lib();
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");

  memset(&serv_addr, 0, sizeof(serv_addr));
  memset(sendBuff, 0, sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  if(bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) != 0)
    {
      printf("Failed to bind\n");
      return 1;
    }

  if(listen(listenfd, 10) == -1)
    {
      printf("Failed to listen\n");
      return 1;
    }

  while(1)
    {
      connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request

      strcpy(sendBuff, "Message from server");
      send(connfd, sendBuff, strlen(sendBuff), 0);

      shutdown(connfd, 2);  // shutdown input and output streams, i.e. close TCP connection
      closesocket(connfd);  // free fd

      sleep(1);
    }


  return 0;
}
