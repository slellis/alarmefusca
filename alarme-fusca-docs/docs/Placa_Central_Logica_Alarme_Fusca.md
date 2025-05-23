# Projeto: Placa Central Lógica do Alarme Automotivo

## Objetivo
Montar uma placa perfurada responsável por todo o controle lógico do sistema de alarme automotivo do Fusca, incluindo entrada de botões e sensores, controle de LEDs, armazenamento de estado na EEPROM e saída para os atuadores de potência.

## Componentes Utilizados

| Componente           | Quantidade | Observações                                         |
|----------------------|------------|----------------------------------------------------|
| Arduino Nano         | 1          | Microcontrolador principal                         |
| Conversor MP1584     | 1          | Para converter 12V do carro em 5V estável          |
| Resistores (10kΩ)    | 3          | Pull-down para entradas (se necessário)            |
| LEDs                 | 3          | Indicadores: Alarme (A1), Bobina (A0), Status (D13)|
| Conectores de Sinal  | 6          | Para D2, D3, D4, D5, D6, D9                         |
| Capacitores (100nF + 470uF) | 1+1     | Desacoplamento na entrada de 5V                   |
| EEPROM interna       | -          | Usada pelo código para armazenar os estados        |
| Botões               | 3          | A (RF), B (RF), Liberação Manual                   |
| Conectores de alimentação | 1       | Entrada de +5V e GND do conversor MP1584           |

## Entradas e Saídas

## Entradas Digitais
- **D2**: Botão A (RF)
- **D3**: Botão B (RF)
- **D4**: Botão de liberação manual da bobina
- **D5**: Sensor de porta (com GND da lataria)

## Saídas Digitais
- **D6**: Controle do relé da bobina (vai para placa de potência)
- **D9**: Controle da sirene via TIP122 (vai para placa de potência)
- **A0**: LED indicador da bobina
- **A1**: LED indicador do alarme
- **D13**: LED de status (piscando a cada 1 segundo)

## Alimentação
- **Entrada**: 12V do carro
- **Conversão**: MP1584 reduz para 5V
- **Distribuição**: 5V alimenta o Arduino e os periféricos

## Ligações com a Placa de Potência
- **D6 → IN do relé (HW-307)**
- **D9 → Base do TIP122**
- **GND compartilhado**
- **+12V fornecido diretamente para a placa de potência**

## Observações Técnicas

## Organização e Proteção
- Capacitores de desacoplamento (100nF cerâmico + 470uF eletrolítico) devem ser usados entre 5V e GND próximos ao Arduino
- Os botões usam lógica HIGH → devem ser ligados ao +5V com pull-down se necessário
- O sensor de porta utiliza o aterramento da carroceria do Fusca (lataria)

## Interconexão
- Os sinais D6 e D9 devem ser levados à placa de potência com fios AWG 22
- O GND deve ser comum entre as duas placas e a fonte de 12V
