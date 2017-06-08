/**
 * @brief: Funciones del módulo client funciones user
 * @file G-2311-01-P2_client_funciones_user.h
 * @author Oscar Garcia de Lara Parreño & Santiago Gomez Aguirre. Grupo: 2311
 * @date 07-05-2017
 */
#ifndef __G_2311_01_P2_CLIENT_FUNCIONES_USER_H
#define __G_2311_01_P2_CLIENT_FUNCIONES_USER_H

#include "G-2311-01-P1_general.h"
#include "G-2311-01-P1_utils.h"
#include <redes2/ircxchat.h>
pf_msg comandos_cliente_usuario[IRC_MAX_COMMANDS];

/**
 * Función que inicializa la lista de comandos del usuario que acepta el cliente.
 * @method inicializar_array_cu
 * @return int: OK dado que no se contempla ningún escenario de error.
 */
int inicializar_array_cu();
/**
 * Función que se activa por defecto al encontrar un comando desconocido
 * del usuario para el cliente.
 * @method funcion_default_cu
 * @param  command         Cadena del comando introducido
 * @param  socket          Socket en uso
 * @return int: ERR dado que es un comando desconocido.
 */
int funcion_default_cu(char *command, int socket);
/**
 * Función que envía la información del comando join introducido
 * en el cliente al servidor.
 * @method funcion_join_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_join_cu(char *command, int socket);
/**
 * Función que envía la información del mensaje privado introducido
 * en el cliente al servidor.
 * @method funcion_priv_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_priv_cu(char *command, int socket);
/**
 * Función que envía la información del comando nick introducido
 * en el cliente al servidor.
 * @method funcion_nick_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_nick_cu(char *command, int socket);
/**
 * Función que envía la información del comando part introducido
 * en el cliente al servidor.
 * @method funcion_part_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_part_cu(char *command, int socket);
/**
 * Función que envía la información del comando names introducido
 * en el cliente al servidor.
 * @method funcion_names_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_names_cu(char *command, int socket);
/**
 * Función que envía la información del comando who introducido
 * en el cliente al servidor.
 * @method funcion_who_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_who_cu(char *command, int socket);
/**
 * Función que envía la información del comando whois introducido
 * en el cliente al servidor.
 * @method funcion_whois_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_whois_cu(char *command, int socket);
/**
 * Función que envía la información del comando motd introducido
 * en el cliente al servidor.
 * @method funcion_motd_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_motd_cu(char *command, int socket);
/**
 * Función que envía la información del comando topic introducido
 * en el cliente al servidor.
 * @method funcion_topic_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_topic_cu(char *command, int socket);
/**
 * Función que envía la información del comando mode introducido
 * en el cliente al servidor.
 * @method funcion_mode_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_mode_cu(char *command, int socket);
/**
 * Función que envía la información del comando kick introducido
 * en el cliente al servidor.
 * @method funcion_kick_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_kick_cu(char *command, int socket);
/**
 * Función que envía la información del comando list introducido
 * en el cliente al servidor.
 * @method funcion_list_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_list_cu(char *command, int socket);
/**
 * Función que envía la información del comando away introducido
 * en el cliente al servidor.
 * @method funcion_away_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_away_cu(char *command, int socket);
/**
 * Función que envía la información del comando query introducido
 * en el cliente al servidor.
 * @method funcion_query_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_query_cu(char *command,int socket);
/**
 * Función que envía la información del comando quit introducido
 * en el cliente al servidor.
 * @method funcion_quit_cu
 * @param  command        Cadena del comando introducido
 * @param  socket         Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_quit_cu(char *command, int socket);

#endif
