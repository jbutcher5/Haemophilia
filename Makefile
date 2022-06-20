SRC = main.c more_math.c object.c player.c
OBJ = ${SRC:.c=.o}

%.o: %.c
	clang -c -g $< -o $@

build: ${OBJ}
	clang $^ -lraylib -ldl -pthread -lGL -lm -o haemophilia
	echo "${OBJ}"

clean:
	rm *.o
