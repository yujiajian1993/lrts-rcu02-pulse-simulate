#ifndef  _UNIFW_H
#define  _UNIFW_H

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @file unifw.h
 * @brief define public header for UNIfied FrameWork
 * @author 003
 * @date 2016-05-25
 *
 * @Log see readme
 *
 * $Id$
 */
#include "unifw_errno.h"

/* for Windows and Linux, using stardard header files */

#if defined(_WINDOWS)
#include <windows.h>
#include <process.h>

#elif defined(__linux)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* for using socket and threads */
typedef int SOCKET;
typedef int HANDLE;
typedef struct sockaddr SOCKADDR;

#define MAX_PATH 1024

#define _getwch getchar
#define _putwch putchar
#define _putch(c) write(1, &c, 1)
#else
#include "arch_config.h"
#endif

#if defined(_WINDOWS) || defined(__linux) || defined(POSIX)
#include <stdint.h>
#include <stdio.h>
#ifdef _WINDOWS
#include <conio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define EnableInterrupts
#define DisableInterrupts

#define printp printf
#define STATIC

#else
#ifndef _STDINT
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef signed   long long int64_t ;
#endif
#endif

/** 定义通用的宏 */

#ifndef FAR
#define FAR
#endif

#ifndef INTERRUPT
#define INTERRUPT
#endif

#ifndef TRUE
#define TRUE 1U
#endif

#ifndef FALSE
#define FALSE 0U
#endif

#ifndef NULL
#define NULL (void *)(0)
#endif

#ifndef MAIN
#define MAIN main
#endif

#ifndef STATIC
#define STATIC static
#endif

#ifndef TICK_BASE
#if defined(_WINDOWS) || defined(__linux)
#define TICK_BASE 100	/**< if not defined, default to 100 ms on OS */
#else
#define TICK_BASE 10	/**< if not defined, default to 10ms on RTOS */
#endif
#endif

typedef int16_t fsm_t;

/** 用于描述模块 for module descriptor */
typedef struct _mod_dscrp
{
	char     name[15]   ;	/**< 模块名称 */
	char     nzero      ;	/**< 填充 */
	char     scmrev[15] ;	/**< 版本字符 */
	char     scmzero    ;	/**< 填充 */
	char     time[31]   ;	/**< 编译时间 */
	char     tzero      ;	/**< 填充 */
	uint8_t  major      ;	/**< 主要版本号 */
	uint8_t  minor      ;	/**< 次要版本号 */
	uint16_t patch      ;	/**< 修订版本号 */
}mod_dscrp_t            ;

/**
 * @brief 数据配置项
 */
typedef struct data_item
{
	char 	*name ; /**< 比较字符串 */
	int16_t width ; /**< 数据的size，正数为无符号数，负数为有符号数 */
	void	*ptr  ; /**< 通用的目标指针 */
}data_item_t      ;

void unit_print_module(const mod_dscrp_t *mod); /*-< supply by unitframe */

/*=== public definitions ===*/

/** 获取消息源ID */
#define get_msg_src(msg) (msg/256U)
/** 获取消息类型 */
#define get_msg_type(msg) (msg%256U)
/** 获取消息长度 */
#define get_msg_len(msg) (msg%256U)
/** 生成消息 */
#define make_msg(src, type) ((uint16_t)((uint16_t)src*256U+type))
/** 生成基本消息 */
#define make_base_msg(base, value) (base+value)

/** 表格变量的名称 */
#define _TAB(n) n##_table
/** 表格长度变量的名称 */
#define _TAB_L(n) n##_table_len
/** 定义表格的长度 */
#define DEF_TAB_L(n) const uint16_t _TAB_L(n)=sizeof(_TAB(n))/sizeof(_TAB(n)[0])
/** 声明表格长度和数组 */
#define DCLR_TAB(n, t) extern const t _TAB(n)[]; extern const uint16_t _TAB_L(n)

/** m处掩码是否设置 */
#define ismask(d, m) (((d)&(m))==(m))

/** 转换为字节数组*/
#define tobytearr(d,n) (((uint8_t*)(&(d)))[n])
/** 转换为双字数组*/
#define towordarr(d,n) (((uint16_t*)(&(d)))[n])
/** 转换为长整数组*/
#define tolongarr(d,n) (((uint32_t*)(&(d)))[n])

/** 把指针转换为字节数组*/
#define ptobytearr(d,n) (((uint8_t*)(d))[n])
/** 把指针转换为双字数组*/
#define ptowordarr(d,n) (((uint16_t*)(d))[n])
/** 把指针转换为长整数组*/
#define ptolongarr(d,n) (((uint32_t*)(d))[n])

/** 转换为字节*/
#define tobyte(d) tobytearr(d,0)
/** 转换为双字*/
#define toword(d) towordarr(d,0)
/** 转换为长整*/
#define tolong(d) tolongarr(d,0)
/** 转换为指定类型的指针*/
#define totype(d, t) ((t*)(&(d)))

/** CPPUNIT 简化整型比较 */
#define CPPUNIT_EASSERT(a,b) CPPUNIT_ASSERT_EQUAL((int)a, (int)b)

/*- 断言宏定义 */
#if defined(_WINDOWS)
#define UNIFW_ASSERT(cond)  /*-< windows下单元测试断言为空 */
#define UNIFW_SAFETY_CRASH
#elif defined(__linux)
#define UNIFW_ASSERT(cond)  /*-< windows下单元测试断言为空 */
#define UNIFW_SAFETY_CRASH
#else
/*- 目标机下断言非真直接死机 */

extern void safety_crash_handler(void); /*-< 安全宕机函数, 由目标机其它文件实现 */

#define UNIFW_ASSERT(cond)  if (!(cond)) \
							{ \
								printp("FATAL:  ----------------UNIFW_ASSERT ERROR at %s %d------------------\r\n", \
                                __FILE__, __LINE__);\
								safety_crash_handler();\
							}

#define UNIFW_SAFETY_CRASH  { \
								printp("FATAL:  ----------------UNIFW_SAFETY_CRASH at %s %d------------------\r\n", \
                                __FILE__, __LINE__);\
								safety_crash_handler();\
							}

#define UNIFW_REASON_CRASH(fmt, ...)  { \
		                        printp(fmt, ##__VA_ARGS__); \
								printp("FATAL:  ----------------UNIFW_SAFETY_CRASH at %s %d------------------\r\n", \
                                __FILE__, __LINE__);\
								safety_crash_handler();\
							}

#endif

#ifdef __cplusplus
}
#endif

#endif   /*_UNIFW_H*/

