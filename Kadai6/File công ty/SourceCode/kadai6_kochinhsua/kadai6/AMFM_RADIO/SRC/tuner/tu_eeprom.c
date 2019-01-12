/********************************************************************************/
/*		tu_eeprom.c																*/
/*																				*/
/*		プロジェクト名：Tuner													*/
/*		ファイル名：															*/
/*		モジュール名：															*/
/*		機能：			EEPROM関係処理											*/
/*																				*/
/*																				*/
/*																				*/
/*			修正履歴	【年月日】	【Version】	【名前】 	【説明】			*/
/*------------------------------------------------------------------------------*/
/*						2010.08.06		0.0			岳		新規作成			*/
/*[20120629chin]	U向け仕様変更 U向け仕様変更 step:200→100kHz				*/
/********************************************************************************/
#include	"../model.h"

#if	_MODEL_RADIO_

#if	(_MODEL_EEPROM_ == 1)

#define	_TU_EEPROM_C_

#include	<string.h>
#include	"../comm/common.h"
#include	"../eeprom/eeprom_ext.h"
#include	"../main/ma_ext.h"
#include	"tu_def.h"
#include	"tu_event.h"
#include	"tu_main.h"
#include	"tu_eeprom.h"
#include	"tu_ext.h"

/********************************************************************************/
/*	関数名  ：Tu_CheckInvalidData												*/
/*	引  数	：checkbuf:チェック用データ先頭アドレス								*/
/*			  maxfm:最大FM周波数												*/
/*			  maxam:最大AM周波数												*/
/*	戻り値	：TRUE:有効データ/FALSE：無効データ									*/
/*	機  能	：EEPROMから取得した二週間メモリデータ有効化チェック				*/
/*	注意事項:																	*/
/*	修正履歴	【年月日】	【Version】	【名前】 	【説明】					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.06		0.0			岳		新規作成					*/
/*				2012.11.28		0.0		yamamoto	QAC対策						*/
/********************************************************************************/
BYTE	Tu_CheckInvalidData(BYTE *checkbuf,WORD maxfm,WORD maxam)
{
	BYTE	*pCheck;
	BYTE	i;
	WORD	freq_tst;
	
	pCheck = checkbuf;
	for(i=0;i< CTU_PRESET ;i++)
	{
		if( (WORD)((*(pCheck+1) * 0x100) + (*pCheck)) > maxam )
		{
			return FALSE;
		}
		pCheck += 2;
	}
	
	pCheck = checkbuf + 0x10;
	for(i=0;i< CTU_PRESET ;i++)
	{
		if( (WORD)((*(pCheck+1) * 0x100) + (*pCheck)) > maxfm )
		{
			return FALSE;
		}
		pCheck += 2;
	}
	
	pCheck = checkbuf + 0x20;
	for(i=0;i< CTU_PRESET;i++)
	{
		if( (WORD)((*(pCheck+1) * 0x100) + (*pCheck)) > maxfm )
		{
			return FALSE;
		}
		pCheck += 2;
	}
	return TRUE;
}

/********************************************************************************/
/*	関数名  ：Tu_ReadEepData													*/
/*	引  数	：savebuf:読み込みデータ先頭アドレス								*/
/*			  dest:仕向け														*/
/*	戻り値	：TRUE:読み込み成功かつ有効/FALSE：読み込み失敗または無効			*/
/*	機  能	：EEPROMから二週間用データ取得と有効化チェック						*/
/*	注意事項:																	*/
/*	修正履歴	【年月日】	【Version】	【名前】 	【説明】					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.06		0.0			岳		新規作成					*/
/********************************************************************************/
BYTE	Tu_ReadEepData(BYTE *savebuf,BYTE dest)
{
	sEEP_PARA	tu_read;
	BYTE	ret;
	WORD	maxfreq_fm;
	WORD	maxfreq_am;
	
	ret = FALSE;
	#if 0
	if( Ma_Chk_EEP_Initial() == TRUE )		/*	EEPROM初期化済みチェック、失敗場合デフォールト値を使う	*/
	#endif
	{
		if ((Ma_model_dest() >= CMA_MODEL_AMFM_J)&&(Ma_model_dest() <= CMA_MODEL_AMFM_X))
		{
			maxfreq_fm = TU_COEF_TBL[dest].fm.max_ch;
		}
		else
		{
			maxfreq_fm = 0x00;
		}
		maxfreq_am = TU_COEF_TBL[dest].am.max_ch;
#if 0
		while(!EEP_CheckIDLE())		/*	読み込み可能かチェック	*/
		{
			;
		}
#endif
		tu_read.EepAddr	= CEEP_ADDR_TUNER;
#if 0
		tu_read.EepAddr	= 0x7A5F;
#endif
		tu_read.Length	= CEEP_LENG_TUNER;
		tu_read.BufAddr	= savebuf;
		if(EEP_ReadReq(tu_read) == TRUE)
		{
			if(Tu_CheckInvalidData(savebuf,maxfreq_fm,maxfreq_am) == TRUE)
			{
				return TRUE;
			}
		}
	}
	return ret;
}


