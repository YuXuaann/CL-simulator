#ifndef AXI_HPP
#define AXI_HPP

// #define RANDOM
#define RANDOM_MAX 20
#define WORK_RANDOM_MAX 4
#define WAIT_TIME 5

#include "../config/config.hpp"
#include "../devices/dev.hpp"

enum read_state { AR,
                  R };
enum write_state { AW,
                   W,
                   B };

// todo: 可变长度变量类型自适应
// 仅保留最基本信息的AXI接口，忽略了burst、lock、cache、prot、resp信号
template <uint64_t ADDR_WIDTH = 64, uint64_t DATA_WIDTH = 64, uint64_t ID_WIDTH = 4>
class simpleAXI {
    static_assert(__builtin_popcount(DATA_WIDTH) == 1, "DATA_WIDTH must be a power of 2");
    static_assert(DATA_WIDTH >= 8, "DATA_WIDTH must be greater than 8");

private:
    read_state rstate;
    write_state wstate;

public:
    // 不在此维护aclk
    // bool aclk;    // Output
    bool aresetn; // Input

    // aw
    uint8_t awid;    // Input
    uint64_t awaddr; // Input
    uint8_t awlen;   // Input
    uint8_t awsize;  // Input
    bool awvalid;    // Input
    bool awready;    // Output

    // w
    uint8_t wid;    // Input
    uint64_t wdata; // Input
    uint8_t wstrb;  // Input
    bool wlast;     // Input
    bool wvalid;    // Input
    bool wready;    // Output

    // b
    uint8_t bid; // Output
    bool bvalid; // Output
    bool bready; // Input

    // ar
    uint8_t arid;    // Input
    uint64_t araddr; // Input
    uint8_t arlen;   // Input
    uint8_t arsize;  // Input
    bool arvalid;    // Input
    bool arready;    // Output

    // r
    uint8_t rid;    // Output
    uint64_t rdata; // Output
    bool rlast;     // Output
    bool rvalid;    // Output
    bool rready;    // Input

    simpleAXI() :
        rstate(AR), wstate(AW), aresetn(false), awid(0), awaddr(0), awlen(0), awsize(0), awvalid(false), awready(false), wid(0), wdata(0), wstrb(0), wlast(false), wvalid(false), wready(false), bid(0), bvalid(false), bready(false), arid(0), araddr(0), arlen(0), arsize(0), arvalid(false), arready(false), rid(0), rdata(0), rlast(false), rvalid(false), rready(false) {
    }
    ~simpleAXI() {};
    void eval(dev *dev);
};

template <uint64_t ADDR_WIDTH, uint64_t DATA_WIDTH, uint64_t ID_WIDTH>
void simpleAXI<ADDR_WIDTH, DATA_WIDTH, ID_WIDTH>::eval(dev *dev) {
    static int r_wait_time = 0;
    static int r_count = 0;
    static int r_len = 0;
    static int r_size = 0;
    static int r_id = 0;
    static std::vector<uint64_t> r_data(8, 0);

    static int w_wait_time = 0;
    static int w_count = 0;
    static int w_len = 0;
    static int w_size = 0;
    static int w_id = 0;
    static uint64_t w_addr = 0;
    static std::vector<uint64_t> w_data(8, 0);

    if (aresetn == false) {
        awready = false;
        wready = false;
        bvalid = false;
        arready = false;
        rvalid = false;
        rstate = AR;
        wstate = AW;
        return;
    }

    // read state machine
    switch (rstate) {
    case AR:
#ifdef DEBUG
        // printf("read_state = AR\n");
#endif
        arready = true;
        rid = 0;
        rdata = 0;
        rlast = false;
        rvalid = false;
        if (arvalid) {
            rstate = R;
            r_count = 0;
            r_len = arlen;
            r_size = arsize;
            r_id = arid;
            for (int i = 0; i <= arlen; i++) {
                r_data[i] = dev->read(araddr + i * (1 << arsize), (1 << arsize));
            }
#ifdef RANDOM
            r_wait_time = rand() % RANDOM_MAX;
#else
            r_wait_time = WAIT_TIME;
#endif
        }
        break;

    case R:
#ifdef DEBUG
        // printf("read_state = R\n");
        // for (int i = 0; i <= r_len; i++) {
        // 	printf("r_data[%d] = %08lx\n", i, r_data[i]);
        // }
#endif
        arready = false;
        if (r_wait_time) {
            rvalid = false;
            --r_wait_time;
        } else {
            if (rready) {
                rid = r_id;
                rdata = r_data[r_count];
                rlast = r_count == r_len;
                rvalid = true;
                ++r_count;
                r_wait_time = rand() % WORK_RANDOM_MAX;
                if (rlast)
                    rstate = AR;
            }
        }
        break;
    }

    // write state machine
    switch (wstate) {
    case AW:
#ifdef DEBUG
        // printf("write_state = AW\n");
#endif
        awready = true;
        wready = false;
        bid = 0;
        bvalid = false;
        if (awvalid) {
            wstate = W;
            w_addr = awaddr;
            w_count = 0;
            w_len = awlen;
            w_size = awsize;
            w_id = awid;
#ifdef RANDOM
            w_wait_time = rand() % RANDOM_MAX;
#else
            w_wait_time = WAIT_TIME;
#endif
        }
        break;

    case W:
#ifdef DEBUG
        // printf("write_state = W\n");
#endif
        awready = false;
        if (w_wait_time) {
            wready = false;
            --w_wait_time;
        } else {
            wready = true;
            // todo: 暂定写操作没有突发
            if (wvalid && w_id == wid) {
                dev->write(w_addr + w_count * (1 << w_size), (1 << w_size), wdata);
#ifdef DEBUG
                printf("write data: %lx\n", dev->read(w_addr + w_count * (1 << w_size), (1 << w_size)));
#endif
                w_wait_time = rand() % WORK_RANDOM_MAX;
                ++w_count;
                if (wlast) {
                    if (w_count != w_len + 1) {
                        RED;
                        printf("写入数据长度错误\n");
                        RESET;
                        exit(1);
                    }
                    wstate = B;
                }
            }
        }
        break;

    case B:
#ifdef DEBUG
        // printf("write_state = B\n");
#endif
        wready = false;
        if (w_wait_time) {
            bvalid = false;
            --w_wait_time;
        } else {
            awready = false;
            wready = false;
            if (bready) {
                bid = w_id;
                bvalid = true;
                wstate = AW;
            }
        }
        break;
    }
}
#endif