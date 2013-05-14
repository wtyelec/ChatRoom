OBJS = main.o ChatManager.o SocketManager.o
LIBEVENT = -Wl,-rpath,/usr/local/lib/ -L/usr/local/lib/ -levent -I/usr/local/include/
CC = g++
SRC_DIR = ./server/
INC_DIR = ./server/
OBJ_DIR = ./objs/
INCLUDE = -I $(SRC_DIR) -I $(INC_DIR) -I $(OBJ_DIR)

vpath %.h $(INC_DIR)
vpath %.cpp $(SRC_DIR)
vpath %.o $(OBJ_DIR)

server : main.o SocketManager.o ChatManager.o main.o
	$(CC) $(addprefix $(OBJ_DIR), $(OBJS)) $(LIBEVENT) -o server.o 
	
main.o : main.cpp SocketManager.h ChatManager.h
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@ 

SocketManager.o : SocketManager.cpp SocketManager.h global.h 
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@
	
ChatManager.o : ChatManager.cpp ChatManager.h global.h
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@

clean:
	-rm $(OBJ_DIR)*.o server.o
