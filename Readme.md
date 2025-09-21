# Basic Software (BSW) Modules

이 프로젝트의 BSW 모듈은 아두이노에서 하드웨어 추상화를 담당하며,  
상위 Application(main.c)에서는 단순히 API 호출만으로 모터/스티어링/통신을 제어할 수 있습니다.

---

## 📌 Steering Module (`steering.h / steering.c`)

### Functions
- `void Steering_Init(void);`  
  - 스티어링 서보 초기화 (Timer1, 50Hz PWM 설정)  
  - 초기 각도를 중앙(0도)으로 설정  

- `void Steering_SetAngle(int16_t angle);`  
  - 스티어링 각도 설정 함수  
  - 입력 범위: **-90° ~ +90°**  
  - 범위를 벗어나면 자동으로 제한 (clamp)  
  - 내부적으로 -90~90을 PWM 펄스폭(1.0ms~2.0ms)으로 변환  

---

## 📌 Motor Module (`motor.h / motor.c`)

### Functions
- `void Motor_Init(void);`  
  - 좌/우 모터 제어 핀(DIR_A, PWM_A, DIR_B, PWM_B)을 출력으로 설정  
  - Timer0 PWM 초기화  
  - 초기 상태는 정지  

- `void Motor_SetSpeed(int16_t left, int16_t right);`  
  - 좌/우 모터 속도 및 방향 제어  
  - 입력 범위: **-255 ~ +255**  
    - 양수: 정방향  
    - 음수: 역방향  
    - 절댓값: 속도 (PWM 듀티)  
  - 내부적으로 `OCR0A`, `OCR0B`에 PWM 듀티 적용  

---

## 📌 UART Module (`uart.h / uart.c`)

### Functions
- `void uart_init(uint16_t ubrr);`  
  - UART 초기화 함수  
  - 보레이트 = `F_CPU / 16 / baud - 1`  

- `uint8_t uart_available(void);`  
  - 수신 버퍼에 데이터가 있으면 1, 없으면 0 반환  

- `uint8_t uart_receive(void);`  
  - 수신된 1바이트를 읽기 (블로킹 방식)  

- `uint8_t uart_receive_packet(int16_t *left, int16_t *right, int16_t *steer);`  
  - Jetson → Arduino 패킷 수신  
  - **텍스트 기반 (기본 활성화)**  
    - 예: `"L=-120,R=200,S=30\n"`  
    - 파싱 후 세 변수에 저장  
  - **바이너리 기반 (주석 처리, 선택 가능)**  
    - 포맷: `[0xAA][L_low][L_high][R_low][R_high][S_low][S_high][CHK]`  
    - 각 값은 `int16_t`  
    - 체크섬 검증 포함  

---

## 📌 Application Layer (`main.c`)

Application은 위 API들을 호출해 동작을 제어합니다.  