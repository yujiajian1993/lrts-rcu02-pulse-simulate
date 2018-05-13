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
 * �ṩƽ̨�ײ��ʼ������
 * ==========================
 */

void plat_init(void);

/**
 * ==========================
 * �ṩƽ̨�����ʼ������
 * ==========================
 */

void plat_service_launch(void);

/**
 * @brief plat_launch �ṩƽ̨��������
 */
void plat_launch(void);

/**
 * @brief plat_parse_config �ṩƽ̨��������
 *
 * @param argc �����������
 * @param argv[]	�����б�
 */
void plat_parse_config(int argc, const char * argv[]);

#ifdef __cplusplus
}
#endif

#endif   /*_UNIFW_PLAT_H*/

