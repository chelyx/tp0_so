#include "client.h"

int main(void){
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	log_info(logger, "Soy un log");

	config = iniciar_config();

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor
	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "ip");
	puerto = config_get_string_value(config, "puerto");
	//Loggear valor de config
	log_info(logger, valor);
	log_info(logger, ip);
	log_info(logger, puerto);

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	//enviar CLAVE al servirdor
	enviar_mensaje(valor, conexion);

	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void){
	t_log* logger = log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO);

	return logger;
}

t_config* iniciar_config(void){
	t_config* nuevo_config = config_create("tp0.config");

	return nuevo_config;
}

void leer_consola(t_log* logger){
	char* leido;

	leido = readline(">");

	while (strcmp(leido, "") != 0) {
		log_info(logger, leido);
		free(leido);
		leido = readline(">");
	}
	free(1); // es asi?

}

void paquete(int conexion){
	//Ahora toca lo divertido!

	char* leido;
	t_paquete* paquete;


}

void terminar_programa(int conexion, t_log* logger, t_config* config){
	log_destroy(logger);
	liberar_conexion(conexion);
}
