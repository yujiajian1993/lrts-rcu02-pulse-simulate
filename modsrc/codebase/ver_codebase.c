/** 
 * @file ver_codebase
 * @brief 
 * @author hrq2547274
 * @date 2016-10-30
 * 
 * $Id$
 */

#include "unifw.h"

/**
===Change Log of module===

 v0.2.7 hu.renqiang  2018.04.20
    - usr_memset: bug修复, 修改head不等于0时的for循环判断条件
    
 v0.2.6 wang.kang  2018.03.01
    - 增加usr_memsafecpy函数及相应的测试用例
    - 修改usr_memcpy/usr_memset/usr_memsafecpy, 提高运行效率

 v0.2.5 hu.renqiang  2017.09.23
    - 增加atoi函数

 v0.2.4 hu.renqiang  2017.07.30
    - CRC计算函数增加const限定符

 v0.2.3 hu.renqiang  2017.07.06
    - 修改usr_memcpy函数原型

 v0.2.2 hu.renqiang  2017.06.06
    - 发布

 v0.1.0 hrq2547274

=====End of change log====
*/

const mod_dscrp_t ver_codebase=
{
/*- name */    "codebase", 0,
/*- scm  */    "$Rev$", 0,
/*- time */    __DATE__" "__TIME__, 0,
/*- major*/    0,
/*- minor*/    2,
/*- patch*/    7,
};

