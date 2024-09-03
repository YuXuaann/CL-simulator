#ifndef TOP_NAME
#define TOP_NAME "mycpu_top" // todo:顶层模块名，未来统一成可设定项
#endif

#define TRACE_ON // todo:是否开启波形文件，未来统一成可设定项

#ifndef WAVE_NAME
#define WAVE_NAME "./wave/waveform.fst" // todo:波形文件路径，未来统一成可设定项
#endif

#ifndef MAX_SIM_TIME
#define MAX_SIM_TIME 1000 // todo:仿真总时钟边沿数，未来统一成可设定项
#endif

#ifndef RESET_TIME
#define RESET_TIME 10 // todo:复位时间，未来统一成可设定项
#endif

#include "Vmycpu_top.h" // todo: 通过TOP_NAME生成
#include "axi/axi.hpp"
#include "devices/mem.hpp"
#include <iostream>
#include <stdlib.h>
#include <verilated.h>
#include <verilated_fst_c.h>

typedef Vmycpu_top dut; // todo: 通过TOP_NAME生成
typedef VerilatedFstC trace;

void sim_step(dut* dut, int& reset_time) {
	static dev* mem = new memory;
	static simpleAXI<>* axi = new simpleAXI<>;

	dut->ext_int = 0;
	dut->aclk ^= 1;

	if (reset_time) {
		--reset_time;
		dut->aresetn = 0;
		axi->aresetn = 0;
		dut->eval();
		return;
	}

	dut->aresetn = 1;
	axi->aresetn = 1;

	axi->arid = dut->arid;
	axi->araddr = dut->araddr;
	axi->arlen = dut->arlen;
	axi->arsize = dut->arsize;
	axi->arvalid = dut->arvalid;
	dut->arready = axi->arready;

	dut->rid = axi->rid;
	dut->rdata = axi->rdata;
	dut->rresp = 0;
	dut->rlast = axi->rlast;
	dut->rvalid = axi->rvalid;
	axi->rready = dut->rready;

	axi->awid = dut->awid;
	axi->awaddr = dut->awaddr;
	axi->awlen = dut->awlen;
	axi->awsize = dut->awsize;
	axi->awvalid = dut->awvalid;
	dut->awready = axi->awready;

	axi->wid = dut->wid;
	axi->wdata = dut->wdata;
	axi->wstrb = dut->wstrb;
	axi->wlast = dut->wlast;
	axi->wvalid = dut->wvalid;
	dut->wready = axi->awready;

	dut->bid = axi->bid;
	dut->bresp = 0;
	dut->bvalid = axi->bvalid;
	axi->bready = dut->bready;

	axi->eval(mem);
	dut->eval();
}

void sim(dut* dut, trace* trace, int& reset_time) {
	for (int sim_time = 0; sim_time < MAX_SIM_TIME; sim_time++) {
		sim_step(dut, reset_time);
#ifdef TRACE_ON
		trace->dump(sim_time);
#endif
	}
}

int main(int argc, char** argv, char** env) {
	dut* cpu = new dut;
	trace* m_trace = new trace;
#ifdef TRACE_ON
	Verilated::traceEverOn(true);
	cpu->trace(m_trace, 5);
	m_trace->open(WAVE_NAME);
#endif
	int reset_time = RESET_TIME;
	sim(cpu, m_trace, reset_time);
	m_trace->close();
	delete cpu;
	exit(EXIT_SUCCESS);
}
