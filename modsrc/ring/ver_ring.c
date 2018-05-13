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
 - ring_pop_new: ��pop�������ݳ��ȵ�����ʱ����ֹpop
 - ring_pop/ring_pop_new: ��pop����ʱ��ʹ��usr_memsafecpy�������ݣ������ԭ��ַ����Ϊ0

 v0.1.5 hu.renqiang, 2017-12-10
 - ring_push() �� ring_push_new() ������ж�û�б�Ҫ��ȥ��
 - rings_reset ���ӳ�ʼ��freeΪ��
 - rings_rpopanull �޸Ŀ��ܵ�����������bug

 v0.1.4 hu.renqiang, 2017-5-21
 - bug fixed
 
 v0.1.3 wangkang, maijianwei, xiechongyin, jianglongping
 - �����߲��뾲̬����

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

