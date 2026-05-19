#include <Servo.h>

// =====================
//   CONFIGURAÇÕES
// =====================
#define PINO_SERVO               9
#define PINO_TRIG                7
#define PINO_ECHO                6

#define ANGULO_FECHADO           105
#define ANGULO_ABERTO            230
#define VELOCIDADE_ABRIR         5
#define VELOCIDADE_FECHAR        3

#define DISTANCIA_CM             20

#define TEMPO_MINIMO_ABERTO      3500
#define COOLDOWN_FECHAMENTO      2000
#define TEMPO_MAO_AUSENTE        3000

// =====================
//   ESTADOS
// =====================
enum Estado { FECHADO, ABERTO, COOLDOWN };
Estado estado = FECHADO;

unsigned long marcadorTempoAberto = 0;
unsigned long marcadorMaoAusente  = 0;
unsigned long marcadorCooldown    = 0;
unsigned long ultimoLog           = 0;
#define INTERVALO_LOG 500

// =====================
//   SERVO
// =====================
Servo servo;
int posicaoAtual = ANGULO_FECHADO; // fonte da verdade da posição

void moverServoPara(int destino, int velocidade) {
  int passo = (destino > posicaoAtual) ? 1 : -1;
  while (posicaoAtual != destino) {
    posicaoAtual += passo;
    servo.write(posicaoAtual);
    delay(velocidade);
  }
}

// =====================
//   SENSOR
// =====================
int distanciaAtual = 0;

int medirDistancia() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);
  long duracao = pulseIn(PINO_ECHO, HIGH, 30000);
  return duracao * 0.034 / 2;
}

bool maoDetectada() {
  distanciaAtual = medirDistancia();
  return (distanciaAtual > 0 && distanciaAtual <= DISTANCIA_CM);
}

bool maoConfirmedamenteAusente() {
  if (maoDetectada()) {
    marcadorMaoAusente = millis();
    return false;
  }
  return (millis() - marcadorMaoAusente >= TEMPO_MAO_AUSENTE);
}

// =====================
//   LOG
// =====================
String barraProgresso(float percentual, int tamanho = 10) {
  int preenchido = (int)(constrain(percentual, 0.0, 1.0) * tamanho);
  String barra = "[";
  for (int i = 0; i < tamanho; i++)
    barra += (i < preenchido) ? "#" : "-";
  barra += "]";
  return barra;
}

void logTransicao(String msg) {
  Serial.println();
  Serial.println(">> " + msg);
}

void logStatus() {
  if (millis() - ultimoLog < INTERVALO_LOG) return;
  ultimoLog = millis();

  String barSensor = barraProgresso(
    1.0 - (distanciaAtual / (float)DISTANCIA_CM)
  );
  Serial.print("Sensor: ");
  Serial.print(barSensor);
  Serial.print("  ");
  Serial.print(distanciaAtual);
  Serial.print("cm");

  switch (estado) {
    case FECHADO:
      Serial.print("  |  FECHADA  |  Aguardando mao...");
      break;

    case ABERTO: {
      unsigned long tempoAberto   = millis() - marcadorTempoAberto;
      unsigned long tempoRestante = 0;
      if (TEMPO_MINIMO_ABERTO > tempoAberto)
        tempoRestante = TEMPO_MINIMO_ABERTO - tempoAberto;

      Serial.print("  |  ABERTA  |  ");
      if (tempoRestante > 0) {
        float prog = tempoAberto / (float)TEMPO_MINIMO_ABERTO;
        Serial.print("Minimo: ");
        Serial.print(barraProgresso(prog));
        Serial.print("  ");
        Serial.print(tempoRestante / 1000.0, 1);
        Serial.print("s");
      } else if (!maoConfirmedamenteAusente()) {
        unsigned long ausente = millis() - marcadorMaoAusente;
        Serial.print("Aguard. mao sair: ");
        Serial.print(barraProgresso(ausente / (float)TEMPO_MAO_AUSENTE));
      } else {
        Serial.print("Fechando em instantes...");
      }
      break;
    }

    case COOLDOWN: {
      unsigned long restante = COOLDOWN_FECHAMENTO - (millis() - marcadorCooldown);
      float prog = 1.0 - restante / (float)COOLDOWN_FECHAMENTO;
      Serial.print("  |  FECHADA  |  Cooldown: ");
      Serial.print(barraProgresso(prog));
      Serial.print("  ");
      Serial.print(restante / 1000.0, 1);
      Serial.print("s");
      break;
    }
  }

  Serial.println();
}

// =====================
//   MÁQUINA DE ESTADOS
// =====================
void loop() {
  logStatus();

  switch (estado) {

    case FECHADO:
      if (maoDetectada()) {
        logTransicao("Mao detectada! Abrindo...");
        moverServoPara(ANGULO_ABERTO, VELOCIDADE_ABRIR);
        marcadorTempoAberto = millis();
        marcadorMaoAusente  = millis();
        estado = ABERTO;
      }
      break;

    case ABERTO: {
      bool minimoDecorrido = (millis() - marcadorTempoAberto >= TEMPO_MINIMO_ABERTO);
      bool maoFora         = maoConfirmedamenteAusente();
      if (minimoDecorrido && maoFora) {
        logTransicao("Mao ausente. Fechando...");
        moverServoPara(ANGULO_FECHADO, VELOCIDADE_FECHAR);
        marcadorCooldown = millis();
        estado = COOLDOWN;
      }
      break;
    }

    case COOLDOWN:
      if (millis() - marcadorCooldown >= COOLDOWN_FECHAMENTO) {
        logTransicao("Pronta para uso!");
        estado = FECHADO;
      }
      break;
  }

  delay(200);
}

// =====================
//   SETUP
// =====================
void setup() {
  Serial.begin(9600);
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);
  servo.attach(PINO_SERVO);
  servo.write(ANGULO_FECHADO);
  delay(1000);
  Serial.println("=== Lixeira Inteligente ===");
  Serial.println();
}
