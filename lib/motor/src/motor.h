#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

/**
 * @brief 모터 드라이버 초기화
 * 
 * - DIR_A, PWM_A, DIR_B, PWM_B 핀을 출력으로 설정
 * - 초기 속도 = 0
 */
void Motor_Init(void);

/**
 * @brief 좌우 모터 속도 설정
 * 
 * @param left  -255 ~ +255 (왼쪽 모터 속도/방향)
 * @param right -255 ~ +255 (오른쪽 모터 속도/방향)
 * 
 * - 값의 부호(sign)로 방향 제어
 *   > 양수: 정방향 (digitalWrite HIGH)
 *   > 음수: 역방향 (digitalWrite LOW)
 * - 값의 절댓값(abs)으로 PWM 속도 제어 (0~255)
 */
void Motor_SetSpeed(int16_t left, int16_t right);

#endif // MOTOR_H