/********************************************************************************/
/*	関数名  ：Tu_CheckInvalidData												*/
/*	引  数	：chkbuf:チェック用データ先頭アドレス								*/
/*			  destno:仕向け														*/
/*	戻り値	：TRUE:有効データ/FALSE：無効データ									*/
/*	機  能	：EEPROMから取得した初期設定と閾値データ有効化チェック				*/
/*	注意事項:																	*/
/*	修正履歴	【年月日】	【Version】	【名前】 	【説明】					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			岳		新規作成					*/
/********************************************************************************/
BYTE	Tu_CheckInvalidPara(BYTE *chkbuf,BYTE destno)
{
	BYTE	*p;
	BYTE	i;
	BYTE	checksum;
	BYTE	ret;
	BYTE	cnt;
	
	checksum = 0;
	p = chkbuf;
	ret = 0;
	cnt = 0;
	for( i=0;i<16;i++ )
	{
		if( *(p + i) == 0x00)
		{
			cnt ++;
		}
	}
	if( cnt < 16 )
	{
		for( i=0;i<15;i++,p++ )
		{
			checksum += *p;
		}
		if( checksum == *p )
		{
			ret |= CTU_EEP_ADR_Q_INF1_CHK;
		}
	}
	if(( destno == CTU_DEST_JPN_A )
	|| ( destno == CTU_DEST_JPN_FA ))
	{
		p = (chkbuf + CTU_EEP_ADR_PARA_FMJ);
	}
	else
	{
		p = (chkbuf + CTU_EEP_ADR_PARA_FMU);
	}
	
	cnt = 0;
	for( i=0;i<16;i++ )
	{
		if( *(p + i) == 0x00)
		{
			cnt ++;
		}
	}
	if( cnt < 16)
	{
		checksum = 0;
		for( i=0;i<15;i++,p++ )
		{
			checksum += *p;
		}
		if( checksum == *p )
		{
			ret |= CTU_EEP_ADR_PARA_FM_CHK;
		}
	}
	
	p = (chkbuf + CTU_EEP_ADR_PARA_AM);
	cnt = 0;
	for( i=0;i<16;i++ )
	{
		if( *(p + i) == 0x00)
		{
			cnt ++;
		}
	}
	if( cnt < 16)
	{
		checksum = 0;
		for( i=0;i<15;i++,p++ )
		{
			checksum += *p;
		}
		if( checksum == *p )
		{
			ret |= CTU_EEP_ADR_PARA_AM_CHK;
		}
	}
	
	p = (chkbuf + CTU_EEP_ADR_Q_INF2);
	cnt = 0;
	for( i=0;i<16;i++ )
	{
		if( *(p + i) == 0x00)
		{
			cnt ++;
		}
	}
	if( cnt < 16)
	{
		checksum = 0;
		for( i=0;i<15;i++,p++ )
		{
			checksum += *p;
		}
		if( checksum == *p )
		{
			ret |= CTU_EEP_ADR_Q_INF2_CHK;
		}
	}
	
	p = (chkbuf + CTU_EEP_ADR_Q_INF3);
	cnt = 0;
	for( i=0;i<16;i++ )
	{
		if( *(p + i) == 0x00)
		{
			cnt ++;
		}
	}
	if( cnt < 16)
	{
		checksum = 0;
		for( i=0;i<15;i++,p++ )
		{
			checksum += *p;
		}
		if( checksum == *p )
		{
			ret |= CTU_EEP_ADR_Q_INF3_CHK;
		}
	}
	
	return ret;
}

