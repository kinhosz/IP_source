#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

#define SERVER_IP "192.168.0.20" 
enum conn_ret_t ans; //

void game_start(){

	fprintf(stderr,"Hello world\n");

	int tsize,size;
	char *msg;
	// so alocacao
	msg = (char *) malloc(sizeof(char) * 50);
	strcpy(msg,"estou conectado!!!");
	tsize = sizeof(char) * strlen(msg);

	// enviando mensagem ao servidor
	size = sendMsgToServer(msg,tsize);
	if(size == SERVER_DISCONNECTED){
		fprintf(stderr,"O servidor foi desconectado!");
		return ;
	}
	else if(size != tsize){
		fprintf(stderr,"pacote nao foi enviado completamente! %d/%d\n",size,tsize);
	}

	// recebendo msg do servidor
	int option = WAIT_FOR_IT;
	// option -> "DONT_WAIT or WAIT_FOR_IT"
	size = recvMsgFromServer(msg,option);

	// controle do teclado
	char c = getchar();

	while(c != 's' && c != 'S'){
		c = getchar();
		if(c == NO_KEY_PRESSED){
			system("clear");
			printf("digita ai, vei\n");
		}
	}
}

int main(){

	// tentando conexao
	int try = 1;
	while(try){
		system("clear");
		ans = connectToServer(SERVER_IP);
		if(ans == SERVER_UP){
			fprintf(stderr,"conexao estabelecida!\n");
			game_start();
		}
		else if(ans == SERVER_DOWN){
			fprintf(stderr,"Servidor nao encontrado\n");
		}
		else if(ans == SERVER_FULL){
			fprintf(stderr,"Capacidade maxima do servidor atingida\n");
		}
		else if(ans == SERVER_CLOSED){
			fprintf(stderr,"Servidor fechado para novas conexoes\n");
		}
		else if(ans == SERVER_TIMEOUT){
			fprintf(stderr,"Tempo esgotado - o servidor nao respondeu\n");
		}
		fprintf(stderr,"tente novamente ? y/n\n");
		char r;
		fscanf(stdin,"%c",&r);
		if(r == 'n' || r == 'N') try = 0;
	}

	fprintf(stderr,"Desconectado\n");

	return 0;
}