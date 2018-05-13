/** 
 * @file ver_b2v
 * @brief 
 * @author dingjian
 * @date 2016-06-15
 * 
 * $Id$
 */

#include "unifw.h"
#include "base/b2v.h"

/**
===Change Log of module===
 v0.2.1 hu.renqiang 2017-08-28
 - 修改函数b2v_check_bytes, 校验字节数据时数组偏移offset偏移需要累加

 v0.2.0 zhuo.kaikuo 2017-08-21
 - 对结构体中增加了对范围防护、枚举类型的保护

 v0.1.6 zhuo.kaikuo
 - 增加b2v_iom_to_var,b2v_var_to_iom用于码位的更新
 - 合并了部分b2v转换的函数形成b2v_conv_byoffset

 v0.1.0 hu.renqiang
 initial

=====End of change log====
*/

const mod_dscrp_t ver_b2v=
{
/*- name */    "b2v", 0,
/*- scm  */    "$Rev$", 0,
/*- time */    __DATE__" "__TIME__, 0,
/*- major*/    0,
/*- minor*/    2,
/*- patch*/    1,
};

