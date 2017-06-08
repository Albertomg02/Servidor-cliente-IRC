#include "../includes/G-2311-01-P2_client.h"
/**
 * @defgroup IRCInterface Interface
 *
 */

/**
 * @defgroup IRCInterfaceCallbacks Callbaks
 * @ingroup IRCInterface
 *
 */

/**
 * @addtogroup IRCInterfaceCallbacks
 * Funciones que van a ser llamadas desde el interface y que deben ser implementadas por el usuario.
 * Todas estas funciones pertenecen al hilo del interfaz.
 *
 * El programador puede, por supuesto, separar todas estas funciones en múltiples ficheros a
 * efectos de desarrollo y modularización.
 *
 * <hr>
 */

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateChannelKey IRCInterface_ActivateChannelKey
 *
 * @brief Llamada por el botón de activación de la clave del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateChannelKey (char * channel, char * key)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la clave del canal. El segundo parámetro es
 * la clave del canal que se desea poner. Si es NULL deberá impedirse la activación
 * con la función implementada a tal efecto. En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manpuladas por el programador, sólo leidas.
 *
 * @param[in] channel canal sobre el que se va a activar la clave.
 * @param[in] key clave para el canal indicado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateChannelKey(char * channel, char *key)
{
	char *enviar=NULL;
	if(key!=NULL){
		unsigned long dev=0;
		dev=IRCMsg_Mode(&enviar, NULL, channel,"+k", key);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
		free(enviar);
	}
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateExternalMessages IRCInterface_ActivateExternalMessages
 *
 * @brief Llamada por el botón de activación de la recepción de mensajes externos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateExternalMessages (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la recepción de mensajes externos.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se activará la recepción de mensajes externos.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateExternalMessages(char * channel)
{
	char *enviar=NULL;
		unsigned long dev=0;
		dev=IRCMsg_Mode(&enviar, NULL, channel,"+n", NULL);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
		free(enviar);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateInvite IRCInterface_ActivateInvite
 *
 * @brief Llamada por el botón de activación de canal de sólo invitación.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateInvite (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de canal de sólo invitación.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se activará la invitación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateInvite(char * channel)
{
	char *enviar=NULL;
		unsigned long dev=0;
		dev=IRCMsg_Mode(&enviar, NULL, channel,"+i", NULL);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
		free(enviar);

}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateModerated IRCInterface_ActivateModerated
 *
 * @brief Llamada por el botón de activación de la moderación del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateModerated (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la moderación del canal.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se activará la moderación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateModerated(char * channel)
{
	char *enviar=NULL;

		unsigned long dev=0;
		dev=IRCMsg_Mode( &enviar, NULL, channel,"+m", NULL);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
send(sk,enviar,strlen(enviar),0);
free(enviar);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateNicksLimit IRCInterface_ActivateNicksLimit
 *
 * @brief Llamada por el botón de activación del límite de usuarios en el canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateNicksLimit (char * channel, int * limit)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación del límite de usuarios en el canal. El segundo es el
 * límite de usuarios que se desea poner. Si el valor es 0 se sobrentiende que se desea eliminar
 * este límite.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se activará el límite de usuarios.
 * @param[in] limit límite de usuarios en el canal indicado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateNicksLimit(char * channel, int limit)
{
	char *enviar=NULL;
	if(limit>0){
		unsigned long dev=0;
		char lc[11];
		sprintf(lc, "%d", limit);
		dev=IRCMsg_Mode( &enviar, NULL, channel,"+l",lc);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
		free(enviar);
	}else if(limit==0){
		unsigned long dev=0;
		dev=IRCMsg_Mode(&enviar, NULL, channel,"-l",NULL);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
		free(enviar);
	}
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivatePrivate IRCInterface_ActivatePrivate
 *
 * @brief Llamada por el botón de activación del modo privado.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivatePrivate (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación del modo privado.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se va a activar la privacidad.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivatePrivate(char * channel)
{
	char *enviar=NULL;

		unsigned long dev=0;
		dev=IRCMsg_Mode(&enviar, NULL, channel,"+p", NULL);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
		free(enviar);

}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateProtectTopic IRCInterface_ActivateProtectTopic
 *
 * @brief Llamada por el botón de activación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateProtectTopic (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la protección de tópico.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se va a activar la protección de tópico.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateProtectTopic(char * channel)
{
	char *enviar=NULL;
	unsigned long dev=0;
	dev=IRCMsg_Mode(&enviar, NULL, channel,"+t", NULL);
	if(dev!=IRC_OK){
		IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
		return;
	}
  send(sk,enviar,strlen(enviar),0);
	free(enviar);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateSecret IRCInterface_ActivateSecret
 *
 * @brief Llamada por el botón de activación de canal secreto.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateSecret (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de canal secreto.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se va a activar el estado de secreto.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateSecret(char * channel)
{
	char *enviar=NULL;
	unsigned long dev=0;
	dev=IRCMsg_Mode(&enviar, NULL, channel,"+s", NULL);
	if(dev!=IRC_OK){
		IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
		return;
	}
	send(sk,enviar,strlen(enviar),0);
	free(enviar);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_BanNick IRCInterface_BanNick
 *
 * @brief Llamada por el botón "Banear".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_BanNick (char * channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Banear". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manpuladas por el programador, sólo leidas.
 *
 * @param[in] channel canal sobre el que se va a realizar el baneo. En principio es un valor innecesario.
 * @param[in] nick nick del usuario que va a ser baneado
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_BanNick(char *channel, char *nick)
{
	char *enviar=NULL;
 	unsigned long dev=0;
 	dev=IRCMsg_Mode(&enviar, NULL, channel,"+b", nick);
 	if(dev!=IRC_OK){
 		IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
 		return;
 	}
 	send(sk,enviar,strlen(enviar),0);
	free(enviar);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_Connect IRCInterface_Connect
 *
 * @brief Llamada por los distintos botones de conexión.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	long IRCInterface_Connect (char *nick, char * user, char * realname, char * password, char * server, int port, boolean ssl)
 * @endcode
 *
 * @description
 * Función a implementar por el programador.
 * Llamada por los distintos botones de conexión. Si implementará la comunicación completa, incluido
 * el registro del usuario en el servidor.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * @param[in] nick nick con el que se va a realizar la conexíón.
 * @param[in] user usuario con el que se va a realizar la conexión.
 * @param[in] realname nombre real con el que se va a realizar la conexión.
 * @param[in] password password del usuario si es necesaria, puede valer NULL.
 * @param[in] server nombre o ip del servidor con el que se va a realizar la conexión.
 * @param[in] port puerto del servidor con el que se va a realizar la conexión.
 * @param[in] ssl puede ser TRUE si la conexión tiene que ser segura y FALSE si no es así.
 *
 * @retval IRC_OK si todo ha sido correcto (debe devolverlo).
 * @retval IRCERR_NOSSL si el valor de SSL es TRUE y no se puede activar la conexión SSL pero sí una
 * conexión no protegida (debe devolverlo).
 * @retval IRCERR_NOCONNECT en caso de que no se pueda realizar la comunicación (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

long IRCInterface_Connect(char *nick, char *user, char *realname, char *password, char *server, int port, boolean ssl)
{
	int descriptor=0;
	char *enviar=NULL;
	struct sockaddr_in Conexion;
	struct hostent *h;
  unsigned long dev;
	pthread_t cliente;

	if(ssl){
		return IRCERR_NOSSL;
	}else{
		if((sk=crear_socket(SOCK_STREAM))==ERR){
			syslog(LOG_ERR,"Error creando socket, cliente");
			return IRCERR_NOCONNECT;
		}
		h = gethostbyname(server);
		if(h==NULL){
			syslog(LOG_ERR,"Error obteniendo la ip, cliente");
			return IRCERR_NOCONNECT;
		}
		//bzero((char*)&Conexion, sizeof(Conexion));
		Conexion.sin_family=AF_INET;
		//bcopy((char*)h->h_addr, (char*)&Conexion.sin_addr.s_addr, h->h_length);
		Conexion.sin_addr.s_addr=((struct in_addr*)(h->h_addr))->s_addr;
		Conexion.sin_port=htons(port);

		if((descriptor = connect(sk,(struct sockaddr *) &Conexion,sizeof(Conexion)))<0){
			syslog(LOG_ERR,"Error aceptando conexion, cliente");
			return IRCERR_NOCONNECT;
		}
		if((dev=IRCTADUser_New(user,nick,realname,password,"micliente.es","127.0.0.1",sk))!=IRC_OK){
			IRC_syslog("CLIENTE: Error crear al usuario",dev,LOG_ERR);
			return IRCERR_NOCONNECT;
		}
		  pthread_create(&cliente,NULL,launch_read,(void *)&sk);

			if(password!=NULL&&strlen(password)>0){
				dev=IRCMsg_Pass(&enviar,NULL,password);
				if(dev!=IRC_OK){
					IRC_syslog("CLIENTE: Error IRCMsg_Pass",dev,LOG_ERR);
					return IRCERR_NOCONNECT;
				}
				send(sk,enviar,strlen(enviar),0);
				IRCInterface_PlaneRegisterOutMessage(enviar);
				free(enviar);
			}


			dev=IRCMsg_Nick(&enviar,NULL,nick,NULL);
			if(dev!=IRC_OK){
				IRC_syslog("CLIENTE: Error IRCMsg_Nick",dev,LOG_ERR);
				return IRCERR_NOCONNECT;
			}
			send(sk,enviar,strlen(enviar),0);
			IRCInterface_PlaneRegisterOutMessage(enviar);
			free(enviar);
			dev=IRCMsg_User(&enviar,NULL,user,"i",realname);
			if(dev!=IRC_OK){
				IRC_syslog("CLIENTE: Error IRCMsg_User",dev,LOG_ERR);
				return IRCERR_NOCONNECT;
			}
			send(sk,enviar,strlen(enviar),0);
			IRCInterface_PlaneRegisterOutMessage(enviar);
			free(enviar);

	}

	return IRC_OK;
}

void* launch_read(void * s){
	int socket=*(int *)s,leido=0;
	char entrada[10000],*in,*comando=NULL;
	long tipo;
	while(1){
		  entrada[0]=0;
			leido=recv(socket, &entrada, sizeof(entrada),0);
		if(leido>0){
			in=(char*)malloc(leido+1);
			strncpy(in,entrada,leido);
			in[leido+1]=0;
			do{
				in=IRC_UnPipelineCommands (in,&comando);
				tipo=IRC_CommandQuery(comando);
				switch (tipo) {
					case IRCERR_NOCOMMAND:
						syslog (LOG_ERR, "Error no comando");
					break;
					case IRCERR_NOPARAMS:
						syslog (LOG_ERR, "Error no parrametros");
					break;
					case IRCERR_UNKNOWNCOMMAND:
						syslog (LOG_ERR, "Error comando desconocido: %s",comando);
					break;
					default:
						//IRCInterface_PlaneRegisterInMessageThread(comando);
						comandos_cliente[tipo](comando,socket);
				}
			}while(in!=NULL);
			if(!comando){
				free(comando);
				comando=NULL;
			}
			if(!in){
				free(in);
				in=NULL;
			}
		}

	}


}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateChannelKey IRCInterface_DeactivateChannelKey
 *
 * @brief Llamada por el botón de desactivación de la clave del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateChannelKey (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de la clave del canal.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se va a desactivar la clave.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateChannelKey(char * channel)
{
	char *enviar=NULL;

	unsigned long dev=0;
	dev=IRCMsg_Mode(&enviar, NULL, channel,"-k",NULL);
	if(dev!=IRC_OK){
		IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
		return;
	}
	send(sk,enviar,strlen(enviar),0);

}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateExternalMessages IRCInterface_DeactivateExternalMessages
 *
 * @brief Llamada por el botón de desactivación de la recepción de mensajes externos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateExternalMessages (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de la recepción de mensajes externos.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se va a deactivar la recepción de mensajes externos.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateExternalMessages(char * channel)
{
	char *enviar=NULL;
		unsigned long dev=0;
		dev=IRCMsg_Mode(&enviar, NULL, channel,"-n", NULL);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateInvite IRCInterface_DeactivateInvite
 *
 * @brief Llamada por el botón de desactivación de canal de sólo invitación.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateInvite (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de canal de sólo invitación.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se va a desactivar la invitación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateInvite(char * channel)
{
	char *enviar=NULL;

	unsigned long dev=0;
	dev=IRCMsg_Mode( &enviar, NULL, channel,"-i",NULL);
	if(dev!=IRC_OK){
		IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
		return;
	}
	send(sk,enviar,strlen(enviar),0);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateModerated IRCInterface_DeactivateModerated
 *
 * @brief Llamada por el botón de desactivación  de la moderación del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateModerated (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación  de la moderación del canal.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se va a desactivar la moderación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateModerated(char * channel)
{
	char *enviar=NULL;

	unsigned long dev=0;
	dev=IRCMsg_Mode( &enviar, NULL, channel,"-m",NULL);
	if(dev!=IRC_OK){
		IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
		return;
	}
	send(sk,enviar,strlen(enviar),0);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateNicksLimit IRCInterface_DeactivateNicksLimit
 *
 * @brief Llamada por el botón de desactivación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateNicksLimit (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación  del límite de usuarios en el canal.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * @param[in] channel canal sobre el que se va a desactivar el límite de usuarios.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateNicksLimit(char * channel)
{
	char *enviar=NULL;

	unsigned long dev=0;
	dev=IRCMsg_Mode( &enviar, NULL, channel,"-l",NULL);
	if(dev!=IRC_OK){
		IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
		return;
	}
	send(sk,enviar,strlen(enviar),0);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivatePrivate IRCInterface_DeactivatePrivate
 *
 * @brief Llamada por el botón de desactivación del modo privado.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivatePrivate (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación del modo privado.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @param[in] channel canal sobre el que se va a desactivar la privacidad.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivatePrivate(char * channel)
{
	char *enviar=NULL;

	unsigned long dev=0;
	dev=IRCMsg_Mode( &enviar, NULL, channel,"-p",NULL);
	if(dev!=IRC_OK){
		IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
		return;
	}
	send(sk,enviar,strlen(enviar),0);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateProtectTopic IRCInterface_DeactivateProtectTopic
 *
 * @brief Llamada por el botón de desactivación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateProtectTopic (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de la protección de tópico.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se va a desactivar la protección de tópico.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateProtectTopic(char * channel)
{
	char *enviar=NULL;

	unsigned long dev=0;
	dev=IRCMsg_Mode( &enviar, NULL, channel,"-t",NULL);
	if(dev!=IRC_OK){
		IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
		return;
	}
	send(sk,enviar,strlen(enviar),0);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateSecret IRCInterface_DeactivateSecret
 *
 * @brief Llamada por el botón de desactivación de canal secreto.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateSecret (char * channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de canal secreto.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se va a desactivar la propiedad de canal secreto.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateSecret(char * channel)
{
	char *enviar=NULL;

	unsigned long dev=0;
	dev=IRCMsg_Mode( &enviar, NULL, channel,"-s",NULL);
	if(dev!=IRC_OK){
		IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
		return;
	}
	send(sk,enviar,strlen(enviar),0);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DisconnectServer IRCInterface_DisconnectServer
 *
 * @brief Llamada por los distintos botones de desconexión.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DisconnectServer (char * server, int port)
 * @endcode
 *
 * @description
 * Llamada por los distintos botones de desconexión. Debe cerrar la conexión con el servidor.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.

 * @param[in] server nombre o ip del servidor del que se va a realizar la desconexión.
 * @param[in] port puerto sobre el que se va a realizar la desconexión.
 *
 * @retval TRUE si se ha cerrado la conexión (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

boolean IRCInterface_DisconnectServer(char *server, int port)
{
	unsigned long dev=0;
	char *reason=NULL,*enviar=NULL;
	dev=IRCMsg_Quit (&enviar,NULL,reason);
	if(dev!=IRC_OK){
		IRC_syslog("funcion_quit_cu: : IRCMsg_Quit",dev,LOG_ERR);
		return ERR;
	}
	send(sk,enviar,strlen(enviar),0);
	free(enviar);
	long u_id=0, creationTS, actionTS;
  char *u_nick=NULL, *u_real=NULL, *host, *IP, *away, *user=NULL;
dev=IRCTADUser_GetData(&u_id, &user, &u_nick, &u_real, &host, &IP, &sk, &creationTS, &actionTS, &away);
  if(dev!=IRC_OK){
    IRC_syslog("funcion_quit_c: IRCTADUser_GetData", dev, LOG_ERR);
    return FALSE;
  }
  IRCTADUser_Delete (u_id, NULL, NULL, NULL);
  IRCInterface_RemoveAllChannelsThread ();
  IRCInterface_ChangeConectionSelectedThread();
  IRC_MFree(5,&user,&u_nick,&u_real,&host,&IP);
  close(sk);
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ExitAudioChat IRCInterface_ExitAudioChat
 *
 * @brief Llamada por el botón "Cancelar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ExitAudioChat (char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Parar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función cierrala comunicación. Evidentemente tiene que
 * actuar sobre el hilo de chat de voz.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] nick nick del usuario que solicita la parada del chat de audio.
 *
 * @retval TRUE si se ha cerrado la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

boolean IRCInterface_ExitAudioChat(char *nick)
{
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_GiveOp IRCInterface_GiveOp
 *
 * @brief Llamada por el botón "Op".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_GiveOp (char * channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Op". Previamente debe seleccionarse un nick del
 * canal para darle "op" a dicho usuario.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] channel canal sobre el que se va dar op al usuario.
 * @param[in] nick nick al que se le va a dar el nivel de op.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_GiveOp(char *channel, char *nick)
{
	char *enviar=NULL;
	if(nick!=NULL){
		unsigned long dev=0;
		dev=IRCMsg_Mode (&enviar, NULL, channel, "+o", nick);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
	}
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_GiveVoice IRCInterface_GiveVoice
 *
 * @brief Llamada por el botón "Dar voz".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_GiveVoice (char * channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Dar voz". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manpuladas por el programador, sólo leidas.
 *
 * @param[in] channel canal sobre el que se va dar voz al usuario.
 * @param[in] nick nick al que se le va a dar voz.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_GiveVoice(char *channel, char *nick)
{
	char *enviar=NULL;
	if(nick!=NULL){
		unsigned long dev=0;
		dev=IRCMsg_Mode (&enviar, NULL, channel, "+v", nick);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
	}
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_KickNick IRCInterface_KickNick
 *
 * @brief Llamada por el botón "Echar".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_KickNick (char * channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Echar". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manpuladas por el programador, sólo leidas.
 *
 * @param[in] channel canal sobre el que se va a expulsar al usuario.
 * @param[in] nick nick del usuario que va a ser expulsado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_KickNick(char *channel, char *nick)
{
	char *enviar=NULL;
	if(nick!=NULL){
		unsigned long dev=0;
		dev=IRCMsg_Kick(&enviar, NULL, channel, nick, "Fuera por pesado");
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
	}
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_NewCommandText IRCInterface_NewCommandText
 *
 * @brief Llamada la tecla ENTER en el campo de texto y comandos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_NewCommandText (char * command)
 * @endcode
 *
 * @description
 * Llamada de la tecla ENTER en el campo de texto y comandos. El texto deberá ser
 * enviado y el comando procesado por las funciones de "parseo" de comandos de
 * usuario.
 *
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] comando introducido por el usuario.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_NewCommandText(char *command)
{
	unsigned long dev=0;
	dev= IRCUser_CommandQuery (command);

	switch (dev) {
		case IRCERR_NOSTRING:
		break;
		case IRCERR_NOUSERCOMMAND:
		//	IRCInterface_PlaneRegisterOutMessage(command);
		  comandos_cliente_usuario[UMSG](command,sk);
		break;
		case IRCERR_NOVALIDUSERCOMMAND:
		break;
		default:
		//	IRCInterface_PlaneRegisterOutMessage(command);
		  comandos_cliente_usuario[dev](command,sk);
	}

}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_NewTopicEnter IRCInterface_NewTopicEnter
 *
 * @brief Llamada cuando se pulsa la tecla ENTER en el campo de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_NewTopicEnter (char * topicdata)
 * @endcode
 *
 * @description
 * Llamada cuando se pulsa la tecla ENTER en el campo de tópico.
 * Deberá intentarse cambiar el tópico del canal.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * param[in] topicdata string con el tópico que se desea poner en el canal.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_NewTopicEnter(char *topicdata)
{
	char *enviar=NULL;

		unsigned long dev=0;
		dev=IRCMsg_Topic(&enviar, NULL, IRCInterface_ActiveChannelName(), topicdata);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);

}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_SendFile IRCInterface_SendFile
 *
 * @brief Llamada por el botón "Enviar Archivo".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_SendFile (char * filename, char *nick, char *data, long unsigned int length)
 * @endcode
 *
 * @description
 * Llamada por el botón "Enviar Archivo". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función como todos los demás callbacks bloquea el interface
 * y por tanto es el programador el que debe determinar si crea un nuevo hilo para enviar el archivo o
 * no lo hace.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manpuladas por el programador, sólo leidas.
 *
 * @param[in] filename nombre del fichero a enviar.
 * @param[in] nick nick del usuario que enviará el fichero.
 * @param[in] data datos a ser enviados.
 * @param[in] length longitud de los datos a ser enviados.
 *
 * @retval TRUE si se ha establecido la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

boolean IRCInterface_SendFile(char *filename, char *nick, char *data, long unsigned int length)
{
	unsigned long dev=0;
	int sockfile;
	char *mensaje=NULL, *enviar=NULL, *ip_host=NULL, *ifc, *ifc_aux;
	char linea[100];
	struct sockaddr_in my_addr;
	struct ifreq ifr;
	socklen_t slen = sizeof(my_addr);
	pthread_t cliente;
	param_sendfile *p=(param_sendfile*)malloc(sizeof(param_sendfile));
	FILE *f;

	if((sockfile=socket(AF_INET, SOCK_STREAM, 0))<0){
		syslog(LOG_ERR,"Error creando socket en IRCInterface_SendFile");
    return FALSE;
	}

	/*Obtenemos la interfaz en uso*/
	f=fopen("/proc/net/route", "r");

	while(fgets(linea, 100, f)){
		ifc=strtok(linea, "\t");
		ifc_aux=strtok(NULL, "\t");

		if(ifc!=NULL && ifc_aux!=NULL){
			if(strcmp(ifc_aux, "00000000")==0){
				printf("interfaz: %s\n", ifc);
				break;
			}
		}
	}

	ifr.ifr_addr.sa_family = AF_INET; //Para obtener IPv4
	strncpy(ifr.ifr_name, ifc, IFNAMSIZ-1); //Para obtener IP de interfaz
	ioctl(sockfile, SIOCGIFADDR, &ifr);
	ip_host=(char *)malloc((strlen(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr)))*sizeof(char));
	strcpy(ip_host, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

	if(listen(sockfile, 1)<0){
		syslog(LOG_ERR,"Error Escuchando conexiones en IRCInterface_SendFile");
    return FALSE;
	}

	getsockname(sockfile, (struct sockaddr*)&my_addr, &slen);


	p->socket=sockfile;

	p->filename=(char *)malloc(strlen(filename)*sizeof(char));
	if(p->filename==NULL){
		syslog(LOG_ERR,"Error en el malloc de filename");
		return ERR;
	}
	strcpy(p->filename, filename);

	p->nick=(char *)malloc(strlen(nick)*sizeof(char));
	if(p->nick==NULL){
		syslog(LOG_ERR,"Error en el malloc de nick");
		return ERR;
	}
	strcpy(p->nick, nick);

	p->data=(char *)malloc(strlen(data)*sizeof(char));
	if(p->data==NULL){
		syslog(LOG_ERR,"Error en el malloc de data");
		return ERR;
	}
	strcpy(p->data, data);

	p->length=length;

	mensaje=(char *)malloc((strlen(filename)+strlen(p->nick)+80)*sizeof(char));
	sprintf(mensaje, "\\001FSEND %s %s %d %lu", filename, ip_host, ntohs(my_addr.sin_port), length);

	dev=IRCMsg_Privmsg(&enviar, NULL, nick, mensaje);
	if(dev!=IRC_OK){
    IRC_syslog("IRCInterface_SendFile: Error en IRCMsg_Privmsg", dev, LOG_ERR);
    return FALSE;
  }

	send(sk,enviar,strlen(enviar),0);

	pthread_create(&cliente,NULL,launch_file,(void *)p);

	return TRUE;//IRCInterface_ReceiveDialogThread (nick,data);

}

void* launch_file(void *pr){
	int des_cliente;
	struct sockaddr con_cliente;
	unsigned int tam_con=sizeof(con_cliente);
	param_sendfile *p=(param_sendfile*)pr;

	if((des_cliente=accept(p->socket,&con_cliente,&tam_con))<0){
		syslog(LOG_ERR,"Error aceptando conexion en launch_file");
		return pr;
	}

	send(des_cliente,p->data,p->length,0);

	close(des_cliente);
	close(p->socket);

	return pr;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_StartAudioChat IRCInterface_StartAudioChat
 *
 * @brief Llamada por el botón "Iniciar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_StartAudioChat (char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Iniciar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función como todos los demás callbacks bloquea el interface
 * y por tanto para mantener la funcionalidad del chat de voz es imprescindible crear un hilo a efectos
 * de comunicación de voz.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] nick nick del usuario con el que se desea conectar.
 *
 * @retval TRUE si se ha establecido la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

boolean IRCInterface_StartAudioChat(char *nick)
{
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_StopAudioChat IRCInterface_StopAudioChat
 *
 * @brief Llamada por el botón "Parar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_StopAudioChat (char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Parar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función sólo para la comunicación que puede ser reiniciada.
 * Evidentemente tiene que actuar sobre el hilo de chat de voz.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manpulada por el programador, sólo leida.
 *
 * @param[in] nick nick del usuario con el que se quiere parar el chat de voz.
 *
 * @retval TRUE si se ha parado la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

boolean IRCInterface_StopAudioChat(char *nick)
{
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_TakeOp IRCInterface_TakeOp
 *
 * @brief Llamada por el botón "Quitar Op".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_TakeOp (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Quitar Op". Previamente debe seleccionarse un nick del
 * canal para quitarle "op" a dicho usuario.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manpuladas por el programador, sólo leidas.
 *
 * @param[in] channel canal sobre el que se va a quitar op al usuario.
 * @param[in] nick nick del usuario al que se le va a quitar op.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_TakeOp(char *channel, char *nick)
{
	char *enviar=NULL;
	if(nick!=NULL){
		unsigned long dev=0;
		dev=IRCMsg_Mode (&enviar, NULL, channel, "-o", nick);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
	}
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_TakeVoice IRCInterface_TakeVoice
 *
 * @brief Llamada por el botón "Quitar voz".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_TakeVoice (char * channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Quitar voz". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manpuladas por el programador, sólo leidas.
 *
 * @param[in] channel canal sobre el que se le va a quitar voz al usuario.
 * @param[in] nick nick del usuario al que se va a quitar la voz.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_TakeVoice(char *channel, char *nick)
{
	char *enviar=NULL;
	if(nick!=NULL){
		unsigned long dev=0;
		dev=IRCMsg_Mode (&enviar, NULL, channel, "-v", nick);
		if(dev!=IRC_OK){
			IRC_syslog("ActivateChannelKey: mode:",dev,LOG_ERR);
			return;
		}
		send(sk,enviar,strlen(enviar),0);
	}
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



int main (int argc, char *argv[])
{
	/* La función IRCInterface_Run debe ser llamada al final      */
	/* del main y es la que activa el interfaz gráfico quedándose */
	/* en esta función hasta que se pulsa alguna salida del       */
	/* interfaz gráfico.                                          */
  unsigned long dev;
  char *data=NULL;
  int port=6668;
	inicializar_array_c();
	inicializar_array_cu();
  dev=IRC_GetOptionsStandard (argc,argv,&data,&port);
  if(dev==IRCERR_PARAMS || dev==IRCERR_NOENOUGHMEMORY){
    IRC_syslog("Error lectura de parametros",dev,LOG_ERR);
    return -1;
  }
  if(dev & IRCOPT_DATA){
    //completar


    free(data);
  }else if (dev & IRCOPT_SSLDATA){
    //completar
  }



	IRCInterface_Run(argc, argv);

	return 0;
}
