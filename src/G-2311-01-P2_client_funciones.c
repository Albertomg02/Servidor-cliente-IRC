#include "../includes/G-2311-01-P2_client_funciones.h"

int inicializar_array_c(){
  int i;
  for(i=0;i<IRC_MAX_COMMANDS;i++){
    comandos_cliente[i]=funcion_default_c;
  }
  comandos_cliente[RPL_WELCOME]=funcion_rplwelcome_c;
  comandos_cliente[JOIN]=funcion_join_c;
  comandos_cliente[PRIVMSG]=funcion_priv_c;
  comandos_cliente[NICK]=funcion_nick_c;
  comandos_cliente[PART]=funcion_part_c;
  comandos_cliente[PING]=funcion_ping_c;
  comandos_cliente[RPL_NAMREPLY]=funcion_namrpl_c;
  comandos_cliente[RPL_ENDOFNAMES]=funcion_namend_c;
  comandos_cliente[RPL_WHOREPLY]= funcion_RplWhoReply_c;
  comandos_cliente[RPL_ENDOFWHO]= funcion_RplWhoEnd_c;
  comandos_cliente[RPL_WHOISUSER]=funcion_whoisuser_c;
  comandos_cliente[RPL_WHOISSERVER]=funcion_whoisserver_c;
  comandos_cliente[RPL_WHOISCHANNELS]=funcion_whoischannels_c;
  comandos_cliente[RPL_WHOISIDLE]=funcion_whoisidle_c;
  comandos_cliente[RPL_ENDOFWHOIS]=funcion_endwhois_c;
  comandos_cliente[RPL_MOTDSTART]=funcion_RplMotdstart_c;
  comandos_cliente[RPL_MOTD]=funcion_RplMotd_c;
  comandos_cliente[RPL_ENDOFMOTD]=funcion_RplendofMotd_c;
  comandos_cliente[RPL_TOPIC]=funcion_RplTopic_c;
  comandos_cliente[RPL_NOTOPIC]=funcion_RplNoTopic_c;
  comandos_cliente[MODE]=funcion_mode_c;
  comandos_cliente[KICK]=funcion_kick_c;
  comandos_cliente[RPL_LIST]=funcion_RplList_c;
  comandos_cliente[RPL_LISTEND]=funcion_RplListEnd_c;
  comandos_cliente[RPL_UNAWAY]=funcion_RplUaway_c;
  comandos_cliente[RPL_NOWAWAY]=funcion_RplNowaway_c;
  comandos_cliente[QUIT]=funcion_quit_c;

return OK;
}

int funcion_default_c(char *command, int socket){

  syslog(LOG_ERR,"Commando no reconocido: %s",command);

  return ERR;
}

int funcion_rplwelcome_c(char *command, int socket){

  unsigned long dev=0;
  char *prefix=NULL,*nick=NULL,*msg=NULL;
  syslog(LOG_INFO,"WELCOME CLIENTE");
  dev=IRCParse_RplWelcome(command, &prefix,&nick,&msg);
  if(dev!=IRC_OK){
    IRC_syslog("rplwelcome_c: : IRCParse_RplWelcome",dev,LOG_ERR);
    return ERR;
  }
  IRCInterface_WriteSystemThread("*",msg);
  return OK;
}

