SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib
LIB_HFD_DIR := $(LIB_DIR)/libhfd
LIB_RADIAL_DIR := $(LIB_DIR)/libradial
LIB_DSYGV_DIR := $(LIB_DIR)/libdsygv
LIB_AMOS644_DIR := $(LIB_DIR)/libamos644

NAME := bmudecay

EXE := $(BIN_DIR)/$(NAME)
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC       := gcc
FC       := gfortran
CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := 
# LDFLAGS  := -L/usr/local/lib/ -L./lib/libamos644 -L./lib/libdsygv -L./lib/libradial -L./lib/libhfd -I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/Accelerate.framework/Versions/Current/Frameworks/vecLib.framework/Headers/
LDFLAGS  := -L/usr/local/lib/ -L./lib/libamos644 -L./lib/libdsygv -L./lib/libradial -L./lib/libhfd
# LDLIBS   := -lblas -lamos644 -ldsygv -lhfd -lradial -lm -lgfortran -framework Accelerate
# LDLIBS   := -framework Accelerate -lamos644 -ldsygv -lhfd -lradial -lm -lgfortran 
LDLIBS   := -lblas -lamos644 -ldsygv -lhfd -lradial -lm -lgfortran

export

.PHONY: all clean clean_all

all: $(EXE) 

libs: libhfd.a libradial.a libdsygv.a libamos644.a

libhfd.a:
	make -C $(LIB_HFD_DIR)

libradial.a:
	make -C $(LIB_RADIAL_DIR)

libdsygv.a:
	make -C $(LIB_DSYGV_DIR)

libamos644.a:
	make -C $(LIB_AMOS644_DIR)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

clean_all:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)
	@$(RM) -v $(LIB_HFD_DIR)/*.o $(LIB_HFD_DIR)/*.a
	@$(RM) -v $(LIB_RADIAL_DIR)/*.o $(LIB_RADIAL_DIR)/*.a $(LIB_RADIAL_DIR)/*.mod
	@$(RM) -v $(LIB_DSYGV_DIR)/*.o $(LIB_DSYGV_DIR)/*.a
	@$(RM) -v $(LIB_AMOS644_DIR)/*.o $(LIB_AMOS644_DIR)/*.a
