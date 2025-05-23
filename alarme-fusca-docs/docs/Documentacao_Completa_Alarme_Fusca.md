# Projeto: Alarme Automotivo para Fusca

## Visão Geral
Este projeto implementa um sistema de alarme automotivo completo baseado em Arduino Nano, com controle via RF, sirene piezoelétrica, relé de bloqueio da bobina e EEPROM para armazenamento dos estados. O sistema foi dividido em duas placas perfuradas: uma placa lógica (central de controle) e uma placa de potência (relé + sirene), para melhor organização e desempenho.

## Especificações do Sistema

## Microcontrolador e Alimentação
- Arduino Nano
- Alimentação 12V automotivo, com conversores MP1584

## Entradas e Saídas
- D6 → Relé de bloqueio da bobina
- D9 → Controle da sirene (via TIP122)
- A0 → LED bobina
- A1 → LED alarme
- D13 → LED status (piscando a cada 1s)
- D2 → Botão A (RF)
- D3 → Botão B (RF)
- D4 → Botão manual
- D5 → Sensor de porta
- EEPROM para salvar estados

## Comportamento do Sistema

## 1. Ativar Alarme (Botão A / D2)
- LED alarme pisca a cada 3s
- Sirene emite 1 bip curto
- Relé desliga a bobina
- Estado salvo na EEPROM

## 2. Desativar Alarme (Botão B / D3)
- LED alarme pisca a cada 1s
- Sirene emite 2 bips curtos
- Bobina permanece bloqueada
- Estado salvo na EEPROM

## 3. Liberar Bobina (Botão D4)
- Somente se alarme desativado
- Relé libera bobina
- LED bobina apaga
- Estado salvo na EEPROM

## 4. Disparo por Porta (Sensor D5)
- Se alarme ativado e porta aberta
- Sirene dispara continuamente até botão B ser pressionado

## Separação em Placas

## Placa 1 – Central Lógica
- Arduino, LEDs, botões, EEPROM
- MP1584 para alimentação 5V
- Saídas D6 e D9 conectadas à outra placa

## Placa 2 – Potência
- Relé HW-307
- TIP122
- Diodos de flyback
- Conectores para +12V e GND com fios 1,5mm²

## Observações Elétricas
- Fios 1,5mm² para cargas, AWG 22 para sinais
- GND compartilhado entre Arduino, relé e sirene
- Sensor de porta usa interruptor original do Fusca

## Arquivos relacionados
- Placa_Potencia_Alarme_Fusca.md
- Tutorial_MkDocs_Instalacao_e_Uso.md
