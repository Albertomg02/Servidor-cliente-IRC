#include "../includes/G-2311-01-P3_echo_cliente.h"

int main(int argc, char** argv){
  int sk=0;
  struct sockaddr_in Conexion;
	struct hostent *h;
  char buffer[1000];

  int len=1;

  SSL_CTX *ctxt=NULL;
	SSL *ssl = NULL;


    if((sk=crear_socket(SOCK_STREAM))==ERR){
      syslog(LOG_ERR,"Error creando socket, cliente");
      return EXIT_FAILURE;
    }
    h = gethostbyname("127.0.0.1");
    if(h==NULL){
    printf("Error obteniendo la ip, cliente");
      return EXIT_FAILURE;
    }
    //bzero((char*)&Conexion, sizeof(Conexion));
    Conexion.sin_family=AF_INET;
    //bcopy((char*)h->h_addr, (char*)&Conexion.sin_addr.s_addr, h->h_length);
    Conexion.sin_addr.s_addr=((struct in_addr*)(h->h_addr))->s_addr;
    Conexion.sin_port=htons(6667);

    ctxt = inicializar_nivel_SSL();
    if(ctxt==NULL){
      printf("Error inicializar_nivel_SSL");
      syslog(LOG_ERR, "inicializar_nivel_SSL");
      return ERR;
    }
  	fijar_contexto_SSL(ctxt, "/home/oscar/Documentos/redes2_practicas/G-2311-01/certs/ca.pem", "/home/oscar/Documentos/redes2_practicas/G-2311-01/certs", "/home/oscar/Documentos/redes2_practicas/G-2311-01/certs/cliente.pem");

    if(connect(sk, (struct sockaddr *)&Conexion, sizeof(struct sockaddr))<0){
  		printf("Error aceptando conexion");
      syslog(LOG_ERR, "Error aceptando conexion");
  		return EXIT_FAILURE;
  	}
  	if((ssl = conectar_canal_seguro_SSL(ctxt, sk))==NULL){
      printf("Error al conectar");
        syslog(LOG_ERR, "Error al conectar");
      return -1;
    }

  	if(evaluar_post_connectar_SSL(ssl) == ERR){
  		printf("Error al evaluar c");
        syslog(LOG_ERR, "Error al evaluar c");
  		cerrar_canal_SSL(ssl, ctxt, sk);
      return -1;
  	}

    while(len!=-1){
        bzero(buffer,1000);
    //  printf("Introduzca lo que va a enviar:");
      fgets(buffer,1000,stdin);
      syslog(LOG_ERR, "ACA!!!%s!!!",buffer);
      enviar_datos_SSL(ssl,buffer, strlen(buffer));
      if(strcmp("exit\n",buffer)==0){
        break;
      }
      bzero(buffer,1000);
      len = recibir_datos_SSL(ssl,buffer,1000);
      if(len!=-1){
          syslog(LOG_ERR, "ACAAAA!!!%s!!!",buffer);
          fprintf(stdout,"%s",buffer);
      }
    }
    cerrar_canal_SSL(ssl, ctxt, sk);
    //free(server);
    return EXIT_SUCCESS;





}
