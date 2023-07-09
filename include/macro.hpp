#ifndef MACRO_H
# define MACRO_H

// common use 
# define NOT_DEFINED -1

// for main
# define TOTAL_KEV 128
# define BUFF 600

// for method (for config and request)
#define METHOD_FLAG_0 (1<<0) // 0000 0000 0000 0001  -> GET
#define METHOD_FLAG_1 (1<<1) // 0000 0000 0000 0010  -> POST
#define METHOD_FLAG_2 (1<<2) // 0000 0000 0000 0100  -> DELETE


#endif //MACRO_H
