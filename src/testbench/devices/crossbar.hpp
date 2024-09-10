#ifndef CROSSBAR_HPP
#define CROSSBAR_HPP

#include "dev.hpp"
#include "confreg.hpp"
#include "mem.hpp"
#include <vector>

class crossbar : public dev {
private:
    std::vector<confreg *> devices;
    memory *mem_space;

public:
    crossbar(std::vector<confreg *> devices, memory *mem_space) :
        mem_space(mem_space),
        devices(devices) {
    }
    ~crossbar() {
        std::vector<confreg *>().swap(devices);
    }
    uint64_t read(uint64_t addr, uint8_t size) override {
        for (auto device : devices) {
            if (addr >= device->start_addr && addr < device->start_addr + device->size) {
                GREEN;
                printf("Confreg Read Address: %lx\n", addr);
                RESET;
                return device->read(addr, size);
            }
        }
        return mem_space->read(addr, size);
    }
    void write(uint64_t addr, uint8_t size, uint64_t data) override {
        for (auto device : devices) {
            if (addr >= device->start_addr && addr < device->start_addr + device->size) {
                GREEN;
                printf("Confreg Write Address: %lx\n", addr);
                RESET;
                return device->write(addr, size, data);
            }
        }
        return mem_space->write(addr, size, data);
    }
};

#endif