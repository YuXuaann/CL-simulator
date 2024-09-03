#include "dev.hpp"
#include <map>

class memory : public dev {
private:
	std::map<uint64_t, uint8_t> mem;

public:
	uint64_t read(uint64_t addr, uint8_t size) {
#ifdef DEBUG
		printf("read addr = %04x, size = %04x\n", addr, size);
		static uint64_t count = 0;
#endif
		uint64_t data = 0;
		for (uint8_t i = 0; i < size; i++) {
			data |= mem[addr + i] << (i * 8);
		}
#ifdef DEBUG
		data = count++;
#endif
		return data;
	}

	void write(uint64_t addr, uint8_t size, uint64_t data) {
		for (uint8_t i = 0; i < size; i++) {
			mem[addr + i] = (data >> (i * 8)) & 0xff;
		}
	}
};