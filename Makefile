splash: lexycal syntax compile
	@echo DONE

lexycal:
	flex splash.lex

syntax:
	/usr/local/opt/bison/bin/bison splash.y

compile:
	cc structures/*.c scope.c action.c output.c utils.c splash_helper.c interpolated.c splash.tab.c -ly -ll -lm -o splash

.PHONY : clean
clean:
	@rm \
		splash.tab.c \
		lex.yy.c \
		y.tab.c \
		splash \
		2> /dev/null || :
