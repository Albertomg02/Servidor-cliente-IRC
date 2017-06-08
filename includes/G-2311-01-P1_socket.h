/**
 * @brief: Funciones del módulo socket
 * @file G-2311-01-P1_socket.h
 * @author Oscar Garcia de Lara Parreño & Santiago Gomez Aguirre. Grupo: 2311
 * @date 07-05-2017
 */
#ifndef __G_2311_02_P1_SOCKET_H
#define __G_2311_02_P1_SOCKET_H

#include "G-2311-01-P1_general.h"
/**
 * daemonizar un programa
 * @method daemonizar
 * @param  servicio   Nombre del proceso
 * @return           -1 en caso de error
 */
int daemonizar(char *servicio);
/**
 * crear_socket
 * @method crear_socket
 * @param  protocolo    Protocolo para indicar si se quiere TCP o UDP
 * @return              el socket o -1 en caso de error
 */
int crear_socket(int protocolo);

#endif
