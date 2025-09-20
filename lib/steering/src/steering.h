#ifndef STEERING_H
#define STEERING_H

#include <stdint.h>

/**
 * @brief 스티어링 모터 초기화 함수
 * 
 * - Timer/PWM 설정
 * - 초기 각도는 0도로 세팅
 */
void Steering_Init(void);

/**
 * @brief 스티어링 각도를 설정하는 함수
 * 
 * @param angle  -90 ~ +90 (degree)
 * 
 * - 입력 각도를 PWM 듀티로 변환하여 서보모터 제어
 * - 범위를 넘어가면 자동으로 제한(clamp)
 */
void Steering_SetAngle(int16_t angle);

#endif // STEERING_H