CXXFLAGS =	-O2 -g -Wall -std=c++11
INCFLAGS += -Iincludes

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cc) $(wildcard $(SRC_DIR)/*/*.cc)

OBJ = $(SRC:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

LDLIBS = -lpthread

TARGET =	bin/echoclient

$(TARGET):	$(OBJ)
	$(CXX) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(INCFLAGS) $(CXXFLAGS) -c $< -o $@

all:	$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
