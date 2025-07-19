# 🌐 Webserv – Custom HTTP Server in C++

![C++](https://img.shields.io/badge/C++98-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)
![Networking](https://img.shields.io/badge/Networking-TCP/IP-blue)
![Sockets](https://img.shields.io/badge/Sockets-Low--Level-orange)

## 📖 About the Project

**Webserv** is a lightweight HTTP/1.1 server developed in C++98 as part of the 42 curriculum. It supports core web server functionalities like serving static files, handling CGI scripts, directory listing, request parsing, and more — all implemented from scratch using low-level system calls and `poll()` for multiplexed I/O.

This project deepened our understanding of TCP/IP networking, file descriptors, non-blocking sockets, and HTTP protocol internals.

## ✨ Features

- 📜 HTTP/1.1 support (GET, POST, DELETE)
- ⚙️ Configurable via `.conf` file (similar to nginx)
- 🧮 MIME type detection & custom error pages
- 🗂️ Auto-index (directory listing) per location block
- 🔁 Multiple virtual servers (host:port based) //Although i didn't implement this
- 🧩 CGI support (e.g., executing PHP scripts)
- 📡 Poll-based I/O multiplexing for concurrent client handling
- 🐛 Robust error handling & logging
- 💻 Compatible with Linux and macOS

## 🔧 Tech Stack

- C++98 (Standard Template Library)
- POSIX system calls
- `poll()` for non-blocking I/O
- File I/O & signal handling
- TCP/IP networking


## 📁 Project Structure

This is the folder layout for `webserv`, a custom HTTP server written in C++98.

- `conf/`: Server configuration files
- `config/`: Configuration parsing
- `http/` & `httpResponse/`: HTTP protocol handling (requests/responses)
- `server/`: Core server loop, connections, sockets
- `utils/`: Reusable helper functions
- `includes/`: Header files
- `public/`: Public HTML/CSS/JS files served to clients
- `basic_CGI/`: Sample CGI scripts (PHP)
- `Non_Blocking_test/`: Experiments with non-blocking I/O
- `d_struct/`: Custom data structures
- `trimmed_photos/`: Frontend or branding assets
- `main.cpp`: Application entry point
- `Makefile`: Compilation logic



## 🚀 Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/Echez-cpu/webserver_42.git

cd webserver_42
```
### 2. Build
```
make
```
### 3. run
```
./webserv conf/Neondrive.conf
```
### 4. connect
```
curl http://localhost:5555 or use a browser.
```

```
🧪 Testing

The server was tested with:

* Chrome, Firefox, and Safari.

* curl, telnet.

* PHP CGI scripts.

* Concurrent connections & image file uploads.
```

## 📚 What I Learned

* Writing a full HTTP server from scratch

* Deep understanding of networking & file descriptor management

* Designing modular server architecture

* Robust error management and debugging
