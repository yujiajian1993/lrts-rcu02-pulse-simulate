#ifndef  _UNIFW_PLAT_H
#define  _UNIFW_PLAT_H

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @file unifw_plat.h
 * @brief 
 * @author 003
 * @version 
 * @date 2016-06-05
 */

/**
 * ==========================
 * 提供平台底层初始化函数
 * ==========================
 */

void plat_init(void);

/**
 * ==========================
 * 提供平台服务初始化函数
 * ==========================
 */

void plat_service_launch(void);

/**
 * @brief plat_launch 提供平台启动函数
 */
void plat_launch(void);

/**
 * @brief plat_parse_config 提供平台解析函数
 *
 * @param argc 输入参数数量
 * @param argv[]	参数列表
 */
void plat_parse_config(int argc, const char * argv[]);

#ifdef __cplusplus
}
#endif

#endif   /*_UNIFW_PLAT_H*/

