#ifndef HOOKIT_FUNC_H
#define HOOKIT_FUNC_H
#include <stdint.h>
// export API
typedef int8_t	  i8;
typedef int16_t	  i16;
typedef int32_t	  i32;
typedef int64_t	  i64;
typedef uint8_t	  u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef	uint64_t  u64;

#define HOOKIT_FUNC_IS_HOOKED 1

#ifdef ARCH_X86_64
#define HOOKIT_FUNC_PUSH 0x55
#define BASELEN 14
u8 code[6] = {0xFF, 0x25, 0x00000000}; // jmp [rip]
#endif

i8 hookit_func(void*, void*);
i8 unhookit_func(void*);
#endif