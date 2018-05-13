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

/** ����ͨ�õĺ� */

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

/** ��������ģ�� for module descriptor */
typedef struct _mod_dscrp
{
	char     name[15]   ;	/**< ģ������ */
	char     nzero      ;	/**< ��� */
	char     scmrev[15] ;	/**< �汾�ַ� */
	char     scmzero    ;	/**< ��� */
	char     time[31]   ;	/**< ����ʱ�� */
	char     tzero      ;	/**< ��� */
	uint8_t  major      ;	/**< ��Ҫ�汾�� */
	uint8_t  minor      ;	/**< ��Ҫ�汾�� */
	uint16_t patch      ;	/**< �޶��汾�� */
}mod_dscrp_t            ;

/**
 * @brief ����������
 */
typedef struct data_item
{
	char 	*name ; /**< �Ƚ��ַ��� */
	int16_t width ; /**< ���ݵ�size������Ϊ�޷�����������Ϊ�з����� */
	void	*ptr  ; /**< ͨ�õ�Ŀ��ָ�� */
}data_item_t      ;

void unit_print_module(const mod_dscrp_t *mod); /*-< supply by unitframe */

/*=== public definitions ===*/

/** ��ȡ��ϢԴID */
#define get_msg_src(msg) (msg/256U)
/** ��ȡ��Ϣ���� */
#define get_msg_type(msg) (msg%256U)
/** ��ȡ��Ϣ���� */
#define get_msg_len(msg) (msg%256U)
/** ������Ϣ */
#define make_msg(src, type) ((uint16_t)((uint16_t)src*256U+type))
/** ���ɻ�����Ϣ */
#define make_base_msg(base, value) (base+value)

/** ������������ */
#define _TAB(n) n##_table
/** ��񳤶ȱ��������� */
#define _TAB_L(n) n##_table_len
/** ������ĳ��� */
#define DEF_TAB_L(n) const uint16_t _TAB_L(n)=sizeof(_TAB(n))/sizeof(_TAB(n)[0])
/** ������񳤶Ⱥ����� */
#define DCLR_TAB(n, t) extern const t _TAB(n)[]; extern const uint16_t _TAB_L(n)

/** m�������Ƿ����� */
#define ismask(d, m) (((d)&(m))==(m))

/** ת��Ϊ�ֽ�����*/
#define tobytearr(d,n) (((uint8_t*)(&(d)))[n])
/** ת��Ϊ˫������*/
#define towordarr(d,n) (((uint16_t*)(&(d)))[n])
/** ת��Ϊ��������*/
#define tolongarr(d,n) (((uint32_t*)(&(d)))[n])

/** ��ָ��ת��Ϊ�ֽ�����*/
#define ptobytearr(d,n) (((uint8_t*)(d))[n])
/** ��ָ��ת��Ϊ˫������*/
#define ptowordarr(d,n) (((uint16_t*)(d))[n])
/** ��ָ��ת��Ϊ��������*/
#define ptolongarr(d,n) (((uint32_t*)(d))[n])

/** ת��Ϊ�ֽ�*/
#define tobyte(d) tobytearr(d,0)
/** ת��Ϊ˫��*/
#define toword(d) towordarr(d,0)
/** ת��Ϊ����*/
#define tolong(d) tolongarr(d,0)
/** ת��Ϊָ�����͵�ָ��*/
#define totype(d, t) ((t*)(&(d)))

/** CPPUNIT �����ͱȽ� */
#define CPPUNIT_EASSERT(a,b) CPPUNIT_ASSERT_EQUAL((int)a, (int)b)

/*- ���Ժ궨�� */
#if defined(_WINDOWS)
#define UNIFW_ASSERT(cond)  /*-< windows�µ�Ԫ���Զ���Ϊ�� */
#define UNIFW_SAFETY_CRASH
#elif defined(__linux)
#define UNIFW_ASSERT(cond)  /*-< windows�µ�Ԫ���Զ���Ϊ�� */
#define UNIFW_SAFETY_CRASH
#else
/*- Ŀ����¶��Է���ֱ������ */

extern void safety_crash_handler(void); /*-< ��ȫ崻�����, ��Ŀ��������ļ�ʵ�� */

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

