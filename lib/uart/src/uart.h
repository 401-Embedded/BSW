#ifndef UART_H
#define UART_H

#include <stdint.h>

/**
 * @brief UART 초기화
 * 
 * @param ubrr 보레이트 설정값 (F_CPU/16/baud - 1)
 */
void uart_init(uint16_t ubrr);

/**
 * @brief 수신 버퍼에 데이터가 있는지 확인
 * 
 * @return 1 = 데이터 있음, 0 = 없음
 */
uint8_t uart_available(void);

/**
 * @brief 수신된 1바이트 읽기 (블로킹)
 * 
 * @return 수신 데이터 (1바이트)
 */
uint8_t uart_receive(void);

/**
 * @brief Jetson으로부터 패킷 수신
 * 
 * @param left   왼쪽 모터 속도 (-255 ~ 255)
 * @param right  오른쪽 모터 속도 (-255 ~ 255)
 * @param steer  스티어링 각도 (-90 ~ 90)
 * 
 * @return 1 = 정상 수신, 0 = 실패
 */
uint8_t uart_receive_packet(int16_t *left, int16_t *right, int16_t *steer);

#endif // UART_H
