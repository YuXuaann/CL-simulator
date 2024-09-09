# 配置----------------------------------------------------------------------------------


TOP = mycpu_top
# todo: 与testbench中的文件名同步
WAVENAME = waveform



# 配置结束------------------------------------------------------------------------------
UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        NPROC = $(shell nproc)
    endif
    ifeq ($(UNAME_S),Darwin)
        NPROC = $(shell sysctl -n hw.ncpu)
    endif
VFLAGS = --cc --trace-fst --trace-structs -Wall --build -j ${NPROC} -Wno-fatal
DUT_SRC_DIR = ./src/ref-core/NoAXI-core
SOURCE_FILE = $(shell find $(DUT_SRC_DIR) -name '*.svh') \
				$(shell find $(DUT_SRC_DIR) -name '*.vh') \
				$(shell find $(DUT_SRC_DIR) -name '*.v') \
				$(shell find $(DUT_SRC_DIR) -name '*.sv')
SOURCE_DIR = $(addprefix -I, $(shell find $(DUT_SRC_DIR) -type d))
TESTBENCH_DIR = ./src/testbench
OBJ_DIR = ./compile
WAVE_DIR = ./wave

SHELL := /bin/bash
.PHONY: all clean help view

all: ${OBJ_DIR} ${WAVE_DIR}

${OBJ_DIR}: ${SOURCE_FILE} ${TESTBENCH_DIR} 
	verilator ${VFLAGS} \
	--Mdir $(OBJ_DIR) \
	--top $(TOP) $(SOURCE_FILE) ${SOURCE_DIR} \
	--exe ${TESTBENCH_DIR}/testbench.cpp

${WAVE_DIR}: ${OBJ_DIR}
	mkdir -p ${WAVE_DIR}
	make -C ${OBJ_DIR} -f V$(TOP).mk V$(TOP) -j ${NPROC}
	${OBJ_DIR}/V$(TOP)

view:
	gtkwave ${WAVE_DIR}/${WAVENAME}.fst *.gtkw

clean:
	rm -rf ${OBJ_DIR}
	rm -rf ${WAVE_DIR}

help:
	@echo -e "\e[33m  Usage: make [ all | view | clean | compile | wave | help ]\e[0m"
	@echo
	@echo -e "(\e[32mdefault\e[0m)all: Compile *.sv and run the testbench, then generate wave"
	@echo -e "        view: View waveform"
	@echo -e "       clean: Clean the generated files"
	@echo
	@echo -e "     compile: Compile *.sv and run the testbench"
	@echo -e "        wave: Generate wave"
	@echo -e "        help: Show this help message\e[0m"
	@echo
	@echo -e "\e[32mtips: Remember to press "ctrl + S" when you are viewing waveform !! \e[0m"