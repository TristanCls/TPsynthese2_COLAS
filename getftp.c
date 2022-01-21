#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <time.h>


const char probleme[] = "erreur de commande\n";


int RRQ(char *filename, char *mode, int sck, struct addrinfo *result){
	
	char *cmd;
	int snt;

	cmd = malloc(4+strlen(mode)+strlen(filename));
	cmd[0] = 0;
	cmd[1] = 1;
	
	strcpy(&cmd[2],filename);
	cmd[2+strlen(filename)] = 0;
	
	strcpy(&cmd[2+strlen(filename)+1],mode);
	cmd[2+strlen(filename)+1+strlen(mode)+1] = 0;
	
	snt = sendto(sck,cmd,4+strlen(mode)+strlen(filename),0,result->ai_addr, result->ai_addrlen);
	
	free(cmd);
	
	return(snt);
}	


int main(int argc, char *argv[]){
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sock, s;
    
	if (argc<3){
		write(STDOUT_FILENO, probleme, sizeof(probleme));
		exit(EXIT_FAILURE);
		}
	
	memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    
    hints.ai_socktype = SOCK_DGRAM; 
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_UDP;
    
    s = getaddrinfo(argv[1],"69", &hints, &result);
    
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		
        sock = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
        
        if (sock == -1){
            continue;
		}
        if (connect(sock, rp->ai_addr, rp->ai_addrlen) != -1){
            break;                  
		}
       close(sock);
    }

   if (rp == NULL) {               
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }
	
	int tailleCommande = RRQ(argv[2],"octet",sock, result);	
	
	return(tailleCommande);
}


		
		
