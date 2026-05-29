#define BLYNK_TEMPLATE_ID "TMPL6U2Nugctc"
#define BLYNK_TEMPLATE_NAME "iot solar panel"
#define BLYNK_AUTH_TOKEN "XQrk-3WZ5fj_CE3QpCMT3PgKMODLWjRL"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <Adafruit_INA219.h>

// ================= WIFI =================
char ssid[] = "Samsung";
char pass[] = "daffa1234";

// ================= INA219 =================
// Sensor Panel
Adafruit_INA219 ina219_panel(0x40);

// Sensor Load
Adafruit_INA219 ina219_load(0x41);

// ================= TIMER =================
BlynkTimer timer;

// =====================================================
// FUNGSI KIRIM DATA
// =====================================================
void sendSensor()
{
  // ========= PANEL =========
  float panelVoltage = ina219_panel.getBusVoltage_V();
  float panelCurrent = ina219_panel.getCurrent_mA();
  float panelPower = panelVoltage * panelCurrent; // mW

  // ========= LOAD =========
  float loadVoltage = ina219_load.getBusVoltage_V();
  float loadCurrent = ina219_load.getCurrent_mA();
  float loadPower = loadVoltage * loadCurrent; // mW

  // ================= SERIAL MONITOR =================
  Serial.println("========= SOLAR PANEL =========");

  Serial.print("Voltage : ");
  Serial.print(panelVoltage, 2);
  Serial.println(" V");

  Serial.print("Current : ");
  Serial.print(panelCurrent, 2);
  Serial.println(" mA");

  Serial.print("Power   : ");
  Serial.print(panelPower, 2);
  Serial.println(" mW");

  Serial.println();

  Serial.println("============= LOAD =============");

  Serial.print("Voltage : ");
  Serial.print(loadVoltage, 2);
  Serial.println(" V");

  Serial.print("Current : ");
  Serial.print(loadCurrent, 2);
  Serial.println(" mA");

  Serial.print("Power   : ");
  Serial.print(loadPower, 2);
  Serial.println(" mW");

  Serial.println("================================\n");

  // ================= BLYNK =================

  // PANEL
  Blynk.virtualWrite(V0, panelVoltage);
  Blynk.virtualWrite(V1, panelCurrent);
  Blynk.virtualWrite(V2, panelPower);

  // LOAD
  Blynk.virtualWrite(V3, loadVoltage);
  Blynk.virtualWrite(V4, loadCurrent);
  Blynk.virtualWrite(V5, loadPower);
}

// =====================================================
// SETUP
// =====================================================
void setup()
{
  Serial.begin(115200);

  // I2C ESP32
  Wire.begin(21, 22);

  // Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // INA219 PANEL
  if (!ina219_panel.begin())
  {
    Serial.println("INA219 PANEL gagal!");
    while (1);
  }

  // INA219 LOAD
  if (!ina219_load.begin())
  {
    Serial.println("INA219 LOAD gagal!");
    while (1);
  }

  Serial.println("Semua sensor siap");

  // Kirim data tiap 1 detik
  timer.setInterval(1000L, sendSensor);
}

// =====================================================
// LOOP
// =====================================================
void loop()
{
  Blynk.run();
  timer.run();
}
