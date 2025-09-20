#include "uart.h"
#include <avr/io.h>
#include <stdlib.h>   // atoi() 사용
#include <string.h>   // strtok() 사용

// --------------------- 기본 UART 함수 ---------------------

void uart_init(uint16_t ubrr){
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0);                       // 수신만 활성화
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);      // 8비트 데이터, 1스톱비트, 비패리티
}

uint8_t uart_available(void){
    return (UCSR0A & (1 << RXC0));   // 수신 완료 플래그
}

uint8_t uart_receive(void){
    while (!(UCSR0A & (1 << RXC0))); // 데이터 들어올 때까지 대기
    return UDR0;
}

// --------------------- 텍스트 기반 패킷 ---------------------
// Jetson이 보낸 문자열 예시: "L=-120,R=200,S=30\n"
uint8_t uart_receive_packet(int16_t *left, int16_t *right, int16_t *steer){
    static char buf[32];
    static uint8_t idx = 0;

    if(!uart_available()) return 0;

    char c = uart_receive();
    if(c == '\n' || c == '\r'){   // 패킷 종료
        buf[idx] = '\0';
        idx = 0;

        // 문자열 파싱
        char *tok;
        tok = strtok(buf, ",");
        while(tok != NULL){
            if(strncmp(tok, "L=", 2) == 0) *left  = atoi(tok+2);
            else if(strncmp(tok, "R=", 2) == 0) *right = atoi(tok+2);
            else if(strncmp(tok, "S=", 2) == 0) *steer = atoi(tok+2);
            tok = strtok(NULL, ",");
        }
        return 1;   // 성공
    }
    else {
        if(idx < sizeof(buf)-1) buf[idx++] = c;
    }
    return 0;
}

/* --------------------- 바이너리 기반 패킷 ---------------------
   Jetson이 보낸 데이터 포맷 (총 8바이트):
   [0xAA][L_low][L_high][R_low][R_high][S_low][S_high][CHK]

   기존 텍스트 기반 패킷보다 더 빠르고 안정적인 통신 가능 (우선 테스트 후 적용 예정)
   - HEADER = 0xAA
   - L, R, S = int16_t (little endian)
   - CHK = (L_low ^ L_high ^ R_low ^ R_high ^ S_low ^ S_high)
-----------------------------------------------------------------
uint8_t uart_receive_packet(int16_t *left, int16_t *right, int16_t *steer){
    static uint8_t state = 0;
    static uint8_t buf[8];
    static uint8_t idx = 0;

    if(!uart_available()) return 0;
    uint8_t c = uart_receive();

    switch(state){
        case 0: // HEADER 대기
            if(c == 0xAA){
                buf[0] = c;
                idx = 1;
                state = 1;
            }
            break;

        case 1: // 나머지 7바이트 수신
            buf[idx++] = c;
            if(idx >= 8){
                // 체크섬 확인
                uint8_t chk = buf[1]^buf[2]^buf[3]^buf[4]^buf[5]^buf[6];
                if(chk == buf[7]){
                    *left  = (int16_t)((buf[2] << 8) | buf[1]);
                    *right = (int16_t)((buf[4] << 8) | buf[3]);
                    *steer = (int16_t)((buf[6] << 8) | buf[5]);
                    state = 0;
                    return 1;
                }
                state = 0; // 실패 시 초기화
            }
            break;
    }
    return 0;
}
----------------------------------------------------------------- */
