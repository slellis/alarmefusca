#include <SPI.h>
#include <MFRC522.h>

// Definições dos pinos
#define RST_PIN 4
#define SS_PIN 5
#define LED_PIN 13
#define BUZZER_PIN 27
#define TRIG_PIN 25
#define ECHO_PIN 26

// Configuração do módulo RC522
MFRC522 rfid(SS_PIN, RST_PIN);

// UID autorizado
byte authorizedUID[] = {0x36, 0xAC, 0xB3, 0xF9};

// Variáveis de estado
bool isAlarmActive = false;
bool isMotionDetected = false;
unsigned long motionStartTime = 0;
const unsigned long motionTimeout = 5000; // 5 segundos para desativar o alarme
bool isAlarmTriggered = false; // Controla se o alarme foi disparado


void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("Sistema inicializado. Passe a tag RFID para ligar/desligar o alarme.");
}

void loop() {
  // Verifica leitura de tag
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    if (checkAuthorizedUID()) {
      toggleAlarm();
    }
    rfid.PICC_HaltA();
  }

  // Verifica movimento
  if (isAlarmActive) {
    detectMotion();
    if (isMotionDetected) {
      handleMotion();
    }
  }
}

// Função para verificar se a tag lida é autorizada
bool checkAuthorizedUID() {
  if (rfid.uid.size != sizeof(authorizedUID)) {
    return false;
  }
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] != authorizedUID[i]) {
      return false;
    }
  }
  return true;
}

// Função para alternar o estado do alarme
void toggleAlarm() {
  isAlarmActive = !isAlarmActive;
  digitalWrite(LED_PIN, isAlarmActive ? HIGH : LOW);

  if (isAlarmActive) {
    Serial.println("Alarme ativado.");
    soundBuzzerPattern(1, 200, 0);       // 1 bip curto
    soundBuzzerPattern(5, 200, 200);    // 5 bips curtos
    soundBuzzerPattern(1, 1000, 0);     // 1 bip longo
  } else {
    Serial.println("Alarme desativado.");
    soundBuzzerPattern(1, 200, 0);      // 1 bip curto
  }
  isMotionDetected = false;
}

// Função para detectar movimento com o HC-SR04
void detectMotion() {
  long duration;
  int distance;

  // Envia um pulso de 10 microsegundos pelo pino TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Lê o tempo do pulso no pino ECHO
  duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout de 30 ms

  // Calcula a distância em centímetros
  distance = duration * 0.034 / 2;

  if (distance >= 10 && distance <= 50) {
    if (!isMotionDetected) {
      isMotionDetected = true;
      motionStartTime = millis();
      Serial.println("Movimento detectado! Aguardando desativação.");
      soundBuzzerPatternWithCheck(5, 200, 200); // 5 bips curtos com verificação de tag
    }
  }
}

// Função para tratar movimento detectado
void handleMotion() {
  if (millis() - motionStartTime > motionTimeout && !isAlarmTriggered) {
    activatePermanentBuzzer();
    isAlarmTriggered = true; // Marca o alarme como disparado
  }
}

// Função para ativar o buzzer permanentemente
void activatePermanentBuzzer() {
  Serial.println("Alarme disparado! Buzzer ativado permanentemente.");
  tone(BUZZER_PIN, 1000); // Bip permanente
}

// Função para emitir padrões de som no buzzer com verificação de tag
void soundBuzzerPatternWithCheck(int times, int duration, int interval) {
  for (int i = 0; i < times; i++) {
    // Verifica se uma tag válida foi lida durante o padrão de som
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial() && checkAuthorizedUID()) {
      toggleAlarm();
      rfid.PICC_HaltA();
      return; // Sai da função se o alarme for desativado
    }
    tone(BUZZER_PIN, 1000, duration);
    //delay(duration + interval);
    delay(duration);       // Duração do bip
    delay(1000 - duration); // Complemento até 1 segundo total
  }
}

// Função para emitir padrões de som no buzzer
void soundBuzzerPattern(int times, int duration, int interval) {
  for (int i = 0; i < times; i++) {
    tone(BUZZER_PIN, 1000, duration);
    delay(duration);       // Duração do bip
    delay(1000 - duration); // Complemento até 1 segundo total
  }
}

