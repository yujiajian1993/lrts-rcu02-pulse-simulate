/**
 *@file  \lrts-rcu02-pulse_simulate\src\main\ate_rcu02_protocol.c
 *@brief  
 *@author 
 *@data  2016-12-3 10:58
 *
 * $Id$
 */

#include "unifw.h"
#include "base/b2v.h"
#include "pulse_simulate.h"

const var_item_t fpga_rw_tbl[FPGA_RW_TBL_LEN] =
{
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, header, frame_start),      }, 
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET_ARR(fpga_rw_t, header, cmd),              },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET_ARR(fpga_rw_t, header, length),           },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET_ARR(fpga_rw_t, header, addr),             },

    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch1_duty_cycle), },
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch2_duty_cycle), },
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch3_duty_cycle), },
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch4_duty_cycle), },
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch1_fre),        },
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch2_fre),        },
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch3_fre),        },
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch4_fre),        },
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch12_phase_diff),},
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch34_phase_diff),},
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch12_pulse_cnt), },
    {UNIFW_BIG_ENDIAN, 4, 4, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, ch34_pulse_cnt), },
    {UNIFW_BIG_ENDIAN, 1, 1, _SIZE_OFFSET_ARR(fpga_rw_t, pul_info, do_output),      },

    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET_ARR(fpga_rw_t, tail, crc16),             },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET_ARR(fpga_rw_t, tail, raw_di),            },
    {UNIFW_BIG_ENDIAN, 2, 2, _SIZE_OFFSET_ARR(fpga_rw_t, tail, frame_end),         },
};