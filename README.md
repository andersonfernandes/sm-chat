# SM Chat

IPC Chat application using Shared Memory

## Structure
``` text
.
├── Makefile
├── bin
│   └── server
│   └── client
├── include
│   ├── message.h
│   └── shm_queue.h
│   └── user.h
├── src
│   └── client.cpp
│   └── server.cpp
│   └── shm_queue.cpp
│   └── user.cpp

```

Sources go in [src/](src/), header files in [include/](include/) and binaries in [bin/](bin). 

## Building

To compile the application, run `make` inside the root directory.

Examples:

``` bash
> make            # Compile the sources
> make compile 	  # Also compile the sources
> make clean	  # Clean the binaries on bin/ folder
```
