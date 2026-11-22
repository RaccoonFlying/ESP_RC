#include "wifi.hpp"

//#include "../ihm/index_html.hpp"
//#include "../ihm/style_css.hpp"
//#include "../ihm/script_js.hpp"
#include "../rede/sbus.hpp"

namespace {
  const char *const WIFI_SSID = "ESP_RC";
  const char *const WIFI_PASSWORD = "??????";
  const char *const PORTAL_URL = "http://192.168.4.1/";
  const IPAddress PORTAL_IP(192, 168, 4, 1);
  const IPAddress SUBNET_MASK(255, 255, 255, 0);
  constexpr uint8_t DNS_PORT = 53;
  constexpr uint32_t CONTROL_TIMEOUT_MS = 500;

  DNSServer dnsServer;
  WebServer webServer(80);
  bool portalStarted = false;
  bool controlActive = false;
  uint32_t lastControlUpdate = 0;
}

bool beginWifiPortal() {
  if (portalStarted) {
    return true;
  }

  WiFi.mode(WIFI_AP);
  if (!WiFi.softAPConfig(PORTAL_IP, PORTAL_IP, SUBNET_MASK)) {
    Serial.println("Falha ao configurar o endereco IP do ponto de acesso.");
    return false;
  }

  if (!WiFi.softAP(WIFI_SSID, WIFI_PASSWORD)) {
    Serial.println("Falha ao iniciar o ponto de acesso Wi-Fi.");
    return false;
  }

  if (!dnsServer.start(DNS_PORT, "*", PORTAL_IP)) {
    Serial.println("Falha ao iniciar o DNS do portal cativo.");
    return false;
  }

  webServer.onNotFound([]() {
    webServer.sendHeader("Location", PORTAL_URL, true);
    webServer.send(302, "text/plain", "");
  });
  webServer.begin();
  portalStarted = true;

  Serial.print("Ponto de acesso iniciado: ");
  Serial.println(WIFI_SSID);
  Serial.print("Portal cativo: ");
  Serial.println(PORTAL_URL);
  return true;
}

void updateWifiPortal() {
  if (!portalStarted) {
    return;
  }

  dnsServer.processNextRequest();
  webServer.handleClient();
}
