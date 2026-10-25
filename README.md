# Multiplexing I/O: Advanced Chat Application

[![University](https://img.shields.io/badge/University-University%20of%20Ioannina-blue.svg)](https://www.uoi.gr)
[![Language](https://img.shields.io/badge/Language-C-blueviolet.svg)]()
[![Syscall](https://img.shields.io/badge/Syscall-select()-yellow.svg)]()

## Overview
This assignment demonstrates advanced socket programming techniques by evolving a rudimentary socket connection into a fully functional, multi-user Chatroom. The core mechanism enabling this non-blocking behavior is the `select()` system call, which allows a single process to monitor multiple file descriptors (sockets and standard input) simultaneously.

## Project Evolution
The project is divided into three iterative directories:

### `ΜΕΡΟΣ_Α`: Bi-directional Client-Server Chat
- Transitioned from simple echo behavior to a two-way terminal chat.
- Utilized `select()` to multiplex between `stdin` (keyboard input) and the network `sockfd`.

### `ΜΕΡΟΣ_Β`: Mediated Client-to-Client Chat
- Modified the server to drop the `fork()` paradigm in favor of Multiplexing I/O.
- The server now acts as a central mediator, storing active client `fd`s and routing payloads from Client A directly to Client B. Clients introduce themselves with a username upon connection.

### `ΜΕΡΟΣ_Γ`: Multi-User Chatroom & Private Messaging
- Expanded server capacity to handle multiple concurrent clients.
- Implemented payload parsing on the server-side to distinguish between public broadcasts and **Direct Messages (DMs)**.
- Users can send private messages using the syntax: `/msg <username> <content>`. The server utilizes `strncmp` and `sscanf` to route the data strictly to the target's socket.

---
**Author:** Maria Alexandra Ntontou (2799)
