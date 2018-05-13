/** 
 * @file ver_ring
 * @brief 
 * @author hrq2547274
 * @date 2016-10-23
 * 
 * $Id$
 */

#include "unifw.h"
#include "base/ring.h"

/**
===Change Log of module===

 v0.1.6 wang.kang, 2018-03-01
 - ring_pop_new: 当pop超过数据长度的数据时，禁止pop
 - ring_pop/ring_pop_new: 当pop数据时，使用usr_memsafecpy拷贝数据，并清除原地址数据为0

 v0.1.5 hu.renqiang, 2017-12-10
 - ring_push() 和 ring_push_new() 最外层判断没有必要，去掉
 - rings_reset 增加初始化free为满
 - rings_rpopanull 修改可能导致溢出错误的bug

 v0.1.4 hu.renqiang, 2017-5-21
 - bug fixed
 
 v0.1.3 wangkang, maijianwei, xiechongyin, jianglongping
 - 代码走查与静态测试

 v0.1.2 zhuo.kaikuo, 2016-11-19
 - add rings_rget and rings_rset, add MACRO

 v0.1.1 zhuo.kaikuo, 2016-11-17
 - add ring struct operation

 v0.1.0 hrq2547274

=====End of change log====
*/

const mod_dscrp_t ver_ring=
{
/*- name */    "ring", 0,
/*- scm  */    "$Rev$", 0,
/*- time */    __DATE__" "__TIME__, 0,
/*- major*/    0,
/*- minor*/    1,
/*- patch*/    6,
};