int funcion_join_c(char *command, int socket){

  unsigned long dev=0,mode;
  char *channel=NULL,*key=NULL,*prefix=NULL,*msg=NULL,*enviar=NULL;
  char *nick=NULL,*user=NULL,*host=NULL,*server=NULL;

  dev=IRCParse_Join(command, &prefix, &channel, &key, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("join_c: : IRCParse_Join",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCParse_ComplexUser(prefix,&nick,&user,&host,&server);
  if(dev!=IRC_OK){
    IRC_syslog("PRIVMSG interfaz: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }
if(IRCInterface_QueryChannelExistThread (msg)==TRUE){
    IRCInterface_AddNickChannelThread (msg, nick, "user", "prefix", "host", VOICE);
}else{
  mode = IRCInterface_ModeToIntMode("+m");
  IRCInterface_AddNewChannelThread(msg, mode);
  dev=IRCMsg_Who (&enviar, NULL, msg, NULL);
  if(dev!=IRC_OK){
    IRC_syslog("who_cu: : IRCMsg_Who",dev,LOG_ERR);
    return ERR;
  }
  send(socket,enviar,strlen(enviar),0);
  IRCInterface_PlaneRegisterOutMessageThread(enviar);
  IRC_MFree(3,&enviar,&msg,&key);
}

  return OK;
}

int funcion_priv_c(char *command, int sckt){
  unsigned long dev=0;
  boolean resp;
  int socket_aux;
  char *target=NULL, *msg=NULL, *prefix=NULL;
  char *nick=NULL,*user=NULL,*host=NULL,*server=NULL;
  char *msg_especial=(char *)malloc(9*sizeof(char));
  char *inicio=NULL, *filename=NULL, *ip_emisor=NULL, *puerto_emisor=NULL, *longitud=NULL;
  pthread_t hilo;


  dev=IRCParse_Privmsg(command, &prefix, &target, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("priv_c: : IRCParse_Privmsg",dev,LOG_ERR);
    free(msg_especial);
    return ERR;
  }


  dev=IRCParse_ComplexUser(prefix,&nick,&user,&host,&server);
  if(dev!=IRC_OK){
    IRC_syslog("PRIVMSG interfaz: Error en IRCParse_ComplexUser", dev, LOG_ERR);
    return ERR;
  }

  inicio=strtok(msg, " ");

  if(strcmp("\\001FSEND", inicio)==0){

    param_recvfile *p=(param_recvfile*)malloc(sizeof(param_recvfile));
    filename=strtok(NULL, " ");
    ip_emisor=strtok(NULL, " ");
    puerto_emisor=strtok(NULL, " ");
    longitud=strtok(NULL, " ");

    resp=IRCInterface_ReceiveDialogThread(nick, filename);
    if(resp==TRUE){
      if((socket_aux=socket(AF_INET, SOCK_STREAM, 0))<0){
    		syslog(LOG_ERR,"Error creando socket en funcion_priv_c");
        free(msg_especial);
        free(p);
        return ERR;
    	}

      p->socket=socket_aux;

      p->filename=(char *)malloc(strlen(filename)*sizeof(char));
      if(p->filename==NULL){
  			syslog(LOG_ERR,"Error en el malloc de filename");
  			return ERR;
  		}
      strcpy(p->filename, filename);

      p->ip=(char *)malloc(strlen(ip_emisor)*sizeof(char));
      if(p->ip==NULL){
  			syslog(LOG_ERR,"Error en el malloc de ip");
  			return ERR;
  		}
      strcpy(p->ip, ip_emisor);

      p->puerto=atoi(puerto_emisor);
    	p->length=atoi(longitud);

      pthread_create(&hilo,NULL,launch_recvfile,(void *)p);

    }
    else{
      free(msg_especial);
      free(p);
      return OK;
    }
  }

  if(target[0]!='#'){
    if(IRCInterface_QueryChannelExistThread(nick)!=TRUE){
      int  mode = IRCInterface_ModeToIntMode("+m");
      IRCInterface_AddNewChannelThread(nick, mode);
    }
    IRCInterface_WriteChannelThread(nick,nick,msg);
  }else{

    IRCInterface_WriteChannelThread(target,nick,msg);
  }

  free(msg_especial);
  return OK;
}

void* launch_recvfile(void *pr){
  int descriptor=0;
  struct sockaddr_in conexion;
  char entrada[10000];
  param_recvfile *p=(param_recvfile*)pr;
  int leido;
  struct hostent *h;
  FILE *f = fopen(p->filename,"wb");
  if(f==NULL){

    return pr;
  }
h = gethostbyname(p->ip);
  conexion.sin_family=AF_INET;
  conexion.sin_addr.s_addr=((struct in_addr*)(h->h_addr))->s_addr;
  conexion.sin_port=htons(p->puerto);

  if((descriptor = connect(p->socket,(struct sockaddr *) &conexion,sizeof(conexion)))<0){
    syslog(LOG_ERR,"Error aceptando conexion, cliente");
    fclose(f);
    return pr;
  }

  while((leido=recv(p->socket, &entrada, sizeof(entrada),0))>0){
    fwrite(entrada,1,leido,f);
    entrada[0]=0;
  }
  fclose(f);
  return pr;
}

int funcion_nick_c(char *command, int socket){
  unsigned long dev=0;
  char *nick=NULL, *msg=NULL, *prefix=NULL;
  long u_id=0, creationTS, actionTS;
  char *u_nick=NULL, *u_real=NULL, *host, *IP, *away, *user=NULL,*pantalla=NULL;
  char **canales=NULL;
  int num=0,i=0;

  /*El nuevo nick lo recoge en msg por algun motivo*/
  dev=IRCParse_Nick(command, &prefix, &nick, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("nick_c: : IRCParse_Nick",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("PRIVMSG interfaz: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

  IRCTADUser_Set(u_id,NULL,NULL,NULL,NULL,msg,NULL);
  IRCInterface_ChangeNickThread(u_nick, msg);
  pantalla=(char *)malloc((strlen(msg)+40)*sizeof(char));
  sprintf(pantalla, "Ahora eres conocido como %s", msg);
  IRCInterface_WriteSystemThread ("*", pantalla);
  IRCInterface_ListAllChannelsThread (&canales, &num);
  for(i=0;i<num;i++){
    IRCInterface_WriteChannelThread (canales[i], "*", pantalla);
  }

  IRC_MFree(8,&user,&msg,&pantalla,&u_nick,&u_real,&host,&IP,&away);
  return OK;
}

int funcion_part_c(char *command, int socket){
  unsigned long dev=0;
  char *channel=NULL, *msg=NULL, *prefix=NULL,*pantalla=NULL;
  char *nick=NULL,*user=NULL,*host=NULL,*server=NULL;
  long u_id=0, creationTS, actionTS;
  char *u_nick=NULL, *u_real=NULL, *host_u=NULL, *IP=NULL, *away=NULL, *user_u=NULL;

  dev=IRCParse_Part(command, &prefix, &channel, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("part_c: : IRCParse_Part",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCParse_ComplexUser(prefix,&nick,&user,&host,&server);
  if(dev!=IRC_OK){
    IRC_syslog("PRIVMSG interfaz: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

dev=IRCTADUser_GetData(&u_id, &user_u, &u_nick, &u_real, &host_u, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("kick_cu: IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }
  if(strcmp(nick,u_nick)==0){
    IRCInterface_RemoveChannelThread(channel);
  }else{
    pantalla=(char *)malloc((strlen(nick)+100)*sizeof(char));
    sprintf(pantalla, "El usuario %s nos ha abandonado :(", nick);
    IRCInterface_WriteChannelThread (channel, "*", pantalla);
    free(pantalla);
    IRCInterface_DeleteNickChannelThread (channel,nick);
  }
  IRC_MFree(13, &prefix, &channel, &msg,&nick,&user,&host,&server,&user_u, &u_nick, &u_real, &host_u, &IP, &away);

  return OK;
}

int funcion_ping_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL,*server=NULL,*server2=NULL,*msg=NULL,*enviar=NULL,pong[30];
  IRCInterface_PlaneRegisterInMessageThread(command);
  dev=IRCParse_Ping(command,&prefix,&server,&server2,&msg);
  if(dev!=IRC_OK){
    IRC_syslog("ping_c: : IRCParse_Ping",dev,LOG_ERR);
    return ERR;
  }
	sprintf(pong, "LAG%d", (int)time(NULL));
  dev=IRCMsg_Pong(&enviar,server,server,NULL,pong);
  if(dev!=IRC_OK){
    IRC_syslog("ping_c: : IRCParse_Pong",dev,LOG_ERR);
    return ERR;
  }
 IRCInterface_PlaneRegisterOutMessageThread(enviar);
  send(socket,enviar,strlen(enviar),0);
  return OK;
}

int funcion_namrpl_c(char *command, int socket){
  unsigned long dev;
  char *prefix=NULL,*nick=NULL,*type=NULL,*channel=NULL,*msg=NULL;
  char **list=NULL,*user=NULL,*real=NULL,*host=NULL;
  char *pantalla;
  long nl=0;
  int i;

  dev=IRCParse_RplNamReply (command, &prefix, &nick, &type, &channel, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("namrpl_c: : IRCParse_RplNamReply",dev,LOG_ERR);
    return ERR;
  }
  dev=IRCParse_ParseLists (msg, &list, &nl);
  if(dev!=IRC_OK){
    IRC_syslog("namrpl_c: : IRCParse_ParseLists",dev,LOG_ERR);
    return ERR;
  }
  for(i=0; i<nl; i++){
    //IRCInterface_GetNickChannelInfoThread(list[i],channel,&user,&real,&host);
    //Hay que buscar la manera de obtener esa informacion
    if(list[i][0]=='@'){
      IRCInterface_AddNickChannelThread (channel, ((list[i])+1), "user", "real", "host", OPERATOR);
    }else {
      IRCInterface_AddNickChannelThread (channel, list[i], "user", "real", "host", VOICE);
    }
    IRC_MFree(3,user,real,host);
  }
  pantalla=(char *)malloc((strlen(channel)+strlen(msg)+20)*sizeof(char));
  sprintf(pantalla, "Usuarios en %s: %s", channel, msg);
  IRCInterface_WriteChannelThread (channel, "*", pantalla);
  IRC_MFree(6,&prefix,&nick,&type,&channel,&msg,&pantalla);

  return OK;
}
int funcion_namend_c(char *command, int socket){

  return OK;
}

int funcion_RplWhoReply_c(char *command, int socket){
  char *prefix=NULL,*nick=NULL,*channel=NULL,*user=NULL,*host=NULL,*server=NULL,*nick2=NULL,*type=NULL,*msg=NULL,*realname=NULL;
  unsigned long dev=0;
  int hopcount;
  IRCInterface_PlaneRegisterInMessage(command);
  dev=IRCParse_RplWhoReply (command,&prefix,&nick,&channel,&user, &host, &server, &nick2, &type, &msg,&hopcount,&realname);
  if(dev!=IRC_OK){
    IRC_syslog("who_cu: : IRCMsg_Who",dev,LOG_ERR);
    return ERR;
  }

  IRCInterface_WriteSystemThread("*",command);
/*  if(IRCInterface_QueryChannelExistThread (channel)==TRUE){
      IRCInterface_DeleteNickChannelThread (channel,nick);
    if(strlen(type)>=2){
      IRCInterface_AddNickChannelThread (channel, nick, user, realname, host, OPERATOR);
    }else{

      IRCInterface_AddNickChannelThread (channel,nick , user, realname, host, VOICE);
    }
  }*/

  return OK;
}

int funcion_RplWhoEnd_c(char *command, int socket){
  IRCInterface_WriteSystemThread("*",command);
  return OK;
}

int funcion_whoisuser_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL, *u_nick=NULL, *target_nick=NULL, *name=NULL, *host=NULL, *realname=NULL;
  char *pantalla;

  dev=IRCParse_RplWhoIsUser(command, &prefix, &u_nick, &target_nick, &name, &host, &realname);
  if(dev!=IRC_OK){
    IRC_syslog("whoisuser_c: : IRCParse_RplWhoIsUser",dev,LOG_ERR);
    return ERR;
  }

  pantalla=(char*)malloc((strlen(target_nick)+strlen(name)+strlen(host)+strlen(realname)+20)*sizeof(char));
  sprintf(pantalla, "[%s] (%s): %s", target_nick, host, realname);
  IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);
  //IRC_MFree(7, &prefix, &u_nick, &target_nick, &name, &host, &realname, &pantalla);

  return OK;
}

int funcion_whoisserver_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL, *u_nick=NULL, *target_nick=NULL, *server=NULL, *serverinfo=NULL;
  char *pantalla;

  dev=IRCParse_RplWhoIsServer(command, &prefix, &u_nick, &target_nick, &server, &serverinfo);
  if(dev!=IRC_OK){
    IRC_syslog("whoisserver_c: : IRCParse_RplWhoIsServer",dev,LOG_ERR);
    return ERR;
  }

  pantalla=(char*)malloc((strlen(target_nick)+strlen(server)+strlen(serverinfo)+20)*sizeof(char));
  sprintf(pantalla, "[%s] %s :%s", target_nick, server, serverinfo);
  IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);
  //IRC_MFree(6, &prefix, &u_nick, &target_nick, &server, &serverinfo, &pantalla);

  return OK;
}

int funcion_whoischannels_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL, *u_nick=NULL, *target_nick=NULL, *channel_list=NULL;
  char *pantalla;

  dev=IRCParse_RplWhoIsChannels(command, &prefix, &u_nick, &target_nick, &channel_list);
  if(dev!=IRC_OK){
    IRC_syslog("whoischannels_c: : IRCParse_RplWhoIsChannels",dev,LOG_ERR);
    return ERR;
  }

  pantalla=(char*)malloc((strlen(target_nick)+strlen(channel_list)+20)*sizeof(char));
  sprintf(pantalla, "[%s] %s", target_nick, channel_list);
  IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);
//  IRC_MFree(5, &prefix, &u_nick, &target_nick, &channel_list, &pantalla);

  return OK;
}

int funcion_whoisidle_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL, *u_nick=NULL, *target_nick=NULL, *msg=NULL;
  char *pantalla;
  int actionTS, creationTS;
  long actionTS_aux, creationTS_aux;

  dev=IRCParse_RplWhoIsIdle(command, &prefix, &u_nick, &target_nick, &actionTS, &creationTS, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("whoisidle_c: : IRCParse_RplWhoIsIdle",dev,LOG_ERR);
    return ERR;
  }

  actionTS_aux=actionTS;
  creationTS_aux=creationTS;

  pantalla=(char*)malloc((strlen(target_nick)+20)*sizeof(char));
  sprintf(pantalla, "[%s] inactividad %d, entro el : %d", target_nick, (int)time(&actionTS_aux), (int)time(&creationTS_aux));
  IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);
  //IRC_MFree(5, &prefix, &u_nick, &target_nick, &msg, &pantalla);

  return OK;
}

int funcion_endwhois_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL, *u_nick=NULL, *target_nick=NULL, *msg=NULL;
  char *pantalla;

  dev=IRCParse_RplEndOfWhoIs(command, &prefix, &u_nick, &target_nick, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("endwhois_c: : IRCParse_RplEndOfWhoIs",dev,LOG_ERR);
    return ERR;
  }

  pantalla=(char*)malloc((strlen(target_nick)+20)*sizeof(char));
  sprintf(pantalla, "[%s] Fin de la lista WHOIS.", target_nick);
  IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);
//  IRC_MFree(5, &prefix, &u_nick, &target_nick, &msg, &pantalla);

  return OK;
}

int funcion_RplMotdstart_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL,*nick=NULL,*msg=NULL,*server=NULL;
  dev=IRCParse_RplMotdStart (command,&prefix, &nick,&msg, &server);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_RplendofMotd_c: : IRCParse_RplEndOfMotd",dev,LOG_ERR);
    return ERR;
  }

  IRCInterface_WriteSystemThread("*",msg);
  IRC_MFree(3,&prefix,&nick,&msg);
  return OK;
}

