tdraw: main.c raw-mode.c render.c initialize.c input.c
	@cc -o tdraw $^

clean:
	@rm tdraw
