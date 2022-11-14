# Builds the tc program

all: tc

clean:
	rm -f tc
	
# builds traffic prog
tc: tc.c
	gcc tc.c -o tc -lpthread -ldl
