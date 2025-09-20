#include "steering.h"
#include <avr/io.h>

// ---------- Servo PWM 설정 (Timer1, OC1A) ----------
#define SERVO_MIN 125   // -90도 → 약 1.0ms
#define SERVO_MAX 625   // +90도 → 약 2.0ms
#define SERVO_CENTER ((SERVO_MIN + SERVO_MAX) / 2)

// 각도(-90~+90) → OCR1A 값
static inline uint16_t angle_to_ocr1a(int16_t angle){
    if(angle < -90) angle = -90;
    if(angle >  90) angle = 90;
    uint16_t input = angle + 90;  // 0~180 범위
    return SERVO_MIN + ((uint32_t)input * (SERVO_MAX - SERVO_MIN)) / 180;
}

void Steering_Init(void){
    // PB1(OC1A, D9) 출력 설정
    DDRB |= (1 << PB1);

    // Timer1 Fast PWM 모드 14, 분주 64
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);

    ICR1   = 4999;              // 20ms 주기 (50Hz)
    OCR1A  = SERVO_CENTER;      // 초기 위치 중앙
}

void Steering_SetAngle(int16_t angle){
    OCR1A = angle_to_ocr1a(angle);
}