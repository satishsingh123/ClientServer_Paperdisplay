#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#define SIZE 1024
#define IP_ADDR "225.0.0.1"
#define PORT 5555

int main(int argc , char **argv)
{
	if ( argc !=2){
		fprintf(stderr,"%s <<FILE_NAME>>",argv[0]);
		printf("\n");
		return;
	}



	unsigned int sid;
	unsigned int addr_len;
	unsigned int fd;
	unsigned int ret;
	unsigned int cnt;
	unsigned int tot_sent = 0;
	char LINEBUF[SIZE];

	struct sockaddr_in saddr;

	sid = socket ( AF_INET, SOCK_DGRAM , 0 );
	
	if(sid < 0){
		perror("socket");
		return;
	}
	puts("\nSocket created sucess\n");

	memset ( &saddr, 0 , sizeof(saddr) );
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons (PORT);
	saddr.sin_addr.s_addr = htonl ( INADDR_ANY);
	addr_len = sizeof(saddr);
		
	if ( argc > 1 ){
	
		saddr.sin_addr.s_addr = inet_addr( IP_ADDR );
		
		fd = open(argv[1] , O_RDONLY);		

		while(1){
			
			cnt = read ( fd , LINEBUF , 1024  );
			if ( cnt == 0){
				ret  = sendto ( sid , "STOP", 4, 0 , ( struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
				break;
			}

			ret  = sendto ( sid , LINEBUF, cnt , 0 , ( struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
			tot_sent += ret;
			
			if ( ret < 0){
				perror("sendto");
				return;
			}
			else{	//printf("sent ----> %d total sent ------> %d\n",ret,tot_sent);				
				memset (LINEBUF , 0 , cnt );
			}
		}
		close(fd);
		printf("server completed\n\n");
		
	}
}






