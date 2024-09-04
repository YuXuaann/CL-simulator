#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#define GREEN printf("\e[32m")
#define RED printf("\e[31m")
#define RESET printf("\e[0m")

#define debugprint(x) printf("%s = %lx\n", #x, x);

#define debugtimeprint(time, ...)                                            \
	time & 1 ? printf("\e[32m[%4sps]\e[0m: ", std::to_string(time).c_str())  \
	         : printf("\e[31m[%4sps]\e[0m: ", std::to_string(time).c_str()); \
	printf("%s = %lx\n", #__VA_ARGS__, __VA_ARGS__);

#define debugtime(time)                                                       \
	time & 1 ? printf("\e[32m[%4sps]\e[0m: \n", std::to_string(time).c_str()) \
	         : printf("\e[31m[%4sps]\e[0m: \n", std::to_string(time).c_str());

#endif