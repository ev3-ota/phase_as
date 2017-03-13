/**
 * TOPPERS/EV3 RT
 *
 * �ȒP�ȃv���O���~���O�ɂ��Mindstorms EV3����
 *
 * 2017/3/8 TCS�FS-NAKA
 * 
 */

#include "ev3api.h"
#include "app.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

static const sensor_port_t
    sonar_sensorF	= EV3_PORT_1,
    sonar_sensorB	= EV3_PORT_4;

static const motor_port_t
    left_motor		= EV3_PORT_C,
    right_motor		= EV3_PORT_D,
    front_motor		= EV3_PORT_A;

#define SONAR_ALERT_DISTANCE1 60 /* �����g�Z���T�ɂ���Q�����m����[cm] ���x��1*/
#define SONAR_ALERT_DISTANCE2 50 /* �����g�Z���T�ɂ���Q�����m����[cm] ���x��2*/
#define SONAR_ALERT_DISTANCE3 30 /* �����g�Z���T�ɂ���Q�����m����[cm] ���x��3*/

static int Global_Count;
static int GCount_InitFlg;

/* �֐��v���g�^�C�v�錾 */
static int sonar_alert(void);

//*****************************************************************************
// �T�v�F���C������
//
//
//*****************************************************************************

void main_task(intptr_t unused) {

	int act_flg;	// ���s�t���O
	int Lev1_State;	// ��Q�����m���x��1��� 0:�Ȃ�1:���x��1�Ɉڍs2:���x��1���
	int Lev2_State;	// ��Q�����m���x��1��� 0:�Ȃ�1:���x��2�Ɉڍs2:���x��2���
	int Lev3_State;	// ��Q�����m���x��1��� 0:�Ȃ�1:���x��3�Ɉڍs2:���x��3���
	
	/* �Z���T�[�|�[�g�̐ݒ� */
	ev3_sensor_config( sonar_sensorF,ULTRASONIC_SENSOR );
	ev3_sensor_config( sonar_sensorB,ULTRASONIC_SENSOR );
	
	/* ���[�^�[�o�̓|�[�g�̐ݒ� */
	ev3_motor_config( left_motor, LARGE_MOTOR );
	ev3_motor_config( right_motor, LARGE_MOTOR );
	ev3_motor_config( front_motor, LARGE_MOTOR );
	
	/* ���[�^�[�G���R�[�_���Z�b�g */
	ev3_motor_reset_counts( left_motor );
	ev3_motor_reset_counts( right_motor );
	ev3_motor_reset_counts( front_motor );
	
	/* �����n���h�����Z�b�g */
	eve3_sta_cyc( COUNT_CYC1 );
	GCounta_InitFlg = 1;
	Global_Count = 0;

	/* ���[�^�[���s */
	act_flg = 1;
	
	while ( act_flg ) {
		
		/* ��Q�����m */
		if ( sonar_alert() == 1 ) { // ���x��1
			
			/*-- �X�s�[�h�_�E�� --*/
		
			// ��Q�����m��Ԃ̐ݒ�
			if ( Lev1_State == 0 ) {
				Lev1_State = 1;
				Lev2_State = 0;
				Lev3_Stete = 0;
			}
			else {
				Lev1_State = 2;
				Lev2_State = 0;
				Lev3_State = 0;
			}
			// �������s �O�i POWER 40%
			ev3_motor_set_power( left_motor, -40 ); 
    		ev3_motor_set_power( right_motor, -40 );		
		}
		else if ( sonar_alert() == 2 ) { // ���x��2
			
			/*-- �X�s�[�h�_�E�� & �������� --*/
			// ��Q�����m��Ԃ̐ݒ�
			if ( Lev2_State == 0 ) {
				Lev1_State = 0;
				Lev2_State = 1;
				Lev3_Stete = 0;
			}
			else {
				Lev1_State = 0;
				Lev2_State = 2;
				Lev3_State = 0;
			}
			
			if ( Lev2_State == 1 ) {
				/* �����n���h���l������ */
				GCounta_InitFlg = 1;
			}
			else {
				
				
			}
			
		}
		else if ( sonar_alert() == 3 ) { // ���x��3
		}
		else {
			
			// ��Q�����m��Ԃ̏�����
			Lev1_State = 0;
			Lev2_State = 0;
			Lev3_State = 0;
			
			// �������s �O�i POWER 70%
			ev3_motor_set_power( left_motor, -70 ); 
    		ev3_motor_set_power( right_motor, -70 );
		}
	}
}

//*****************************************************************************
// �T�v�F�����g�Z���T�ɂ���Q�����m
//
// �߂�l 1:���x��1�A2:���x��2�A3:���x��3
//*****************************************************************************
static int sonar_alert(void)
{
    static unsigned int counter = 0;
    static int alert = 0;

    signed int distance;

    if ( ++counter == 40/4 ) /* ��40msec�������ɏ�Q�����m  */
    {
        /*
         * �����g�Z���T�ɂ�鋗����������́A�����g�̌��������Ɉˑ����܂��B
         * NXT�̏ꍇ�́A40msec�������x���o����̍ŒZ��������ł��B
         * EV3�̏ꍇ�́A�v�m�F
         */
        distance = ev3_ultrasonic_sensor_get_distance( sonar_sensorF );
        if ( ( distance <= SONAR_ALERT_DISTANCE1 ) && ( distance > SONAR_ALERT_DISTANCE2 ) )
        {
            alert = 1; /* ��Q�������m */
        }
        else if ( ( distance <= SONAR_ALERT_DISTANCE2 ) && ( distance > SONAR_ALERT_DISTANCE3 ) )
        {
            alert = 2; /* ��Q�������m */
        }
        else if ( ( distance <= SONAR_ALERT_DISTANCE3 ) && ( distance >= 0 ) )
        {
            alert = 3; /* ��Q�������m */
        }
        else
        {
            alert = 0; /* ��Q������ */
        }
        counter = 0;
    }

    return alert;
}

//*****************************************************************************
// �T�v�F1000ms���[�`��
//
// �����F1000(ms)
//*****************************************************************************
void Counter_1000cyc(intptr_t idx) {
	if ( GCount_InitFlg == 1 ) {
		Global_Count = 0;
	}
	else {
		Global_Count++;
	}
}

