#pragma once

#include "../config/config.hpp"

class dev {
private:
	bool cached;
	uint64_t L, R;

public:
	virtual uint64_t read(uint64_t addr, uint8_t size) = 0;
	virtual void write(uint64_t addr, uint8_t size, uint64_t data) = 0;
};
