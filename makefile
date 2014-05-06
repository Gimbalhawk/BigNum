
COMP = g++ -Wall -c
LINK = g++ -Wall -o

# Extra flags for release and debug modes
RLS = -O2
DBG = -g

# The location of our source code
CODE = Code

# The location of our compiled .o files
OBJS = $(CODE)/obj

# The name of our generated unit tests file
TESTFILE = $(CODE)/tests.cpp

release: COMP += $(RLS)
release: build

debug: COMP += $(DBG)
debug: build

#tests: COMP += $(DBG)
tests: 
	cxxtestgen --error-printer -o $(TESTFILE) $(CODE)/Tests.h
	g++ $(DBG) -o Testing $(TESTFILE)
	@rm $(TESTFILE)

buildtests:
	g++ $(DBG) -o Testing $(TESTFILE)

build: Main.o
	$(LINK) BigNum $(OBJS)/Main.o

%.o: $(CODE)/%.cpp
	$(COMP) $(CODE)/$*.cpp -o $(OBJS)/$*.o

%.h: $(CODE)/%.h

clean:
	@echo Cleaning binaries
	@rm -f $(OBJS)/* $(TESTFILE) BigNum Testing