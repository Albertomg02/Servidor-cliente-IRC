/**
 * @brief: Funciones del módulo server
 * @file G-2311-01-P1_server.h
 * @author Oscar Garcia de Lara Parreño & Santiago Gomez Aguirre. Grupo: 2311
 * @date 07-05-2017
 */
#ifndef __G_2311_01_P1_SERVER_H
#define __G_2311_01_P1_SERVER_H

#include <getopt.h>
#include "G-2311-01-P1_server_funciones.h"
#include "G-2311-01-P1_general.h"
#include "G-2311-01-P1_socket.h"
#include "G-2311-01-P1_semaforos.h"


#define FILEKEY "/bin/cat"

int max_descriptor =0;
int num_clientes=0;
int *semaforos=NULL, *clientes=NULL;

typedef struct{
  int index;
  char* comando;
}param;
/**
 * [max(a, b) description]
 * @method max(a, b)
 * @param  a      [description]
 * @param b
 * @return        [description]
 */
#define max(a, b) (((a) > (b)) ? (a) : (b))

/**
 * Función que inicia el servidor.
 * @method iniciar_server
 * @param  puerto         Puerto indicado para el servidor
 * @return int:
 *         Socket habilitado del servidor.
 *         ERR si se produce cualquier error.
 */
int iniciar_server(int puerto);
/**
 * Función que mantiene activo el servidor.
 * @method run_server
 * @param  descriptor     Socket habilitado para el servidor.
 * @return int: ERR dado que si sale del bucle infinito es debido a un error o
 *              a que el servidor ha dejado de estar activo.
 */
int run_server(int descriptor);
/**
 * Función que lleva a cabo la funcionalidad del protocolo PingPong para el servidor.
 * @method protocolo_PINGPONG
 * @param  sig            Señal de activación del protocolo
 */
void protocolo_PINGPONG(int sig);
/**
 * Función que crea hilos para cada cliente del servidor.
 * @method lanza_hilos
 * @param  index          ID del cliente
 * @return int: OK
 */
int lanza_hilos(int index);
/**
 * Función que inicia la funcionalidad de un comando enviado por el cliente al servidor.
 * @method launch_service
 * @param  pr     Estructura que almacenada el ID y el comando del cliente.
 */
void* launch_service(void * pr);
/**
 * Función que mantiene activo el servidor echo
 * @method run_server_echo
 * @param  descriptor     Socket habilitado para el servidor.
 * @return int: ERR dado que si sale del bucle infinito es debido a un error o
 *              a que el servidor ha dejado de estar activo.
 */
int run_server_echo(int descriptor);
/**
 * Función que monta el servidor echo.
 * @method servidor_echo
 * @param  puerto        Puerto indicado para el servidor
 */
void servidor_echo(int puerto);
/**
 * Función que monta el servidor irc.
 * @method servidor_irc
 * @param  puerto        Puerto indicado para el servidor
 * @return int: EXIT_FAILURE
 */
int servidor_irc(int puerto);
/**
 * Función que devuelve el valor del último socket de los clientes del servidor.
 * @method maxDescriptorCliente
 * @return int: Número del último socket de los clientes.
 */
int maxDescriptorCliente();
/**
 * Elimina del Array de usuarios y clientes los que estan marcados como -1 y lo comprime
 * @method eliminaCerrados
 *
 */
void eliminaCerrados();
/**
 * Función que termina la sesión de un cliente del servidor.
 * @method eliminarUser
 * @param socket         Socket del cliente
 */
void eliminarUser(int socket);


#endif
