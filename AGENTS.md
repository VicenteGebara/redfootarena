# AGENTS.md

## Projeto

Este projeto se chama RedFoot Arena.

É um jogo de futebol 3D arcade em Unreal Engine, com câmera atrás do jogador, visual estilizado e foco em partidas rápidas em arena.

## Prioridade Atual

A prioridade atual é criar o MVP offline.

Não implementar ainda:
- multiplayer online;
- modo carreira;
- loja;
- moedas;
- skins;
- ranking;
- várias arenas;
- temporadas;
- transferências.

## MVP Atual

O MVP deve conter:

- jogador controlável;
- câmera em terceira pessoa;
- movimentação arcade;
- corrida com stamina;
- bola física;
- chute;
- passe simples;
- dois gols;
- placar;
- tempo de partida;
- reinício após gol;
- arena simples;
- bot adversário básico.

## Estilo de Código

Usar nomes claros.
Separar responsabilidades.
Evitar classes gigantes.
Criar sistemas pensando em expansão futura.
Comentar apenas quando o código não for óbvio.

## Arquitetura Sugerida

Classes/sistemas principais:

- PlayerCharacter
- BallActor
- GoalActor
- MatchManager
- ScoreManager
- SimpleBotController
- ArenaManager
- GameMode
- PlayerStatsComponent
- StaminaComponent

## Direção de Gameplay

O jogo deve ser arcade, responsivo e divertido.

Priorizar:
- controle simples;
- chute satisfatório;
- bola previsível;
- partidas rápidas;
- sensação de velocidade;
- arena pequena;
- aprendizado fácil.

## Futuro do Projeto

Depois do MVP offline, o projeto poderá evoluir para:

1. bots melhores;
2. times 2v2;
3. times 4v4 com bots;
4. multiplayer online;
5. modo carreira;
6. skins;
7. moedas opcionais;
8. expansões;
9. publicação na Steam.
