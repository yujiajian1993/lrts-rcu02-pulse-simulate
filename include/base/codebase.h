#ifndef  __CODEBASE_H
#define  __CODEBASE_H
#include "unifw.h"
#ifdef __cplusplus
extern "C"{
#endif

/**
 * @file codebase.h
 * @brief code base definition for framework
 * @author 
 * @version 
 * @date 2016-05-07
 */

/*- 外部变量 */
extern const mod_dscrp_t ver_codebase;
extern const uint32_t s_msb_num[16];

/*- 交换高低位 */
#define swap_w(n)           n=((n>>8)+(n<<8))

/*- cm/s转换为km/h */
#define tokmh(v) ((v)*9/250)
/** km/h转换为cm/s*/
#define tocms(v) ((v)*250/9)

#define SQRT_TBL_MAX_BITS    (15u)      /*-< 开方近似根最大位数 */

static uint16_t vstb_bsearch(uint32_t value, uint32_t const tbl[], uint16_t len);


uint32_t cb_vLshift32(uint32_t var, uint8_t r);
uint32_t cb_vRshift32(uint32_t var, uint8_t r);

uint16_t cb_sqrt(uint32_t data);

void cb_ptr_set          ( uint8_t *src, int16_t width, int32_t data                ) ;
void cb_ptr_set_index    ( uint8_t *src, int16_t width, int32_t data, uint16_t index ) ;
int32_t cb_ptr_get       ( uint8_t *src, int16_t width                              ) ;
int32_t cb_ptr_get_index ( uint8_t *src, int16_t width, uint16_t index               ) ;

void usr_memcpy(void *to, const void *from, uint32_t size  ) ;
void usr_memsafecpy(void *to, const void *from, uint32_t size);
void usr_memset(void *to, uint8_t val, uint32_t size ) ;


uint8_t min_u8(uint8_t a, uint8_t b);
uint16_t min_u16(uint16_t a, uint16_t b);
uint32_t min_u32(uint32_t a, uint32_t b);
int8_t min_i8(int8_t a, int8_t b);
int16_t min_i16(int16_t a, int16_t b);
int32_t min_i32(int32_t a, int32_t b);
uint8_t max_u8(uint8_t a, uint8_t b);
uint16_t max_u16(uint16_t a, uint16_t b);
uint32_t max_u32(uint32_t a, uint32_t b);
int8_t max_i8(int8_t a, int8_t b);
int16_t max_i16(int16_t a, int16_t b);
int32_t max_i32(int32_t a, int32_t b);
uint8_t abs_value_i8(int8_t a);
uint16_t abs_value_i16(int16_t a);
uint32_t abs_value_i32(int32_t a);
uint8_t abs_value_u8(uint8_t a);
uint16_t abs_value_u16(uint16_t a);
uint32_t abs_value_u32(uint32_t a);
uint8_t diff_value_u8(uint8_t a, uint8_t b);
uint16_t diff_value_u16(uint16_t a, uint16_t b);
uint32_t diff_value_u32(uint32_t a, uint32_t b);
int8_t diff_value_i8(int8_t a, int8_t b);
int16_t diff_value_i16(int16_t a, int16_t b);
int32_t diff_value_i32(int32_t a, int32_t b);

int32_t cb_atoi(char * str, int8_t *perr);

#ifdef __cplusplus
}
#endif

#endif /*__CODEBASE_H*/
