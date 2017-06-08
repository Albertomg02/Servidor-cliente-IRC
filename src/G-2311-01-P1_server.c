#include "../includes/G-2311-01-P1_server.h"

/**
 * Para matar el servidor cerrando el puerto
 * @method handleSignal
 * @param  nsignal      [description]
 */
void handleSignal(int nsignal){
/*Funcion que hay que usar
	int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);

	Posible implementacion
	setsockopt(mi_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reutilizo, sizeof(BOOL) )
*/
	exit(EXIT_SUCCESS);
}

int iniciar_server(int puerto){
  int descriptor=0;
  struct sockaddr_in direccion;

  if((descriptor=crear_socket(SOCK_STREAM))==ERR){
    syslog(LOG_ERR,"Error creando socket");
    return ERR;
  }

  direccion.sin_family=AF_INET;

  direccion.sin_port=htons(puerto);
  direccion.sin_addr.s_addr=htonl(INADDR_ANY);
  bzero((void * )&(direccion.sin_zero),8);

  syslog(LOG_INFO,"Usando bind:%d",descriptor);
  if(bind(descriptor,(struct sockaddr *)&direccion,sizeof(direccion))<0){
    switch (errno) {
      case EACCES:
        syslog(LOG_ERR,"Error en funcion bind:EACCES");
      break;
      case EADDRINUSE:
        syslog(LOG_ERR,"Error en funcion bind:EADDRINUSE");
      break;
      case EBADF:
        syslog(LOG_ERR,"Error en funcion bind:EBADF");
      break;
      case EINVAL:
        syslog(LOG_ERR,"Error en funcion bind:EINVAL");
      break;
      case ENOTSOCK:
        syslog(LOG_ERR,"Error en funcion bind:ENOTSOCK");
      break;
      case EADDRNOTAVAIL:
        syslog(LOG_ERR,"Error en funcion bind:EADDRNOTAVAIL");
      break;
      case EFAULT:
        syslog(LOG_ERR,"Error en funcion bind:EFAULT");
      break;
      case ELOOP:
        syslog(LOG_ERR,"Error en funcion bind:ELOOP");
      break;
      case ENAMETOOLONG:
        syslog(LOG_ERR,"Error en funcion bind:ENAMETOOLONG");
      break;
      case ENOENT:
        syslog(LOG_ERR,"Error en funcion bind:ENOENT");
      break;
      case ENOTDIR:
        syslog(LOG_ERR,"Error en funcion bind:ENOTDIR");
      break;
      case EROFS:
        syslog(LOG_ERR,"Error en funcion bind:EROFS");
      break;
      default:
        syslog(LOG_ERR,"Error en funcion bind: DEP!!");
      break;

    }

    return ERR;
  }

  syslog(LOG_INFO,"Escuchando conexiones");
  if(listen(descriptor,MAX_CONNECTIONS)<0){
    syslog(LOG_ERR,"Error Escuchando conexiones");
    return ERR;
  }
	inicializar_array();
  return descriptor;
}

