/**
 *@file  \lrts-rcu02-pulse_simulate\src\include\pulse_simulate.h
 *@brief  
 *@author 
 *@data  2016-12-3 10:28
 *
 * $Id$
 */
#include "unifw.h"
#include "base/b2v.h"
#include "pulse_simulate.h"

/*- ssimp头配置解析表 */
const var_item_t ssimp_hdr_tbl[SSIMP_HDR_TBL_LEN] =
{
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(ssimp_hdr_t, src_type),},
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(ssimp_hdr_t, src_id),  },
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(ssimp_hdr_t, dst_type),},
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(ssimp_hdr_t, dst_id),  },
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(ssimp_hdr_t, serial),  },
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(ssimp_hdr_t, pkg_num), },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET(ssimp_hdr_t, pkg_len), },
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(ssimp_hdr_t, cmd),     },
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(ssimp_hdr_t, param),   },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET(ssimp_hdr_t, len),     },
};

/*- 车辆信息解析配置表 */
const var_item_t train_info_tbl[TRAIN_INFO_TBL_LEN] =
{
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET(train_info_t, diameter),       },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET(train_info_t, pul_percycle),   },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET(train_info_t, ch1_speed),      },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET(train_info_t, ch2_speed),      },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET(train_info_t, ch3_speed),      },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET(train_info_t, ch4_speed),      },
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(train_info_t, ch12_dir),       },
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(train_info_t, ch12_phase_diff),},
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(train_info_t, ch34_dir),       },
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET(train_info_t, ch34_phase_diff),},
};
