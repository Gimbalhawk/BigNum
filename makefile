
COMP = g++ -Wall -c
LINK = g++ -Wall -o

# The location of our source code
CODE = Code

# The location of our compiled .o files
OBJS = $(CODE)/obj

release: COMP += -O2
release: build

debug: COMP += -g
debug: build

build: tests Main.o
	$(LINK) BigNum $(OBJS)/Main.o

tests: Tests.o
	$(LINK) Testing $(OBJS)/Tests.o

%.o: $(CODE)/%.cpp
	$(COMP) $(CODE)/$*.cpp -o $(OBJS)/$*.o

clean:
	@echo Cleaning binaries
	@rm -f $(OBJS)/* BigNum Testing