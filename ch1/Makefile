EXEC = main
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h*)
OBJECTS = $(SOURCES:.cpp=.o)
LIBS += -L/usr/lib/boost_1_57_0 -lboost_system -lboost_thread -lboost_regex
INCS += -I/usr/include/boost_1_57_0

all: $(EXEC)

main: $(OBJECTS)
	g++ -static  $(LIBS) $(OBJECTS) -o $(EXEC)

%.o: %.cpp $(HEADERS)
	g++ $(INCS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)

