#include "../includes/G-2311-01-P1_server_funciones.h"

int inicializar_array(){
  int i=0;

  for(i=0;i<IRC_MAX_COMMANDS;i++){
    lista_comandos[i]=funcion_default;
  }
  lista_comandos[NICK]=funcion_nick;
  lista_comandos[USER]=funcion_user;
  lista_comandos[JOIN]=funcion_join;
  lista_comandos[LIST]=funcion_list;
  lista_comandos[NAMES]=funcion_names;
  lista_comandos[WHOIS]=funcion_whois;
  lista_comandos[PRIVMSG]=funcion_privmsg;
  lista_comandos[PING]=funcion_ping;
  lista_comandos[PART]=funcion_part;
  lista_comandos[TOPIC]=funcion_topic;
  lista_comandos[KICK]=funcion_kick;
  lista_comandos[AWAY]=funcion_away;
//  lista_comandos[QUIT]=funcion_quit;
  lista_comandos[MOTD]=funcion_motd;
  lista_comandos[MODE]=funcion_mode;


  return OK;
}

int funcion_default(char *command, int socket){

  syslog(LOG_ERR,"Commando no reconocido: %s",command);

  return ERR;
}

int funcion_nick(char *command,int socket){
  long id=0,idc=0;
  unsigned long dev=0;
  struct sockaddr_in  con_cliente;
  socklen_t addr_size = sizeof(con_cliente);
  char *prefix=NULL,*nick=NULL,*msg=NULL,*enviar=NULL;
  char *user=NULL,*n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL;
  char *uc=NULL,*rc=NULL,*hc=NULL,*ipc=NULL,*ac=NULL;
  long int ctsc=0,atsc=0;
  int sc=0;
  long int cts=0,ats=0;
  //IRCTADUser_ShowAll();
  dev=IRCParse_Nick(command,&prefix,&nick,&msg);

  if(dev==IRCERR_NOSTRING){
    syslog(LOG_ERR,"NICK: No se ha introducido ninguna cadena para parsear.");
    return ERR;
  }else if(dev==IRCERR_ERRONEUSCOMMAND){
    syslog(LOG_ERR,"NICK: No se encuentran todos los parámetros obligatorios.");
    return ERR;
  }

  dev=IRCTADUser_GetData(&id,&user,&n,&real,&host,&ip,&socket,&cts,&ats,&away);
//  IRC_MFree(5,user,real,host,ip,away);
  if(dev==IRC_OK){
    if(IRCTADUser_Set(id,NULL,NULL,NULL,NULL,nick,NULL)==IRC_OK){
      if(prefix!=NULL){
      free(prefix);
      prefix=NULL;
  }
       IRC_Prefix(&prefix,n,user,HOST_NAME,ip);
       IRCMsg_Nick(&enviar,prefix,NULL,nick);
       send(socket,enviar,strlen(enviar),0);
    }else{
      IRCMsg_ErrNickNameInUse(&enviar,HOST_NAME,n,nick);
      send(socket,enviar,strlen(enviar),0);
    }
    syslog(LOG_INFO,"NICK: Cambio de nick(%d):%s",socket,enviar);
  }else if(dev==IRCERR_NOENOUGHMEMORY){
  //  IRC_MFree(4,&nick,&prefix,&msg,&n);
    syslog(LOG_ERR,"NICK: Busqueda sin memoria");
    return ERR;
  }else{
    getpeername(socket,(struct sockaddr*) &con_cliente, &addr_size );
    dev=IRCTADUser_GetData(&idc,&uc,&nick,&rc,&hc,&ipc,&sc,&ctsc,&atsc,&ac);
    if(dev==IRC_OK || (dev=IRCTADUser_New("",nick,"",NULL,HOST_NAME,inet_ntoa(con_cliente.sin_addr),socket))!=IRC_OK){
    //  IRC_MFree(4,nick,prefix,msg,n);
      IRC_perror("ERROR DE IRCTADUser_New",dev);
      IRC_syslog("NICK: crear usuario ",dev,LOG_ERR);
      if(dev==IRCERR_NICKUSED){

          IRCMsg_ErrNickNameInUse(&enviar,HOST_NAME,n,nick);
          send(socket,enviar,strlen(enviar),0);
          return OK;
        }
        return ERR;

    }
      syslog(LOG_INFO,"NICK: Creacion de usuario(%d)",socket);
  }
//IRC_MFree(4,nick,prefix,msg,n);
  return OK;
}

