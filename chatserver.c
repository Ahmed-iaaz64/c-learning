#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 10

int clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast(char *message, int sender_fd) {
	pthread_mutex_lock(&clients_mutex);
	for (int i = 0; i < client_count; i++) {
		if (clients[i] != sender_fd) {
			write(clients[i], message, strlen(message));
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int client_fd) {
	pthread_mutex_lock(&clients_mutex);
	int index = -1;
	for (int i = 0; i < client_count; i++) {
		if (clients[i] == client_fd) {
			index = i;
			break;
		}
	}

	if (index != -1) {
		for (int j = index; j < client_count - 1; j++) {
			clients[j] = clients[j + 1];
		}
		client_count--;
	}

	pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
	int client_fd = *(int *)arg;
	free(arg);
	printf("client connected\n");

	pthread_mutex_lock(&clients_mutex);
	clients[client_count++] = client_fd;
	pthread_mutex_unlock(&clients_mutex);
	
	char buffer[1024];
	int bytes;

	while ((bytes = read(client_fd, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[bytes] = '\0';
		printf("message: %s", buffer);
		broadcast(buffer, client_fd);
	}

	remove_client(client_fd);

	printf("client disconnected\n");
	close(client_fd);
	return NULL;
}

int main() {

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);

	bind(server_fd, (struct sockaddr *)&address, sizeof(address));

	listen(server_fd, 10);

	while (1) {
		int client_fd = accept(server_fd, NULL, NULL); 

		pthread_t thread;
		int *client_fd_ptr = malloc(sizeof(int));
		*client_fd_ptr = client_fd;
		pthread_create(&thread, NULL, handle_client, client_fd_ptr);
		pthread_detach(thread);
	}
}
