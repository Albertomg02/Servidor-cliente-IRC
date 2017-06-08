#include "../includes/G-2311-01-P2_client_funciones_user.h"

int inicializar_array_cu(){
  int i;
  for(i=0;i<IRC_MAX_COMMANDS;i++){
    comandos_cliente_usuario[i]=funcion_default_cu;
  }
  comandos_cliente_usuario[UJOIN]=funcion_join_cu;
  comandos_cliente_usuario[UMSG]=funcion_priv_cu;
  comandos_cliente_usuario[UNICK]=funcion_nick_cu;
  comandos_cliente_usuario[UPART]=funcion_part_cu;
  comandos_cliente_usuario[UNAMES]=funcion_names_cu;
  comandos_cliente_usuario[UWHO]= funcion_who_cu;
  comandos_cliente_usuario[UWHOIS]=funcion_whois_cu;
  comandos_cliente_usuario[UMOTD] =funcion_motd_cu;
  comandos_cliente_usuario[UTOPIC]=funcion_topic_cu;
  comandos_cliente_usuario[UKICK]=funcion_kick_cu;
  comandos_cliente_usuario[ULIST]=funcion_list_cu;
  comandos_cliente_usuario[UAWAY]=funcion_away_cu;
  comandos_cliente_usuario[UQUERY]=funcion_query_cu;
  comandos_cliente_usuario[UMODE]=funcion_mode_cu;
  comandos_cliente_usuario[UQUIT]=funcion_quit_cu;

  return OK;
}

int funcion_default_cu(char *command, int socket){

  syslog(LOG_ERR,"Commando no reconocido: %s",command);

  return ERR;
}

