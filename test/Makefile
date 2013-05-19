CC = g++
RM = rm

CDEBUG = -g
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = .
LIB_DIR = ../lib
BIN_DIR = ../bin

all: $(BIN_DIR)/CStrUtilTest

SRC = \
CStrUtilTest.cpp

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = -I$(INC_DIR) -I.

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(BIN_DIR)/CStrUtilTest

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(BIN_DIR)/CStrUtilTest: $(OBJS) $(LIB_DIR)/libCStrUtil.a
	$(CC) $(LDEBUG) -o $(BIN_DIR)/CStrUtilTest $(OBJS) -lCStrUtil -L$(LIB_DIR)