/********************************************************************************/
/*	関数名  ：Tu_ReadEepPara													*/
/*	引  数	：readbuf:読み込みデータ先頭アドレス								*/
/*			  dest:仕向け														*/
/*	戻り値	：TRUE:読み込み成功かつ有効/FALSE：読み込み失敗または無効			*/
/*	機  能	：EEPROMから初期設定と閾値データ取得と有効化チェック				*/
/*	注意事項:																	*/
/*	修正履歴	【年月日】	【Version】	【名前】 	【説明】					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			岳		新規作成					*/
/*				2012.11.28		0.0		yamamoto	QAC対策						*/
/********************************************************************************/
BYTE	Tu_ReadEepPara(BYTE *readbuf,BYTE dest)
{
	sEEP_PARA	tu_read;
	BYTE	ret;
	
	ret = FALSE;
	tu_read.EepAddr	= CEEP_ADDR_ATOMIC;
	tu_read.Length	= 96;		/*	6*16	*/
	tu_read.BufAddr	= readbuf;
	if(EEP_ReadReq(tu_read) == TRUE)
	{
		ret = Tu_CheckInvalidPara(readbuf,dest);
	}
	return ret;
}
#if 0 /* 未使用のためコメントアウト */
/********************************************************************************/
/*	関数名  ：Tu_EepCmpValid													*/
/*	引  数	：buf:チェックデータ先頭アドレス									*/
/*			  cmpdata:チェックデータ											*/
/*			  len:長さ															*/
/*	戻り値	：TRUE:有効/FALSE：無効												*/
/*	機  能	：指定バッファ内容は指定BYTE内容と同じのデータあるかチェック		*/
/*	注意事項:																	*/
/*	修正履歴	【年月日】	【Version】	【名前】 	【説明】					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			岳		新規作成					*/
/********************************************************************************/
BYTE	Tu_EepCmpValid(BYTE *buf,BYTE cmpdata,BYTE len)
{
	BYTE	i;
	for( i=0;i<len;i++ )
	{
		if( *(buf+i) == cmpdata )
		{
			return FALSE;
		}
	}
	return TRUE;
}
#endif
/********************************************************************************/
/*	関数名  ：Tu_ReadEepRq														*/
/*	引  数	：rdbuf:読み込みデータ先頭アドレス									*/
/*			  offset:オフセット													*/
/*			  rdlen:長さ														*/
/*	戻り値	：TRUE:読み込み成功/FALSE：読み込み失敗								*/
/*	機  能	：EEPROMから指定オフセットと長さでデータ読み出す処理				*/
/*	注意事項:																	*/
/*	修正履歴	【年月日】	【Version】	【名前】 	【説明】					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			岳		新規作成					*/
/********************************************************************************/
BYTE	Tu_ReadEepRq(BYTE *rdbuf,BYTE offset,BYTE rdlen)
{
	sEEP_PARA	tu_read;
	BYTE	ret;
	
	ret = FALSE;
	tu_read.EepAddr	= (BYTE)(CTU_EEPADR_PARA + offset);
	tu_read.Length	= rdlen;
	tu_read.BufAddr	= rdbuf;
	if(EEP_CheckIDLE() == TRUE)		/*	書き込み可能かチェック	*/
	{
		if(EEP_ReadReq(tu_read) == TRUE)
		{
			ret = TRUE;
		}
	}
	
	return ret;
}

/********************************************************************************/
/*	関数名  ：Tu_WriteEepRq														*/
/*	引  数	：wrbuf:書き込みデータ先頭アドレス									*/
/*			  offset:オフセット													*/
/*			  wrlen:長さ														*/
/*	戻り値	：TRUE:書き込み可能/FALSE：書き込み失敗								*/
/*	機  能	：EEPROMに指定オフセットと長さでデータ書き込む処理					*/
/*	注意事項:																	*/
/*	修正履歴	【年月日】	【Version】	【名前】 	【説明】					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			岳		新規作成					*/
/********************************************************************************/
BYTE	Tu_WriteEepRq(BYTE *wrbuf,BYTE offset,BYTE wrlen)
{
	sEEP_PARA	tu_write;
	BYTE	ret;
	
	ret = FALSE;
	tu_write.EepAddr	= (BYTE)(CTU_EEPADR_PARA + offset);
	tu_write.Length		= wrlen;
	tu_write.BufAddr	= wrbuf;
	if(EEP_CheckIDLE() == TRUE)		/*	書き込み可能かチェック	*/
	{
		EEP_WriteReq(tu_write);
		ret = TRUE;
	}
	
	return ret;
}

/********************************************************************************/
/*	関数名  ：Tu_WriteEepData													*/
/*	引  数	：なし																*/
/*	戻り値	：なし																*/
/*	機  能	：EEPROMへ指定内容を書き込み処理									*/
/*	注意事項:																	*/
/*	修正履歴	【年月日】	【Version】	【名前】 	【説明】					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.11		0.0			岳		新規作成					*/
/********************************************************************************/
void	Tu_WriteEepData(void)
{
	if(EEP_CheckIDLE() == TRUE)		/*	書き込み可能かチェック	*/
	{
		EEP_WriteReq(tu_eepreq);
		ftu_weepreq = OFF;
	}
	else
	{
		tu_eep_cnt ++;
		if( tu_eep_cnt > 3 )
		{
			ftu_weepreq = OFF;
		}
	}
}


/********************************************************************************/
/*	関数名  ：Tu_SaveEepData													*/
/*	引  数	：無し																*/
/*	戻り値	：無し																*/
/*	機  能	：EEPROMへデータ保存処理											*/
/*	注意事項:																	*/
/*	修正履歴	【年月日】	【Version】	【名前】 	【説明】					*/
/*------------------------------------------------------------------------------*/
/*				2010.08.06		0.0			岳		新規作成					*/
/********************************************************************************/
void	Tu_SaveEepData(BYTE status,BYTE *BufAddr)
{
	BYTE	*p;
	BYTE	iLoop1,iLoop2;
	
	p = BufAddr;
	if( status == CTU_EEPWR_PST )
	{
		for (iLoop1 = 0; iLoop1 < CTU_BAND; iLoop1 ++)
		{
			for (iLoop2 = 1; iLoop2 < (CTU_PRESET+1); iLoop2 ++)
			{
				*p = (BYTE)(tu_mem[iLoop1].pset[iLoop2].freq & 0xFF);
				p++;
				*p = (BYTE)((tu_mem[iLoop1].pset[iLoop2].freq >> 8)&0xFF);
				p++;
			}
		}
	}
}

#undef	_TU_EEPROM_C_

#endif
#endif
