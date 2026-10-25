# Introduction to TCP Sockets

[![University](https://img.shields.io/badge/University-University%20of%20Ioannina-blue.svg)](https://www.uoi.gr)
[![Language](https://img.shields.io/badge/Language-C-blueviolet.svg)]()
[![Protocol](https://img.shields.io/badge/Protocol-TCP%2FIPv4-orange.svg)]()

## Overview
This project explores the implementation of connection-oriented networking using the **Berkeley Sockets API** in C. It demonstrates how processes communicate over a network via the Transmission Control Protocol (TCP).

## Technical Implementation
### The Client-Server Architecture
- **Server (`server1.c`):** Creates a stream socket (`SOCK_STREAM`, `AF_INET`), binds to a specified port, listens for incoming connections, and accepts them. Upon connection, it transmits a greeting payload and safely closes the file descriptor.
- **Client (`client.c`, `client1.c`):** Dynamically accepts an IPv4 address and a Port via command-line arguments. It utilizes `inet_pton()` to convert standard IPv4 dotted-decimal strings into network byte order, establishing a reliable connection to the target server to retrieve data.

### Use Case
The client was tested against public NIST and University time servers (Daytime Protocol, Port 13) to successfully fetch the current synchronized UTC time.

---
**Author:** Maria Alexandra Ntontou (2799)
