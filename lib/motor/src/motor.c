#include "motor.h"
#include <avr/io.h>
#include <stdlib.h> // abs() 함수 사용을 위해

// --- 핀 정의 (L298N 연결 가정) ---
// 왼쪽 모터 (Motor A)
#define IN1     PD4   // D4 -> IN1
#define IN2     PD3   // D3 -> IN2
#define PWM_A   PD5   // D5(OC0B) -> ENA

// 오른쪽 모터 (Motor B)
#define IN3     PD7   // D7 -> IN3
#define IN4     PB0   // D8 -> IN4 (주의: D8은 PORTB의 0번 핀입니다)
#define PWM_B   PD6   // D6(OC0A) -> ENB

void Motor_Init(void){
    // 1. 방향 핀 출력 설정 (DDR 설정)
    // D3, D4, D5, D6, D7은 PORTD에 속함
    DDRD |= (1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << PWM_A) | (1 << PWM_B);
    
    // D8은 PORTB에 속함 (중요!)
    DDRB |= (1 << IN4);

    // 2. PWM 설정 (Timer0 Fast PWM)
    // Fast PWM 모드, 비반전(Non-Inverting), 분주비 64
    // TCCR0A: COM0A1(1), COM0B1(1), WGM01(1), WGM00(1) -> Fast PWM
    // TCCR0B: CS01(1), CS00(1) -> Prescaler 64
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00);

    // 3. 초기 상태: 정지
    OCR0A = 0; // PWM Duty 0
    OCR0B = 0;
    
    // 방향 핀 초기화 (Low, Low = Free Stop)
    PORTD &= ~((1 << IN1) | (1 << IN2) | (1 << IN3));
    PORTB &= ~(1 << IN4);
}

void Motor_SetSpeed(int16_t left, int16_t right){
    // --- 왼쪽 모터 (Motor A) 제어 ---
    // IN1(High), IN2(Low) -> 정회전
    // IN1(Low), IN2(High) -> 역회전
    
    if(left >= 0){
        // 정회전: IN1 = H, IN2 = L
        PORTD |= (1 << IN1);
        PORTD &= ~(1 << IN2);
        
        // 속도 제한 (0~255)
        OCR0B = (left > 255) ? 255 : left; 
    } else {
        // 역회전: IN1 = L, IN2 = H
        PORTD &= ~(1 << IN1);
        PORTD |= (1 << IN2);
        
        // 음수 속도를 양수로 변환
        int16_t spd = -left;
        OCR0B = (spd > 255) ? 255 : spd;
    }

    // --- 오른쪽 모터 (Motor B) 제어 ---
    // IN3(High), IN4(Low) -> 정회전
    // IN3(Low), IN4(High) -> 역회전
    // 주의: IN3는 PORTD, IN4는 PORTB입니다.

    if(right >= 0){
        // 정회전: IN3 = H, IN4 = L
        PORTD |= (1 << IN3);
        PORTB &= ~(1 << IN4); // D8 pin (PORTB)
        
        OCR0A = (right > 255) ? 255 : right;
    } else {
        // 역회전: IN3 = L, IN4 = H
        PORTD &= ~(1 << IN3);
        PORTB |= (1 << IN4); // D8 pin (PORTB)
        
        int16_t spd = -right;
        OCR0A = (spd > 255) ? 255 : spd;
    }
}