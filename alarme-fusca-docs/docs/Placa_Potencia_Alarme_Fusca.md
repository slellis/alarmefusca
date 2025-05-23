# Projeto: Placa de Potência do Alarme Automotivo (Relê da Bobina + Sirene)

## Objetivo
Montar uma placa perfurada dedicada aos circuitos de potência do sistema de alarme do Fusca.

## Componentes Utilizados

| Componente       | Quantidade | Observações                          |
|------------------|------------|--------------------------------------|
| TIP122           | 1          | Controle da sirene                   |
| Relê HW-307      | 1          | Bloqueio da bobina                   |
| Diodo 1N4007     | 2          | Flyback para sirene e relê           |
| Resistor 1kOhm   | 1          | Limitador de corrente da base TIP122 |
| Bornes p/ 12V    | 2          | Alimentação                          |
| Conectores de sinal | 2       | Entradas D6 e D9                     |
| Placa perfurada  | 1          | Montagem                             |
| Fios e solda     | -          | Conexões internas                    |

## Ligações Elétricas

## Alimentação
- +12V e GND conectados por fios 1,5mm²

## Relê da Bobina (HW-307)
- IN: sinal D6
- COM: bobina
- NA: continuidade
- Diodo 1N4007 entre VCC e GND (cátodo no +12V)

## Sirene
- Coletor do TIP122 no negativo da sirene
- Emissor no GND
- Resistor 1kOhm entre base e D9
- Diodo 1N4007 em paralelo com a sirene

## Checklist de Montagem

## Etapas
- [ ] Fixar relê e TIP122
- [ ] Conectar alimentação
- [ ] Soldar resistor
- [ ] Ligar sirene e bobina
- [ ] Soldar diodos
- [ ] Testar os dois canais

## Observações Técnicas
- Fios de sinal podem ser AWG 22
- GND comum com Arduino
