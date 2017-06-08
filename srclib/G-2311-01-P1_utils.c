#include "../includes/G-2311-01-P1_utils.h"


/**
 * [registrarMensajes description]
 * @method registrarMensajes
 * @param  codigo            [description]
 * @param  handleModule      [description]
 * @param  msg_registrados   [description]
 * @return                   [description]
 */
int registrarMensajes(long codigo, pf_msg handleModule,pf_msg* msg_registrados){

	if(msg_registrados==NULL ||  handleModule==NULL){
		printf("Error: registrarProtocolo(): entradas nulas.\n");
		return ERR;
	}
	else
		msg_registrados[codigo]=handleModule;
	return OK;
}

void freeListas(char ***lista, int num_elems){
	int i;

	if(num_elems<=0 || lista==NULL || *lista==NULL){
		return;
	}
	else{
		for(i=0; i<num_elems; i++){
			if(*lista[i]!=NULL){
				free(*lista[i]);
			}
		}

		free(*lista);
		*lista=NULL;

		return;
	}
}
