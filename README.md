[![Build Status](https://travis-ci.org/andersonfernandes/sm-chat.svg?branch=master)](https://travis-ci.org/andersonfernandes/sm-chat)

# SM Chat

IPC Chat application using Shared Memory

## Structure
``` text
.
├── Makefile
├── bin
│   └── foo
├── include
│   ├── foo.h
├── src
│   └── foo.cpp
├── tests
│   └── foo_test.cpp

```

Sources go in [src/](src/), header files in [include/](include/), tests in [tests/](tests/) and binaries in [bin/](bin). 

## Building

To compile the application, run `make` inside the root directory.

Examples:

``` bash
> make            # Compile the sources
> make compile 	  # Also compile the sources
> make clean	  # Clean the binaries on bin/ folder
> make test	    # Run tests
```
