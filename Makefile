G++ = g++
CFLAGS = -std=c++20

cruceArchivos: cruceArchivos.cpp
		$(G++) $(CFLAGS) $< -o $@