int funcion_RplMotd_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL,*nick=NULL,*msg=NULL;
  dev=IRCParse_RplMotd(command, &prefix,&nick, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_RplMotd_c: : IRCParse_RplMotd",dev,LOG_ERR);
    return ERR;
  }

  IRCInterface_WriteSystemThread("*",msg);
  IRC_MFree(3,&prefix,&nick,&msg);
  return OK;
}

int funcion_RplendofMotd_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL,*nick=NULL,*msg=NULL;
  dev=IRCParse_RplEndOfMotd (command, &prefix,&nick, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_RplendofMotd_c: : IRCParse_RplEndOfMotd",dev,LOG_ERR);
    return ERR;
  }

  IRCInterface_WriteSystemThread("*",msg);
  IRC_MFree(3,&prefix,&nick,&msg);
  return OK;

//  IRCInterface_PlaneRegisterOutMessageThread(enviar);

}

int funcion_RplTopic_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL, *u_nick=NULL, *channel=NULL, *topic=NULL;
  char *pantalla;

  dev=IRCParse_RplTopic(command, &prefix, &u_nick, &channel, &topic);
  if(dev!=IRC_OK){
    IRC_syslog("RplTopic_c: : IRCParse_RplTopic",dev,LOG_ERR);
    return ERR;
  }
if(topic!=NULL){
  pantalla=(char*)malloc((strlen(topic)+strlen(u_nick)+40)*sizeof(char));
  sprintf(pantalla, "%s ha cambiado el tema a: %s", u_nick, topic);
  //IRCInterface_SetTopic(topic);
  IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);
  //IRC_MFree(5, &prefix, &u_nick, &channel, &topic, &pantalla);
}

  return OK;
}

int funcion_RplNoTopic_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL, *u_nick=NULL, *channel=NULL, *topic=NULL;
  char *pantalla;

  dev=IRCParse_RplNoTopic(command, &prefix, &u_nick, &channel, &topic);
  if(dev!=IRC_OK){
    IRC_syslog("RplNoTopic_c: : IRCParse_RplNoTopic",dev,LOG_ERR);
    return ERR;
  }

  if(topic==NULL){
    pantalla=(char*)malloc((strlen(channel)+20)*sizeof(char));
    sprintf(pantalla, "%s :No topic is set", channel);
    IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);
  }
  else{
    pantalla=(char*)malloc((strlen(channel)+strlen(topic)+20)*sizeof(char));
    sprintf(pantalla, "Tema para %s es: %s", channel, topic);
    IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);
  }


  //IRC_MFree(5, &prefix, &u_nick, &channel, &topic, &pantalla);

  return OK;
}

