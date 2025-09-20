#include "motor.h"
#include <avr/io.h>

// 핀 정의
#define DIR_A   PD4   // D4 → PORTD4
#define PWM_A   PD5   // D5(OC0B) → PORTD5
#define DIR_B   PD7   // D7 → PORTD7
#define PWM_B   PD6   // D6(OC0A) → PORTD6

void Motor_Init(void){
    // 방향 핀 출력 설정
    DDRD |= (1 << DIR_A) | (1 << DIR_B);

    // PWM 핀 출력 설정
    DDRD |= (1 << PWM_A) | (1 << PWM_B);

    // Timer0 Fast PWM, 비반전 모드, 분주 64
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00);

    // 초기 정지
    OCR0A = 0;
    OCR0B = 0;
}

void Motor_SetSpeed(int16_t left, int16_t right){
    // 왼쪽 모터
    if(left >= 0){
        PORTD |= (1 << DIR_A);   // 정방향
        OCR0B = (left > 255) ? 255 : left;   // 속도 (0~255) → OC0B(PD5)
    } else {
        PORTD &= ~(1 << DIR_A);  // 역방향
        int16_t spd = -left;
        OCR0B = (spd > 255) ? 255 : spd;
    }

    // 오른쪽 모터
    if(right >= 0){
        PORTD |= (1 << DIR_B);   // 정방향
        OCR0A = (right > 255) ? 255 : right; // 속도 (0~255) → OC0A(PD6)
    } else {
        PORTD &= ~(1 << DIR_B);  // 역방향
        int16_t spd = -right;
        OCR0A = (spd > 255) ? 255 : spd;
    }
}