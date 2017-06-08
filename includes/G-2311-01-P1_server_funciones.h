/**
 * @brief: Funciones del módulo server funciones
 * @file G-2311-01-P1_server_funciones.h
 * @author Oscar Garcia de Lara Parreño & Santiago Gomez Aguirre. Grupo: 2311
 * @date 07-05-2017
 */
#ifndef __G_2311_01_P1_SERVER_FUNCIONES_H
#define __G_2311_01_P1_SERVER_FUNCIONES_H

#include "G-2311-01-P1_general.h"
#include "G-2311-01-P1_utils.h"

/**
 * Función que inicializa la lista de comandos que acepta el servidor.
 * @method inicializar_array
 * @return int: OK dado que no se contempla ningún escenario de error.
 */
int inicializar_array();
/**
 * Función que se activa por defecto al encontrar un comando desconocido para el servidor.
 * @method funcion_default
 * @param  command         Cadena del comando recibido
 * @param  socket          Socket en uso
 * @return int: ERR dado que es un comando desconocido.
 */
int funcion_default(char *command, int socket);
/**
 * Función que da un nickname al usuario o cambia el existente.
 * @method funcion_nick
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si el nickname se actualiza sin errores o no se puede actualizar
 *            porque ya existe uno igual en uso.
 *         ERR si se produce cualquier otro error.
 */
int funcion_nick(char *command, int socket);
/**
 * Función que especifíca el username, hostname y realname de un nuevo usuario.
 * @method funcion_user
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_user(char *command,int socket);
/**
 * Función que incluye al cliente a un canal específico.
 * @method funcion_join
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si se incluye al usuario en el canal o no se puede incluir por:
 *            -Canal lleno.
 *            -Nombre de canal mal introducido.
 *            -Usuario banneado en el canal.
 *            -Usuario sin invitación al canal.
 *            -Contraseña del canal incorrecta.
 *            -Falta de algún parámetro obligatorio.
 *         ERR si se produce cualquier otro error.
 */
int funcion_join(char *command, int socket);
/**
 * Función que lista los canales y sus topics.
 * @method funcion_list
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_list(char *command, int socket);
/**
 * Función que devuelve toda la información de los usuarios objetivo.
 * @method funcion_whois
 * @param  command       Cadena del comando recibido
 * @param  socket        Socket en uso
 * @return int:
 *         OK si se muestra la información sin errores o falta algún argumento obligatorio.
 *         ERR si se produce cualquier error.
 */
int funcion_whois(char *command, int socket);
/**
 * Función que muestra todos los usuarios visibles en un canal.
 * @method funcion_names
 * @param  command       Cadena del comando recibido
 * @param  socket        Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_names(char *command, int socket);
/**
 * Función que envía un mensaje privado entre dos usuarios o a un canal.
 * @method funcion_privmsg
 * @param  command         Cadena del comando recibido
 * @param  socket          Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_privmsg(char *command, int socket);
/**
 * Función que envía un mensaje ping a un usuario o responde al servidor
 * con un mensaje pong.
 * @method funcion_ping
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_ping(char *command, int socket);
/**
 * Función que saca al cliente de la lista de usuarios activos de los canales indicados.
 * @method funcion_part
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_part(char *command, int socket);
/**
 * Función que establece o cambia el topic de un canal.
 * @method funcion_topic
 * @param  command       Cadena del comando recibido
 * @param  socket        Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad o no se puede cumplir por:
 *            -Nombre de canal incorrecto.
 *            -Canal con topic protegido del que no se es operador.
 *         ERR si se produce cualquier otro error.
 */
int funcion_topic(char *command, int socket);
/**
 * Función que expulsa de un canal a un usuario objetivo.
 * @method funcion_kick
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si expulsa del canal al usuario objetivo o no se puede realizar por no ser operador.
 *         ERR si se produce cualquier error.
 */
int funcion_kick(char *command, int socket);
/**
 * Función que establece que cualquier comando de mensaje privado se dirija
 * al cliente en vez de a un canal.
 * @method funcion_away
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_away(char *command, int socket);
/**
 * Función que termina la sesión del cliente con un mensaje de cierre.
 * @method funcion_quit
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_quit(char *command, int socket);
/**
 * Función que envía el mensaje predefinido del servidor.
 * @method funcion_motd
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores.
 *         ERR si se produce cualquier error.
 */
int funcion_motd(char *command, int socket);

/**
 * Función que permite al cliente cambiar o consultar características de un canal.
 * @method funcion_mode
 * @param  command      Cadena del comando recibido
 * @param  socket       Socket en uso
 * @return int:
 *         OK si cumple su funcionalidad sin errores o no se es operador del canal.
 *         ERR si se produce cualquier otro error.
 */
int funcion_mode(char *command, int socket);

pf_msg lista_comandos[IRC_MAX_COMMANDS];

#endif
