#include <Arduino.h>

void proximaRodada();
void reproduzirSequencia();
void aguardarJogador();
void aguardarJogada();
void perderVida();
void vitoria();
void gameOver();

#define SPEAKER_PIN 13

// Frequências para cada cor (em Hz).
// Estas são próximas das notas do Genius original (Sol, Dó, Mi, Sol mais agudo)
// Cor 0 (Pino 12) = Sol (G4)
// Cor 1 (Pino 10) = Dó (C5)
// Cor 2 (Pino 8)  = Mi (E5)
// Cor 3 (Pino 6)  = Sol mais agudo (G5)
int frequencias[4] = {392, 523, 659, 784};

// ALTERAR CASO NECESSÁRIO
int const MAX_RODADAS = 12;
int vidas = 3;

int sequencia[MAX_RODADAS] = {};

int rodada_atual = 0;
int passo_atual_na_sequencia = 0;

int pinosLeds[4] = {12, 10, 8, 6};
int pinosLedsVida[3] = {2, 3, 4};
int pinosBotoes[4] = {11, 9, 7, 5};

int botao_pressionado = 0;

int perdeu_o_jogo = false;
int acertou_a_jogada = false;

void setup()
{
  randomSeed(analogRead(A0));
  // Definindo o modo dos pinos dos Leds como saída.
  for (int i = 0; i <= 3; i++)
  {
    pinMode(pinosLeds[i], OUTPUT);
  }

  // Definindo o modo dos pinos dos Leds de vida como saída.
  for (int i = 0; i <= 2; i++)
  {
    pinMode(pinosLedsVida[i], OUTPUT);
    digitalWrite(pinosLedsVida[i], HIGH);
  }

  // Definindo o modo dos pinos dos Botões como pullup interno.
  for (int i = 0; i <= 3; i++)
  {
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }

  // Inicializa a primeira rodada.
  proximaRodada();
}

void loop()
{
  // Se perdeu o jogo reinicializamos todas as variáveis.
  if (perdeu_o_jogo)
  {
    sequencia[MAX_RODADAS] = {};
    rodada_atual = 0;
    passo_atual_na_sequencia = 0;
    vidas = 3;
    for (int i = 0; i <= 2; i++)
    {
      digitalWrite(pinosLedsVida[i], HIGH);
    }
    perdeu_o_jogo = false;
    proximaRodada();
  };

  if (rodada_atual == MAX_RODADAS && acertou_a_jogada == true)
  {
    vitoria();
    sequencia[MAX_RODADAS] = {};
    rodada_atual = 0;
    passo_atual_na_sequencia = 0;
    vidas = 3;
    for (int i = 0; i <= 2; i++)
    {
      digitalWrite(pinosLedsVida[i], HIGH);
    }
    perdeu_o_jogo = false;
  };

  // Chama a função que inicializa a próxima rodada.
  if (acertou_a_jogada == true)
  {
    proximaRodada();
  }
  
  // Reproduz a sequência atual.
  reproduzirSequencia();
  // Aguarda os botões serem pressionados pelo jogador.
  aguardarJogador();

  // Aguarda 1 segundo entre cada jogada.
  delay(1000);
}

void proximaRodada()
{
  int numero_sorteado = random(0, 4);
  sequencia[rodada_atual++] = numero_sorteado;
}

// Reproduz a sequência para ser memorizada.
void reproduzirSequencia()
{
  for (int i = 0; i < rodada_atual; i++)
  {

    int cor_atual = sequencia[i]; 
    int freq_atual = frequencias[cor_atual];

    digitalWrite(pinosLeds[cor_atual], HIGH);
    tone(SPEAKER_PIN, freq_atual);
    
    delay(500);


    digitalWrite(pinosLeds[cor_atual], LOW);
    noTone(SPEAKER_PIN);

    delay(100);
  }
}

// Aguarda o jogador iniciar sua jogada.
void aguardarJogador()
{
  for (int i = 0; i < rodada_atual; i++)
  {
    aguardarJogada();

    // verifica a jogada
    if (sequencia[passo_atual_na_sequencia] != botao_pressionado)
    {
      perderVida();
      acertou_a_jogada = false;
      break;
    }

    // para o jogo se perdeu
    if (perdeu_o_jogo)
    {
      break;
    }
    acertou_a_jogada = true;
    passo_atual_na_sequencia++;
  }

  // Redefine a variável para 0.
  passo_atual_na_sequencia = 0;
}

void aguardarJogada()
{
  boolean jogada_efetuada = false;
  while (!jogada_efetuada)
  {
    for (int i = 0; i <= 3; i++)
    {
      if (!digitalRead(pinosBotoes[i]))
      {

        botao_pressionado = i;

        digitalWrite(pinosLeds[i], HIGH);
        tone(SPEAKER_PIN, frequencias[i]);

        delay(300); 

        digitalWrite(pinosLeds[i], LOW);
        noTone(SPEAKER_PIN);


        jogada_efetuada = true;
        
        delay(50); 
      }
    }
    delay(10);
  }
}

void perderVida()
{
  vidas--;
  digitalWrite(pinosLedsVida[vidas], LOW);
  delay(1000);
  if (vidas == 0)
  {
    gameOver();
  }
}

void vitoria(){

  tone(SPEAKER_PIN, 523, 200); delay(200);
  tone(SPEAKER_PIN, 659, 200); delay(200);
  tone(SPEAKER_PIN, 784, 200); delay(200);
  tone(SPEAKER_PIN, 1046, 400); delay(400);

  for (int i = 0; i <= 3; i++)
  {
    digitalWrite(pinosLeds[i], HIGH);
  }
  delay(500);
  for (int i = 0; i <= 3; i++)
  {
    digitalWrite(pinosLeds[i], LOW);
  }
  delay(500);
  for (int i = 0; i <= 3; i++)
  {
    digitalWrite(pinosLeds[i], HIGH);
  }
  delay(500);
  for (int i = 0; i <= 3; i++)
  {
    digitalWrite(pinosLeds[i], LOW);
  }
  delay(500);
  for (int i = 0; i <= 3; i++)
  {
    digitalWrite(pinosLeds[i], HIGH);
  }
  delay(2000);
  for (int i = 0; i <= 3; i++)
  {
    digitalWrite(pinosLeds[i], LOW);
  }
  delay(500);
}

void gameOver()
{
  // GAME OVER.

  tone(SPEAKER_PIN, 150, 1500); // Tom de 150Hz por 1.5 segundos

  for (int i = 0; i <= 3; i++)
  {

    digitalWrite(pinosLeds[i], HIGH);
    delay(200);
    digitalWrite(pinosLeds[i], LOW);
  }

  for (int i = 0; i <= 3; i++)
  {
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