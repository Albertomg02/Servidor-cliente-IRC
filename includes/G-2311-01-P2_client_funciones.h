/**
 * @brief: Funciones del módulo client funciones
 * @file G-2311-01-P2_client_funciones.h
 * @author Oscar Garcia de Lara Parreño & Santiago Gomez Aguirre. Grupo: 2311
 * @date 07-05-2017
 */
#ifndef __G_2311_01_P2_CLIENT_FUNCIONES_H
#define __G_2311_01_P2_CLIENT_FUNCIONES_H

#include "G-2311-01-P1_general.h"
#include "G-2311-01-P1_utils.h"
#include <redes2/ircxchat.h>
pf_msg comandos_cliente[IRC_MAX_COMMANDS];

typedef struct{
  int socket;
  char* filename;
  char* ip;
  int puerto;
  int length;
}param_recvfile;

/**
 * Función que inicializa la lista de comandos que acepta el cliente.
 * @method inicializar_array_c
 * @return int: OK dado que no se contempla ningún escenario de error.
 */
int inicializar_array_c();
/**
 * Función que se activa por defecto al encontrar un comando desconocido para el cliente.
 * @method funcion_default_c
 * @param  command         Cadena del comando recibido
 * @param  socket          Socket en uso
 * @return int: ERR dado que es un comando desconocido.
 */
int funcion_default_c(char *command, int socket);
/**
 * Función que muestra en la interfaz del cliente el mensaje de bienvenida.
 * @method funcion_rplwelcome_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_rplwelcome_c(char *command, int socket);
/**
 * Función que incluye al cliente en un nuevo canal.
 * @method funcion_join_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_join_c(char *command, int socket);
/**
 * Función que muestra los mensajes privados recibidos o enviados en el cliente,
 * incluyendo las peticiones de envío de archivos.
 * @method funcion_priv_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_priv_c(char *command, int socket);
/**
 * Función que recibe los archivos que envían al cliente.
 * @method launch_recvfile
 * @param pr                    Estructura con la información necesaria para poder recibir el archivo
 */
void* launch_recvfile(void *pr);
/**
 * Función que cambia el nick del cliente.
 * @method funcion_nick_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_nick_c(char *command, int socket);
/**
 * Función que quita del cliente un canal en el que se estaba incluido.
 * @method funcion_part_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_part_c(char *command, int socket);
/**
 * Función que muestra los mensajes de ping y pong en el cliente.
 * @method funcion_ping_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_ping_c(char *command, int socket);
/**
 * Función que muestra en el cliente los usuarios visibles en un canal.
 * @method funcion_namrpl_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_namrpl_c(char *command, int socket);
/**
 * Función del cliente que recibe el comando name_end.
 * @method funcion_namend_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int: OK dado que no se plantea ningún escenario de error.
 */
int funcion_namend_c(char *command, int socket);
/**
 * Función que representa el comando who en el cliente.
 * @method funcion_RplWhoReply_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_RplWhoReply_c(char *command, int socket);
/**
 * Función del cliente que recibe el comando who_end.
 * @method funcion_RplWhoEnd_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int: OK dado que no se plantea ningún escenario de error.
 */
int funcion_RplWhoEnd_c(char *command, int socket);
/**
 * Función que muestra en el cliente la información relativa a un usuario
 * sobre el que se ha realizado un comando whois.
 * @method funcion_whoisuser_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_whoisuser_c(char *command, int socket);
/**
 * Función que muestra en el cliente la información relativa al servidor
 * en el que se ha realizado un comando whois.
 * @method funcion_whoisserver_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_whoisserver_c(char *command, int socket);
/**
 * Función que muestra en el cliente la información relativa a los canales
 * en los que se encuentra el usuario sobre el que se ha realizado el
 * comando whois.
 * @method funcion_whoischannels_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_whoischannels_c(char *command, int socket);
/**
 * Función que muestra en el cliente la información relativa a la actividad
 * del usuario sobre el que se ha realizado el comando whois.
 * @method funcion_whoisidle_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_whoisidle_c(char *command, int socket);
/**
 * Función que muestra en el cliente la información del final del comando whois.
 * @method funcion_endwhois_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_endwhois_c(char *command, int socket);
/**
 * Función que muestra en el cliente el comienzo del mensaje
 * predefinido del servidor.
 * @method funcion_RplMotdstart_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_RplMotdstart_c(char *command, int socket);
/**
 * Función que muestra en el cliente el cuerpo del mensaje predefinido
 * del servidor.
 * @method funcion_RplMotd_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_RplMotd_c(char *command, int socket);
/**
 * Función que muestra en el cliente el final del mensaje predefinido
 * del servidor.
 * @method funcion_RplendofMotd_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_RplendofMotd_c(char *command, int socket);
/**
 * Función que muestra en el cliente el cambio del topic de un canal.
 * @method funcion_RplTopic_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_RplTopic_c(char *command, int socket);
/**
 * Función que muestra en el cliente el topic del canal o el mensaje
 * informativo de que no hay topic en el canal.
 * @method funcion_RplNoTopic_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_RplNoTopic_c(char *command, int socket);
/**
 * Función que muestra en el cliente la información relativa a la acción
 * que se haya realizado con el comando mode.
 * @method funcion_mode_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_mode_c(char *command, int socket);
/**
 * Función que expulsa a un usuario de un canal del cliente.
 * @method funcion_kick_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_kick_c(char *command, int socket);
/**
 * Función que muestra en el cliente toda la información obtenida de
 * la realización del comando list.
 * @method funcion_RplList_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_RplList_c(char *command, int socket);
/**
 * Función que muestra en el cliente el mensaje final de la realización
 * del comando list.
 * @method funcion_RplListEnd_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_RplListEnd_c(char *command, int socket);
/**
 * Función que muestra en el cliente el mensaje de desactivación del comando away.
 * @method funcion_RplUaway_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int: OK dado que no se plantea ningún escenario de error.
 */
int funcion_RplUaway_c(char *command, int socket);
/**
 * Función que muestra en el cliente el mensaje de realizar el comando away.
 * @method funcion_RplNowaway_c
 * @param  command               Cadena del comando recibido
 * @param  socket                Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_RplNowaway_c(char *command, int socket);
/**
 * Función que cierra la sesión del cliente.
 * @method funcion_quit_c
 * @param  command              Cadena del comando recibido
 * @param  socket               Socket en uso
 * @return int: ERR si se produce cualquier error que impida el cierre de sesión.
 */
int funcion_quit_c(char *command, int socket);


int funcion_quit_c(char *command, int socket);

#endif
