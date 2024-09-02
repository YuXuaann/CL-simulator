# 配置----------------------------------------------------------------------------------
TOP = alu
WAVENAME = waveform

# 配置结束------------------------------------------------------------------------------
VFLAGS = --cc --trace-fst --trace-structs -Wall --build -j `nproc`
DUT_SRC_DIR = ./src/dut-core/test
SOURCE_FILE = $(shell find $(DUT_SRC_DIR) -name '*.svh') \
				$(shell find $(DUT_SRC_DIR) -name '*.vh') \
				$(shell find $(DUT_SRC_DIR) -name '*.v') \
				$(shell find $(DUT_SRC_DIR) -name '*.sv')
SOURCE_DIR = $(addprefix -I, $(shell find $(DUT_SRC_DIR) -type d))
TESTBENCH = ./src/testbench/test/alu_tb.cpp
OBJ_DIR = ./compile
WAVE_DIR = ./wave

SHELL := /bin/bash
.PHONY: all clean help view

all: ${OBJ_DIR} ${WAVE_DIR}
	rm -rf ${OBJ_DIR}

${OBJ_DIR}: ${SOURCE_FILE} ${TESTBENCH} 
	verilator ${VFLAGS} \
	--Mdir $(OBJ_DIR) \
	--top $(TOP) $(SOURCE_FILE) ${SOURCE_DIR} \
	--exe ${TESTBENCH} 

${WAVE_DIR}: ${OBJ_DIR}
	mkdir -p ${WAVE_DIR}
	make -C ${OBJ_DIR} -f V$(TOP).mk V$(TOP) -j `nproc`
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