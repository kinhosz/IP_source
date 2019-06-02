#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

#define MAX_CLIENT 2
struct msg_ret_t NOTICE;

int main(){


	int ret;
	int players = 0;
	// inicializa servidor
	serverInit(MAX_CLIENT);

	while(players < MAX_CLIENT){
		// retorna o id caso aceito
		// retorna NO_CONNECTION caso contrario
		ret = acceptConnection();
		if(ret != NO_CONNECTION){
			players++;
		}
	}
	// negando novas conexoes (servidor cheio)
	// retorna "SERVER_CLOSED" para o client
	rejectConnection();

	int sair = 0;
	char *buffer;
	buffer = (char *) malloc(sizeof(char)*500);

	while(!sair){

		// funcao nao bloqueante
		// NOTICE.client_id = id do client
		// NOTICE.status = 
		//		- MESSAGE_OK -> ha mensagens para processar
		// 		- DISCONNECT_MSG -> usuario esta se desconectando
		//		- NO_MESSAGE -> nao ha mensagens 
		// NOTICE.quant_bytes -> quantidades de bytes da msg recebida
		NOTICE = recvMsg(buffer); // recebendo mensagem geral
		if(NOTICE.status == DISCONNECT_MSG){
			// usuario desconectado. 
			free(buffer);
			sair = 1;
		}
		else if(NOTICE.status == MESSAGE_OK){
			// ha mensagens
			fprintf(stderr,"Usuario[%d]: %s\n",NOTICE.client_id,buffer);
		}
	}

	int teste = 0;
	int option;
	char *msg;
	int id_premiado;

	msg = (char *) malloc(sizeof(char) * 500);

	while(teste){
		// recebendo msg de cliente especifico
		// id_premiado =: recebe mensagem do cliente escolhido
		// option = DONT_WAIT (nao bloqueante) WAIT_FOR_IT (bloqueante)
		NOTICE = recvMsgFromClient(msg,id_premiado,option);
		// se o id nao for valido, status = NOT_VALID_CLIENT_ID
	}
	free(msg);

	/*********************
	*	  send msg       *
	*********************/

	teste = 0;
	msg = (char *) malloc(sizeof(char) * 100);
	int size;
	int tsize;

	while(teste){

		// enviando msg para cliente especifico
		strcpy(msg,"Voce esta conectado ao servidor!");
		tsize = sizeof(char) * strlen(msg);
		size = sendMsgToClient(msg,tsize,id_premiado);
		if(size != tsize){
			fprintf(stderr,"Erro ao enviar pacote. Enviado: %d -> esperado: %d\n",size,tsize);
		}

		// enviado msg para todos clients conectados
		broadcast(msg,tsize);

		free(msg);
		teste = 0;
	}

	// desconectando clientes
	// o id_premiado eh liberado e pode ser utilizado usando acceptConnect()
	disconnectClient(id_premiado);

	// verificando id
	int value = isValidId(id_premiado);
	if(value){
		fprintf(stderr,"Id valido!\n");
	}
	else{
		fprintf(stderr,"Id nao eh valido\n");
	}

	// resetando sevidor
	serverReset();
	return 0;
}
