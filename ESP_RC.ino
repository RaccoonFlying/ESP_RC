#include "ESP_RC.hpp"

void setup() {
  Serial.begin(115200); // debug via USB

  if (!beginWifiPortal()) {
    Serial.println("Falha ao iniciar o portal Wi-Fi.");
  }

  beginSbus();
  const bool controlsSet = setSbusControlChannels(1500, 1500, 1500, 1000);
  if (!controlsSet || !auxSet) ;//Serial.println("Falha ao inicializar os canais SBUS.");
  else ;//Serial.println("SBUS TX iniciado.");

}

void loop() {
  updateSbus();
}
