# SandBox-Broker

## Overview
SandBox-Broker is a security tool that monitors and hooks syscalls & functions related to file and network. It allows real-time inspection and modification of network communication and file interactions.

## Features
- Monitors file operations (open, read, write, close).
- Monitors network operations (socket, connect, send, recv).
- Allows interactive modification of network destinations.
- Provides logging for analysis.

## Installation

git clone 
cd SandBox-Broker
make

## Usage
#### Running the sandbox

```
./sandbox ./example {params}
```

## Hooked Functions

#### File Hooks
- syscalls: `open` , `read`, `write`, `close`.
- wrappers: `fopen` , `fread`, `fwrite`, `fclose`.


#### Network Hooks
- functions: `socket`, `connect`, `send`, `recv`.
```