int funcion_user(char *command, int socket){

  unsigned long dev=0;
  long int cts=0,ats=0;
  char *prefix=NULL,*user=NULL,*modehost=NULL,*serverother=NULL, *realname=NULL;
  char *n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL,*u=NULL,*enviar=NULL;
  long id=0;

    syslog(LOG_INFO,"USER: Parse(%d)",socket);
  dev=IRCParse_User (command, &prefix,&user,&modehost,&serverother,&realname);
  free(prefix);
  if(dev==IRCERR_NOSTRING){
    syslog(LOG_ERR,"USER: No se ha introducido ninguna cadena para parsear.");
    return ERR;
  }else if(dev==IRCERR_ERRONEUSCOMMAND){
    syslog(LOG_ERR,"USER: No se encuentran todos los parámetros obligatorios.");
    return ERR;
  }
  dev=IRCTADUser_GetData(&id,&u,&n,&real,&host,&ip,&socket,&cts,&ats,&away);
  if(dev==IRC_OK){
    if((dev=IRCTADUser_Set(id,NULL,NULL,NULL,user,NULL,realname))==IRC_OK){
        if(prefix!=NULL){
          free(prefix);
          prefix=NULL;
        }

       dev=IRC_ComplexUser(&prefix, n,user, NULL, ip);
       if(dev!=IRC_OK){
         IRC_syslog("USER: Error en IRC_Prefix", dev, LOG_ERR);
         return ERR;
       }
       IRCMsg_RplWelcome (&enviar,prefix,n,n,user,ip);
       send(socket,enviar,strlen(enviar),0);
       free(enviar);
       syslog(LOG_INFO,"USER:Cambio de user y realname");
       dev=IRCTADUser_SetActionTS(id,NULL,NULL,NULL);
       if(dev!=IRC_OK){
         IRC_syslog("USER: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
         return ERR;
       }
     }else{
        IRC_syslog("USER:Error cambio de user y realname",dev,LOG_ERR);
     }
  }else if(dev==IRCERR_NOENOUGHMEMORY){

    syslog(LOG_ERR,"USER: Busqueda sin memoria");
    return ERR;
  }else{
    IRC_syslog("USER:Error no encuentra el user",dev,LOG_ERR);
  }


  return OK;
}

int funcion_join(char *command, int socket){
  unsigned long dev=0;
  int i;
  long u_id=0, creationTS, actionTS, numnicks;
  char *prefix, *channel=NULL, *key=NULL, *msg=NULL, *enviar=NULL, *topic=NULL, *nicks=NULL;
  char *u_nick=NULL, *u_real=NULL, *host, *IP, *away, *user=NULL, *prefix_aux=NULL;
  char **nicklist=NULL;

  /*Obtenemos user*/
  dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("JOIN: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCTADUser_SetActionTS(u_id,NULL,NULL,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("JOIN: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
    return ERR;
  }

  /*Obtenemos el prefix*/
  dev=IRC_ComplexUser(&prefix, u_nick, user, host, HOST_NAME);
  if(dev!=IRC_OK){
    IRC_syslog("JOIN: Error en IRC_Prefix", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCParse_Join(command, &prefix_aux, &channel, &key, &msg);
  if(dev!=IRC_OK){
    if(dev==IRCERR_ERRONEUSCOMMAND){
      syslog(LOG_ERR, "JOIN:	No se ha introducido algun parametro obligatorio.");
      dev=IRCMsg_ErrNeedMoreParams(&enviar, prefix, u_nick, command);
      send(socket,enviar,strlen(enviar),0);
      return OK;
    }
    else{
      IRC_syslog("JOIN: Error en IRCParse_Join", dev, LOG_ERR);
      return ERR;
    }
  }

  if(channel[0]!='#'){
    syslog(LOG_ERR, "JOIN:	El nombre del canal esta mal introducido.");
    dev=IRCMsg_ErrNoSuchChannel(&enviar, prefix, u_nick, channel);
    send(socket,enviar,strlen(enviar),0);
    return OK;
  }

  /*Hacemos el join*/
  dev=IRCTAD_Join(channel, u_nick, "a", key);
  if(dev!=IRC_OK){
    if(dev==IRCERR_USERSLIMITEXCEEDED){
      syslog(LOG_ERR, "JOIN:	No se admiten más usuarios en el canal.");
      dev=IRCMsg_ErrChannelIsFull(&enviar, prefix, u_nick, channel);
      send(socket,enviar,strlen(enviar),0);
      return OK;
    }
    else if(dev==IRCERR_BANEDUSERONCHANNEL){
      syslog(LOG_ERR, "JOIN:	El usuario no puede unirse al canal por estar banneado y no estar en las excepciones de ban.");
      dev=IRCMsg_ErrBannedFromChan(&enviar, prefix, u_nick, channel);
      send(socket,enviar,strlen(enviar),0);
      return OK;
    }
    else if(dev==IRCERR_NOINVITEDUSER){
      syslog(LOG_ERR, "JOIN: el usuario no ha sido invitado a un canal con invitación.");
      dev=IRCMsg_ErrInviteOnlyChan(&enviar, prefix, u_nick, channel);
      send(socket,enviar,strlen(enviar),0);
      return OK;
    }
    else if(dev==IRCERR_FAIL){ /*COM_ERR_BADCHANNELKEY*/
      syslog(LOG_ERR, "JOIN: el usuario no ha introducido la password correcta.");
      dev=IRCMsg_ErrBadChannelKey(&enviar, prefix, u_nick, channel);
      send(socket,enviar,strlen(enviar),0);
      return OK;
    }
    else{
      IRC_syslog("JOIN: Error en IRCTAD_Join", dev, LOG_ERR);
      return ERR;
    }
  }

  dev=IRCMsg_Join(&enviar, prefix, NULL, NULL, channel);
  if(dev!=IRC_OK){
    IRC_syslog("JOIN: Error en IRCMsg_Join", dev, LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);

  /*Gestionamos el topic*/
  dev=IRCTAD_GetTopic(channel, &topic);
  if(dev!=IRC_OK){
    IRC_syslog("JOIN: Error en IRCTAD_GetTopic", dev, LOG_ERR);
    return ERR;
  }

  if(topic==NULL){
    dev=IRCMsg_RplNoTopic(&enviar, prefix, u_nick, channel);
    if(dev!=IRC_OK){
      IRC_syslog("JOIN: Error en IRCMsg_RplNoTopic", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);
    free(enviar);
  }
  else{
    dev=IRCMsg_RplTopic(&enviar, prefix, u_nick, channel, topic);
    if(dev!=IRC_OK){
      IRC_syslog("JOIN: Error en IRCMsg_RplTopic", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);
    free(enviar);
  }

  //Gestionamos los usuarios en el canal
  dev=IRCTAD_ListNicksOnChannelArray(channel, &nicklist, &numnicks);
  if(dev!=IRC_OK){
    IRC_syslog("JOIN: Error en IRCTAD_ListNicksOnChannel", dev, LOG_ERR);
    return ERR;
  }


  nicks=(char*)malloc(12*numnicks);
  if(nicks==NULL){
  syslog( LOG_ERR,"JOIN: Error en malloc");
    return ERR;
  }
  nicks[0]='\0';


  for(i=0; i<numnicks; i++){
    dev=IRCTAD_GetUserModeOnChannel(channel, nicklist[i]);
    if(dev&&IRCUMODE_OPERATOR){
      strcat(nicks,"@");
      strcat(nicks,nicklist[i]);
      strcat(nicks," ");
    }else{
      strcat(nicks,nicklist[i]);
      strcat(nicks," ");
    }
  }

  dev=IRCMsg_RplNamReply(&enviar, prefix, u_nick, "=", channel, nicks);
  if(dev!=IRC_OK){
    IRC_syslog("JOIN: Error en IRCMsg_RplNamReply", dev, LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);

  syslog(LOG_INFO,"JOIN: finalizado");

  //IRC_MFree(11, prefix, channel, key, msg, enviar, u_nick, u_real, host, IP, away, user);

  return OK;

}

int funcion_list(char *command, int socket){

  char *channel=NULL,*target=NULL,*topic=NULL,*enviar=NULL,*prefix=NULL;
  char **list_c=NULL;
  long num_c=0,id=0,cts=0,ats=0;
  char *u=NULL,*n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL,numU[10];
  unsigned long dev=0;
  int i=0;

  syslog(LOG_INFO,"LIST: iniciando");

 dev=IRCParse_List(command,&prefix, &channel, &target);

 if(dev!=IRC_OK){
    IRC_syslog("LIST: Error parse",dev,LOG_ERR);
    return ERR;
  }

  free(prefix);
  prefix=NULL;

  dev=IRCTADUser_GetData(&id, &u, &n, &real, &host, &ip, &socket, &cts, &ats, &away);
  if(dev!=IRC_OK){
    IRC_syslog("LIST: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }
  dev=IRCTADUser_SetActionTS(id,NULL,NULL,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("LIST: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
    return ERR;
  }

  dev=IRC_ComplexUser(&prefix, n,u, NULL, ip);
  if(dev!=IRC_OK){
    IRC_syslog("LIST: Error en IRC_Prefix", dev, LOG_ERR);
    return ERR;
  }
  IRCMsg_RplListStart(&enviar,prefix,n);
  if(dev!=IRC_OK){
    IRC_syslog("LIST: Error en IRCMsg_RplListStart", dev, LOG_ERR);
    return ERR;
  }
  send(socket,enviar,strlen(enviar),0);
  free(enviar);
  enviar=NULL;

  if(channel!=NULL){
    //caso de un canal especifico
    if(strchr(IRCTADChan_GetModeChar(channel), 115)!=NULL){
      dev=IRCTAD_TestChannelOfUser(channel, n);
      if(dev==IRC_OK){
        dev=IRCTAD_GetTopic(channel,&topic);
        if(dev!=IRC_OK){
          IRC_syslog("LIST: Error en IRCTAD_GetTopic->chanel", dev, LOG_ERR);
          return ERR;
        }

        sprintf(numU, "%ld", IRCTADChan_GetNumberOfUsers(list_c[i]));
        IRCMsg_RplList(&enviar,prefix,n,channel,numU,topic);
        if(dev!=IRC_OK){
          IRC_syslog("LIST: Error en IRCMsg_RplList->chanel", dev, LOG_ERR);
          return ERR;
        }

        send(socket,enviar,strlen(enviar),0);
        free(enviar);
        enviar=NULL;

      }
    }
    else{
      dev=IRCTAD_GetTopic(channel,&topic);
      if(dev!=IRC_OK){
        IRC_syslog("LIST: Error en IRCTAD_GetTopic->chanel", dev, LOG_ERR);
        return ERR;
      }

      sprintf(numU, "%ld", IRCTADChan_GetNumberOfUsers(list_c[i]));
      IRCMsg_RplList(&enviar,prefix,n,channel,numU,topic);
      if(dev!=IRC_OK){
        IRC_syslog("LIST: Error en IRCMsg_RplList->chanel", dev, LOG_ERR);
        return ERR;
      }
      send(socket,enviar,strlen(enviar),0);
      free(enviar);
      enviar=NULL;
    }

  }else{
    //caso de general
    if(IRCTADChan_GetList(&list_c,&num_c,NULL)!=IRC_OK){
      syslog(LOG_ERR,"LIST: Error no hay suficiente memoria, Chan_getList");
      return ERR;
    }

    for(i=0;i<num_c;i++){
      if(strchr(IRCTADChan_GetModeChar(list_c[i]), 115)!=NULL){
        dev=IRCTAD_TestChannelOfUser(list_c[i], n);
        if(dev==IRC_OK){
          syslog(LOG_INFO,"LIST: entra en la funcion de mierda esta2");
          IRCTAD_GetTopic(list_c[i],&topic);
          if(dev!=IRC_OK){
            IRC_syslog("LIST: Error en IRCTAD_GetTopic", dev, LOG_ERR);
            return ERR;
          }
          sprintf(numU, "%ld", IRCTADChan_GetNumberOfUsers(list_c[i]));
          IRCMsg_RplList(&enviar,prefix,n,list_c[i],numU,topic);
          if(dev!=IRC_OK){
            IRC_syslog("LIST: Error en IRCMsg_RplList", dev, LOG_ERR);
            return ERR;
          }
          send(socket,enviar,strlen(enviar),0);
          free(enviar);
          free(topic);
          enviar=NULL;
          topic=NULL;
        }
      }
      else{
        IRCTAD_GetTopic(list_c[i],&topic);
        if(dev!=IRC_OK){
          IRC_syslog("LIST: Error en IRCTAD_GetTopic", dev, LOG_ERR);
          return ERR;
        }
        sprintf(numU, "%ld", IRCTADChan_GetNumberOfUsers(list_c[i]));
        IRCMsg_RplList(&enviar,prefix,n,list_c[i],numU,topic);
        if(dev!=IRC_OK){
          IRC_syslog("LIST: Error en IRCMsg_RplList", dev, LOG_ERR);
          return ERR;
        }
        send(socket,enviar,strlen(enviar),0);
        free(enviar);
        free(topic);
        enviar=NULL;
        topic=NULL;
      }
    }
    free(list_c);
  }

  IRCMsg_RplListEnd(&enviar,prefix,n);

  send(socket,enviar,strlen(enviar),0);
  free(enviar);

  return OK;

}

int funcion_whois(char *command, int socket){
  unsigned long dev=0;
  int i;
  long u_id=0, creationTS, actionTS, numtargets;
  char *prefix=NULL, *target=NULL, *mask=NULL, *enviar=NULL;
  char *u_nick=NULL, *u_real=NULL, *host, *IP, *away, *user=NULL, *prefix_aux=NULL;
  char **target_list=NULL;
  int socket_aux=0;
  long u_id_aux=0, creationTS_aux, actionTS_aux, numchannels;
  char *u_real_aux=NULL, *host_aux, *IP_aux, *away_aux, *user_aux=NULL, *channel_list;
  char mensaje[50];



  /*Obtenemos user*/
  dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("WHOIS: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

  /*Obtenemos el prefix*/
  dev=IRC_ComplexUser(&prefix, u_nick, user, host, HOST_NAME);
  if(dev!=IRC_OK){
    IRC_syslog("WHOIS: Error en IRC_Prefix", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCParse_Whois(command, &prefix_aux, &target, &mask);
  if(dev!=IRC_OK){
    if(dev==IRCERR_ERRONEUSCOMMAND){
      syslog(LOG_ERR, "WHOIS:	No se ha introducido algun parametro obligatorio.");
      dev=IRCMsg_ErrNoNickNameGiven(&enviar, prefix, u_nick);
      send(socket,enviar,strlen(enviar),0);
      return OK;
    }
    else{
      IRC_syslog("WHOIS: Error en IRCParse_Whois", dev, LOG_ERR);
      return ERR;
    }
  }

  /*Por algun motivo desconocido el objetivo del comando los pasa en mask*/
  dev=IRCParse_ParseLists(mask, &target_list, &numtargets);
  if(dev!=IRC_OK){
    IRC_syslog("WHOIS: Error en IRCParse_ParseLists", dev, LOG_ERR);
    return ERR;
  }

  for(i=0; i<numtargets; i++){
    /*Obtenemos usuario de la lista*/
    dev=IRCTADUser_GetData(&u_id_aux, &user_aux, &target_list[i], &u_real_aux, &host_aux, &IP_aux, &socket_aux, &creationTS_aux, &actionTS_aux, &away_aux);
    if(dev!=IRC_OK){
      IRC_syslog("WHOIS: Error en IRCTADUser_GetData de target_list[i]", dev, LOG_ERR);
      return ERR;
    }

    /*Enviamos informacion del usuario*/
    dev=IRCMsg_RplWhoIsUser(&enviar, prefix, u_nick, target_list[i], user_aux, host_aux, u_real_aux);
    if(dev!=IRC_OK){
      IRC_syslog("WHOIS: Error en IRCMsg_RplWhoIsUser", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);

    /*Enviamos informacion del server*/
    dev=IRCMsg_RplWhoIsServer(&enviar, prefix, u_nick, target_list[i], HOST_NAME, HOST_INFO);
    if(dev!=IRC_OK){
      IRC_syslog("WHOIS: Error en IRCMsg_RplWhoIsServer", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);

    /*Obtenemos canales del usuario*/
    dev=IRCTAD_ListChannelsOfUser(user_aux, target_list[i], &channel_list, &numchannels);
    if(dev!=IRC_OK){
      IRC_syslog("WHOIS: Error en IRCTAD_ListChannelsOfUser", dev, LOG_ERR);
      return ERR;
    }

    /*Enviamos informacion de los canales si hay*/
    if(numchannels>0){
      /*
      dev= IRCParse_ParseLists (channel_list,&chanL, &chL);
      if(dev!=IRC_OK){
        IRC_syslog("WHOIS: Error en IRCParse_ParseLists de channels", dev, LOG_ERR);
        return ERR;
      }

      chann=(char*)malloc(12*chL);
      if(chann==NULL){
        syslog( LOG_ERR,"WHOIS: Error en malloc");
        return ERR;
      }

     chann[0]='\0';

      for(j=0; j<chL; j++){
        dev=IRCTAD_GetUserModeOnChannel(chanL[j], target_list[i]);
        if(dev&&IRCUMODE_OPERATOR){
          strcat(chann,"@");
          strcat(chann,chanL[j]);
          strcat(chann," ");
          //strcpy(&channel_list[j], chann);
        }else{
          strcat(chann,chanL[j]);
          strcat(chann," ");
          //strcpy(&channel_list[j], chann);
        }
      }*/
      dev=IRCMsg_RplWhoIsChannels(&enviar, prefix, u_nick, target_list[i], channel_list);
    //  dev=IRCMsg_RplWhoIsChannels(&enviar, prefix, u_nick, target_list[i], chann);
      if(dev!=IRC_OK){
        IRC_syslog("WHOIS: Error en IRCMsg_RplWhoIsChannels", dev, LOG_ERR);
        return ERR;
      }

      send(socket,enviar,strlen(enviar),0);
    }


    dev=IRCMsg_RplWhoIsOperator(&enviar, prefix, u_nick, target_list[i]);
    if(dev!=IRC_OK){
      IRC_syslog("WHOIS: Error en IRCMsg_RplWhoIsOperator", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);

    sprintf(mensaje, ", entro el: %ld", creationTS_aux);
    /*Falta crear el mensaje de cuando se creo, usando creationTS_aux*/
    dev=IRCMsg_RplWhoIsIdle(&enviar, prefix, u_nick, target_list[i], (int)time(&actionTS_aux), mensaje);
    if(dev!=IRC_OK){
      IRC_syslog("WHOIS: Error en IRCMsg_RplWhoIsIdle", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);

  if(away!=NULL && strlen(away)>0){

      dev=IRCMsg_RplAway (&enviar, prefix, u_nick,u_nick, away);
      if(dev!=IRC_OK){
        IRC_syslog("WHOIS: Error en IRCMsg_RplEndOfWhoIs", dev, LOG_ERR);
        return ERR;
      }
      send(socket,enviar,strlen(enviar),0);
    }

    dev=IRCMsg_RplEndOfWhoIs(&enviar, prefix, u_nick, target_list[i]);
    if(dev!=IRC_OK){
      IRC_syslog("WHOIS: Error en IRCMsg_RplEndOfWhoIs", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);

  }

  syslog(LOG_INFO,"WHOIS: finalizado");

  return OK;
}

int funcion_names(char *command, int socket){
  long id=0,cts=0,ats=0,dev,numN=0;
  char *u=NULL,*n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL,*enviar=NULL;
  char *prefix=NULL,*channel=NULL,*target=NULL;
  char *nicks=NULL,**matrizN=NULL;
  int i=0;

  dev=IRCTADUser_GetData(&id, &u, &n, &real, &host, &ip, &socket, &cts, &ats, &away);
  if(dev!=IRC_OK){
    IRC_syslog("NAMES: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }
  dev=IRCTADUser_SetActionTS(id,NULL,NULL,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("NAMES: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCParse_Names(command,&prefix,&channel,&target);
  if(dev!=IRC_OK){
    IRC_syslog("NAMES: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }
  free(prefix);
  prefix=NULL;

  dev=IRC_ComplexUser(&prefix, n,u, NULL, ip);
  if(dev!=IRC_OK){
    IRC_syslog("NAMES: Error en IRC_Prefix", dev, LOG_ERR);
    return ERR;
  }

  if(IRCTADChan_GetModeInt(channel)==IRCMODE_SECRET){
    dev= IRCTAD_ListNicksOnChannelArray(channel,&matrizN, &numN);
    if(dev!=IRC_OK){
      IRC_syslog("NAMES: Error en IRCTAD_ListNicksOnChannel", dev, LOG_ERR);
      dev=IRCMsg_RplEndOfNames(&enviar,prefix,n,channel);
      if(dev!=IRC_OK){
        IRC_syslog("NAMES: Error en IRCMsg_RplEndOfNames", dev, LOG_ERR);
        return ERR;
      }
      send(socket,enviar,strlen(enviar),0);
      return ERR;
    }

    dev=IRCTAD_TestUserOnChannel(channel, n);
    if(dev==IRC_OK){
      nicks=(char*)malloc(12*numN);
      if(nicks==NULL){
      syslog( LOG_ERR,"NAMES: Error en malloc");
        return ERR;
      }
      nicks[0]='\0';

      for(i=0;i<numN;i++){
        dev=IRCTAD_GetUserModeOnChannel(channel,matrizN[i]);
        if(dev && IRCUMODE_OPERATOR){
          strcat(nicks,"@");
          strcat(nicks,matrizN[i]);
          strcat(nicks," ");
        }else{
          strcat(nicks,matrizN[i]);
          strcat(nicks," ");
        }
      }

      dev=IRCMsg_RplNamReply(&enviar,prefix,n,"=",channel,nicks);
      if(dev!=IRC_OK){
        IRC_syslog("NAMES: Error en IRCMsg_RplNameReply", dev, LOG_ERR);
        return ERR;
      }

      send(socket,enviar,strlen(enviar),0);
      free(enviar);
      enviar=NULL;


    }
  }
  else{
    dev= IRCTAD_ListNicksOnChannelArray(channel,&matrizN, &numN);
    if(dev!=IRC_OK){
      IRC_syslog("NAMES: Error en IRCTAD_ListNicksOnChannel", dev, LOG_ERR);
      dev=IRCMsg_RplEndOfNames(&enviar,prefix,n,channel);
      if(dev!=IRC_OK){
        IRC_syslog("NAMES: Error en IRCMsg_RplEndOfNames", dev, LOG_ERR);
        return ERR;
      }
      send(socket,enviar,strlen(enviar),0);
      return ERR;
    }
    if(numN>0){

      nicks=(char*)malloc(12*numN);
      if(nicks==NULL){
      syslog( LOG_ERR,"NAMES: Error en malloc");
        return ERR;
      }
      nicks[0]='\0';

      for(i=0;i<numN;i++){
        dev=IRCTAD_GetUserModeOnChannel(channel,matrizN[i]);
        if(dev && IRCUMODE_OPERATOR){
          strcat(nicks,"@");
          strcat(nicks,matrizN[i]);
          strcat(nicks," ");
        }else{
          strcat(nicks,matrizN[i]);
          strcat(nicks," ");
        }
      }

      dev=IRCMsg_RplNamReply(&enviar,prefix,n,"=",channel,nicks);
      if(dev!=IRC_OK){
        IRC_syslog("NAMES: Error en IRCMsg_RplNameReply", dev, LOG_ERR);
        return ERR;
      }
      send(socket,enviar,strlen(enviar),0);
      free(enviar);
      enviar=NULL;
    }
  }

  dev=IRCMsg_RplEndOfNames(&enviar,prefix,n,channel);
  if(dev!=IRC_OK){
    IRC_syslog("NAMES: Error en IRCMsg_RplEndOfNames", dev, LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);
  free(enviar);
  return OK;
}

int funcion_privmsg(char *command, int socket){
  long id=0,cts=0,ats=0,dev,numN=0;
  int s=0,i=0;
  char *u=NULL,*n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL,*enviar=NULL;
  char *prefix=NULL,*target=NULL,*msg=NULL,*prefixUser=NULL;
  char **matrizN=NULL;


  dev=IRCTADUser_GetData(&id, &u, &n, &real, &host, &ip, &socket, &cts, &ats, &away);
  if(dev!=IRC_OK){
    IRC_syslog("PRIVMSG: Error en IRCTADUser_GetData", dev, LOG_ERR);
    //IRCMsg_ErrWasNoSuchNick
    return ERR;
  }
  dev=IRCTADUser_SetActionTS(id,NULL,NULL,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("PRIVMSG: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
    return ERR;
  }
  /*Obtenemos el prefix*/
  dev=IRC_ComplexUser(&prefixUser, n,u, NULL, ip);
  if(dev!=IRC_OK){
    IRC_syslog("WHOIS: Error en IRC_Prefix", dev, LOG_ERR);
    return ERR;
  }
//  IRC_MFree(6,&u,&real,&host,&ip,&away);
  free(u);
  free(real);
  free(host);
  free(ip);
  u=NULL;
  real=NULL;
  host=NULL;
  ip=NULL;
  id=0;
  cts=0;
  ats=0;
  s=0;
  dev=IRCParse_Privmsg(command,&prefix,&target,&msg);

  if(dev!=IRC_OK){
    IRC_syslog("PRIVMSG: Error en IRCParse_Privmsg", dev, LOG_ERR);
    return ERR;
  }else if(msg!=NULL && strlen(msg)==0){
    IRC_syslog("PRIVMSG: mensaje vacio", dev, LOG_INFO);
    dev=IRCMsg_ErrNoTextToSend(&enviar,prefixUser,n);
    if(dev!=IRC_OK){
      IRC_syslog("PRIVMSG: Error en IRCMsg_ErrNoTextToSend", dev, LOG_ERR);
      return ERR;
    }
    send(socket,enviar,strlen(enviar),0);
    free(enviar);
    free(n);


    return OK;
  }


  if(target[0]=='#'){
    syslog(LOG_INFO,"PRIVMSG: Canal");

    dev=IRCTAD_ListNicksOnChannelArray(target, &matrizN, &numN);

    if(dev!=IRC_OK){
      IRC_syslog("PRIVMSG: Error en IRCTAD_ListNicksOnChannel", dev, LOG_ERR);
      //comprobar si es porque no existe el canal
      return ERR;
    }

    if(numN>0){
    //  free(prefixUser);
      //prefixUser=NULL;
      free(n);

        for(i=0;i<numN;i++){
          dev=IRCTADUser_GetData(&id, &u, &matrizN[i], &real, &host, &ip, &s, &cts, &ats, &away);

          if(dev!=IRC_OK){
            IRC_syslog("PRIVMSG: Error en IRCTADUser_GetData->chanel", dev, LOG_ERR);
            //IRCMsg_ErrWasNoSuchNick
            return ERR;
          }
          if(s!=socket){
            dev=IRC_ComplexUser(&prefixUser, matrizN[i],u, NULL, ip);
            if(dev!=IRC_OK){
              IRC_syslog("WHOIS: Error en IRC_Prefix", dev, LOG_ERR);
              return ERR;
            }
            dev=IRCMsg_Privmsg(&enviar,prefixUser,target,msg);
            if(dev!=IRC_OK){
              IRC_syslog("PRIVMSG: Error en IRCMsg_Privmsg->chanel", dev, LOG_ERR);
              return ERR;
            }
            send(s,enviar,strlen(enviar),0);
            free(prefixUser);
            prefixUser=NULL;
            free(enviar);
            enviar=NULL;
          }

          //IRC_MFree(7,&enviar,&u,&real,&host,&ip,&away,&prefixUser);
          free(u);
          free(real);
          free(host);
          free(ip);
          u=NULL;
          real=NULL;
          host=NULL;
          ip=NULL;
          s=0;
          cts=0;
          ats=0;
          id=0;
        }
    }else{
      //mirar si tiene error
    }

  }else{
    syslog(LOG_INFO,"PRIVMSG: User");
    dev=IRCTADUser_GetData(&id, &u, &target, &real, &host, &ip, &s, &cts, &ats, &away);
    if(dev!=IRC_OK){
      IRC_syslog("PRIVMSG: Error en IRCTADUser_GetData->User", dev, LOG_ERR);
      dev=IRCMsg_ErrNoSuchNick(&enviar,HOST_NAME,n,target);
      if(dev!=IRC_OK){
          IRC_syslog("PRIVMSG: Error en IRCMsg_ErrWasNoSuchNick->User", dev, LOG_ERR);
      }
      send(socket,enviar,strlen(enviar),0);
      free(enviar);
      return ERR;
    }
    dev=IRCMsg_Privmsg(&enviar,prefixUser,target,msg);
    if(dev!=IRC_OK){
      IRC_syslog("PRIVMSG: Error en IRCMsg_Privmsg->user", dev, LOG_ERR);
      return ERR;
    }
    send(s,enviar,strlen(enviar),0);
    free(enviar);
    if(away!=NULL){
      dev=IRCMsg_RplAway(&enviar,prefixUser,target,target,away);
      if(dev!=IRC_OK){
        IRC_syslog("PRIVMSG: Error en IRCMsg_RplAway", dev, LOG_ERR);
        return ERR;
      }
      send(s,enviar,strlen(enviar),0);
      free(enviar);
    }
  }
  syslog(LOG_INFO,"PRIVMSG: finalizado");
  return OK;

}

int funcion_ping(char *command, int socket){
  long dev=0, u_id=0, creationTS, actionTS;
  char *prefix=NULL, *server=NULL, *target=NULL, *msg=NULL, *prefix_aux=NULL;
  char *enviar=NULL, *u_nick=NULL, *user=NULL, *u_real=NULL, *host, *IP, *away;
  int socket_target=0;
  long u_id_tgt=0, creationTS_tgt, actionTS_tgt;
  char *user_tgt=NULL, *u_real_tgt=NULL, *host_tgt, *IP_tgt, *away_tgt;

  /*Obtenemos user*/
  dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("PING: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCTADUser_SetActionTS(u_id,NULL,NULL,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("PING: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
    return ERR;
  }
  /*Obtenemos el prefix*/
  dev=IRC_ComplexUser(&prefix, u_nick, user, host, HOST_NAME);
  if(dev!=IRC_OK){
    IRC_syslog("PING: Error en IRC_Prefix", dev, LOG_ERR);
    return ERR;
  }

  /*El numerito lo devuelve en server en vez de msg*/
  dev=IRCParse_Ping(command, &prefix_aux, &server, &target, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("PING: Error en IRCParse_Ping", dev, LOG_ERR);
    return ERR;
  }

  /*Si recibe un objetivo envia el ping a este, sino responde con un pong*/
  if(target!=NULL){
    dev=IRCTADUser_GetData(&u_id_tgt, &user_tgt, &target, &u_real_tgt, &host_tgt, &IP_tgt, &socket_target, &creationTS_tgt, &actionTS_tgt, &away_tgt);
    if(dev!=IRC_OK){
      IRC_syslog("PING: Error en IRCTADUser_GetData de target", dev, LOG_ERR);
      return ERR;
    }
    //hace falta server?
    dev=IRCMsg_Ping(&enviar, prefix, HOST_NAME, target);
    if(dev!=IRC_OK){
      IRC_syslog("PING: Error en IRCMsg_Ping", dev, LOG_ERR);
      return ERR;
    }

    send(socket_target, enviar,strlen(enviar),0);
    syslog(LOG_INFO,"PING: ping enviado a objetivo");

  }
  else{
    dev=IRCMsg_Pong(&enviar, HOST_NAME, HOST_NAME, target, server);
    if(dev!=IRC_OK){
      IRC_syslog("PING: Error en IRCMsg_Pong", dev, LOG_ERR);
      return ERR;
    }

    send(socket, enviar,strlen(enviar),0);
    syslog(LOG_INFO,"PING: ping enviado");
  }

  //IRC_MFree(5, prefix, server, target, msg, enviar);
  return OK;
}

int funcion_part(char *command, int socket){
    long id=0,cts=0,ats=0,dev;
    char *u=NULL,*n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL,*enviar=NULL;
    char *prefix=NULL,*channel=NULL;
    char *msg=NULL;

      syslog(LOG_INFO,"PART: empezando");
    dev=IRCTADUser_GetData(&id, &u, &n, &real, &host, &ip, &socket, &cts, &ats, &away);
    if(dev!=IRC_OK){
      IRC_syslog("PART: Error en IRCTADUser_GetData", dev, LOG_ERR);
      return ERR;
    }
    dev=IRCTADUser_SetActionTS(id,NULL,NULL,NULL);
    if(dev!=IRC_OK){
      IRC_syslog("PART: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
      return ERR;
    }

    dev= IRCParse_Part (command,&prefix, &channel, &msg);
    if(dev!=IRC_OK){
      IRC_syslog("PART: Error en IRCParse_Part", dev, LOG_ERR);
      return ERR;
    }

    if(prefix!=NULL){
      free(prefix);
      prefix=NULL;
    }

    dev=IRC_ComplexUser(&prefix, n,u, NULL, ip);
    if(dev!=IRC_OK){
      IRC_syslog("PART: Error en IRC_ComplexUser", dev, LOG_ERR);
      return ERR;
    }

    dev=IRCTAD_TestChannelOfUser(channel,n);

    switch (dev) {
      case IRC_OK:
        dev=IRCTAD_Part (channel,n);
        if(dev!=IRC_OK){
          IRC_syslog("PART: Error en IRCMsg_Part", dev, LOG_ERR);
          return ERR;
        }
        dev=IRCMsg_Part (&enviar, prefix,channel,msg);
        if(dev!=IRC_OK){
          IRC_syslog("PART: Error en IRCMsg_Part", dev, LOG_ERR);
          return ERR;
        }
        send(socket, enviar,strlen(enviar),0);
      break;
      case IRCERR_NOVALIDUSER:
      case IRCERR_FAIL:
        dev=IRCMsg_ErrUserOnChannel (&enviar, HOST_NAME, n, u, channel);
        if(dev!=IRC_OK){
          IRC_syslog("PART: Error en IRCMsg_ErrNotOnChannel", dev, LOG_ERR);
          return ERR;
        }
        send(socket, enviar,strlen(enviar),0);
      break;
      case IRCERR_NOVALIDCHANNEL:
      //devolver el caso de que no exista el canal
      dev=IRCMsg_ErrNoSuchChannel(&enviar, HOST_NAME, n,channel);
      if(dev!=IRC_OK){
        IRC_syslog("PART: Error en IRCMsg_ErrNotOnChannel", dev, LOG_ERR);
        return ERR;
      }
      send(socket, enviar,strlen(enviar),0);
      break;
    }

  syslog(LOG_INFO,"PART: finalizado");
  return OK;
}

int funcion_topic(char *command, int socket){
  char *prefix=NULL, *prefix_aux=NULL, *channel=NULL,*topic=NULL,*t=NULL;
  char *enviar=NULL,*u=NULL,*n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL;
  long id=0,cts=0,ats=0;

  unsigned long dev=0;
  syslog(LOG_INFO,"TOPIC Iniciando");
  dev=IRCParse_Topic (command, &prefix_aux, &channel, &topic);
  if(dev!=IRC_OK){
    IRC_syslog("TOPIC: Error en IRCParse_Topic", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCTADUser_GetData(&id, &u, &n, &real, &host, &ip, &socket, &cts, &ats, &away);
  if(dev!=IRC_OK){
    IRC_syslog("TOPIC: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCTADUser_SetActionTS(id,NULL,NULL,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("TOPIC: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
    return ERR;
  }

  /*Obtenemos el prefix*/
  dev=IRC_ComplexUser(&prefix, n, u, host, HOST_NAME);
  if(dev!=IRC_OK){
    IRC_syslog("TOPIC: Error en IRC_Prefix", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCTADChan_GetModeInt(channel);

  if(dev&IRCMODE_TOPICOP){
    dev=IRCTAD_GetUserModeOnChannel(channel, n);
    if(!(dev && IRCUMODE_OPERATOR)){
      dev=IRCMsg_ErrChanOPrivsNeeded(&enviar, prefix, n, channel);
      send(socket,enviar,strlen(enviar),0);
      return OK;
    }
  }

  dev=IRCTAD_GetTopic (channel, &t);
  if(dev!=IRC_OK){
    IRC_syslog("TOPIC: Error en IRCTAD_GetTopic", dev, LOG_ERR);
    if(dev==IRCERR_INVALIDCHANNELNAME){
      IRCMsg_ErrNoSuchChannel (&enviar, HOST_NAME, n,channel);
      send(socket, enviar,strlen(enviar),0);

      return OK;
    }

    return ERR;
  }

  if(topic==NULL){
    //caso de que pide el topic

      if(t==NULL){
        syslog(LOG_INFO,"TOPIC no topic");
        dev=IRCMsg_RplNoTopic(&enviar, HOST_NAME, n, channel);
        if(dev!=IRC_OK){
          IRC_syslog("TOPIC: Error en IRCMsg_RplNoTopic", dev, LOG_ERR);
          return ERR;
        }

        send(socket,enviar,strlen(enviar),0);
        free(enviar);
      }
      else{
        syslog(LOG_INFO,"TOPIC tiene topic");
        dev=IRCMsg_RplTopic(&enviar, HOST_NAME, n, channel, t);
        if(dev!=IRC_OK){
          IRC_syslog("TOPIC: Error en IRCMsg_RplTopic", dev, LOG_ERR);
          return ERR;
        }

        send(socket,enviar,strlen(enviar),0);
        free(enviar);
      }

  }else{

    dev=IRCTAD_SetTopic (channel, n, topic);
    if(dev!=IRC_OK){
      IRC_syslog("TOPIC: Error en IRCTAD_SetTopic", dev, LOG_ERR);
      return ERR;
    }
    dev=IRC_ComplexUser(&prefix, n, u, NULL, HOST_NAME);
    if(dev!=IRC_OK){
      IRC_syslog("TOPIC: Error en IRC_ComplexUser", dev, LOG_ERR);
      return ERR;
    }
    dev=IRCMsg_Topic (&enviar,prefix, channel, topic);
    if(dev!=IRC_OK){
      IRC_syslog("TOPIC: Error en IRCMsg_Topic", dev, LOG_ERR);
      return ERR;
    }
    send(socket,enviar,strlen(enviar),0);
    free(enviar);
    syslog(LOG_INFO,"TOPIC CAMBIADO");

  }
syslog(LOG_INFO,"TOPIC Finalizado");
return OK;
}

int funcion_kick(char *command, int socket){
  long dev=0, u_id=0, creationTS, actionTS, numtargets;
  char *enviar=NULL, *u_nick=NULL, *user=NULL, *u_real=NULL, *host, *IP, *away;
  char *prefix=NULL, *prefix_aux=NULL, *channel=NULL, *target=NULL, *msg=NULL;
  char **target_list=NULL;
  int i, socket_aux=0;
  long u_id_aux=0, creationTS_aux, actionTS_aux;
  char *user_aux=NULL, *u_real_aux=NULL, *host_aux, *IP_aux, *away_aux, *prefix_target=NULL;

  /*Obtenemos user*/
  dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("KICK: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCTADUser_SetActionTS(u_id,NULL,NULL,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("KICK: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
    return ERR;
  }
  /*Obtenemos el prefix*/
  dev=IRC_ComplexUser(&prefix, u_nick, user, host, HOST_NAME);
  if(dev!=IRC_OK){
    IRC_syslog("KICK: Error en IRC_Prefix", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCParse_Kick(command, &prefix_aux, &channel, &target, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("KICK: Error en IRCParse_Kick", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCTAD_GetUserModeOnChannel(channel, u_nick);
  if(!(dev && IRCUMODE_OPERATOR)){
    dev=IRCMsg_ErrChanOPrivsNeeded(&enviar, prefix, u_nick, channel);
    send(socket,enviar,strlen(enviar),0);
    return OK;
  }

  dev=IRCParse_ParseLists(target, &target_list, &numtargets);
  if(dev!=IRC_OK){
    IRC_syslog("KICK: Error en IRCParse_ParseLists", dev, LOG_ERR);
    return ERR;
  }

  for(i=0; i<numtargets; i++){
    /*Obtenemos usuario de la lista*/
    dev=IRCTADUser_GetData(&u_id_aux, &user_aux, &target_list[i], &u_real_aux, &host_aux, &IP_aux, &socket_aux, &creationTS_aux, &actionTS_aux, &away_aux);
    if(dev!=IRC_OK){
      IRC_syslog("KICK: Error en IRCTADUser_GetData de target_list[i]", dev, LOG_ERR);
      return ERR;
    }

    dev=IRC_ComplexUser(&prefix_target, target_list[i], user_aux, host_aux, HOST_NAME);
    if(dev!=IRC_OK){
      IRC_syslog("KICK: Error en IRC_Prefix target", dev, LOG_ERR);
      return ERR;
    }

    dev=IRCTAD_KickUserFromChannel(channel, target_list[i]);
    if(dev!=IRC_OK){
      IRC_syslog("KICK: Error en IRCTAD_KickUserFromChannel", dev, LOG_ERR);
      return ERR;
    }

    dev=IRCMsg_Kick(&enviar, prefix, channel, target_list[i], msg);
    if(dev!=IRC_OK){
      IRC_syslog("KICK: Error en IRCMsg_Kick", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);

    dev=IRCMsg_Kick(&enviar, prefix_target, channel, target_list[i], msg);
    if(dev!=IRC_OK){
      IRC_syslog("KICK: Error en IRCMsg_Kick target", dev, LOG_ERR);
      return ERR;
    }

    send(socket_aux,enviar,strlen(enviar),0);

  }

  syslog(LOG_INFO,"KICK: finalizado");

  return OK;
}

int funcion_away(char *command, int socket){

  char *prefix=NULL,*msg=NULL;
  unsigned long dev;
  char *enviar=NULL,*u=NULL,*n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL;
  long id=0,cts=0,ats=0;
  dev=IRCParse_Away (command, &prefix, &msg);

  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRCParse_Away", dev, LOG_ERR);
    return ERR;
  }
  dev=IRCTADUser_GetData(&id, &u, &n, &real, &host, &ip, &socket, &cts, &ats, &away);
  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }
  dev=IRCTADUser_SetActionTS(id,NULL,NULL,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
    return ERR;
  }

  dev=IRC_ComplexUser(&prefix, n,u, NULL, ip);
  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRC_ComplexUser", dev, LOG_ERR);
    return ERR;
  }

  if(msg==NULL){
    //Caso de quitar el away
    if(away!=NULL || strlen(away)!=0){
      dev=IRCTADUser_SetAway (id, NULL, NULL,NULL, NULL);
      if(dev!=IRC_OK){
        IRC_syslog("AWAY: Error en IRCTADUser_SetAway", dev, LOG_ERR);
        return ERR;
      }
      dev= IRCMsg_RplUnaway (&enviar, prefix, n);
      if(dev!=IRC_OK){
        IRC_syslog("AWAY: Error en IRCMsg_RplUnaway", dev, LOG_ERR);
        return ERR;
      }
      send(socket,enviar,strlen(enviar),0);
      free(enviar);
    }else{
      //mirar el error de quitar el away que no tiene
    }
  }else{
    //Caso de poner away
    dev=IRCTADUser_SetAway (id, NULL, NULL,NULL, msg);
    if(dev!=IRC_OK){
      IRC_syslog("AWAY: Error en IRCTADUser_SetAway", dev, LOG_ERR);
      return ERR;
    }
     dev=IRCMsg_RplNowAway (&enviar,prefix, n);
    //dev=IRCMsg_RplAway (&enviar,prefix, n,n, "You have been marked as being away");
    if(dev!=IRC_OK){
      IRC_syslog("AWAY: Error en IRCTADUser_SetAway", dev, LOG_ERR);
      return ERR;
    }
    send(socket,enviar,strlen(enviar),0);
    free(enviar);
  }
  return OK;
}

int funcion_quit(char *command, int socket){

  char *prefix=NULL,*msg=NULL;
  unsigned long dev;
  char *enviar=NULL,*u=NULL,*n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL;
  long id=0,cts=0,ats=0;

  dev=IRCParse_Quit (command, &prefix, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("QUIT: Error en IRCParse_Quit", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCTADUser_GetData(&id, &u, &n, &real, &host, &ip, &socket, &cts, &ats, &away);
  if(dev!=IRC_OK){
    IRC_syslog("QUIT: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

  IRCTAD_Quit(n);
  if(msg!=NULL){
    dev= IRCMsg_Notice (&enviar, HOST_NAME, n,msg);
    if(dev!=IRC_OK){
      IRC_syslog("QUIT: Error en IRCMsg_Quit->msg", dev, LOG_ERR);
      return ERR;
    }

  }else{
    dev= IRCMsg_Notice (&enviar, HOST_NAME,n, n);
    if(dev!=IRC_OK){
      IRC_syslog("QUIT: Error en IRCMsg_Quit->n", dev, LOG_ERR);
      return ERR;
    }
  }
  send(socket,enviar,strlen(enviar),0);
  close(socket);
  IRC_MFree(9, &prefix, &msg, &enviar, &u, &n, &real, &host, &ip, &away);

  return OK;
}

int funcion_motd(char *command, int socket){
  char *prefix=NULL,*msg=NULL;
  unsigned long dev;
  char *enviar=NULL,*u=NULL,*n=NULL,*real=NULL,*host=NULL,*ip=NULL,*away=NULL;
  long id=0,cts=0,ats=0;
  dev=IRCParse_Motd (command, &prefix, &msg);

  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRCParse_Away", dev, LOG_ERR);
    return ERR;
  }
  dev=IRCTADUser_GetData(&id, &u, &n, &real, &host, &ip, &socket, &cts, &ats, &away);
  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }
  dev=IRCTADUser_SetActionTS(id,NULL,NULL,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
    return ERR;
  }

  dev=IRC_ComplexUser(&prefix, n,u, NULL, ip);
  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRC_ComplexUser", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCMsg_RplMotdStart(&enviar,prefix,n,HOST_NAME);
  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRC_ComplexUser", dev, LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);
  free(enviar);

  dev=IRCMsg_RplMotd(&enviar,prefix,n,"Bienvenidos al servidor VERY DEP: Esperemos que disfruten de su estancia");
  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRC_ComplexUser", dev, LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);
  free(enviar);

  dev=IRCMsg_RplEndOfMotd(&enviar,prefix,n);
  if(dev!=IRC_OK){
    IRC_syslog("AWAY: Error en IRC_ComplexUser", dev, LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);
  free(enviar);
  free(prefix);

  return OK;
}

int funcion_mode(char *command, int socket){
  long dev=0, u_id=0, creationTS, actionTS;
  char *enviar=NULL, *u_nick=NULL, *user=NULL, *u_real=NULL, *host, *IP, *away;
  char *prefix=NULL, *prefix_aux=NULL, *channel=NULL, *mode=NULL, *msg=NULL;

  syslog(LOG_INFO,"MODE: empezando %s",command);
  /*Obtenemos user*/
  dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("MODE: Error en IRCTADUser_GetData", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCTADUser_SetActionTS(u_id,NULL,NULL,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("MODE: Error en IRCTADUser_SetActionTS", dev, LOG_ERR);
    return ERR;
  }
  /*Obtenemos el prefix*/
  dev=IRC_ComplexUser(&prefix, u_nick, user, host, HOST_NAME);
  if(dev!=IRC_OK){
    IRC_syslog("MODE: Error en IRC_Prefix", dev, LOG_ERR);
    return ERR;
  }

  dev=IRCParse_Mode(command, &prefix_aux, &channel, &mode, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("MODE: Error en IRCParse_Mode", dev, LOG_ERR);
    return ERR;
  }

  if(mode==NULL){
    return OK;
  }

  dev=IRCTAD_GetUserModeOnChannel(channel, u_nick);
  if(!(dev && IRCUMODE_OPERATOR)){
    dev=IRCMsg_ErrChanOPrivsNeeded(&enviar, prefix, u_nick, channel);
    send(socket,enviar,strlen(enviar),0);
    return OK;
  }

  if(strcmp(mode, "+t")==0){
    syslog(LOG_INFO,"MODE: +t");
    dev=IRCTAD_Mode(channel, u_nick, mode);
    if(dev!=IRC_OK){
      IRC_syslog("MODE: Error en IRCTAD_Mode t", dev, LOG_ERR);
      return ERR;
    }

    dev=IRCMsg_Mode(&enviar, prefix, channel, mode, u_nick);
    if(dev!=IRC_OK){
      IRC_syslog("MODE: Error en IRCMsg_Mode t", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);
    return OK;

  }
  else if(strcmp(mode, "+s")==0){
    syslog(LOG_INFO,"MODE: +s");
    dev=IRCTAD_Mode(channel, u_nick, mode);
    if(dev!=IRC_OK){
      IRC_syslog("MODE: Error en IRCTAD_Mode s", dev, LOG_ERR);
      return ERR;
    }

    dev=IRCMsg_Mode(&enviar, prefix, channel, mode, u_nick);
    if(dev!=IRC_OK){
      IRC_syslog("MODE: Error en IRCMsg_Mode s", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);
    return OK;

  }
  else if(strcmp(mode, "+k")==0||strcmp(mode, "\\+k")==0){
    syslog(LOG_INFO,"MODE: +k");
    dev=IRCTAD_Mode(channel, u_nick, mode);
    if(dev!=IRC_OK){
      IRC_syslog("MODE: Error en IRCTAD_Mode s", dev, LOG_ERR);
      return ERR;
    }
    dev=IRCTADChan_SetPassword (channel,msg);
    if(dev!=IRC_OK){
      IRC_syslog("MODE: Error en IRCTADChan_SetPassword", dev, LOG_ERR);
      return ERR;
    }
    dev=IRCMsg_Mode(&enviar, prefix, channel, mode, u_nick);
    if(dev!=IRC_OK){
      IRC_syslog("MODE: Error en IRCMsg_Mode s", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);
    return OK;
  }
  else{
    syslog(LOG_INFO,"MODE: desconocido");
    dev=IRCMsg_ErrUModeUnknownFlag(&enviar, prefix, u_nick);
    if(dev!=IRC_OK){
      IRC_syslog("MODE: Error en IRCMsg_ErrUModeUnknownFlag", dev, LOG_ERR);
      return ERR;
    }

    send(socket,enviar,strlen(enviar),0);
    syslog(LOG_INFO,"MODE: terminado");
    return OK;
  }

}
