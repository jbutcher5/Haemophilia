%.o: %.c
	clang -c -g $< -o $@

build: main.o more_math.o object.o player.o
	clang $^ -lraylib -ldl -pthread -lGL -lm -o haemophilia

clean:
	rm *.o
