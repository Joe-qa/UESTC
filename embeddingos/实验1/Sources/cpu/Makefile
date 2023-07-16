

include  $(RTOS_PATH)/bsps/v_makefile.mk

include  $(RTOS_PATH)/base.make


SRC_PATHS = .
SRC_FILES = $(notdir ${wildcard $(patsubst %, %/*.S, $(SRC_PATHS))}  \
             ${wildcard $(patsubst %, %/*.c, $(SRC_PATHS))})

SRC_OBJS = $(patsubst %.S, %.o, $(patsubst %.c, %.o, $(SRC_FILES)))


EXTRA_CFLAGS = $(DEFAULT_SEARCH_PATH)



VPATH = ${SRC_PATHS}


all: ${SRC_OBJS}


clean:
	rm -rf ${SRC_OBJS}

include $(RTOS_PATH)/Rules.make

