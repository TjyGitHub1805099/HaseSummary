#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H
/*========================[I N C L U D E S]========================*/

/*========================[GLOBAL MACROS]========================*/

#define CPU_TYPE_8       8u
#define CPU_TYPE_16      16u
#define CPU_TYPE_32      32u

#define MSB_FIRST        0u    /* big endian bit ordering */
#define LSB_FIRST        1u    /* little endian bit ordering */

#define HIGH_BYTE_FIRST  0u    /* big endian byte ordering */
#define LOW_BYTE_FIRST   1u    /* little endian byte ordering */

#define CPU_TYPE         CPU_TYPE_32

#define CPU_BIT_ORDER    MSB_FIRST        /* big endian bit ordering */

#define CPU_BYTE_ORDER   HIGH_BYTE_FIRST  /* big endian byte ordering */

#ifndef TRUE
#define TRUE            1u
#endif

#ifndef FALSE
#define FALSE           0u
#endif

typedef unsigned char         boolean;      /*TRUE/FALSE*/
typedef signed char           sint8;         /*-128 .. +127*/
typedef unsigned char         uint8;         /*0 .. 255*/
typedef signed short          sint16;        /*-32768 .. +32767*/
typedef unsigned short        uint16;        /*0 .. 65535*/
typedef signed int            sint32;        /*-2147483648 .. +2147483647*/
typedef unsigned int          uint32;        /*0 .. 4294967295*/
typedef signed long long      sint64;
typedef unsigned long long    uint64;

typedef signed int            sint8_least;   /*At least 7 bit + 1 bit sign*/
typedef unsigned int          uint8_least;   /*At least 8 bit*/
typedef signed int            sint16_least;  /*At least 15 bit + 1 bit sign*/
typedef unsigned int          uint16_least;  /*At least 16 bit*/
typedef signed int            sint32_least;  /*At least 31 bit + 1 bit sign*/
typedef unsigned int          uint32_least;  /*At least 32 bit*/

typedef float                 float32;
typedef double                float64;

typedef unsigned long    clock_time;

#endif  /* PLATFORM_TYPES_H */
/*========END OF FILE: Platform_Types.h=========*/
