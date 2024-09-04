#ifndef MEM_HPP
#define MEM_HPP

#include "dev.hpp"
#include <fstream>
#include <map>

class memory : public dev {
private:
	std::map<uint64_t, uint8_t> mem;

public:
	memory() {}
	~memory() {}

	// LoongArch32R 的指令起始地址为1c00_0000
	void read_bin(const char* file, uint64_t start_addr = 0x1c000000) {
		FILE* fp = fopen(file, "rb");

		if (fp == NULL) {
			printf("\e[31mbin文件路径错误, Error: cannot open file %s\e[0m\n", file);
			exit(1);
		}

		fseek(fp, 0, SEEK_END);
		uint64_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		uint8_t* buf = new uint8_t[size];
		fread(buf, 1, size, fp);
		fclose(fp);

		GREEN;
		printf("bin文件路径正确\n");
		printf("bin文件size = %lx\n", size);
		RESET;

		for (uint64_t i = 0; i < size; i++) {
			mem[start_addr + i] = buf[i];
		}
		delete buf;
	}

	uint64_t read(uint64_t addr, uint8_t size) {
#ifdef DEBUG
		// printf("read addr = %04lx, size = %x\n", addr, size);
		static uint64_t count = 0;
#endif
		uint64_t data = 0;
		for (uint8_t i = 0; i < size; i++) {
			data |= mem[addr + i] << (i * 8);
		}
#ifdef DEBUG
		// data = count++;
#endif
		return data;
	}

	void write(uint64_t addr, uint8_t size, uint64_t data) {
#ifdef DEBUG
		// printf("write addr = %04lx, size = %04x\n", addr, size);
#endif
		for (uint8_t i = 0; i < size; i++) {
			mem[addr + i] = (data >> (i * 8)) & 0xff;
		}
	}
};

#endif