# Builds the tc program

all: tc jonathan

clean:
	rm -f tc jonathan
	
# builds traffic prog
tc: tc.c
	gcc tc.c -o tc -lpthread -ldl

jonathan: jonathan.c
	gcc jonathan.c -o jonathan -lpthread