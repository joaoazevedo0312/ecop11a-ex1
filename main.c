#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *html_content = "<html><head><title>Página de Exemplo</title></head><body><h1>Ola, Mundo!</h1></body></html>";

    // Criando o socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Vinculando o socket à porta
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erro ao vincular o socket");
        exit(EXIT_FAILURE);
    }

    // Escutando por conexões
    if (listen(server_fd, 3) < 0) {
        perror("Erro ao escutar");
        exit(EXIT_FAILURE);
    }

    printf("Servidor escutando na porta %d...\n", PORT);

    while (1) {
        // Aceitando conexões
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Erro ao aceitar a conexão");
            exit(EXIT_FAILURE);
        }

        // Lendo a requisição HTTP
        read(new_socket, buffer, BUFFER_SIZE);

        // Respondendo com a página HTML
        char response[BUFFER_SIZE];
        sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(html_content), html_content);
        send(new_socket, response, strlen(response), 0);

        printf("Página HTML enviada ao cliente.\n");

        // Fechando o socket
        close(new_socket);
    }

    return 0;
}