int run_server(int descriptor_server){
  struct sockaddr con_cliente;
	fd_set descriptoresLectura;
  int des_cliente, key, id_zone;
  unsigned int tam_con=sizeof(con_cliente);
	int *aux=NULL,i=0;
	int *buffer; /*En realidad seria algo en plan cliente *buffer asi que puede que esto sea similar*/
	unsigned short valor_m[]={1};

	max_descriptor=descriptor_server;

	/*Crear memoria compartida*/
	key = ftok(FILEKEY, 1400);
	if (key == -1) {
		syslog (LOG_ERR, "Error con key");
		return ERR;
	}

	id_zone = shmget (key, sizeof(clientes[0])*1, IPC_CREAT | SHM_R | SHM_W);
	if (id_zone == -1) {
		syslog (LOG_ERR, "Error con id_zone");
		return ERR;
	}

	buffer =(int *) shmat (id_zone, (char *)0, 0);
	if (buffer == NULL) {
		syslog (LOG_ERR, "Error reservando memoria compartida");
		shmdt ((char *)buffer);
		shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
		return ERR;
	}

  syslog(LOG_INFO,"Iniciando server");

	if(signal(SIGALRM,protocolo_PINGPONG)==SIG_ERR){
		syslog(LOG_ERR, "Error al inicializar un manejador de señales para alarma");
		return ERR;
	}
  //alarm(30);

  while(1){

			eliminaCerrados();
			FD_ZERO (&descriptoresLectura);
			FD_SET (descriptor_server, &descriptoresLectura);

			for(i=0;i<num_clientes;i++){
				FD_SET(clientes[i],&descriptoresLectura);
			}

			max_descriptor=maxDescriptorCliente();
			if(max_descriptor<descriptor_server){
				max_descriptor=descriptor_server;
			}

		if(select (max_descriptor+1, &descriptoresLectura, NULL, NULL, NULL)<0){
			switch (errno) {
				case EBADF:
					syslog(LOG_ERR,"Error en el select: EBADF");
					break;
				case EINTR:
					continue;
				case EINVAL:
					syslog(LOG_ERR,"Error en el select: EINVAL");
					break;
				case ENOMEM:
					syslog(LOG_ERR,"Error en el select: ENOMEM");
					break;
				default:
					syslog(LOG_ERR,"Error en el select: desconocido");
			}

			return ERR;
		}
	//	num_clientes=(clientes==NULL)?0:sizeof(clientes)/sizeof(int);
		for(i=0;i<num_clientes;i++){
			if(FD_ISSET (clientes[i], &descriptoresLectura) && lanza_hilos(i)==0){
					eliminarUser(clientes[i]);
					close(clientes[i]);
					clientes[i]=-1;
					Borrar_Semaforo(semaforos[i]);
					semaforos[i]=-1;
			}
		}

		if(FD_ISSET (descriptor_server, &descriptoresLectura)){
			if((des_cliente=accept(descriptor_server,&con_cliente,&tam_con))<0){
				syslog(LOG_ERR,"Error aceptando conexion");
				return ERR;
			}

			aux=(int *) realloc(clientes,(num_clientes+1)*sizeof(int));
			if(aux==NULL){
				syslog(LOG_ERR,"Error en el realloc");
				close(descriptor_server);
				return ERR;
			}

			clientes=aux;
			clientes[num_clientes]=des_cliente;

			aux=(int *) realloc(semaforos,(num_clientes+1)*sizeof(int));
			if(aux==NULL){
					syslog(LOG_ERR,"Error en el realloc");
					close(descriptor_server);
					return ERR;
			}
			semaforos=aux;
			/*Inicializar mutex*/
			key = ftok(FILEKEY, 1300);
			if (key == -1) {
				syslog (LOG_ERR, "Error con key");
				shmdt ((char *)buffer);
				shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
				return ERR;
			}

			if(Crear_Semaforo(key,1, &semaforos[num_clientes])==ERR){
				syslog (LOG_ERR, "Error al crear mutex");
				shmdt ((char *)buffer);
				shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
				return ERR;
			}


			if(Inicializar_Semaforo(semaforos[num_clientes], valor_m)==ERR){
				syslog (LOG_ERR, "Error al inicializar mutex");
				Borrar_Semaforo(semaforos[num_clientes]);
				shmdt ((char *)buffer);
				shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
				return ERR;
			}
			num_clientes++;

		}

  }
  close(descriptor_server);
  return ERR;

}

void protocolo_PINGPONG(int sig){
	//signal(sig, SIG_IGN);
	long nelements=0, u_id=0, creationTS=0, actionTS=0;
	unsigned long dev=0;
	char *u_nick=NULL, *u_real=NULL,*prefix=NULL,*enviar=NULL;
	char *host=NULL, *IP=NULL, *away=NULL;
	char **userlist;
	int sock=0, i=0,j=0;
	time_t comienzo;
	double diff;

	dev=IRCTADUser_GetUserList(&userlist, &nelements);
	if(dev==IRCERR_NOENOUGHMEMORY){
		syslog(LOG_ERR,"No hay suficiente memoria para almacenar el parametro");
    return ;
	}
	comienzo=time(NULL);
	for(i=0; i<nelements; i++){
		//cambio socket por sock porque da error
		dev=IRCTADUser_GetData (&u_id, &userlist[i], &u_nick, &u_real,&host, &IP, &sock, &creationTS, &actionTS, &away);

		if(dev!=IRC_OK){
		  IRC_syslog("ALARM: Error en IRCMsg_Privmsg", dev, LOG_ERR);
		}
		diff=difftime(comienzo, actionTS);
		if(diff>90){
			 IRCTADUser_Delete(u_id,NULL, NULL, NULL);
			 for(j=0;j<num_clientes;j++){
				 	if(clientes[j]==sock){
						clientes[j]=-1;
						semaforos[j]=-1;
						break;
					}
			 }
		}else if(diff>30){
			dev=IRC_Prefix(&prefix,u_nick,userlist[i],HOST_NAME,IP);
			if(dev!=IRC_OK){
				IRC_syslog("PRIVMSG: Error en IRC_Prefix", dev, LOG_ERR);
			}
			dev=IRCMsg_Ping (&enviar, prefix,HOST_NAME, userlist[i]);
			if(dev!=IRC_OK){
				IRC_syslog("ALARM: Error en IRCMsg_Ping", dev, LOG_ERR);
			}
			send(sock,enviar,strlen(enviar),0);
			free(enviar);
			free(prefix);
		}
	//	IRC_MFree(5,u_nick,u_real,host,IP,away);
	}
	IRCTADUser_FreeList(userlist, nelements);

	alarm(30);
	signal(sig, protocolo_PINGPONG);
}

