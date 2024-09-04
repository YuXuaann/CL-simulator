#ifndef DEV_HPP
#define DEV_HPP

#include "../config/config.hpp"

class dev {
public:
	virtual uint64_t read(uint64_t addr, uint8_t size) = 0;
	virtual void write(uint64_t addr, uint8_t size, uint64_t data) = 0;
};

#endif