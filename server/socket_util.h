#include <signal.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>

int atoport( char *service, char *proto );
struct in_addr *atoaddr( char *address);
int get_connection( int socket_type, u_short port, int *listener );
int make_connection( char *service, int type, char *netaddress );
int sock_read( int sockfd, char *buf, size_t count );
int sock_write( int sockfd, const void *buf, size_t count );
int sock_gets( int sockfd, void *str, size_t count );
int sock_puts( int sockfd, void  *str , size_t size);
void ignore_pipe(void);

