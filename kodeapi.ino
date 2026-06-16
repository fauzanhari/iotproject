#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi
const char* ssid = "fauzanhari";
const char* password = "11111111";

WebServer server(80);

#define MQ2_PIN 32
#define LED_HIJAU 25
#define LED_MERAH 26
#define BUZZER 27

int batasGas = 1800;
int nilaiGas = 0;
String statusGas = "AMAN";

void handleRoot() {

  String html = "<!DOCTYPE html><html>";
  html += "<head><meta http-equiv='refresh' content='2'>";
  html += "<title>Deteksi Gas ESP32</title></head>";
  html += "<body style='font-family:Arial;text-align:center;'>";

  html += "<h1>SISTEM DETEKSI GAS</h1>";

  html += "<h2>Nilai Gas: ";
  html += String(nilaiGas);
  html += "</h2>";

  html += "<h2>Status: ";
  html += statusGas;
  html += "</h2>";

  if(statusGas == "BAHAYA"){
    html += "<h2 style='color:red;'>BAHAYA GAS!</h2>";
  } else {
    html += "<h2 style='color:green;'>AMAN</h2>";
  }

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {

  Serial.begin(115200);

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Menghubungkan");
  lcd.setCursor(0,1);
  lcd.print("WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Terhubung");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IP:");
  lcd.print(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();

  delay(3000);
}

void loop() {

  server.handleClient();

  nilaiGas = analogRead(MQ2_PIN);

  Serial.print("Nilai Gas = ");
  Serial.println(nilaiGas);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gas:");
  lcd.print(nilaiGas);

  if(nilaiGas > batasGas){

    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(BUZZER, HIGH);

    statusGas = "BAHAYA";

    lcd.setCursor(0,1);
    lcd.print("BAHAYA GAS!!!");

  }
  else{

    digitalWrite(LED_HIJAU, HIGH);
    digitalWrite(LED_MERAH, LOW);
    digitalWrite(BUZZER, LOW);

    statusGas = "AMAN";

    lcd.setCursor(0,1);
    lcd.print("STATUS: AMAN");
  }

  delay(500);
}