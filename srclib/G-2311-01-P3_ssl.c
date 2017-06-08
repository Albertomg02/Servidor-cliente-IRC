#include "../includes/G-2311-01-P3_ssl.h"

SSL_CTX* inicializar_nivel_SSL(){
  SSL_load_error_strings();

  ERR_print_errors_fp(stderr);

  if(SSL_library_init()<0){
    return NULL;
  }

  return SSL_CTX_new(SSLv23_method());
}


void fijar_contexto_SSL(SSL_CTX *cnt, const char *CA, const char *CApath, const char *f){

  SSL_CTX_load_verify_locations(cnt, CA, CApath);

  if(SSL_CTX_set_default_verify_paths(cnt)<1){
    ERR_print_errors_fp(stderr);
    return;
  }

  if(SSL_CTX_use_certificate_chain_file(cnt, f)<1){
    ERR_print_errors_fp(stderr);
    return;
  }

  SSL_CTX_use_PrivateKey_file(cnt, f, SSL_FILETYPE_PEM);

  SSL_CTX_set_verify(cnt, SSL_VERIFY_PEER, NULL);
}

SSL *conectar_canal_seguro_SSL(SSL_CTX *cnt, int socket){

  SSL *sec=SSL_new(cnt);

  if(cnt==NULL){
    ERR_print_errors_fp(stderr);
    return NULL;
  }

  SSL_set_fd(sec, socket);

  if(SSL_connect(sec)!=1){
    ERR_print_errors_fp(stderr);
    return NULL;
  }

  return sec;

}

SSL *aceptar_canal_seguro_SSL(SSL_CTX *cnt, int socket){
  SSL *sec=SSL_new(cnt);

  if(cnt==NULL){
    ERR_print_errors_fp(stderr);
    return NULL;
  }

  SSL_set_fd(sec, socket);

  if(SSL_accept(sec)!=1){
    ERR_print_errors_fp(stderr);
    return NULL;
  }

  return sec;
}

int evaluar_post_connectar_SSL(const SSL *ssl){

  if(SSL_get_peer_certificate(ssl)==NULL){
    ERR_print_errors_fp(stderr);
    return ERR;
  }

  if(SSL_get_verify_result(ssl) != X509_V_OK){
    ERR_print_errors_fp(stderr);
    return ERR;
  }

  return OK;
}


int enviar_datos_SSL(SSL *ssl, const void *buffer, int len){
	int enviado=0;

	if((enviado=SSL_write(ssl, buffer, len))>0){
		return enviado;
	}

  ERR_print_errors_fp(stderr);
	return -1;


}

int recibir_datos_SSL(SSL *ssl, void *buffer, int len){
	int leido=0;

  if((leido=SSL_read(ssl, buffer, len))>0){
    return leido;
  }

  ERR_print_errors_fp(stderr);
  return -1;

}

int cerrar_canal_SSL(SSL *ssl, SSL_CTX *cnt, int socket){

	if(SSL_shutdown(ssl)!=1){
		ERR_print_errors_fp(stderr);
		return ERR;
	}

	SSL_free(ssl);
	SSL_CTX_free(cnt);
	close(socket);
	return OK;
}
