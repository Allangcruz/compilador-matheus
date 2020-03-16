default: build

# Faz o Build
build:
	@gcc compilador.c -o compilador && ./compilador