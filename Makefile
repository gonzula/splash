splash: lexycal syntax compile
	@echo DONE

lexycal:
	flex -o src/lex.yy.c src/splash.lex
syntax:
	/usr/local/opt/bison/bin/bison src/splash.y -o src/splash.tab.c

compile:
	cc src/structures/*.c src/scope.c src/action.c src/output.c src/utils.c src/splash_helper.c src/interpolated.c src/splash.tab.c -ly -ll -lm -o splash

.PHONY : clean
clean:
	@rm \
		src/splash.tab.c \
		src/lex.yy.c \
		src/y.tab.c \
		splash \
		2> /dev/null || :
