#include "httpserver.h"

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr,client_addr;
	int server_fd, cfd;
	socklen_t addr_len = sizeof(client_addr);
	int opt = 1;

	/* Creating server socket file descriptor */
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("SERVER: socket create failed (%s)\n", strerror(errno));
		exit(EXIT_FAILURE);
	}


	/* Construct server socket address, bind socket to it,
	   and make this a listening socket */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portnum);

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		printf("SERVER: bind failed (%s)\n", strerror(errno));
		exit(1);
	}

	/*listen for connections*/
	if (listen(server_fd, BACKLOG) == -1) {
		printf("SERVER: listen failed (%s)\n", strerror(errno));
		exit(1);
	}

	for (;;) {		/* Handle client connections iteratively */

		/* Accept a connection. The connection is returned on a new
		   socket, 'cfd'; the listening socket ('server_fd') remains open
		   and can be used to accept further connections. */

		cfd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
		if (cfd == -1) {
			printf("SERVER: accept failed (%s)\n", strerror(errno));
			continue;
		}
		handle_request(cfd);
		close(cfd);
	}
	close(server_fd);
	return 0;
}
/*function to handle client req */
void handle_request(int cfd) {
	char buffer[BUF_SIZE], method[16], path[256], full_path[512];

	int bytes_received = recv(cfd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_received <= 0) {
		perror("Receive failed");
		return;
	}

	buffer[bytes_received] = '\0';
	sscanf(buffer, "%s %s", method, path);

	/* Form the full file path */
	snprintf(full_path, sizeof(full_path), ".%s", path);

	struct stat st;
	if (stat(full_path, &st) < 0) {
		send_response(cfd, "404 Not Found", "text/plain", "Resource not found :(");
		return;
	}
	/*check on type of resource DIR,RegularFile*/
	if (S_ISDIR(st.st_mode)) {
		/*list dir contents*/
		list_directory(cfd, full_path);
	} 
	else if (S_ISREG(st.st_mode)) {
		/*check if it's .cgi file to execute it*/
		if (strstr(full_path, ".cgi")) {
			execute_cgi(cfd, full_path);
		} else {
			/*open file*/
			send_file(cfd, full_path);
		}
	}
	/*other type*/	
	else {
		send_response(cfd, "403 Forbidden", "text/plain", "Access denied !!!");
	}
}

/*show response based on request*/
void send_response(int cfd, const char *status, const char *content_type, const char *body) {
	char response[BUF_SIZE];
	snprintf(response, sizeof(response),
			"HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n%s",
			status, content_type, strlen(body), body);
	send(cfd, response, strlen(response), 0);
}


/*to open file and show its contents*/
void send_file(int cfd, const char *path) {
	/*Open the file in binary mode*/
	FILE *file = fopen(path, "rb");
	if (!file) {
		send_response(cfd, "500 Internal Server Error", "text/plain", "Failed to open file");
		return;/*show response based on request*/
		void send_response(int cfd, const char *status, const char *content_type, const char *body) {
			char response[BUF_SIZE];
			snprintf(response, sizeof(response),
					"HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n%s",
					status, content_type, strlen(body), body);
			send(cfd, response, strlen(response), 0);
		}

	}

	/*Determine MIME type based on file extension*/
	const char *mime_type = "application/octet-stream"; /* Default MIME type*/

	if (strstr(path, ".txt")) {
		mime_type = "text/plain";
	} else if (strstr(path, ".html") || strstr(path, ".htm")) {
		mime_type = "text/html";
	} else if (strstr(path, ".jpg") || strstr(path, ".jpeg")) {
		mime_type = "image/jpeg";
	} else if (strstr(path, ".png")) {
		mime_type = "image/png";
	} else if (strstr(path, ".gif")) {
		mime_type = "image/gif";
	} else if (strstr(path, ".css")) {
		mime_type = "text/css";
	} else if (strstr(path, ".js")) {
		mime_type = "application/javascript";
	}
	else if (strstr(path, ".c")) {
		mime_type = "text/x-c";  
	} else if (strstr(path, ".h")) {
		mime_type = "text/x-header"; 
	}

	/*Get file size*/
	struct stat st;
	if (stat(path, &st) < 0) {
		fclose(file);
		send_response(cfd, "500 Internal Server Error", "text/plain", "Failed to get file size");
		return;
	}
	long file_size = st.st_size;

	/*Send HTTP response headers*/
	char header[BUF_SIZE];
	snprintf(header, sizeof(header),
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: %s\r\n"
			"Content-Length: %ld\r\n"
			"Content-Disposition: inline\r\n"
			"\r\n",
			mime_type, file_size);
	send(cfd, header, strlen(header), 0);

	/*Send file content*/
	char buffer[BUF_SIZE];
	size_t bytes_read;
	while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
		send(cfd, buffer, bytes_read, 0);
	}

	fclose(file);
}


/*to show files of dir*/
void list_directory(int cfd, const char *path) {
	DIR *dir = opendir(path);
	if (!dir) {
		send_response(cfd, "500 Internal Server Error", "text/plain", "Failed to open directory");
		return;
	}

	char buffer[BUF_SIZE];
	snprintf(buffer, sizeof(buffer), "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	send(cfd, buffer, strlen(buffer), 0);

	snprintf(buffer, sizeof(buffer), "<html><body><h1>Directory Listing</h1><ul>");
	send(cfd, buffer, strlen(buffer), 0);

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		snprintf(buffer, sizeof(buffer), "<li><a href=\"%s/%s\">%s</a></li>", path, entry->d_name, entry->d_name);
		send(cfd, buffer, strlen(buffer), 0);
	}

	snprintf(buffer, sizeof(buffer), "</ul></body></html>");
	send(cfd, buffer, strlen(buffer), 0);

	closedir(dir);
}
/*execute file if it's cgi file*/
void execute_cgi(int cfd, const char *path) {
        int pipefd[2];
        if (pipe(pipefd) < 0) {
                send_response(cfd, "500 Internal Server Error", "text/plain", "Failed to execute CGI");
                return;
        }

        pid_t pid = fork();
        if (pid < 0) {
                send_response(cfd, "500 Internal Server Error", "text/plain", "Fork failed");
                return;
        }

        if (pid == 0) { 
                /* Child process */
                close(pipefd[0]);  /* Close read end */
                dup2(pipefd[1], STDOUT_FILENO); /* Redirect stdout to pipe */
                execl(path, path, NULL); /* Execute CGI script */
                perror("execl failed");
                exit(EXIT_FAILURE);
        } else { 
                /* Parent process */
                close(pipefd[1]); /* Close write end */
                char buffer[BUF_SIZE];
                snprintf(buffer, sizeof(buffer), "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
                send(cfd, buffer, strlen(buffer), 0);

                int n;
                while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
                        send(cfd, buffer, n, 0);
                }

                close(pipefd[0]);

                /* Wait for the child process to finish using waitpid */
                int status;
                waitpid(pid, &status, 0);

                /*check the exit status of the child process*/
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                        /*if cgi script fails with a non-zero exit status*/
                        send_response(cfd, "500 Internal Server Error", "text/plain", "CGI script failed");
                }
        }
}
