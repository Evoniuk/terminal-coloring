tdraw: main.c raw-mode.c render.c editor.c input.c
	@cc -o tdraw $^

clean:
	@rm tdraw
