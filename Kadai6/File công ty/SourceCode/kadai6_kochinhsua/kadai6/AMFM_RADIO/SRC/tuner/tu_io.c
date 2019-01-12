/******************************************************************************
..     �N�����I���a����                    2007����

		�v���W�F�N�g��	:
		�t�@�C����		: tu_io.c
		�@�\			: TUNER IO����
------------------------------------------------------------------------------
		�C������	�y�N�����z�yVersion�z�y���O�z �y�����z
------------------------------------------------------------------------------
[matsushima20120719]	�V�K�쐬
******************************************************************************/
#include	"../model.h"

#if	_MODEL_RADIO_
#define		_TU_IO_C_

#include		"../comm/common.h"
#include		"../io/IO_inc.h"

#include	"tu_io.h"
#include	"tu_ext.h"
#include	"tu_main.h"

static void	Tu_i2c_err(void);
/************************************************************************/
/* �֐��� :void	Tu_i2c_err(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	Tuner Ӱ�ޒʐMERR��������									*/
/* �ڍ�   :																*/
/************************************************************************/
static void	Tu_i2c_err(void)
{
	if(tu_ic_data.status == CTU_I2C_ERR)
	{
//		Tu_job_clear();
		tu_ic_data.status = CTU_I2C_IDLE;
#if 0
		if(tu_reset_cnt < 3)
		{
			tu_reset_cnt ++;
			Tu_e_func_off();
			tu_resettim = (100/10+1);
		}
		else
		{
			tu_reset_cnt = 4;
		}
#endif
	}
	
}

/************************************************************************/
/* �֐��� :BYTE	Tu_data_write(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	Tuner�w��addr��length�̑��M����								*/
/* �ڍ�   :																*/
/************************************************************************/
BYTE	Tu_data_write(void)
{
	BYTE txbuff[18];
	BYTE cnt,ret;
	
	ret = tu_ic_data.msa.bit.addr;
	if((tu_ic_data.status == CTU_I2C_ERR)||
		((ret+tu_ic_data.length) > 0x10))
	{
		return(NG);
	}
	wMemSet(txbuff,0,18);
	txbuff[0] = TTU_WRITE_SLAVE;
	txbuff[1] = tu_ic_data.msa.subaddr;
	for(cnt = 0; cnt < tu_ic_data.length;cnt++ )
	{
		txbuff[2+cnt] = tu_ic_data.txbuf[ret+cnt];
	}
	for(cnt = 0; cnt < 3;cnt++ )		/* ACK�G���[���ɁA���g���CMAX 3�� */
	{
		IO_Tuner_Send_Start();
		ret = IO_Tuner_Send(txbuff,(BYTE)(tu_ic_data.length + 0x02 ));
		if( ret == OK )
		{
			IO_Tuner_Send_Stop();
			break;
		}
		IO_Tuner_Send_Stop();
	}
#if 0
	if((cnt >= 3)&&(ret != OK ))
	{
		tu_ic_data.status = CTU_I2C_ERR;
		Tu_i2c_err();
	}
#endif
	return(ret);
}

/************************************************************************/
/* �֐��� :BYTE	Tu_data_read(void)										*/
/* --------------------------------------------------------------------	*/
/* �@�\   :	Tuner�w��addr��length�̎�M����								*/
/* �ڍ�   :																*/
/************************************************************************/
BYTE	Tu_data_read(void)
{
	BYTE result,icnt,rslave[1],len;
	
	len = tu_ic_data.length;
	tu_ic_data.length = 0;
	result = Tu_data_write();
	tu_ic_data.length = len;
	if( result != OK )
	{
		return(NG);
	}
	rslave[0] = TTU_READ_SLAVE;
	for(icnt = 0; icnt < 3;icnt++ )		/* ACK�G���[���ɁA���g���CMAX 3�� */
	{
#if 1		/*	��IO��IF�Ɉ��BYTE���M���āASTOP�������Ȃ��֐��Ȃ��A�����K�v��	*/
		IO_Tuner_Send_Start();
		result = IO_Tuner_Send(rslave,1);
		if( result == OK )
		{
			IO_Tuner_Receive(&tu_ic_data.rxbuf[tu_ic_data.msa.bit.addr],len);
			IO_Tuner_Send_Stop();
			break;
		}
		IO_Tuner_Send_Stop();
#endif
	}
#if 0
	if((icnt >= 3)&&(result != OK ))
	{
		tu_ic_data.status = CTU_I2C_ERR;
		Tu_i2c_err();
	}
	else
	{
		IO_Tuner_Receive(&tu_ic_data.rxbuf[tu_ic_data.msa.bit.addr],len);
		IO_Tuner_Send_Stop();
	}
#endif
	return(result);
}


#undef  _TU_IO_C_

#endif
