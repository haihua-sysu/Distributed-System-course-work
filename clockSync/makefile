OBJ_DIR = ./obj
SRC_DIR = ./src
BIN_DIR = ./bin
#VPATH = $(SRC_DIR) 

targets = server_1 server_2 client
targetsobjs = $(foreach i, $(targets), $(i).o)
objs = socket.o time.o
objsfile = $(targetsobjs) $(objs) 

all: $(targets) 

$(targets) : $(objs) $(targetsobjs)
	g++ -o $(BIN_DIR)/$@ $(OBJ_DIR)/$(addsuffix .o, $@) $(foreach i, $(objs), $(OBJ_DIR)/$(i)) 

%.o : $(SRC_DIR)/%.cpp
	g++ -c $< -o $(OBJ_DIR)/$@ -std=c++11

#kvdatabase.o: kvdatabase.cpp kvconfig.h kvdatabase.h kvfile.h kvindex.h hashmap.h kvcache.h kvhelper.h
#kvindex.o: kvindex.cpp kvindex.h kvfile.h kvconfig.h hashmap.h
#kvfile.o: kvfile.cpp kvfile.h kvconfig.h
#kvcache.o: kvcache.cpp kvcache.h hashmap.h kvhelper.h
#threadpool.o: threadpool.cpp threadpool.h runnable.h lockfreequeue.h
#socket.o: socket.cpp socket.h
#
#simplekvserver.o: simplekvserver.cpp socket.h threadpool.h runnable.h \
#	  lockfreequeue.h kvdatabase.h kvfile.h kvconfig.h kvindex.h hashmap.h kvcache.h kvhelper.h
#testdb.o: testdb.cpp kvdatabase.h kvfile.h kvconfig.h kvindex.h hashmap.h kvcache.h kvhelper.h

.PHONY: clean
clean:
	-rm -f $(foreach i, $(objsfile), $(OBJ_DIR)/$(i))
