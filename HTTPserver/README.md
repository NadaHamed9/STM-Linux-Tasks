# HTTP Server in C

This project implements a simple HTTP server in C that supports basic web functionality, including handling requests for directories, regular files, and CGI scripts. The server can serve content to a web browser and return appropriate HTTP responses.

## Features

- Accept connections from web browsers
- Parse incoming HTTP requests
- Process requests for the following:
  1. **Directory**: List its contents
  2. **Regular File**: Output its contents
  3. **CGI Script**: Execute the script and send output
  4. **Non-existing Resource**: Return proper HTTP error (404)
- Construct and send HTTP responses back to the client

## Structure

The HTTP server performs the following actions based on the type of request:

- **Directory Request**: If the requested resource is a directory, the server lists its contents.
- **File Request**: If the requested resource is a regular file, the server sends the file contents as a response.
- **CGI Request**: If the requested resource is a CGI file (e.g., `.cgi`), the server executes the CGI script and returns its output.
- **Error Handling**: If the resource doesn't exist, the server returns a `404 Not Found` error. If there is any internal server error, it responds with a `500 Internal Server Error`.


## Running the server

1. **Compile the server**:

    ```bash
    gcc -o httpserver httpserver.c
    ```

2. **Make the CGI scripts executable**:

    Make sure the scripts are executable , such as `greeting.cgi` :

    ```bash
    chmod +x greeting.cgi
    ```

3. **Start the server**:

    Run the server with:

    ```bash
    ./httpserver
    ```

    The server will listen on port `8080` by default.

4. **Access the server**:

    Open a web browser and navigate to the following URL to test the server:

    - For regular files (e.g., `.txt`, `.html`):
      ```
      http://127.0.0.1:8080/yourfile.txt
      ```
    - For directories:
      ```
      http://127.0.0.1:8080/yourdirectory/
      ```
    - For CGI scripts:
      ```
      http://127.0.0.1:8080/greeting.cgi
      ```


