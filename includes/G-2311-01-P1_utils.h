/**
 * @brief: Funciones del módulo utils
 * @file G-2311-01-P1_utils.h
 * @author Oscar Garcia de Lara Parreño & Santiago Gomez Aguirre. Grupo: 2311
 * @date 07-05-2017
 */
#ifndef __G_2311_01_P1_UTILS_H
#define __G_2311_01_P1_UTILS_H

#include "G-2311-01-P1_general.h"

/**
 * Registra la funcion en el array
 * @method registrarMensajes
 * @param  codigo            indice en el array
 * @param  handleModule      funcion a registrar
 * @param  msg_registrados   array de funciones
 * @return                   -1 en caso de error
 */
int registrarMensajes(long codigo, pf_msg handleModule,pf_msg* msg_registrados);
/**
 * Libera las memoria de una matriz de char
 * @method freeListas
 * @param  lista      [description]
 * @param  num_elems  [description]
 */
void freeListas(char ***lista, int num_elems);

#endif
