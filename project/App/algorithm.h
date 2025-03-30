/** \file algorithm.h ******************************************************
*
* Project: MAXREFDES117#
* Filename: algorithm.h
* Description: 此模块是心率/SpO2计算算法头文件
*
* Revision History:
*\n 1-18-2016 Rev 01.00 SK Initial release.
*\n
*
* --------------------------------------------------------------------
*
* This code follows the following naming conventions:
*
*\n char              ch_pmod_value
*\n char (array)      s_pmod_s_string[16]
*\n float             f_pmod_value
*\n int32_t           n_pmod_value
*\n int32_t (array)   an_pmod_value[16]
*\n int16_t           w_pmod_value
*\n int16_t (array)   aw_pmod_value[16]
*\n uint16_t          uw_pmod_value
*\n uint16_t (array)  auw_pmod_value[16]
*\n uint8_t           uch_pmod_value
*\n uint8_t (array)   auch_pmod_buffer[16]
*\n uint32_t          un_pmod_value
*\n int32_t *         pn_pmod_value
*
* ------------------------------------------------------------------------- */
/*******************************************************************************
版权所有（C）2015马克西姆集成产品，股份有限公司，保留所有权利。
*
*特此免费授予任何获得
*本软件和相关文档文件（“软件”）的副本，
*无限制地处理软件，包括但不限于
*使用、复制、修改、合并、发布、分发、分许可的权利，
*和/或出售软件副本，并允许
*根据以下条件提供软件：
*
*应包括上述版权声明和本许可声明
*在软件的所有副本或实质部分中。
*
*该软件是“按原样”提供的，没有任何形式的担保，明示
*或隐含的，包括但不限于
*适销性、特定用途的适用性和非侵权性。
*在任何情况下，MAXIM INTEGRATED均不对任何索赔、损害赔偿负责
*或其他责任，无论是在合同诉讼、侵权诉讼还是其他诉讼中，
*由软件或使用引起或与之相关
*软件的其他交易。
*
*除本通知另有规定外，Maxim Integrated的名称
*不得使用股份有限公司的产品，除非马克西姆综合
*产品，股份有限公司品牌政策。
*
*仅转让此软件并不意味着任何许可证
*商业秘密、专有技术、版权、专利，
*商标、面具作品权或任何其他形式的知识产权
*财产。股份有限公司保留所有
*所有权。
*******************************************************************************
*/
#ifndef ALGORITHM_H_
#define ALGORITHM_H_
 
#include "ht32f5xxxx_01.h"              // Device header
#include "stdbool.h"
 
#define true 1
#define false 0
#define FS 100
#define BUFFER_SIZE  (FS* 5) 
#define HR_FIFO_SIZE 7
#define MA4_SIZE  4 // DO NOT CHANGE
#define HAMMING_SIZE  5// DO NOT CHANGE
#define min(x,y) ((x) < (y) ? (x) : (y))
 
 
void maxim_heart_rate_and_oxygen_saturation(uint32_t *pun_ir_buffer ,  int32_t n_ir_buffer_length, uint32_t *pun_red_buffer ,   int32_t *pn_spo2, int8_t *pch_spo2_valid ,  int32_t *pn_heart_rate , int8_t  *pch_hr_valid);
void maxim_find_peaks( int32_t *pn_locs, int32_t *pn_npks,  int32_t *pn_x, int32_t n_size, int32_t n_min_height, int32_t n_min_distance, int32_t n_max_num );
void maxim_peaks_above_min_height( int32_t *pn_locs, int32_t *pn_npks,  int32_t *pn_x, int32_t n_size, int32_t n_min_height );
void maxim_remove_close_peaks( int32_t *pn_locs, int32_t *pn_npks,   int32_t  *pn_x, int32_t n_min_distance );
void maxim_sort_ascend( int32_t *pn_x, int32_t n_size );
void maxim_sort_indices_descend(  int32_t  *pn_x, int32_t *pn_indx, int32_t n_size);
 
#endif /* ALGORITHM_H_ */

