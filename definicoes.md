# RedFoot Arena — Documento Inicial do Projeto

## 1. Visão Geral

RedFoot Arena é um jogo de futebol 3D arcade, em terceira pessoa, com câmera atrás do jogador. O jogo terá partidas rápidas, jogabilidade dinâmica, visual estilizado e foco em arenas pequenas.

A proposta principal é criar uma experiência de futebol mais direta, rápida e divertida, inspirada em jogos de arena e futebol arcade. No futuro, o jogo terá partidas online 4v4 e um modo carreira com progressão do jogador.

## 2. Nome do Jogo

Nome provisório e principal:

RedFoot Arena

## 3. Gênero

Futebol arcade 3D em arena.

## 4. Plataforma Alvo

Inicialmente Windows.

Plataforma futura desejada: Steam.

## 5. Estilo de Jogo

O jogo será arcade, não simulação realista.

A jogabilidade deve priorizar:

* partidas rápidas;
* controle simples;
* diversão imediata;
* movimentação fluida;
* chute responsivo;
* partidas com poucos jogadores;
* arena compacta;
* visual estilizado.

## 6. Câmera

A câmera principal será em terceira pessoa, posicionada atrás do jogador.

A câmera deve acompanhar o atleta controlado pelo jogador e permitir boa visualização da bola, do gol e dos adversários próximos.

## 7. Estrutura das Partidas

O jogo começará offline, com jogador contra bot.

No futuro, o objetivo será implementar partidas online 4v4.

A progressão recomendada é:

1. Jogador contra ambiente sem bot.
2. Jogador contra um bot.
3. Jogador com bots aliados e adversários.
4. Partidas 2v2 com bots.
5. Partidas 4v4 com bots.
6. Testes online 1v1.
7. Testes online 2v2.
8. Partidas online 4v4.

## 8. Posições

O jogo não terá posições complexas no começo.

As categorias iniciais serão apenas:

* Goleiro;
* Linha.

Jogadores de linha poderão atacar, defender, passar, chutar e correr livremente pela arena.

## 9. Campos e Arenas

O jogo terá vários tipos de campo no futuro.

Ideias de arenas:

* arena de bairro;
* quadra de rua;
* campo society;
* arena futurista;
* arena de praia;
* arena urbana noturna;
* arena escolar;
* arena profissional;
* arena mundial.

No primeiro MVP, haverá apenas uma arena simples, fechada, com dois gols e paredes laterais.

## 10. Visual

O visual será estilizado, não realista.

A direção artística deve buscar um estilo bonito, leve e viável para uma equipe pequena.

Evitar realismo extremo, rostos realistas, estádios gigantes e animações complexas no início.

## 11. Modo Carreira

O modo carreira será implementado depois do núcleo de gameplay estar funcionando.

A ideia inicial é permitir que o jogador crie um atleta e evolua com partidas.

Elementos futuros do modo carreira:

* criação de jogador;
* nome;
* aparência;
* evolução por XP;
* atributos;
* skins;
* times fictícios;
* torneios;
* ligas;
* desbloqueio de arenas;
* desafios especiais.

## 12. Atributos do Jogador

Atributos iniciais sugeridos:

* Velocidade;
* Chute;
* Passe;
* Drible;
* Defesa;
* Físico;
* Stamina;
* Reflexo, no caso do goleiro.

## 13. Monetização

O jogo será pago.

Além do jogo base, poderão existir conteúdos extras opcionais:

* expansões;
* skins;
* uniformes;
* arenas;
* bolas especiais;
* efeitos visuais;
* moedas internas opcionais.

A monetização não deve tornar o jogo pay-to-win.

As moedas devem ser usadas preferencialmente para itens cosméticos e desbloqueios que não prejudiquem o equilíbrio competitivo.

## 14. Primeiro MVP

O primeiro MVP não terá online, carreira, loja, moedas, ranking ou múltiplas arenas.

O primeiro MVP deve conter apenas:

* jogador controlável;
* movimentação em terceira pessoa;
* corrida;
* stamina simples;
* bola física;
* chute;
* passe simples;
* dois gols;
* placar;
* tempo de partida;
* reinício após gol;
* uma arena simples;
* um bot adversário básico.

## 15. Tecnologia

Engine principal:

Unreal Engine.

Desenvolvimento com ajuda do Codex.

O Codex será usado para:

* organizar o projeto;
* gerar classes;
* criar sistemas;
* refatorar código;
* escrever documentação;
* criar lógica de jogo;
* corrigir bugs;
* sugerir melhorias;
* manter o projeto organizado.

## 16. Regra Principal do Projeto

Não implementar online, carreira, loja ou moedas antes do jogo base estar divertido offline.

A prioridade é:

1. Fazer o jogador andar bem.
2. Fazer a bola responder bem.
3. Fazer o chute ser divertido.
4. Fazer o gol funcionar.
5. Fazer o bot jogar minimamente.
6. Só depois pensar em online e carreira.
