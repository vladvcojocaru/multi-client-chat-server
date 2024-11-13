# multi-client-chat-server

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [How It Works](#how-it-works)
- [Code Structure](#code-structure)
- [Explanation of Key Functions](#explanation-of-key-functions)
- [Example Output](#example-output)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Introduction

This project is a simple chat server written in **C** that supports multiple clients. The server listens for connections from clients, and any message sent by one client is broadcasted to all other connected clients. This project is a great way to learn about socket programming, multi-threading, and client-server communication in C.

## Features
- Handles multiple clients simultaneously using threads.
- Broadcasts messages from one client to all connected clients.
- Demonstrates fundamental concepts of networking and concurrency in C.
- Uses sockets for communication between the server and clients.

## Requirements

Ensure your system has the following installed:
- **GCC compiler** (`gcc`)
- **Make** (`make`)
- **Linux system** (tested on Fedora)

## Installation

Follow these steps to set up the project:

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/multi-client-chat-server.git
   cd multi-client-chat-server

Linux system (tested on Fedora)
Installation
Clone the repository:
