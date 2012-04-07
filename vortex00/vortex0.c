#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
 
#define HOST "vortex.labs.overthewire.org"
#define PORT "5842"
 
#define BUF_SIZ 100
 
void *get_in_addr(struct sockaddr *sa) {
 if (sa->sa_family == AF_INET) {
  return &(((struct sockaddr_in*)sa)->sin_addr);
 }
 return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
 
int main (int argc, const char * argv[]) {
 
 struct addrinfo hints, *res, *p;
 int numbytes, totalbytes, status, i;
 uint32_t sum, *in;
 char ipstr[INET6_ADDRSTRLEN];
 char buf[BUF_SIZ];
 int sockfd = 0;
  
 memset(&hints, 0, sizeof(hints));
 hints.ai_family = AF_INET;
 hints.ai_socktype = SOCK_STREAM;
  
 if ((status = getaddrinfo(HOST, PORT, &hints, &res)) != 0) {
  fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
  return 1;
 }
  
 for (p = res; p != NULL; p = p->ai_next) {
  if ((sockfd = socket(PF_INET, SOCK_STREAM, p->ai_protocol)) == -1) {
   perror("client: socket");
   continue;
  }
   
  if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
   close(sockfd);
   perror("client: connect");
   continue;
  }
   
  break;
 }
  
 if (p == NULL) {
  fprintf(stderr, "client: failed to connect.\n");
  return 1;
 }
  
 inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr), ipstr, sizeof(ipstr));
 printf("Connecting to %s (%s)\n", HOST, ipstr);
 
 freeaddrinfo(res);
 
 // get 4 ints
 totalbytes = 0;
 while (totalbytes < 16) {
  if ((numbytes = recv(sockfd, buf + totalbytes, BUF_SIZ - totalbytes - 1, 0)) == -1) {
   perror("recv");
   return 1;
  }
  totalbytes += numbytes;
 }
 printf("Received %d bytes.\n", totalbytes);
  
 // compute sum
 sum = 0;
 in = (uint32_t*) buf;
 for (i = 0; i < totalbytes / 4; i++) {
  printf("network: 0x%x (%u)\thost: 0x%x (%u)\n", in[i], in[i], ntohl(in[i]), ntohl(in[i]));
  //sum += ntohl(in[i]);
  sum += in[i];
 }
 printf("Sum: 0x%x (%u).\n", sum, sum);
  
 
 // return the result
// sum = htonl(sum);
 if (send(sockfd, &sum, sizeof(uint32_t), 0) == -1) {
  perror("send");
  return 1;
 }
  
 // get response from server
 if ((numbytes = recv(sockfd, buf, BUF_SIZ-1, 0)) == -1) {
  perror("recv");
  return 1;
 }
 buf[numbytes] = '\0';
 printf("Response: %s\n", buf);
  
 close(sockfd);
    return 0;
}

