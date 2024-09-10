#ifndef COFREG_HPP
#define COFREG_HPP

#include "dev.hpp"
#include <vector>
#include <map>

// nscscc下的定义
#define CR0_ADDR 0x8000          // 32'hbfaf_8000
#define CR1_ADDR 0x8010          // 32'hbfaf_8004
#define CR2_ADDR 0x8020          // 32'hbfaf_8008
#define CR3_ADDR 0x8030          // 32'hbfaf_800c
#define CR4_ADDR 0x8040          // 32'hbfaf_8010
#define CR5_ADDR 0x8050          // 32'hbfaf_8014
#define CR6_ADDR 0x8060          // 32'hbfaf_8018
#define CR7_ADDR 0x8070          // 32'hbfaf_801c
#define LED_ADDR 0xf020          // 32'hbfaf_f000
#define LED_RG0_ADDR 0xf030      // 32'hbfaf_f004
#define LED_RG1_ADDR 0xf040      // 32'hbfaf_f008
#define NUM_ADDR 0xf050          // 32'hbfaf_f010
#define SWITCH_ADDR 0xf060       // 32'hbfaf_f020
#define BTN_KEY_ADDR 0xf070      // 32'hbfaf_f024
#define BTN_STEP_ADDR 0xf080     // 32'hbfaf_f028
#define SW_INTER_ADDR 0xf090     // 32'hbfaf_f02c
#define TIMER_ADDR 0xe000        // 32'hbfaf_e000
#define IO_SIMU_ADDR 0xff00      // 32'hbfaf_ffec
#define VIRTUAL_UART_ADDR 0xff10 // 32'hbfaf_fff0
#define SIMU_FLAG_ADDR 0xff20    // 32'hbfaf_fff4
#define OPEN_TRACE_ADDR 0xff30   // 32'hbfaf_fff8
#define NUM_MONITOR_ADDR 0xff40  // 32'hbfaf_fffc

#define CONFREG_VECTOR                                                                                                \
    {new CR0(), new CR1(), new CR2(), new CR3(), new CR4(), new CR5(), new CR6(), new CR7(),                          \
     new LED(), new LED_RG0(), new LED_RG1(), new NUM(), new SWITCH(), new BTN_KEY(), new BTN_STEP(), new SW_INTER(), \
     new TIMER(), new IO_SIMU(), new VIRTUAL_UART(), new SIMU_FLAG(), new OPEN_TRACE(), new NUM_MONITOR()}

#define ERRORSIZE(x)                                         \
    if (x != 0x4) {                                          \
        RED;                                                 \
        printf("Error! confreg read/write size is not 4\n"); \
        RESET;                                               \
        exit(1);                                             \
    }

#define ERRORWRITE(x)                                  \
    RED;                                               \
    printf("Error! write to %s is not allowed\n", #x); \
    RESET;                                             \
    exit(1);

template <uint64_t START_ADDR, uint64_t SIZE>
class normal_RW_confreg : public confreg {
public:
    uint64_t mem;
    normal_RW_confreg() :
        confreg(START_ADDR, SIZE) {
        mem = 0;
    }
    uint64_t read(uint64_t addr, uint8_t size) override {
        ERRORSIZE(size);
        return mem;
    }
    void write(uint64_t addr, uint8_t size, uint64_t data) override {
        ERRORSIZE(size);
        mem = data;
        return;
    }
};

template <uint64_t START_ADDR, uint64_t SIZE>
class normal_R_confreg : public confreg {
public:
    uint64_t mem;
    normal_R_confreg() :
        confreg(START_ADDR, SIZE) {
        mem = 0;
    }
    uint64_t read(uint64_t addr, uint8_t size) override {
        ERRORSIZE(size);
        return mem;
    }
    void write(uint64_t addr, uint8_t size, uint64_t data) override {
        ERRORWRITE(normal_R_confreg);
    }
};

class CR0 : public normal_RW_confreg<CR0_ADDR, 0x4> {};
class CR1 : public normal_RW_confreg<CR1_ADDR, 0x4> {};
class CR2 : public normal_RW_confreg<CR2_ADDR, 0x4> {};
class CR3 : public normal_RW_confreg<CR3_ADDR, 0x4> {};
class CR4 : public normal_RW_confreg<CR4_ADDR, 0x4> {};
class CR5 : public normal_RW_confreg<CR5_ADDR, 0x4> {};
class CR6 : public normal_RW_confreg<CR6_ADDR, 0x4> {};
class CR7 : public normal_RW_confreg<CR7_ADDR, 0x4> {};
class LED : public normal_R_confreg<LED_ADDR, 0x4> {};
class LED_RG0 : public normal_R_confreg<LED_RG0_ADDR, 0x4> {};
class LED_RG1 : public normal_R_confreg<LED_RG1_ADDR, 0x4> {};
class NUM : public normal_RW_confreg<NUM_ADDR, 0x4> {};
class SWITCH : public normal_R_confreg<SWITCH_ADDR, 0x4> {};
class BTN_KEY : public normal_R_confreg<BTN_KEY_ADDR, 0x4> {
    uint64_t read(uint64_t addr, uint8_t size) override {
        ERRORSIZE(size);
        return 0;
    }
};
class BTN_STEP : public normal_R_confreg<BTN_STEP_ADDR, 0x4> {
    uint64_t read(uint64_t addr, uint8_t size) override {
        ERRORSIZE(size);
        return 0;
    }
};
class SW_INTER : public normal_R_confreg<SW_INTER_ADDR, 0x4> {};
class TIMER : public normal_RW_confreg<TIMER_ADDR, 0x4> {};
class IO_SIMU : public normal_RW_confreg<IO_SIMU_ADDR, 0x4> {
    void write(uint64_t addr, uint8_t size, uint64_t data) override {
        mem = ((data & 0xffff) << 16) | (data >> 16);
        return;
    }
};
class VIRTUAL_UART : public normal_RW_confreg<VIRTUAL_UART_ADDR, 0x4> {};
class SIMU_FLAG : public normal_RW_confreg<SIMU_FLAG_ADDR, 0x4> {};
class OPEN_TRACE : public normal_RW_confreg<OPEN_TRACE_ADDR, 0x4> {
    void write(uint64_t addr, uint8_t size, uint64_t data) override {
        mem = data != 0;
        return;
    }
};
class NUM_MONITOR : public normal_RW_confreg<NUM_MONITOR_ADDR, 0x4> {
    void write(uint64_t addr, uint8_t size, uint64_t data) override {
        mem = data & 1;
        return;
    }
};

#endif