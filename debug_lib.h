#ifndef DEBUG_LIB_H

#define DEBUG_LIB_H

#include <stdio.h>

#define PRINT_LINE {printf("I'm at %s at line %d in %s\n", __PRETTY_FUNCTION__, __LINE__, __FILE__); fflush(stdout);}

#define PRINT_PTR(ptr) printf("pointer %s at %p at line %d\n", #ptr, ptr, __LINE__);

#define PRINT(str) printf("%s\n", #str);

#define PRINT_UL(num) printf("[%s:%lu] %s = %lu\n", __PRETTY_FUNCTION__, __LINE__, #num, num);

#endif
