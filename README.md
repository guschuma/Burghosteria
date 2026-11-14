\<img src="assets/sprites/logo.png">: Um Fantástico Simulador de Burgueria, por Gustavo S. F.

https://github.com/user-attachments/assets/a4bead00-bca6-43d0-8ca0-693186149c0d

## Descrição

Este jogo foi desenvolvido para a disciplina de Estruturas de Dados da Universidade Federal do Rio Grande do Sul (*UFRGS*).

É um simples simulador de uma hamburgueria, no qual você é um cozinheiro encarregado de preparar hambúrgueres para seus valiosos clientes. Mas há um detalhe: eles são fantasmas, e você é uma bruxa!

Use seus feitiços para montar as pilhas de hambúrgueres de acordo com os pedidos dos clientes, mas tome cuidado com seus recursos!

## Executando o programa

* No macOS, execute o comando `make run PEDIDOS=pedidos.txt HIST=historico.txt` no terminal.

* No Linux, execute o comando `make run_linux PEDIDOS=pedidos.txt HIST=historico.txt` no terminal.

**Nota:** Os argumentos `PEDIDOS=pedidos.txt` e `HIST=historico.txt` são obrigatórios para a atividade, mas podem ser removidos.

### Controles

* Enter: Finalizar pedido.
* Delete: Remover o ingrediente do topo (permanentemente).
* Mouse (botão esquerdo ou direito): Todas as demais ações!

## Como o jogo foi desenvolvido?

### Informações sobre os recursos utilizados

O jogo foi desenvolvido na linguagem de programação C, utilizando a biblioteca [Raylib](https://www.raylib.com/), voltada para aplicações gráficas.

Também foram utilizadas sugestões do [ChatGPT](https://chatgpt.com).

Para criar os sprites, utilizei o editor de pixel art online [PixilArt](https://www.pixilart.com/).

A música foi produzida utilizando o Ultrabox. Você pode encontrar os links (e editá-las por conta própria!) em [music.txt](assets/music/music.txt).

Alguns efeitos sonoros foram feitos por mim, mas a maioria foi criada pelas pessoas cujos nomes e trabalhos podem ser encontrados [aqui](assets/sounds/sounds.txt).
