symnmf: symnmf.o symnmf.h
	gcc -o symnmf symnmf.o -ansi -Wall -Wextra -Werror -pedantic-errors -lm

symnmf.o: symnmf.c
	gcc -c symnmf.c -ansi -Wall -Wextra -Werror -pedantic-errors -lm