int funcion_mode_c(char *command, int socket){
  unsigned long dev=0;
  char *user=NULL, *mode=NULL,*channeluser=NULL,*prefix=NULL,*pantalla=NULL;
   IRCInterface_PlaneRegisterInMessageThread(command);
  dev=IRCParse_Mode (command, &prefix, &channeluser, &mode, &user);
  if(dev!=IRC_OK){
    IRC_syslog("mode_c: IRCParse_Mode",dev,LOG_ERR);
    return ERR;
  }
  if(channeluser!=NULL && channeluser[0]=='#'){
      if(user!=NULL){
        switch (mode[0]) {
          case '-':
           IRCInterface_ChangeNickStateChannelThread(channeluser, user, NONE);
           if(mode[1]=='o'){
             pantalla=(char*)malloc((strlen(user)+80)*sizeof(char));
             sprintf(pantalla, "El usuario %s ya no es operador", user);
             IRCInterface_WriteChannelThread(channeluser, "*", pantalla);
             free(pantalla);
             pantalla=NULL;
           }
          break;
          default:
          if(strpbrk(mode,"o")!=NULL){
            IRCInterface_ChangeNickStateChannelThread(channeluser, user, OPERATOR);
            pantalla=(char*)malloc((strlen(user)+80)*sizeof(char));
            sprintf(pantalla, "El usuario %s es operador", user);
            IRCInterface_WriteChannelThread(channeluser, "*", pantalla);
            free(pantalla);
            pantalla=NULL;
          }else if(strpbrk(mode,"v")!=NULL){
              IRCInterface_ChangeNickStateChannelThread(channeluser, user, VOICE);
          }
        }
      }else{
        switch (mode[0]) {
          case '+':
            IRCInterface_AddModeChannelThread (channeluser, IRCInterface_ModeToIntModeThread(++mode));
          break;
          case '-':
            IRCInterface_DeleteModeChannelThread (channeluser, IRCInterface_ModeToIntModeThread(++mode));
          break;
          default:
            IRCInterface_SetModeChannelThread (channeluser, IRCInterface_ModeToIntModeThread(mode));
        }
        pantalla=(char*)malloc((strlen(mode)+80)*sizeof(char));
        sprintf(pantalla, "El modo del canal es %s", mode);
        IRCInterface_WriteChannelThread(channeluser, "*", pantalla);
        free(pantalla);
        pantalla=NULL;
      }
  }else{
    pantalla=(char*)malloc((strlen(mode)+80)*sizeof(char));
    sprintf(pantalla, "Tu modo es %s", mode);
    IRCInterface_WriteChannelThread(channeluser, "*", pantalla);
    free(pantalla);
    pantalla=NULL;
  }
/*if(user!=NULL){
  IRC_MFree(4,  &prefix, &channeluser, &mode, &user);
}else{
  IRC_MFree(3,  &prefix, &channeluser, &mode);
}*/
  return OK;
}

