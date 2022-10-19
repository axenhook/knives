

#ifndef _OS_TYPES_H_
#define _OS_TYPES_H_

#ifdef	__cplusplus
extern "C" {
#endif

//==========================================================
// Complier options
//  1. System options
//  2. User defined options
//==========================================================
//#pragma pack(1)  // Aligned by 1 byte
//#pragma pack()  // Use the default alignment

//==========================================================
// Basic types
//==========================================================
typedef unsigned int         bool_t;

typedef unsigned char        uint8_t;   // unsigned 8-bit
typedef unsigned short       uint16_t;  // unsigned 16-bit
typedef unsigned int         uint32_t;  // unsigned 32-bit

typedef short                int16_t;  // signed 16-bit
typedef int                  int32_t;  // signed 32-bit

#ifdef WIN32
typedef char                 int8_t;   // signed 8-bit
typedef unsigned long long       ptr_t;  // pointer
typedef long long            int64_t;  // signed 64-bit
typedef unsigned long long   u64_t;  // unsigned 64-bit
#else
typedef unsigned long            ptr_t;  // pointer
typedef unsigned long long   u64_t;  // unsigned 64-bit

#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifdef	__cplusplus
}
#endif

#endif

