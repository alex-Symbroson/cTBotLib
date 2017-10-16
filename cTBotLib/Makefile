
MAIN = main

	# build, source, include directories
DIR_BUILD = build/
DIR_SRC   = src/
DIR_INC   = inc/

	# compiler
COMP = gcc

	# compiler flags
CFLAGS  = -I inc -Wall

	# linker flags
LDFLAGS  = -pthread -lcurl

	# clean extensions
FILES_CLEAN = *.o

	# output file name
ifeq ($(out),)
	OUTPUT = $(MAIN)
else
	OUTPUT = $(out)
endif

	# list of source files
SRC = $(wildcard $(foreach dir,$(DIR_SRC), $(dir)*.c))

	# list of header files
HDR = $(wildcard $(foreach dir,$(DIR_INC), $(dir)*.h))

	# list of object files
OBJ = $(patsubst $(DIR_SRC)%,$(DIR_BUILD)%,$(foreach source, \
  $(basename $(SRC)),$(source).o))

	# ignore implicit chain and therefore deletion of object files
.SECONDARY: $(OBJECTS_NORMAL)

	# define phony targets for make commands
.PHONY: all all_init clean run

all: all_init $(OUTPUT).out
	@printf "Compilation done - No Errors.\n"

all_init:
#	@printf "\n#### compiling project\n"
	mkdir -p $(DIR_BUILD)

$(OUTPUT).out: $(OBJ) $(MAIN).c
#	@printf "\n#### linking project\n"
	ld  -r -o lcTBotLib $(OBJ)
	$(COMP)    $(CFLAGS) -o $@ $(MAIN).c lcTBotLib $(LDFLAGS)

$(DIR_BUILD)%.o: $(DIR_SRC)%.c $(HDR)
#	@printf "\n#### compiling \"$@\" based on \"$^\"\n"
	$(COMP) -c $(CFLAGS) -o $@ $<

run: all
#	@printf "\n#### executing $(OUTPUT).out\n"
	@echo
	./$(OUTPUT).out

clean:
#	@printf "\n#### cleaning project\n"
	rm -f $(foreach file,$(FILES_CLEAN),$(DIR_BUILD)$(file)) *.out