int funcion_kick_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL, *channel=NULL, *nick=NULL, *msg=NULL;
  char *pantalla;
  long u_id=0, creationTS, actionTS;
  char *u_nick=NULL, *u_real=NULL, *host, *IP, *away, *user=NULL;

  dev=IRCParse_Kick(command, &prefix, &channel, &nick, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("kick_cu: : IRCMsg_Kick",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("kick_cu: IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

  pantalla=(char*)malloc((strlen(channel)+strlen(nick)+strlen(u_nick)+40)*sizeof(char));
  sprintf(pantalla, "%s ha expulsado a %s de %s", u_nick, nick, channel);
  IRCInterface_DeleteNickChannelThread(channel, nick);
  IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);

  //IRC_MFree(4, &nick, &msg, &prefix, &channel);
  return OK;
}

int funcion_RplList_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL, *nick=NULL, *channel=NULL, *visibilidad=NULL, *topic=NULL;
  char *pantalla;

  dev=IRCParse_RplList(command, &prefix, &nick, &channel, &visibilidad, &topic);
  if(dev!=IRC_OK){
    IRC_syslog("RplList_c: : IRCParse_RplList",dev,LOG_ERR);
    return ERR;
  }

  if(topic==NULL){
    pantalla=(char*)malloc((strlen(channel)+strlen(nick)+strlen(visibilidad)+strlen(prefix)+40)*sizeof(char));
    sprintf(pantalla, "%s %s %s %s", prefix, nick, channel, visibilidad);
  }
  else{
    pantalla=(char*)malloc((strlen(channel)+strlen(nick)+strlen(topic)+strlen(visibilidad)+strlen(prefix)+40)*sizeof(char));
    sprintf(pantalla, "%s %s %s %s :%s", prefix, nick, channel, visibilidad, topic);
  }

  IRCInterface_WriteSystemThread("*", pantalla);

  //IRC_MFree(5, &nick, &visibilidad, &prefix, &channel, &topic);
  return OK;
}

