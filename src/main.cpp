// Alunos: Pedro Kourly; Fernanda Oliveira; Vinicius Camilo
#include <Arduino.h>

void proximaRodada();
void reproduzirSequencia();
void aguardarJogador();
void aguardarJogada();
void perderVida();
void vitoria();
void gameOver();

// ALTERAR CASO NECESSÁRIO
int const MAX_RODADAS = 10;
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
    digitalWrite(pinosLeds[sequencia[i]], HIGH);
    delay(500);
    digitalWrite(pinosLeds[sequencia[i]], LOW);
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