int lanza_hilos(int index){
	pthread_t cliente;
	int leido;
	char entrada[10000];
	param *p=(param*)malloc(sizeof(param));
	p->index=index;
	syslog(LOG_INFO,"HILO:Nuevo acceso");
	leido=recv(clientes[index], &entrada, sizeof(entrada),0);
	if(leido>0){
		p->comando=(char *)malloc(leido+1);
		if(p->comando==NULL){
			syslog(LOG_ERR,"Error en el malloc de leer el comando");
			free(p->comando);
			free(p);
			return 0;
		}
		 syslog(LOG_INFO,"HILO: Lanzando hilo");
		strncpy(p->comando,entrada,leido);
		p->comando[leido]=0;
    pthread_create(&cliente,NULL,launch_service,(void *)p);
		return leido;
	}else{
		free(p);
		return 0;
	}

//	pthread_tryjoin_np(cliente,NULL);
	//pthread_join(cliente,NULL);
}

void* launch_service(void * pr){
char *enviar=NULL;
	char *user=NULL,*n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL;
	long id=0;
	long int cts=0,ats=0;
	param *p=(param*)pr;
  int conexion=(*p).index;
  unsigned long tipo=0;
	char *comando=NULL,*in=(*p).comando;

	//semaforo on
	if(Down_Semaforo(semaforos[conexion], 0, SEM_UNDO)==ERR){
		syslog (LOG_ERR, "Error en down mutex");
		Borrar_Semaforo(semaforos[conexion]);
	//	shmdt ((char *)buffer);
	//	shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
	//	return ERR;
	}
	 syslog(LOG_INFO,"LS:Inicio del Pipeline");
		//syslog(LOG_INFO,"Leido(%d):%s",leido,in);
		//printf("\n\nINICIAL: %s\n",(*p).comando);
		while(in!=NULL){
			in=IRC_UnPipelineCommands (in,&comando);
			tipo=IRC_CommandQuery(comando);
		//	printf("Comando: %s y in: %s\n",comando,in);
			switch (tipo) {
				case IRCERR_NOCOMMAND:
					syslog (LOG_ERR, "Error no comando");
				break;
				case IRCERR_NOPARAMS:
					syslog (LOG_ERR, "Error no parrametros");
				break;

				case IRCERR_UNKNOWNCOMMAND:
					syslog (LOG_ERR, "Error comando desconocido: %s",comando);
				if(tipo==IRCERR_UNKNOWNCOMMAND){
						IRCTADUser_GetData(&id,&user,&n,&real,&host,&ip,&clientes[conexion],&cts,&ats,&away);
						IRCMsg_ErrUnKnownCommand(&enviar,HOST_NAME,n,comando);
						send(clientes[conexion],enviar,strlen(enviar),0);
						id=0;cts=0;ats=0;
						if(user!=NULL){
							free(user);}
							user=NULL;
						if(n!=NULL){
							free(n);
							n=NULL;
						}
						if(real!=NULL){
							free(real);
							real=NULL;
						}
						if(host!=NULL){
							free(host);
							host=NULL;
						}
						if(enviar!=NULL){
							free(enviar);
							enviar=NULL;
						}
						if(away!=NULL){
							free(away);
							away=NULL;
						}
						if(ip!=NULL){
							free(ip);
							ip=NULL;
						}
				}

				break;

				default:
					lista_comandos[tipo](comando,clientes[conexion]);
			}
			free(comando);
			if(tipo==QUIT)break;
	}

	if((*p).comando!=NULL){
		free((*p).comando);
		(*p).comando=NULL;
	}
	if(p!=NULL){
		free(p);
		p=NULL;
	}

	//semaforo off
	if(Up_Semaforo(semaforos[conexion], 0, SEM_UNDO)==ERR){
		syslog (LOG_ERR, "Error en up mutex");
		Borrar_Semaforo(semaforos[conexion]);
		//shmdt ((char *)buffer);
		//shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
	//	return ERR;
	}
	syslog(LOG_INFO,"LS:Finalizado levantando semaforo");


	return p;

}

int run_server_echo(int descriptor){
  struct sockaddr con_cliente;
  int des_cliente,leido=0;
  unsigned int tam_con=sizeof(con_cliente);
  char cadena[2000];
  char *enviar;

  syslog(LOG_INFO,"Iniciando server echo");

  if((des_cliente=accept(descriptor,&con_cliente,&tam_con))<0){
    syslog(LOG_ERR,"Error aceptando conexion");
    return ERR;
  }

  while(1){

   	leido=read(des_cliente, &cadena, sizeof(cadena));
    enviar=(char*)malloc(leido);
    strncpy(enviar,cadena,leido);
   	write(des_cliente,&cadena,strlen(enviar));
    free(enviar);

  }
  close(descriptor);
  return ERR;

}

