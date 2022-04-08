#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#include <winsock.h>
struct WSAData wsaData;
void init_socket_lib() { WSAStartup(MAKEWORD(1, 1), &wsaData); }
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
typedef int SOCKET;
#define INVALID_SOCKET -1
#define closesocket(s) close(s)
void init_socket_lib() {}
#endif

int main(void)
{
  SOCKET sockfd = INVALID_SOCKET;
  int n = 0;
  char recvBuff[1500]; // to fit better within the Ethernet MTU
  struct sockaddr_in serv_addr;

  init_socket_lib();
  memset(recvBuff, 0, sizeof(recvBuff));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
      printf("\n Error : Could not create socket \n");
      return 1;
    }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    {
      printf("\n Error : Connect Failed \n");
      return 1;
    }

  while((n = recv(sockfd, recvBuff, sizeof(recvBuff)-1, 0)) > 0)
    {
      recvBuff[n] = 0;
      if(fputs(recvBuff, stdout) == EOF)
      {
        printf("\n Error : Fputs error");
      }
      printf("\n");
    }

  if( n < 0)
    {
      printf("\n Read Error \n");
    }

  shutdown(sockfd, 2);  // shutdown input and output streams, i.e. close TCP connection
  closesocket(sockfd);  // free fd

  return 0;
}
