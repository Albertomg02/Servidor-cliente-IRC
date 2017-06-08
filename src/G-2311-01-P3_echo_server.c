#include "../includes/G-2311-01-P3_echo_server.h"

int main(int argc, char** argv){
  int sk=0,skc=0;
  struct sockaddr_in Conexion;

  char buffer[1000];
  int len=1;
  struct sockaddr con_cliente;
  unsigned int tam_con=sizeof(con_cliente);
  SSL_CTX *ctxt=NULL;
  SSL *ssl = NULL;


  if((sk=crear_socket(SOCK_STREAM))==ERR){
    syslog(LOG_ERR,"Error creando socket");
    return ERR;
  }

  Conexion.sin_family=AF_INET;

  Conexion.sin_port=htons(6667);
  Conexion.sin_addr.s_addr=htonl(INADDR_ANY);
  bzero((void * )&(Conexion.sin_zero),8);

  syslog(LOG_INFO,"Usando bind:%d",sk);
  if(bind(sk,(struct sockaddr *)&Conexion,sizeof(Conexion))<0){
    printf("Error bind");
      return EXIT_FAILURE;
  }

  if(listen(sk,10)<0){
    printf("Error Escuchando conexiones");
    return ERR;
  }

  ctxt = inicializar_nivel_SSL();
  if(ctxt==NULL){
    printf("Error inicializar_nivel_SSL");
    return ERR;
  }
	fijar_contexto_SSL(ctxt, "/home/oscar/Documentos/redes2_practicas/G-2311-01/certs/ca.pem", "/home/oscar/Documentos/redes2_practicas/G-2311-01/certs", "/home/oscar/Documentos/redes2_practicas/G-2311-01/certs/servidor.pem");

  if((skc = accept(sk, &con_cliente,&tam_con))<0){
    printf("Error acept");
    return ERR;
	}

  if((ssl = aceptar_canal_seguro_SSL(ctxt,skc))==NULL){
    printf("Error al aceptar");
      syslog(LOG_ERR, "Error al aceptar");
    return -1;
  }
  if(evaluar_post_connectar_SSL(ssl) == ERR){
    printf("Error al evaluar s");
      syslog(LOG_ERR, "Error al evaluar s");
    cerrar_canal_SSL(ssl, ctxt, sk);

    close(sk);
    return -1;
  }

  while(len!=-1){
    bzero(buffer,1000);
    len=recibir_datos_SSL(ssl,buffer,1000);
    if(len!=-1){
        enviar_datos_SSL(ssl,buffer, strlen(buffer));
    }
    if(strcmp("exit",buffer)==0){
      break;
    }
  }
cerrar_canal_SSL(ssl, ctxt, skc);
return EXIT_SUCCESS;



}