void servidor_echo(int puerto){
  int descriptor=0;

  if((descriptor=iniciar_server(puerto))<0){
    syslog(LOG_ERR,"Error en iniciar_server");
    return;
  }

  if(run_server_echo(descriptor)==ERR)
    syslog(LOG_ERR,"Error en el servidor echo");

}

int servidor_irc(int puerto){
  int descriptor;
  if((descriptor=iniciar_server(puerto))<0){
    syslog(LOG_ERR,"Error en iniciar_server");
    return EXIT_FAILURE;
  }

  if(run_server(descriptor)==ERR)
    syslog(LOG_ERR,"Error en el servidor");
    return EXIT_FAILURE;
}

int maxDescriptorCliente(){
	int max,i;
	max=(num_clientes==0)?0:clientes[0];
	for(i=1;i<num_clientes;i++){
		if(max<clientes[i]){
			max=clientes[i];
		}
	}
	return max;
}

void eliminaCerrados(){
	int i,j;
	if(num_clientes==0)return;
	else if(num_clientes==1&&clientes[0]==-1){
		num_clientes=0;
		return;
	}
	for(i=0,j=0;i<num_clientes;i++){
		if(clientes[i]!=-1){
			clientes[j]=clientes[i];
			semaforos[j]=semaforos[i];
			j++;
		}
	}
	num_clientes=j;
	return;
}

void eliminarUser(int socket){
	char *user=NULL,*nick=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL;
	long id=0,cts=0,ats=0;
	IRCTADUser_GetData(&id,&user,&nick,&real,&host,&ip,&socket,&cts,&ats,&away);
	IRCTAD_Quit (nick);
//	IRCTADUser_Delete(id,NULL,NULL,NULL);
}

/***************************************************************************************************/
/***************************************************************************************************/
/**                                                                                               **/
/** MMMMMMMMMM               MMMMM           AAAAAAA           IIIIIII NNNNNNNNNN          NNNNNN **/
/**  MMMMMMMMMM             MMMMM            AAAAAAAA           IIIII   NNNNNNNNNN          NNNN  **/
/**   MMMMM MMMM           MM MM            AAAAA   AA           III     NNNNN NNNN          NN   **/
/**   MMMMM  MMMM         MM  MM            AAAAA   AA           III     NNNNN  NNNN         NN   **/
/**   MMMMM   MMMM       MM   MM           AAAAA     AA          III     NNNNN   NNNN        NN   **/
/**   MMMMM    MMMM     MM    MM           AAAAA     AA          III     NNNNN    NNNN       NN   **/
/**   MMMMM     MMMM   MM     MM          AAAAA       AA         III     NNNNN     NNNN      NN   **/
/**   MMMMM      MMMM MM      MM          AAAAAAAAAAAAAA         III     NNNNN      NNNN     NN   **/
/**   MMMMM       MMMMM       MM         AAAAA         AA        III     NNNNN       NNNN    NN   **/
/**   MMMMM        MMM        MM         AAAAA         AA        III     NNNNN        NNNN   NN   **/
/**   MMMMM                   MM        AAAAA           AA       III     NNNNN         NNNN  NN   **/
/**   MMMMM                   MM        AAAAA           AA       III     NNNNN          NNNN NN   **/
/**  MMMMMMM                 MMMM     AAAAAA            AAAA    IIIII   NNNNNN           NNNNNNN  **/
/** MMMMMMMMM               MMMMMM  AAAAAAAA           AAAAAA  IIIIIII NNNNNNN            NNNNNNN **/
/**                                                                                               **/
/***************************************************************************************************/
/***************************************************************************************************/


/**
 * [main  description]
 * @method main
 * @param  argc [description]
 * @param  argv [description]
 * @return      [description]
 */
int main (int argc, char *argv[])
{
	int i=0;
	for(i=0;i<0;i++){
		printf("\nDEP!!\n");
	}


  if(argc==2 && strcmp(argv[1],"e")==0){
    daemonizar("Servidor_DEP");
    servidor_echo(7);
    return EXIT_SUCCESS;
  }else if(argc==3 && strcmp(argv[1],"p")==0){

    daemonizar("Servidor_DEP");
    servidor_irc(atoi(argv[2]));
		close(atoi(argv[2]));
		return EXIT_FAILURE;
  }else{
    printf("Error numero de parametros:\nsudo ./server e\n sudo ./server -p dddd\n" );
    return EXIT_FAILURE;
  }

	return EXIT_SUCCESS;
}
