CPP = g++
COMPILERFLAGS = -g -Wall -Wextra -Wno-sign-compare 

LINKLIBS = -lpthread

CSMAOBJECTS = obj/csma.o

.PHONY: all clean


all : obj csma

csma: $(CSMAOBJECTS)
	$(CPP) $(COMPILERFLAGS) $^ -o $@ $(LINKLIBS)

clean :
	$(RM) obj/*.o csma

obj/%.o: src/%.cpp
	$(CPP) $(COMPILERFLAGS) -c -o $@ $<
obj:
	mkdir -p obj
