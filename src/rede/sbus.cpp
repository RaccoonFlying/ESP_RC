#include "sbus.hpp"

namespace {
  constexpr int SBUS_TX_PIN = 17;
  constexpr int SBUS_RX_PIN = 18;
  constexpr int SBUS_UART_NUM = 1;
  constexpr uint32_t SBUS_FRAME_INTERVAL_MS = 9;


HardwareSerial sbusSerial(SBUS_UART_NUM);

uint16_t channels[16] = {
  992, 992, 192, 992, 992, 992, 992, 992,
  992, 992, 992, 992, 992, 992, 992, 992
};

bool failsafe = false;
bool frameLost = false;
uint32_t lastFrameTime = 0;

void buildSbusFrame(uint8_t packet[25]) {
  packet[0] = 0x0F; // start byte

  packet[1]  = (uint8_t)(channels[0] & 0x07FF);
  packet[2]  = (uint8_t)((channels[0]  >> 8 | channels[1]  << 3) & 0xFF);
  packet[3]  = (uint8_t)((channels[1]  >> 5 | channels[2]  << 6) & 0xFF);
  packet[4]  = (uint8_t)((channels[2]  >> 2) & 0xFF);
  packet[5]  = (uint8_t)((channels[2]  >> 10 | channels[3] << 1) & 0xFF);
  packet[6]  = (uint8_t)((channels[3]  >> 7 | channels[4]  << 4) & 0xFF);
  packet[7]  = (uint8_t)((channels[4]  >> 4 | channels[5]  << 7) & 0xFF);
  packet[8]  = (uint8_t)((channels[5]  >> 1) & 0xFF);
  packet[9]  = (uint8_t)((channels[5]  >> 9 | channels[6]  << 2) & 0xFF);
  packet[10] = (uint8_t)((channels[6]  >> 6 | channels[7]  << 5) & 0xFF);
  packet[11] = (uint8_t)((channels[7]  >> 3) & 0xFF);
  packet[12] = (uint8_t)((channels[8]  & 0x07FF));
  packet[13] = (uint8_t)((channels[8]  >> 8 | channels[9]  << 3) & 0xFF);
  packet[14] = (uint8_t)((channels[9]  >> 5 | channels[10] << 6) & 0xFF);
  packet[15] = (uint8_t)((channels[10] >> 2) & 0xFF);
  packet[16] = (uint8_t)((channels[10] >> 10 | channels[11] << 1) & 0xFF);
  packet[17] = (uint8_t)((channels[11] >> 7 | channels[12] << 4) & 0xFF);
  packet[18] = (uint8_t)((channels[12] >> 4 | channels[13] << 7) & 0xFF);
  packet[19] = (uint8_t)((channels[13] >> 1) & 0xFF);
  packet[20] = (uint8_t)((channels[13] >> 9 | channels[14] << 2) & 0xFF);
  packet[21] = (uint8_t)((channels[14] >> 6 | channels[15] << 5) & 0xFF);
  packet[22] = (uint8_t)((channels[15] >> 3) & 0xFF);

  uint8_t flags = 0;
  if (frameLost) flags |= (1 << 2);
  if (failsafe)  flags |= (1 << 3);
  packet[23] = flags;

  packet[24] = 0x00; // end byte
}

void sendSbusFrame() {
  uint8_t packet[25];
  buildSbusFrame(packet);
  sbusSerial.write(packet, 25);
}
}

void beginSbus() {
  sbusSerial.begin(100000, SERIAL_8E2, SBUS_RX_PIN, SBUS_TX_PIN, true);
}

bool setSbusControlChannels(uint16_t roll, uint16_t pitch, uint16_t yaw, uint16_t throttle) {
  auto inRange = [](uint16_t v) { return v >= 1000 && v <= 2000; };
  if (!inRange(roll) || !inRange(pitch) || !inRange(yaw) || !inRange(throttle)) {
    return false;
  }

  channels[0] = usToSbus(roll);
  channels[1] = usToSbus(pitch);
  channels[2] = usToSbus(throttle);
  channels[3] = usToSbus(yaw);
  return true;
}

void updateSbus() {
  const uint32_t now = millis();
  if (now - lastFrameTime >= SBUS_FRAME_INTERVAL_MS) {
    lastFrameTime = now;
    sendSbusFrame();
  }
}

uint16_t usToSbus(uint16_t us) {
  us = constrain(us, 1000, 2000);
  return (uint16_t)((us - 880) * 1.6f + 0.5f);  // 1000->192, 1500->992, 2000->1792
}