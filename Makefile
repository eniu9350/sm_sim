CC = gcc
INCLUDE_ADT = ../adt

all:
	$(CC) -g -I$(INCLUDE_ADT) input_generator.c user.c ch.c sm_client.c ev.c ev_loop.c sm_server.c sim_env.c ev_handler.c main.c -lm