int funcion_RplListEnd_c(char *command, int socket){
  unsigned long dev=0;
  char *prefix=NULL, *nick=NULL, *msg=NULL;
  char *pantalla;

  dev=IRCParse_RplListEnd(command, &prefix, &nick, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("RplList_c: : IRCParse_RplList",dev,LOG_ERR);
    return ERR;
  }

  pantalla=(char*)malloc((strlen(nick)+strlen(msg)+strlen(prefix)+40)*sizeof(char));
  sprintf(pantalla, "%s %s :%s", prefix, nick, msg);
  IRCInterface_WriteSystemThread("*", pantalla);

  //IRC_MFree(3, &nick, &msg, &prefix);
  return OK;
}

int funcion_RplUaway_c(char *command, int socket){

  char pantalla[]="Ya no estas away";

  IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);
  return OK;

}

int funcion_RplNowaway_c(char *command, int socket){

  char *prefix=NULL, *nick=NULL, *msg=NULL,*pantalla=NULL;
  IRCParse_RplNowAway (command, &prefix, &nick,&msg);

  pantalla=(char*)malloc((strlen(nick)+strlen(msg)+200)*sizeof(char));
  sprintf(pantalla, "El usuario %s esta away: %s",  nick, msg);
  IRCInterface_WriteChannelThread(IRCInterface_ActiveChannelNameThread(), "*", pantalla);
  IRC_MFree(4,&prefix,&nick,&msg,&pantalla);

  return OK;

}

int funcion_quit_c(char *command, int socket){
  unsigned long dev=0;
  char *msg=NULL,*prefix=NULL;
  long u_id=0, creationTS, actionTS;
  char *u_nick=NULL, *u_real=NULL, *host, *IP, *away, *user=NULL;
  char *nick=NULL,*u=NULL,*h=NULL,*s=NULL;
  dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_quit_c: IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

  dev= IRCParse_Quit (command, &prefix, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_quit_c: IRCParse_Quit", dev, LOG_ERR);
    return ERR;
  }
  dev=IRCParse_ComplexUser(prefix,&nick,&u,&h,&s);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_quit_c: Error en IRCParse_ComplexUser", dev, LOG_ERR);
    return ERR;
  }

  if(strcmp(nick,u_nick)==0){
    IRCTADUser_Delete (u_id, NULL, NULL, NULL);
    IRCInterface_RemoveAllChannelsThread ();
    IRCInterface_ChangeConectionSelectedThread();
    IRC_MFree(5,&user,&u_nick,&u_real,&host,&IP);
    close(socket);
    exit(0);
  }else {
    char **list=NULL;
    int num=0,i=0;
    IRCInterface_ListAllChannelsThread (&list, &num);
    for(i=0;i<num;i++){
      if(list[i][0]=='#'){
         IRCInterface_DeleteNickChannelThread (list[i], nick);
      }else if(strcmp(nick,list[i])==0){
          IRCInterface_RemoveChannelThread (list[i]);
      }
    }
    IRCInterface_FreeListAllChannelsThread (list, num);
  }

  return OK;
}
