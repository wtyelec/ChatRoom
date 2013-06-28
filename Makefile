OBJS = main.o ChatManager.o SocketManager.o Log.o Util.o
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
	
main.o : main.cpp SocketManager.h ChatManager.h
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@ 

SocketManager.o : SocketManager.cpp SocketManager.h Global.h 
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@
	
ChatManager.o : ChatManager.cpp ChatManager.h Global.h Util.h
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@

Log.o : Log.cpp Log.h 
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@

Util.o : Util.cpp Util.h
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@

clean:
	-rm $(OBJ_DIR)*.o server.o
