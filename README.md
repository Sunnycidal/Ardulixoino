# Lixeira Automática com Arduino Uno

Projeto desenvolvido para a disciplina **Conceitos de Computação** do curso de Análise e Desenvolvimento de Sistemas — Centro Universitário Senac (2026).

> Tecnologia Frugal: fazer mais com menos.

---

## Sobre o Projeto

Uma lixeira automática que abre a tampa sem contato físico, utilizando um sensor ultrassônico para detectar a aproximação da mão e um servo motor para acionar a tampa. O projeto foi construído com papelão e eletrônica de entrada, demonstrando que é possível criar automação inteligente com baixo custo.

Este repositório contém a **Fase 1** do projeto (automação local). A Fase 2 prevê integração com ESP32 e conectividade Wi-Fi para monitoramento remoto via IoT.

**Referência base:** [youtube.com/watch?v=h7AmHDmgpJY](https://www.youtube.com/watch?v=h7AmHDmgpJY)

---

## Como Funciona

O sistema opera como uma **Máquina de Estados** com três estados:

| Estado | Descrição |
|--------|-----------|
| `FECHADO` | Lixeira parada, monitorando o sensor |
| `ABERTO` | Tampa aberta, aguardando a mão se afastar |
| `COOLDOWN` | Pausa de segurança após fechar, evita reativações acidentais |

**Fluxo resumido:**
1. Sensor detecta mão a ≤ 20 cm
2. Servo abre a tampa
3. Sistema aguarda tempo mínimo e confirmação de que a mão saiu
4. Tampa fecha automaticamente
5. Cooldown de 2 segundos antes de aceitar novo uso

---

## Componentes

| Componente | Quantidade | Custo Médio |
|------------|:----------:|:-----------:|
| Arduino Uno | 1 | R$ 35,00 |
| Sensor HC-SR04 | 1 | R$ 19,00 |
| Servo Motor SG90 | 1 | R$ 20,00 |
| Kit Jumpers | 20 | R$ 28,00 |
| **Total** | | **R$ 102,00** |

---

## Conexões

| Componente | Pino do Componente | Arduino | Cor do Fio |
|---|---|---|---|
| HC-SR04 | VCC | 5V | Vermelho |
| HC-SR04 | GND | GND | Preto |
| HC-SR04 | TRIG | Digital 7 | Azul |
| HC-SR04 | ECHO | Digital 6 | Verde |
| SG90 | VCC | 5V | Vermelho |
| SG90 | GND | GND | Marrom/Preto |
| SG90 | PWM | Digital 9 | Laranja/Amarelo |

---

## Como Usar

1. Monte o circuito conforme a tabela de conexões acima
2. Abra o arquivo `.ino` na Arduino IDE
3. Selecione a placa **Arduino Uno** e a porta correta
4. Faça o upload do código
5. Abra o **Serial Monitor** (9600 baud) para acompanhar os logs em tempo real

---

## Estrutura do Repositório

```
ardulixoino/
├── lixeira_automatica.ino   # Código principal
└── README.md
```

---

## Melhorias Futuras

**Curto prazo:**
- Buzzer sonoro de feedback
- Sensor de capacidade (lixeira cheia)
- Estrutura em acrílico

**Longo prazo (IoT):**
- Integração com ESP32 e Wi-Fi
- Notificações no celular via app
- Monitoramento remoto
- Recarga por energia solar

---

## Autores

- Altino Ávila
- Guilherme Diniz
- Samuel
- Jessiê

**Orientador:** Fábio Brussolo  
**Instituição:** Centro Universitário Senac — ADS
