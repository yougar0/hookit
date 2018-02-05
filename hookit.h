#ifndef HOOKIT_H
#define HOOKIT_H
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

typedef u32		  hookit_offset;
typedef	u64		  hookit_addr;

typedef struct {
  u8* parent;
  u8* target;
  u8* player;
  u8* patch_point;
  hookit_offset target_original_offset;
  hookit_offset player_offset;
}hookit_entry;
  
hookit_entry* hookit_init(void*, void*, void*);
i8 hookit(hookit_entry*);  
i8 unhookit(void*, void*);
i8 unhookall();
#endif
