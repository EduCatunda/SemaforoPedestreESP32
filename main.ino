// Configuração de variáveis e seus pinos
// Speaker
int autoFalante = 5;

// Nota com tom agudo
#define NOTE_FS5 740

// Botão
int botao = 13;

// LEDs do semáforo de veículos, na ordem verde, amarelo e vermelho
int ledVermelhoVeiculo = 27;
int ledAmareloVeiculo = 26;
int ledVerdeVeiculo = 32;

// LEDs do semáforo de pedestres, na ordem verde e vermelho
int ledVermelhoPedestre = 25;
int ledVerdePedestre = 33;

// LEDs de visor de 7 segmentos com seu respectivo pino
//                A   B  C  D   E   F   G
int visor[7]  = {21, 19, 4, 2, 15, 22, 23};

// Números exibidos no display, sendo 0 valor alto (ligado) e 1 baixo (desligado)
//                      A  B  C  D  E  F  G
int numeros[10][7] = {  0, 0, 0, 0, 0, 0, 1,      // zero
                        1, 0, 0, 1, 1, 1, 1,      // um
                        0, 0, 1, 0, 0, 1, 0,      // dois
                        0, 0, 0, 0, 1, 1, 0,      // três
                        1, 0, 0, 1, 1, 0, 0,      // quatro
                        0, 1, 0, 0, 1, 0, 0,      // cinco
                        0, 1, 0, 0, 0, 0, 0,      // seis
                        0, 0, 0, 1, 1, 1, 1,      // sete
                        0, 0, 0, 0, 0, 0, 0,      // oito
                        0, 0, 0, 1, 1, 0, 0       // nove
                     };

int tempoDelay = 1000;             // Delay entre os semáforos; padrão: 1500
int tempoDisplay = 1000;          // padrão: 1000
int tempoVerdeVeiculo = 10;        // Tempo em segundos; padrão: 10
int tempoVerdePedestre = 2000;    // padrão: 2000
int contagemRegressiva = 9;       // Tempo em segundos; máximo: 9; padrão: 9
boolean botaoPressionado = false;

// Variáveis para manter registro do tempo de interrupções recentes
unsigned long tempoBotao = 0;  
unsigned long tempoUltimoBotao = 0; 

/**
* Método de interrupção do botão com debouncing em software
*/
void IRAM_ATTR botaoInterrupdor() {
  tempoBotao = millis();
  if (tempoBotao - tempoUltimoBotao > 250 && digitalRead(ledVerdeVeiculo)) {    
    botaoPressionado = true;
    tempoUltimoBotao = tempoBotao;
  }   
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 7; i++) {
    pinMode(visor[i], OUTPUT);
  }

  pinMode(ledVermelhoVeiculo, OUTPUT);
  pinMode(ledAmareloVeiculo, OUTPUT);
  pinMode(ledVerdeVeiculo, OUTPUT);
  pinMode(ledVermelhoPedestre, OUTPUT);
  pinMode(ledVerdePedestre, OUTPUT);

  pinMode(autoFalante, OUTPUT);

  pinMode(botao, INPUT_PULLUP);

  attachInterrupt(botao, botaoInterrupdor, RISING);
}

void loop() {

  apagaSegmentos();

  digitalWrite(ledVermelhoPedestre, HIGH);

  verdeVeiculo();
  if (botaoPressionado) {
    botaoPressionado = false;
  }
  semaforoVeiculo();
  semaforoPedestre();
}

void verdeVeiculo() {
  // Desliga vermelho
  digitalWrite(ledVermelhoVeiculo, LOW);

  // Pisca verde com um intervalo de 10 segundos apenas se o botão não foi 
  // pressionado
  digitalWrite(ledVerdeVeiculo, HIGH);

  // Verifica a cada segundo se o botão foi pressionado. Caso verdadeiro, um 
  // intervalo de 3 segundos é executado para que os leds amarelo e vermelho liguem
  for(int i = 0; i < tempoVerdeVeiculo; i++) {
    if (botaoPressionado) {
      delay(tempoVerdePedestre);
      break;
    }
    else {
      delay(1000);
    }
  }
}

void semaforoVeiculo() {
  // Desliga verde
  digitalWrite(ledVerdeVeiculo, LOW);

  // Pisca amarelo
  digitalWrite(ledAmareloVeiculo, HIGH);
  delay(tempoDelay);
  digitalWrite(ledAmareloVeiculo, LOW);

  // Liga vermelho
  digitalWrite(ledVermelhoVeiculo, HIGH);
  delay(tempoDelay);
}

void semaforoPedestre() {
  // Desliga vermelho
  digitalWrite(ledVermelhoPedestre, LOW);

  // Liga verde
  digitalWrite(ledVerdePedestre, HIGH);
  contador();
  digitalWrite(ledVerdePedestre, LOW);
}

void apagaSegmentos() {
  for (int j = 0; j < 7; j++) {
    digitalWrite(visor[j], 1);
  }
}

void contador() {

  int tempoTotal = 0;

  for (int i = contagemRegressiva; i >= 0; i--) {
    tempoTotal = tempoDisplay;

    for (int j = 0; j < 7; j++) {
      digitalWrite(visor[j], numeros[i][j]);
    }
    // Quando estiver faltando apenas 3 segundos, o vermelho do pedestre pisca, o 
    // verde é desligado e o bip se intensifica.
    if (i <= 3) {
      // Desliga verde
      digitalWrite(ledVerdePedestre, LOW);
      
      // Pisca vermelho
      for(int k = 0; k < 3; k++) {
        digitalWrite(ledVermelhoPedestre, HIGH);
        delay(100);
        digitalWrite(ledVermelhoPedestre, LOW);
        delay(100);
        tone(autoFalante, NOTE_FS5, 50);
      }
      
      // Compensa tempo perdido
      tempoTotal = tempoDisplay - 600;
    }
    else {
      // Bip
      tone(autoFalante, NOTE_FS5, 125);
    }

    delay(tempoTotal);
  }
  apagaSegmentos();
}
