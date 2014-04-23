
COMP = g++ -Wall -c
LINK = g++ -Wall -o

# Extra flags for release and debug modes
RLS = -O2
DBG = -g

# The location of our source code
CODE = Code

# The location of our compiled .o files
OBJS = $(CODE)/obj

release: COMP += $(RLS)
release: build

debug: COMP += $(DBG)
debug: build

tests: COMP += $(DBG)
tests: Tests.o
	$(LINK) Testing $(OBJS)/Tests.o

build: Main.o
	$(LINK) BigNum $(OBJS)/Main.o

%.o: $(CODE)/%.cpp
	$(COMP) $(CODE)/$*.cpp -o $(OBJS)/$*.o

clean:
	@echo Cleaning binaries
	@rm -f $(OBJS)/* BigNum Testing