int funcion_join_cu(char *command, int socket){

  unsigned long dev=0;
  char *channel=NULL,*password=NULL,*enviar=NULL;

  dev=IRCUserParse_Join(command,&channel, &password);
  if(dev!=IRC_OK){
    IRC_syslog("rpljoin_cu: : IRCUserParse_Join",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCMsg_Join (&enviar, NULL,channel, password,NULL);
  if(dev!=IRC_OK){
    IRC_syslog("rpljoin_cu: : IRCMsg_Join",dev,LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);
//  IRCInterface_PlaneRegisterOutMessageThread(enviar);
  IRC_MFree(3,&enviar,&channel,&password);

  return OK;
}

int funcion_priv_cu(char *command, int socket){
  unsigned long dev=0;
  char *enviar=NULL;
  long u_id=0, creationTS, actionTS;
  char *u_nick=NULL, *u_real=NULL, *host, *IP, *away, *user=NULL,*canal=NULL;
/*  dev=IRCUserParse_Priv(command, &target, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("rplpriv_cu: : IRCUserParse_Priv",dev,LOG_ERR);
    return ERR;
  }*/
  dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("rplpriv_cu: : IRCTADUser_GetData",dev,LOG_ERR);
    return ERR;
  }
  canal=IRCInterface_ActiveChannelName();
  dev=IRCMsg_Privmsg (&enviar, NULL, canal, command);
  if(dev!=IRC_OK){
    IRC_syslog("rplpriv_cu: : IRCMsg_Privmsg",dev,LOG_ERR);
    return ERR;
  }
  IRCInterface_WriteChannel(canal, u_nick, command);
  send(socket,enviar,strlen(enviar),0);
//  IRCInterface_PlaneRegisterOutMessageThread(enviar);
  IRC_MFree(1,&enviar);

  return OK;
}

int funcion_nick_cu(char *command, int socket){
  unsigned long dev=0;
  char *nick=NULL, *enviar=NULL;

  dev=IRCUserParse_Nick(command, &nick);
  if(dev!=IRC_OK){
    IRC_syslog("rplnick_cu: : IRCUserParse_Nick",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCMsg_Nick(&enviar, NULL, nick, NULL);
  if(dev!=IRC_OK){
    IRC_syslog("rplnick_cu: : IRCMsg_Nick",dev,LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);
//  IRCInterface_PlaneRegisterOutMessageThread(enviar);
  IRC_MFree(2,&enviar,&nick);

  return OK;
}

int funcion_part_cu(char *command, int socket){
  unsigned long dev=0;
  char *channel=NULL, *enviar=NULL;

  dev=IRCUserParse_Part(command, &channel);
  if(dev!=IRC_OK){
    IRC_syslog("rplpart_cu: : IRCUserParse_Part",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCMsg_Part(&enviar, NULL, channel, NULL);
  if(dev!=IRC_OK){
    IRC_syslog("rplpart_cu: : IRCMsg_Part",dev,LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);
  //IRCInterface_PlaneRegisterOutMessageThread(enviar);
  IRC_MFree(2,&enviar,&channel);

  return OK;
}

int funcion_names_cu(char *command, int socket){
  unsigned long dev=0;
  char *channel=NULL,*targetserver=NULL,*enviar=NULL;
  dev=IRCUserParse_Names( command,&channel, &targetserver);
  if(dev!=IRC_OK){
    IRC_syslog("names_cu: : IRCUserParse_Names",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCMsg_Names (&enviar, NULL,((channel!=NULL)?channel : IRCInterface_ActiveChannelName()),targetserver);
  if(dev!=IRC_OK){
    IRC_syslog("names_cu: IRCMsg_Names",dev,LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);
  IRCInterface_PlaneRegisterOutMessage(enviar);
  IRC_MFree(3,&channel,&targetserver,&enviar);
  return OK;
}

int funcion_who_cu(char *command, int socket){
  char *mask=NULL,*enviar=NULL;
  unsigned long dev=0;
  dev=IRCUserParse_Who(command,&mask);

  if(dev!=IRC_OK){
    IRC_syslog("who_cu: : IRCUserParse_Who",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCMsg_Who (&enviar, NULL, mask, NULL);
  if(dev!=IRC_OK){
    IRC_syslog("who_cu: : IRCMsg_Who",dev,LOG_ERR);
    return ERR;
  }
  send(socket,enviar,strlen(enviar),0);
  IRCInterface_PlaneRegisterOutMessage(enviar);
  IRC_MFree(2,&mask,&enviar);
  return OK;
}

int funcion_whois_cu(char *command, int socket){
  unsigned long dev=0;
  char *target=NULL, *enviar=NULL;

  dev=IRCUserParse_Whois(command, &target);
  if(dev!=IRC_OK){
    IRC_syslog("whois_cu: : IRCUserParse_Whois",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCMsg_Whois(&enviar, NULL, NULL, target);
  if(dev!=IRC_OK){
    IRC_syslog("whois_cu: : IRCMsg_Whois",dev,LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);
//  IRCInterface_PlaneRegisterOutMessage(enviar);

//  IRC_MFree(2, &target, &enviar);
  return OK;
}

int funcion_motd_cu(char *command, int socket){
  unsigned long dev=0;
  char *server=NULL,*enviar=NULL;
  dev=IRCUserParse_Motd (command, &server);
  if(dev!=IRC_OK){
    IRC_syslog("endwhois_c: : IRCParse_RplEndOfWhoIs",dev,LOG_ERR);
    return ERR;
  }
  dev=IRCMsg_Motd (&enviar, NULL, server);
  if(dev!=IRC_OK){
    IRC_syslog("endwhois_c: : IRCParse_RplEndOfWhoIs",dev,LOG_ERR);
}
  IRC_MFree(2,&server,&enviar);
  return OK;
}

int funcion_topic_cu(char *command, int socket){
  unsigned long dev=0;
  char *topic=NULL, *enviar=NULL;

  dev=IRCUserParse_Topic(command, &topic);
  if(dev!=IRC_OK){
    IRC_syslog("topic_cu: : IRCUserParse_Topic",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCMsg_Topic(&enviar, NULL, IRCInterface_ActiveChannelName(), topic);
  if(dev!=IRC_OK){
    IRC_syslog("topic_cu: : IRCMsg_Topic",dev,LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);

    IRC_MFree(2, &topic, &enviar);
    return OK;

}

int funcion_mode_cu(char *command, int socket){
  unsigned long dev=0;
  char *filter=NULL, *mode=NULL,*enviar=NULL;
  dev=IRCUserParse_Mode (command, &filter, &mode);
  if(dev!=IRC_OK){
    IRC_syslog("mode_cu: : IRCUserParse_Mode",dev,LOG_ERR);
    return ERR;
  }
  dev=IRCMsg_Mode (&enviar, NULL,IRCInterface_ActiveChannelName() ,mode, filter);
  if(dev!=IRC_OK){
    IRC_syslog("mode_cu: : IRCMsg_Mode",dev,LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);
  IRCInterface_PlaneRegisterOutMessage(enviar);
  IRC_MFree(3, &filter, &enviar,&mode);
  return OK;
}

int funcion_kick_cu(char *command, int socket){
  unsigned long dev=0;
  char *nick=NULL, *msg=NULL, *enviar=NULL;

  dev=IRCUserParse_Kick(command, &nick, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("kick_cu: : IRCUserParse_Kick",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCMsg_Kick(&enviar, NULL, IRCInterface_ActiveChannelName(), nick, msg);
  if(dev!=IRC_OK){
    IRC_syslog("kick_cu: : IRCMsg_Kick",dev,LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);

  IRC_MFree(3, &nick, &msg, &enviar);
  return OK;
}

int funcion_list_cu(char *command, int socket){
  unsigned long dev=0;
  char *channel=NULL, *cadena=NULL, *enviar=NULL;

  dev=IRCUserParse_List(command, &channel, &cadena);
  if(dev!=IRC_OK){
    IRC_syslog("list_cu: : IRCUserParse_List",dev,LOG_ERR);
    return ERR;
  }

  dev=IRCMsg_List(&enviar, NULL, channel, cadena);
  if(dev!=IRC_OK){
    IRC_syslog("list_cu: : IRCMsg_List",dev,LOG_ERR);
    return ERR;
  }

  send(socket,enviar,strlen(enviar),0);

  //IRC_MFree(3, &channel, &cadena, &enviar);
  return OK;
}

int funcion_away_cu(char *command, int socket){
  unsigned long dev=0;
  char *reason=NULL,*enviar=NULL;

  dev=IRCUserParse_Away (command, &reason);
  if(dev!=IRC_OK){
    IRC_syslog("list_cu: : IRCUserParse_List",dev,LOG_ERR);
    return ERR;
  }
  dev=IRCMsg_Away (&enviar, NULL, reason);
  if(dev!=IRC_OK){
    IRC_syslog("list_cu: : IRCUserParse_List",dev,LOG_ERR);
    return ERR;
  }
  send(socket,enviar,strlen(enviar),0);

  IRC_MFree(2, &reason, &enviar);
  return OK;
}

int funcion_query_cu(char *command,int socket){
  unsigned long dev=0;
  char *nickorchannel=NULL,*enviar=NULL,*msg=NULL;
  int mode=0;
  long u_id=0, creationTS, actionTS;
  char *u_nick=NULL, *u_real=NULL, *host, *IP, *away, *user=NULL;
  dev=IRCUserParse_Query (command, &nickorchannel, &msg);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_quey_cu: : IRCUserParse_Query",dev,LOG_ERR);
    return ERR;
  }
  if(nickorchannel[0]!='#'){
    if(IRCInterface_QueryChannelExist (nickorchannel)==FALSE){
      mode = IRCInterface_ModeToIntMode("+m");
      IRCInterface_AddNewChannel(nickorchannel, mode);
    }
  }else{
    //caso de ser un canal, mirar si necesita algo mas
    if(IRCInterface_QueryChannelExist (nickorchannel)==FALSE){
      return ERR;
    }
  }
  dev=IRCMsg_Privmsg (&enviar, NULL, nickorchannel, msg);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_quey_cu: : IRCMsg_Privmsg",dev,LOG_ERR);
    return ERR;
  }
  send(socket,enviar,strlen(enviar),0);
  dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &socket, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("rplpriv_cu: : IRCTADUser_GetData",dev,LOG_ERR);
    return ERR;
  }
  IRCInterface_WriteChannel(nickorchannel, u_nick, msg);
  IRC_MFree(3, &nickorchannel, &msg,&enviar);
  return OK;

}

int funcion_quit_cu(char *command, int socket) {
  unsigned long dev=0;
  char *reason=NULL,*enviar=NULL;
  dev=IRCUserParse_Quit (command,&reason);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_quit_cu: : IRCUserParse_Quit",dev,LOG_ERR);
    return ERR;
  }
  dev=IRCMsg_Quit (&enviar,NULL,reason);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_quit_cu: : IRCMsg_Quit",dev,LOG_ERR);
    return ERR;
  }
  send(socket,enviar,strlen(enviar),0);
  IRC_MFree(2, &reason,&enviar);
  exit(EXIT_SUCCESS);

}
