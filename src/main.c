#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "motor.h"
#include "steering.h"

int main(void){
    int16_t left_speed = 0;
    int16_t right_speed = 0;
    int16_t steer_angle = 0;

    // --- 초기화 ---
    uart_init(F_CPU/16/9600 - 1);   // UART 9600bps
    Motor_Init();                   // 모터 드라이버 초기화
    Steering_Init();                // 스티어링 초기화

    // --- 메인 루프 ---
    while(1){
        // Jetson에서 패킷 수신 → 세 변수에 저장
        if(uart_receive_packet(&left_speed, &right_speed, &steer_angle)){
            // 수신된 값 바로 반영
            Motor_SetSpeed(left_speed, right_speed);
            Steering_SetAngle(steer_angle);
        }
    }
}
