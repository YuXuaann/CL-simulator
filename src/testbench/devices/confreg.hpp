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

const std::vector<uint64_t> led_addr = {LED_ADDR, LED_RG0_ADDR};
const uint8_t led_size = 0x10;

class CR : public confreg {
private:
    std::map<uint64_t, uint8_t> mem;

public:
    const std::vector<uint64_t> cr_addr = {CR0_ADDR, CR1_ADDR, CR2_ADDR, CR3_ADDR, CR4_ADDR, CR5_ADDR, CR6_ADDR, CR7_ADDR};
    const uint8_t cr_size = 0x10;
    CR() :
        start_addr(cr_addr[0]), size(cr_size) {
    }
    uint64_t read(uint64_t addr, uint8_t size) override {
        if (size != 0x4) {
            RED;
            printf("Error! size is not 4\n");
            RESET;
            exit(1);
        }
    }
    void write(uint64_t addr, uint8_t size, uint64_t data) override {
        return;
    }
};

#endif