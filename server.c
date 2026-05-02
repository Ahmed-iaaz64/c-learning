#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);

	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	listen(server_fd, 3);

	printf("Server listening on port 8080\n");

	while (1) {
		int client_fd = accept(server_fd, NULL, NULL);
		printf("Client connected\n");

		// read the request
		char buffer[1024];
		read(client_fd, buffer, 1024);
		printf("Request:\n%s\n", buffer);

		char method[10], path[100];
		sscanf(buffer, "%s %s", method, path);
		printf("Method: %s, Path: %s\n", method, path);

		// send request
		char *response;
		if (strcmp(path, "/") == 0) {
			response = 
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html\r\n"
				"\r\n"
				"<html><body><h1>Hello from my C server!</h1></body></html>";
		}
		else if (strcmp(path, "/about") == 0) {
			response = 
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html\r\n"
				"\r\n"
				"<html><body><h1>My about page!</h1></body></html>";
		}
		else {
			response = 
				"HTTP/1.1 404 Not Found\r\n"
				"Content-Type: text/html\r\n"
				"\r\n"
				"<html><body><h1>404 - Page Not Found</h1></body></html>";
		}

		write(client_fd, response, strlen(response));
		close(client_fd);
	}
	close(server_fd);
	return 0;
}

