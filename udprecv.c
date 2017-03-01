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
	
	unsigned int sid;
	unsigned int addr_len;
	unsigned int fd;
	unsigned int ret;
	unsigned int cnt;
	unsigned int tot_sent = 0;
	char LINEBUF[SIZE];
	FILE *fp;

	struct sockaddr_in addr;

	sid = socket ( AF_INET, SOCK_DGRAM , 0 );
	
	if(sid < 0){
		perror("socket");
		return;
	}
	puts("\nSocket created sucess\n");



	fp = fopen ("question.pdf","wb");
	if ( fp < 0){
		perror("fopen");
		return;
	}

		
	int opt_val = 1;
	char LINEBUF1[SIZE];
	
	if(setsockopt(sid , SOL_SOCKET , SO_REUSEADDR, &opt_val ,sizeof(opt_val)) < 0){
		perror("setsockopt");
		return;
	}



	memset ( &addr, 0 , sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons ( PORT);
	addr.sin_addr.s_addr = inet_addr( IP_ADDR );
	addr_len = sizeof(addr);
	


	if ( bind (sid, (struct sockaddr *)&addr,sizeof(addr)) < 0){
		perror("bind");
		return;
	}
	


	struct ip_mreq mreq;
	
	mreq.imr_multiaddr.s_addr = inet_addr("225.0.0.1");
	mreq.imr_interface.s_addr = htonl (INADDR_ANY);

	if(setsockopt(sid , IPPROTO_IP,IP_ADD_MEMBERSHIP, &mreq , sizeof (mreq)) < 0 ){
		perror("setsockopt");
		return;
	}


	int wt;
	while(1){
		cnt = recvfrom( sid , LINEBUF , 1024 , 0 , (struct sockaddr *)&addr, &addr_len);
		//printf("cnt is %d\n",cnt);
		
		if (cnt < 0){
			fclose(fp);
			perror("recvfrom");
			return;
		}
		else if ( cnt == 4){
			if(!(strncmp(LINEBUF ,"STOP",4)))
				fclose(fp);
				printf("ENDEDDDD");
				break;
			}
			
			wt = fwrite(LINEBUF , cnt, 1, fp );
			
			if( wt < 0){
				puts("fwrite fail");
				return;
			}		
		
	}
	//fclose(fp);
	system("chmod 444 question.pdf");
system("evince question.pdf > /dev/null");
system("rm -f question.pdf");

	puts("\nserver completed writing\n\n");
	return 0;

		
}







