/** \file algorithm.h ******************************************************
*
* Project: MAXREFDES117#
* Filename: algorithm.h
* Description: ��ģ��������/SpO2�����㷨ͷ�ļ�
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
��Ȩ���У�C��2015�����ķ���ɲ�Ʒ���ɷ����޹�˾����������Ȩ����
*
*�ش���������κλ��
*�����������ĵ��ļ�������������ĸ�����
*�����Ƶش��������������������
*ʹ�á����ơ��޸ġ��ϲ����������ַ�������ɵ�Ȩ����
*��/��������������������
*�������������ṩ�����
*
*Ӧ����������Ȩ�����ͱ��������
*����������и�����ʵ�ʲ����С�
*
*������ǡ���ԭ�����ṩ�ģ�û���κ���ʽ�ĵ�������ʾ
*�������ģ�������������
*�����ԡ��ض���;�������Ժͷ���Ȩ�ԡ�
*���κ�����£�MAXIM INTEGRATED�������κ����⡢���⳥����
*���������Σ��������ں�ͬ���ϡ���Ȩ���ϻ������������У�
*�������ʹ���������֮���
*������������ס�
*
*����֪ͨ���й涨�⣬Maxim Integrated������
*����ʹ�ùɷ����޹�˾�Ĳ�Ʒ�����������ķ�ۺ�
*��Ʒ���ɷ����޹�˾Ʒ�����ߡ�
*
*��ת�ô����������ζ���κ����֤
*��ҵ���ܡ�ר�м�������Ȩ��ר����
*�̱ꡢ�����ƷȨ���κ�������ʽ��֪ʶ��Ȩ
*�Ʋ����ɷ����޹�˾��������
*����Ȩ��
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

