#ifndef SBUS_HPP
#define SBUS_HPP

#include <Arduino.h>
#include <HardwareSerial.h>

// Inicia a UART para enviar frames SBUS
void beginSbus();

/**
 * Define os valores SBUS dos canais de roll, pitch, yaw e throttle
 *
 * @param roll Valor do canal de roll (0 a 2047)
 * @param pitch Valor do canal de pitch (0 a 2047)
 * @param yaw Valor do canal de yaw (0 a 2047)
 * @param throttle Valor do canal de throttle (0 a 2047)
 * @return false se algum valor exceder os 11 bits do protocolo.
 */
bool setSbusControlChannels(uint16_t roll, uint16_t pitch, uint16_t yaw, uint16_t throttle);

// Atualiza os canais SBUS com os valores definidos anteriormente.
void updateSbus();

// Converte um valor em microssegundos (1000 a 2000) para o valor SBUS correspondente (0 a 2047).
uint16_t usToSbus(uint16_t us);

#endif // SBUS_HPP