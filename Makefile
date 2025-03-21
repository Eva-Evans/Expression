# # Makefile
# CXX = g++
# CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# SRC = Expression.cpp ExpressionTests.cpp ExpressionParser.cpp
# OBJ = $(SRC:.cpp=.o)
# TARGET = test_expressions

# all: $(TARGET)

# $(TARGET): $(OBJ)
# 	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# test: $(TARGET)
# 	./$(TARGET)

# clean:
# 	rm -f $(OBJ) $(TARGET)

# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC = Expression.cpp ExpressionParser.cpp main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = differentiator

TEST_SRC = ExpressionTests.cpp Expression.cpp ExpressionParser.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)
TEST_TARGET = test_expressions

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

$(TEST_TARGET): $(TEST_OBJ) # ExpressionParser.o
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJ)
tests: $(TEST_TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)
