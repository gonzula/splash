splash: lexycal syntax compile
	@echo DONE

lexycal:
	flex splash.lex

syntax:
	/usr/local/opt/bison/bin/bison splash.y

compile:
	cc structures/*.c output.c utils.c splash_helper.c splash.tab.c -ly -ll

.PHONY : clean
clean:
	@rm \
		splash.tab.c \
		lex.yy.c \
		y.tab.c \
		a.out \
		2> /dev/null || :
