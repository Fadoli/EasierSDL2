CC= g++ -std=c++14
DBG= gdb

DFLAGS= -g
OFLAGS= -O3
CPPFLAGS= -Wall -Wextra -fpermissive $(OFLAGS)

TARGET= sdl2.bin

DIR1= src
DIR2= lib
LIBS= -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -llua5.2

SOURCES = $(wildcard $(DIR1)/*.cpp) $(wildcard $(DIR2)/*.cpp)

## End sources definition
INCLUDE= -Ilib/
## end more includes

OBJECTS= $(addprefix obj/, $(notdir $(SOURCES:.cpp=.o)))

## Default rule executed
all: obj_setup $(TARGET)

obj_setup:
	mkdir -p ./obj

obj_clean:
	rm -rf ./obj
## Clean Rule
clean:
	rm -f $(TARGET) $(OBJECTS)

## Debug
debug: $(TARGET)
	$(DBG) ./$(TARGET)

## Rule for making the actual target
$(TARGET): $(OBJECTS)
	@-echo "============="
	@-echo "Linking the target $@"
	@-echo "============="
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@-echo "-- Link finished --"

# create a list of auto dependencies
AUTODEPS:= $(OBJECTS:.o=.d)
# include by auto dependencies
-include $(AUTODEPS)

obj/%.d: */%.cpp
	$(CC) -M $(CPPFLAGS) $< $(INCLUDE) > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.*; \
	echo $@ | tr '.d' '.o' | xargs rm -f 

# compile and generate dependency 
obj/%.o: */%.cpp
	@# Build
	$(CC) $(CPPFLAGS) -c -o $@ $< $(INCLUDE)
