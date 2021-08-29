#include"utils.h"

int iniciar_servidor(void){
	int socket_servidor;

    struct addrinfo hints, *server_info, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &server_info);

    // Creamos el socket de escucha del servidor
    socket_servidor = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    // Asociamos el socket a un puerto
    bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);

    // Escuchamos las conexiones entrantes
    listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(server_info);

    log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;
}

int esperar_cliente(int socket_servidor){
	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);

	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	uint32_t handshake;
	uint32_t resultOk = 0;
	uint32_t resultError = -1;

	recv(socket_cliente, &handshake, sizeof(uint32_t), MSG_WAITALL);
	if(handshake == 1)
	   send(socket_cliente, &resultOk, sizeof(uint32_t), NULL);
	else
	   send(socket_cliente, &resultError, sizeof(uint32_t), NULL);

	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente){
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

//podemos usar la lista de valores para poder hablar del for y de como recorrer la lista
t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
	return NULL;
}
