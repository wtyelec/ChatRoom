OBJS = main.o chat_manager.o socket_manager.o log.o util.o
LIBEVENT = -Wl,-rpath,/usr/local/lib/ -L/usr/local/lib/ -levent -I/usr/local/include/
CC = g++
SRC_DIR = ./server/
INC_DIR = ./server/
OBJ_DIR = ./objs/
INCLUDE = -I $(SRC_DIR) -I $(INC_DIR) -I $(OBJ_DIR)

vpath %.h $(INC_DIR)
vpath %.cpp $(SRC_DIR)
vpath %.o $(OBJ_DIR)

server : main.o $(OBJS)
	$(CC) $(addprefix $(OBJ_DIR), $(OBJS)) $(LIBEVENT) -o server.o 
	
main.o : main.cpp socket_manager.h chat_manager.h
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@ 

socket_manager.o : socket_manager.cpp socket_manager.h global.h 
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@
	
chat_manager.o : chat_manager.cpp chat_manager.h global.h util.h
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@

log.o : log.cpp log.h 
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@

util.o : util.cpp util.h
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@

clean:
	-rm $(OBJ_DIR)*.o server.o
