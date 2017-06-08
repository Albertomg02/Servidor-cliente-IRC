/**
 * @brief: Funciones del módulo SSL
 * @file G-2311-01-P3_ssl.h
 * @author Oscar Garcia de Lara Parreño & Santiago Gomez Aguirre. Grupo: 2311
 * @date 07-05-2017
 */
#ifndef __G_2311_01_P3_SSL_H
#define __G_2311_01_P3_SSL_H

#include "G-2311-01-P1_general.h"
/**
 * Esta función se encargará de realizar todas las llamadas necesarias para que la aplicación pueda usar la capa segura SSL
 * @method inicializar_nivel_SSL
 * @return contexto ssl o null en caso de error
 */
SSL_CTX* inicializar_nivel_SSL();
/**
 * Esta función se encargará de inicializar correctamente el contexto que será utilizado para la creación de canales seguros mediante SSL.
 * @method fijar_contexto_SSL
 * @param  cnt                contexto ssl
 * @param  CA                 archivo ca.pem
 * @param  CApath             ruta donde esta el archivo ca.pem
 * @param  f                  certificado
 */
void fijar_contexto_SSL(SSL_CTX *cnt, const char *CA, const char *CApath, const char *f);
/**
 * Dado un contexto SSL y un descriptor de socket esta función se encargará de obtener un canal seguro SSL iniciando el proceso de handshake con el otro extremo.
 * @method conectar_canal_seguro_SSL
 * @param  cnt                       contexto ssl
 * @param  socket                    socket
 * @return                           socket ssl o null en caso de error
 */
SSL *conectar_canal_seguro_SSL(SSL_CTX *cnt, int socket);
/**
 * Dado un contexto SSL y un descriptor de socket esta función se encargará de bloquear la aplicación, que se quedará esperando hasta recibir un handshake por parte del cliente.
 * @method aceptar_canal_seguro_SSL
 * @param  cnt                      contexto ssl
 * @param  socket                   socket
 * @return                          socket ssl o null en caso de error
 */
SSL *aceptar_canal_seguro_SSL(SSL_CTX *cnt, int socket);

/**
 * Esta función comprobará una vez realizado el handshake que el canal de comunicación se puede considerar seguro.
 * @method evaluar_post_connectar_SSL
 * @param  ssl                        socekt ssl
 * @return                            -1 en caso de error
 */
int evaluar_post_connectar_SSL(const SSL *ssl);
/**
 * Esta función será el equivalente a la función de envío de mensajes que se realizó en la práctica 1, pero será utilizada para enviar datos a través del canal seguro.
 * @method enviar_datos_SSL
 * @param  ssl              socket ssl
 * @param  buffer           lo que se va aescribir
 * @param  len              tamaño de lo que se vaa escribir
 * @return                 tamaño de lo escrito, -1 en caso de error
 */
int enviar_datos_SSL(SSL *ssl, const void *buffer, int len);
/**
 * Esta función será el equivalente a la función de lectura de mensajes que se realizó en la práctica 1, pero será utilizada para enviar datos a través del canal seguro.
 * @method recibir_datos_SSL
 * @param  ssl               socket ssl
 * @param  buffer            donde se guardan los datos
 * @param  len               tamaño del buffer
 * @return                   tamaño de lo leido, -1 en caso de error
 */
int recibir_datos_SSL(SSL *ssl, void *buffer, int len);
/**
 * Esta función liberará todos los recursos y cerrará el canal de comunicación seguro creado previamente.
 * @method cerrar_canal_SSL
 * @param  ssl              socket ssl
 * @param  cnt              contexto ssl
 * @param  socket           socket en que se ha estable conexion
 * @return                 -1 en caso de error
 */
int cerrar_canal_SSL(SSL *ssl, SSL_CTX *cnt, int socket);

#endif
