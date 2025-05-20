#include <EEPROM.h>

#define LED_ALARME A1
#define LED_BOBINA A0
#define LED_STATUS 13
#define SIRENE 9
#define RELE_BOBINA 6
#define BOTAO_A 2  // INT0
#define BOTAO_B 3  // INT1
#define BOTAO_MANUAL 4
#define SENSOR_PORTA 5

#define EEPROM_ALARME_ADDR 0
#define EEPROM_BOBINA_ADDR 1
#define EEPROM_SIRENE_ADDR 2

volatile bool alarmeAtivado = false;
volatile bool bobinaLiberada = false;
volatile bool sireneAtivada = false;

void bipCurto(int quantidade) {
    Serial.print("Bip curto - bips: ");
    Serial.println(quantidade);
    // for (int i = 0; i < quantidade; i++) {
    //     digitalWrite(SIRENE, HIGH);
    //     delay(100);  // duração do bip
    //     digitalWrite(SIRENE, LOW);
    //     delay(500);  // pausa entre bips
    // }
    for (int i = 0; i < quantidade; i++) {
        digitalWrite(SIRENE, HIGH);
        delay(500);  // Aumentado
        digitalWrite(SIRENE, LOW);
        if (i < quantidade - 1) delay(300);
    }
}

void sireneDisparo(bool ligado) {
    Serial.println("*** Disparo de sirene ***");
    digitalWrite(SIRENE, ligado ? HIGH : LOW);
}

void salvarEstado() {
    EEPROM.update(EEPROM_ALARME_ADDR, alarmeAtivado);
    EEPROM.update(EEPROM_BOBINA_ADDR, bobinaLiberada);
    EEPROM.update(EEPROM_SIRENE_ADDR, sireneAtivada);
    Serial.println("Estado salvo na EEPROM.");
    Serial.print("alarmeAtivado: ");
    Serial.println(alarmeAtivado);
    Serial.print("bobinaLiberada: ");
    Serial.println(bobinaLiberada);
    Serial.print("sireneAtivada: ");
    Serial.println(sireneAtivada);
}

void restaurarEstado() {
    alarmeAtivado = EEPROM.read(EEPROM_ALARME_ADDR);
    bobinaLiberada = EEPROM.read(EEPROM_BOBINA_ADDR);
    sireneAtivada = EEPROM.read(EEPROM_SIRENE_ADDR);
    Serial.print("Estado restaurado: Alarme ");
    Serial.print(alarmeAtivado ? "ATIVADO" : "DESATIVADO");
    Serial.print(", Bobina ");
    Serial.print(bobinaLiberada ? "LIBERADA" : "BLOQUEADA");
    Serial.print(", Sirene ");
    Serial.println(sireneAtivada ? "ATIVADA" : "DESATIVADA");
}

void ativarAlarme() {
    static unsigned long lastActivation = 0;
    if (millis() - lastActivation < 200) return;
    lastActivation = millis();

    alarmeAtivado = true;
    bobinaLiberada = false;
    sireneAtivada = false;
    digitalWrite(LED_ALARME, HIGH);
    digitalWrite(RELE_BOBINA, HIGH);
    digitalWrite(LED_BOBINA, HIGH);
    bipCurto(1);
    Serial.println("ALARME ATIVADO: LED piscando a cada 3s, buzzer emitindo 1 bip, motor bloqueado.");
    salvarEstado();
}

void desativarAlarme() {
    static unsigned long lastDeactivation = 0;
    if (millis() - lastDeactivation < 200) return;
    lastDeactivation = millis();

    alarmeAtivado = false;
    sireneAtivada = false;
    digitalWrite(LED_ALARME, LOW);
    bipCurto(2);
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
    pinMode(SIRENE, OUTPUT);
    pinMode(RELE_BOBINA, OUTPUT);
    pinMode(BOTAO_A, INPUT);
    pinMode(BOTAO_B, INPUT);
    pinMode(BOTAO_MANUAL, INPUT_PULLUP);
    pinMode(SENSOR_PORTA, INPUT);
    
    restaurarEstado();

    digitalWrite(LED_ALARME, alarmeAtivado ? HIGH : LOW);
    digitalWrite(RELE_BOBINA, bobinaLiberada ? LOW : HIGH);
    digitalWrite(LED_BOBINA, bobinaLiberada ? LOW : HIGH);
    sireneDisparo(sireneAtivada);
    
    attachInterrupt(digitalPinToInterrupt(BOTAO_A), ativarAlarme, RISING);
    attachInterrupt(digitalPinToInterrupt(BOTAO_B), desativarAlarme, RISING);
}

void loop() {
    static unsigned long prevMillisLED = 0;
    static bool ledStatus = false;
    unsigned long currentMillis = millis();

    // Piscar LED de status
    if (currentMillis - prevMillisLED >= 1000) {
        prevMillisLED = currentMillis;
        ledStatus = !ledStatus;
        digitalWrite(LED_STATUS, ledStatus);
    }

    // Botão manual
    if (digitalRead(BOTAO_MANUAL) == LOW) {
        liberarBobina();
        delay(500);
    }

    // Porta aberta com alarme ativado
    if (digitalRead(SENSOR_PORTA) == HIGH && alarmeAtivado && !sireneAtivada) {
        Serial.println("!!! ALERTA !!! PORTA ABERTA - SIRENE ATIVADA!");
        sireneAtivada = true;
        sireneDisparo(true);
        salvarEstado();

        while (digitalRead(BOTAO_B) != HIGH) {
            delay(100);
        }

        sireneDisparo(false);
        sireneAtivada = false;
        Serial.println("Alarme Resetado.");
        salvarEstado();
    }
}
