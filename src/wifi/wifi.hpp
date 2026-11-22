#ifndef WIFI_HPP
#define WIFI_HPP

#include <Arduino.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFi.h>

/**
 * Inicia o ponto de acesso Wi-Fi protegido e o portal cativo.
 *
 * @return true se o ponto de acesso, o DNS e o servidor HTTP forem iniciados.
 */
bool beginWifiPortal();

/**
 * Processa as consultas DNS e as requisicoes HTTP do portal cativo.
 *
 * Deve ser chamada repetidamente dentro de loop().
 */
void updateWifiPortal();

#endif // WIFI_HPP
