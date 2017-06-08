/**
 * @brief: Funciones del módulo general
 * @file G-2311-01-P1_general.h
 * @author Oscar Garcia de Lara Parreño & Santiago Gomez Aguirre. Grupo: 2311
 * @date 07-05-2017
 */

#ifndef __G_2311_01_P1_GENERAL_H
#define __G_2311_01_P1_GENERAL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <syslog.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <pthread.h>
#include <signal.h>

#include <redes2/irc.h>

//#include <redes2/ircxchat.h>

#define MAX_CONNECTIONS 50
#define HOST_NAME "es.irc.verydep"
#define HOST_INFO "Un servidor muy dep"
/**
 * [int  description]
 * @method int
 * @param  pf_msg [description]
 * @return        [description]
 */
typedef int (*pf_msg)(char *command,int socket);

/**
* @brief Lista de posibles errores
*/
enum{OK=0,ERR=-1};




#endif
