COMPILER = g++
FLAGS = 


C = $(COMPILER) $(FLAGS)

TARGETS = main.cpp json.cpp cli_parser.cpp
X = main

all: $X

run : $X 
	./$<

test : clean $X
	./$X -a "stone's reach"
	
clean : 
	rm -f *.o
	rm -f $X

$X : $(TARGETS:.cpp=.o)
	$C -o $@ $^

%.o : %.cpp 
	$C -c $<



