all:
	gcc -Wall -Werror -fsanitize=address -lm -std=c11 second.c -o second
deg:
	gcc -Wall -Werror -fsanitize=address -lm -std=c11 -g second.c -o second
val:
	gcc -Wall -Werror -lm -std=c11 -g second.c -o second
ignore:
	gcc -lm -std=c11 -g second.c -o second
clean:
	rm -f second
