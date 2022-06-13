%.o: %.c
	clang -c -g $< -o $@

build: *.o
	clang $< -lraylib -ldl -pthread -lGL -lm -o haemophilia

clean:
	rm *.o
