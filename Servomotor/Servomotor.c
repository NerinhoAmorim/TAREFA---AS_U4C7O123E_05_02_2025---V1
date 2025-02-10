#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definições de pinos e constantes
#define SERVO_PIN 22 // Pino do servomotor
#define LED_PIN 12   // Pino do LED
#define PWM_FREQ 50 // 50Hz -> Período de 20ms
#define PWM_WRAP 20000 // Para um clock de 1MHz, 20ms = 20000 ticks

// Função para configurar o PWM
void setup_pwm() {
    // Configura o pino do servomotor como PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num_servo = pwm_gpio_to_slice_num(SERVO_PIN);
    
    // Configuração do PWM
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f); // Ajuste para 1MHz (125MHz/125)
    pwm_config_set_wrap(&config, PWM_WRAP);
    pwm_init(slice_num_servo, &config, true);
    
    // Configura o pino do LED como PWM
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    uint slice_num_led = pwm_gpio_to_slice_num(LED_PIN);
    pwm_init(slice_num_led, &config, true); // Usa a mesma configuração do PWM
}

// Função para definir o ciclo ativo do PWM
void set_pwm_duty(uint pin, uint duty) {
    pwm_set_gpio_level(pin, duty);
}

int main() {
    stdio_init_all(); // Inicializa a entrada e saída padrão
    setup_pwm(); // Configura o PWM para o servomotor e o LED

    // Controle de ângulo do servo
    printf("Ângulo mudando para 180 graus\n");
    set_pwm_duty(SERVO_PIN, 2400); // 2400μs para 180 graus
    set_pwm_duty(LED_PIN, 2400); // Brilho máximo do LED
    sleep_ms(5000); // Aguarda 5 segundos

    printf("Ângulo mudando para 90 graus\n");
    set_pwm_duty(SERVO_PIN, 1470); // 1470μs para 90 graus
    set_pwm_duty(LED_PIN, 1470); // Brilho do LED para 90 graus
    sleep_ms(5000); // Aguarda 5 segundos

    printf("Ângulo mudando para 0 graus\n");
    set_pwm_duty(SERVO_PIN, 500); // 500μs para 0 graus
    set_pwm_duty(LED_PIN, 500); // Brilho do LED para 0 graus
    sleep_ms(5000); // Aguarda 5 segundos

    // Movimentação periódica entre 0 e 180 graus
    while (true) {
        // Incrementa de 0 para 180 graus
        for (uint duty = 500; duty <= 2400; duty += 5) {
            set_pwm_duty(SERVO_PIN, duty); // Ajusta o ângulo do servomotor
            set_pwm_duty(LED_PIN, duty); // Ajusta o brilho do LED conforme o ângulo
            sleep_ms(10); // Atraso de 10ms
        }
        
        // Decrementa de 180 para 0 graus
        for (uint duty = 2400; duty >= 500; duty -= 5) {
            set_pwm_duty(SERVO_PIN, duty); // Ajusta o ângulo do servomotor
            set_pwm_duty(LED_PIN, duty); // Ajusta o brilho do LED conforme o ângulo
            sleep_ms(10); // Atraso de 10ms
        }
    }

    return 0; // Finaliza o programa
}