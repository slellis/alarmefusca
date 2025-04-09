#include <EEPROM.h>

#define LED_ALARME A1
#define LED_BOBINA A0
#define LED_STATUS 13
#define BUZZER 8
#define RELE_BOBINA 6
#define BOTAO_A 2  // INT0
#define BOTAO_B 3  // INT1
#define BOTAO_MANUAL 4
#define SENSOR_PORTA 5

#define EEPROM_ALARME_ADDR 0
#define EEPROM_BOBINA_ADDR 1

volatile bool alarmeAtivado = false;
volatile bool bobinaLiberada = false;

void salvarEstado() {
    EEPROM.update(EEPROM_ALARME_ADDR, alarmeAtivado);
    EEPROM.update(EEPROM_BOBINA_ADDR, bobinaLiberada);
    Serial.println("Estado salvo na EEPROM.");
    Serial.print("alarmeAtivado: ");
    Serial.println(alarmeAtivado);
    Serial.print("bobinaLiberada: ");
    Serial.println(bobinaLiberada);
}

void restaurarEstado() {
    alarmeAtivado = EEPROM.read(EEPROM_ALARME_ADDR);
    bobinaLiberada = EEPROM.read(EEPROM_BOBINA_ADDR);
    Serial.print("Estado restaurado: Alarme ");
    Serial.print(alarmeAtivado ? "ATIVADO" : "DESATIVADO");
    Serial.print(", Bobina ");
    Serial.println(bobinaLiberada ? "LIBERADA" : "BLOQUEADA");
}

void ativarAlarme() {
    // Debounce
    static unsigned long lastActivation = 0;
    if (millis() - lastActivation < 200) return; // Debounce de 200ms
    lastActivation = millis();

    alarmeAtivado = true;
    bobinaLiberada = false;
    digitalWrite(LED_ALARME, HIGH);
    digitalWrite(RELE_BOBINA, HIGH);
    digitalWrite(LED_BOBINA, HIGH);
    tone(BUZZER, 1000, 100); // BIP curto
    Serial.println("ALARME ATIVADO: LED piscando a cada 3s, buzzer emitindo 1 bip, motor bloqueado.");
    salvarEstado();
}

void desativarAlarme() {
    // Debounce
    static unsigned long lastDeactivation = 0;
    if (millis() - lastDeactivation < 200) return; // Debounce de 200ms
    lastDeactivation = millis();

    alarmeAtivado = false;
    digitalWrite(LED_ALARME, LOW);
    noTone(BUZZER);
    Serial.println("ALARME DESATIVADO: LED piscando a cada 1s, buzzer emitindo 2 bips.");
    salvarEstado();
}



void liberarBobina() {
    if (!alarmeAtivado) {
        bobinaLiberada = true;
        digitalWrite(RELE_BOBINA, LOW);
        digitalWrite(LED_BOBINA, LOW);
        Serial.println("BOBINA LIBERADA: Motor desbloqueado, LED apagado.");
        salvarEstado();
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(LED_ALARME, OUTPUT);
    pinMode(LED_BOBINA, OUTPUT);
    pinMode(LED_STATUS, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(RELE_BOBINA, OUTPUT);
    pinMode(BOTAO_A, INPUT);
    pinMode(BOTAO_B, INPUT);
    pinMode(BOTAO_MANUAL, INPUT_PULLUP);
    pinMode(SENSOR_PORTA, INPUT_PULLUP);
    
    restaurarEstado();
    
    digitalWrite(LED_ALARME, alarmeAtivado ? HIGH : LOW);
    digitalWrite(RELE_BOBINA, bobinaLiberada ? LOW : HIGH);
    digitalWrite(LED_BOBINA, bobinaLiberada ? LOW : HIGH);
    
    attachInterrupt(digitalPinToInterrupt(BOTAO_A), ativarAlarme, RISING);
    attachInterrupt(digitalPinToInterrupt(BOTAO_B), desativarAlarme, RISING);
}

void loop() {
    static unsigned long prevMillisLED = 0;
    static bool ledStatus = false;
    unsigned long currentMillis = millis();
    
    // LED de Status piscando a cada 1s
    if (currentMillis - prevMillisLED >= 1000) {
        prevMillisLED = currentMillis;
        ledStatus = !ledStatus;
        digitalWrite(LED_STATUS, ledStatus);
    //    Serial.println("LED de Status: piscando a cada 1 segundo.");
    }
    
    // Verificação do botão manual
    if (digitalRead(BOTAO_MANUAL) == LOW) {
        liberarBobina();
        delay(500);  // Debounce
    }
    
    // Detecção da abertura da porta
    if (digitalRead(SENSOR_PORTA) == LOW && alarmeAtivado) {
        Serial.println("!!! ALERTA !!! PORTA ABERTA - SIRENE ATIVADA!");
        tone(BUZZER, 1500);
        while (digitalRead(BOTAO_B) != HIGH) {
            delay(100);
        }
        noTone(BUZZER);
        Serial.println("Alarme Resetado.");
    }
}
