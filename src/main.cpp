#include <Arduino.h>

void proximaRodada();
void reproduzirSequencia();
void aguardarJogador();
void aguardarJogada();
void gameOver();

// Nossa sequência de até 100 itens vai começar vazia.
int sequencia[4] = {};
// Indica a rodada atual que o jogo se encontra.
int rodada_atual = 0;
// Indica o passo atual dentro da sequência, é usado enquanto a sequência
// está sendo reproduzida.
int passo_atual_na_sequencia = 0;

int pinosLeds[4] = { 28, 26, 24, 22 };
int pinosBotoes[4] = { 34, 36, 38, 40 };

// Indica se um botão foi pressionado durante o loop principal.
int botao_pressionado = 0;
// Flag indicando se o jogo acabou.
int perdeu_o_jogo = false;

void setup() {
  // Definindo o modo dos pinos dos Leds como saída.
  for (int i = 0; i <= 3; i++) {
    pinMode(pinosLeds[i], OUTPUT);
  }
 
  // Definindo o modo dos pinos dos Botões como pullup interno.
  for (int i = 0; i <= 3; i++) {
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }


}

void loop() {
  // Se perdeu o jogo reinicializamos todas as variáveis.
  if (perdeu_o_jogo) {
    int sequencia[4] = {};
    rodada_atual = 0;
    passo_atual_na_sequencia = 0;
    perdeu_o_jogo = false;
  };

  // Chama a função que inicializa a próxima rodada.
  proximaRodada();
  // Reproduz a sequência atual.
  reproduzirSequencia();
  // Aguarda os botões serem pressionados pelo jogador.
  aguardarJogador();
 
  // Aguarda 1 segundo entre cada jogada.
  delay(1000);

}

void proximaRodada() {
  int numero_sorteado = random(0, 4);
  sequencia[rodada_atual++] = numero_sorteado;
}

// Reproduz a sequência para ser memorizada.
void reproduzirSequencia() {
  for (int i = 0; i < rodada_atual; i++) {
    digitalWrite(pinosLeds[sequencia[i]], HIGH);
    delay(500);
    digitalWrite(pinosLeds[sequencia[i]], LOW);
    delay(100);
  }

}

// Aguarda o jogador iniciar sua jogada.
void aguardarJogador() {
  for (int i = 0; i < rodada_atual; i++) {
    aguardarJogada();
    
  // verifica a jogada  
  if (sequencia[passo_atual_na_sequencia] != botao_pressionado) {
      gameOver(); // perdeu
   }
   
   // para o jogo se perdeu
    if (perdeu_o_jogo) {
      break;
    }
     passo_atual_na_sequencia++;
  }
 
  // Redefine a variável para 0.
  passo_atual_na_sequencia = 0;
}

void aguardarJogada() {
  boolean jogada_efetuada = false;
  while (!jogada_efetuada) {
    for (int i = 0; i <= 3; i++) {
      if (!digitalRead(pinosBotoes[i])) {
        // Dizendo qual foi o botao pressionado.
        botao_pressionado = i;
 
        digitalWrite(pinosLeds[i], HIGH);
        delay(300);
        digitalWrite(pinosLeds[i], LOW);
 
        jogada_efetuada = true;
      }
    }
    delay(10);
  }
}

void gameOver() {
    // GAME OVER.
    for (int i = 0; i <= 3; i++) {

      digitalWrite(pinosLeds[i], HIGH);
      delay(200);
      digitalWrite(pinosLeds[i], LOW);

    }
 
    for (int i = 0; i <= 3; i++) {
      digitalWrite(pinosLeds[0], HIGH);
      digitalWrite(pinosLeds[1], HIGH);
      digitalWrite(pinosLeds[2], HIGH);
      digitalWrite(pinosLeds[3], HIGH);
      delay(100);
      digitalWrite(pinosLeds[0], LOW);
      digitalWrite(pinosLeds[1], LOW);
      digitalWrite(pinosLeds[2], LOW);
      digitalWrite(pinosLeds[3], LOW);
      delay(100);
    }
 
    perdeu_o_jogo = true;  
} 