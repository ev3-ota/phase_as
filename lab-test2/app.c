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

#define SONAR_ALERT_DISTANCE 100 /* �����g�Z���T�ɂ���Q�����m����[cm] */

/* �֐��v���g�^�C�v�錾 */
static int sonar_alert(void);

//*****************************************************************************
// �T�v�F���C������
//
//
//*****************************************************************************

void main_task(intptr_t unused) {

	int act_flg;	// ���s�t���O
	
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


	/* ���[�^�[���s */
	act_flg = 1;
	
	while ( act_flg ) {
		
		/* ��Q�����m */
		if ( sonar_alert() == 1 ) {
			
			/* ������~ */
			ev3_motor_stop(left_motor, true);
			ev3_motor_stop(right_motor, true);
			
			act_flg = 0;
		}
		else {
			/* �������s �O�i POWER 30% */
			ev3_motor_set_power( left_motor, -100 ); 
    		ev3_motor_set_power( right_motor, -100 );
		}
	}



}

//*****************************************************************************
// �T�v�F�����g�Z���T�ɂ���Q�����m
//
// �����F1000ms(app.cfg�Őݒ�j
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
        if ( ( distance <= SONAR_ALERT_DISTANCE ) && ( distance >= 0 ) )
        {
            alert = 1; /* ��Q�������m */
        }
        else
        {
            alert = 0; /* ��Q������ */
        }
        counter = 0;
    }

    return alert;
}

