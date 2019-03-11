splash: lexycal syntax compile
	@echo DONE

lexycal:
	flex -o compiler/lex.yy.c compiler/splash.lm
syntax:
	bison -d compiler/splash.ym -o compiler/splash.tab.c

compile:
	cc compiler/structures/*.c compiler/scope.c compiler/action.c compiler/output.c compiler/utils.c compiler/splash_helper.c compiler/interpolated.c compiler/lex.yy.c compiler/splash.tab.c compiler/main.c -ly -ll -lm -o splash

.PHONY : clean
clean:
	@rm \
		compiler/splash.tab.c \
		compiler/lex.yy.c \
		compiler/y.tab.c \
		splash \
		2> /dev/null || :
