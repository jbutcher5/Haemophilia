SRC = main.c more_math.c aabb.c player.c
OBJ = ${SRC:.c=.o}

%.o: %.c
	clang -c -g $< -o $@

build: ${OBJ}
	clang $^ -lraylib -ldl -pthread -lGL -lm -o haemophilia

clean:
	rm *.o
