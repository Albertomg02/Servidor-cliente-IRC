#include "../includes/G-2311-01-P1_socket.h"

/**
 * [crear_socket description]
 * @method crear_socket
 * @param  protocolo    [description]
 * @return              [description]
 */
int crear_socket(int protocolo){
  int descriptor;

  syslog(LOG_INFO,"Creando socket");
  if((descriptor=socket(AF_INET,protocolo,0))<0){
    syslog(LOG_ERR,"Error obteniendo descriptor");
    return ERR;
  }

  return descriptor;
}

/**
 * [daemonizar description]
 * @method daemonizar
 * @param  servicio   [description]
 * @return            [description]
 */
int daemonizar(char *servicio){

  pid_t pid;

  int i=0,tamano_tab_descriptor=0;
  pid=fork();
  if(pid<0){exit(EXIT_FAILURE);}
  if(pid>0){exit(EXIT_SUCCESS);}

  umask(0);
  setlogmask(LOG_UPTO(LOG_INFO));
  openlog(servicio,LOG_CONS|LOG_PID|LOG_NDELAY,LOG_LOCAL3);
  syslog(LOG_ERR,"Iniciando servidor.");

  if(setsid()<0){
    syslog(LOG_ERR,"Error creando un nuevo SID para el proceso hijo.");
    return ERR;
  }

  if(chdir("/")<0){
    syslog(LOG_ERR,"Error cambiando el actual directorio de trabajo =\"/\"");
    return ERR;
  }

  syslog(LOG_INFO,"Cerrando descriptores de fichero");
  if((tamano_tab_descriptor=getdtablesize())<0){
    syslog(LOG_ERR,"Error obteniendo tamaño de la tabla de descriptores");
    return ERR;
  }
  for(i=0;i<tamano_tab_descriptor;i++){
    close(i);
  }

  return OK;
}
