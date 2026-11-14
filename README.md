<img src="assets/sprites/logo.png">: a Fantastic Burgueria Simulator by Gustavo S. F.

![A preview of the game](assets/2026-11-14 16-04-45.mp4)

## Description

This game was made for the Data Structures class at Universidade Federal do Rio Grande do Sul (*UFRGS*).  
It's a simple burgueria simulator where you're a cook in charge of making some burguers for your valuable clients. But there's a catch! They're ghosts, and you're a witch!
Use your spells to build the burguer stack based on the client's order, but be careful about your resources!

## Executing the program

* On macOS, run the command `make run PEDIDOS=pedidos.txt HIST=historico.txt` on the terminal.

* On Linux, run the command `make run_linux PEDIDOS=pedidos.txt HIST=historico.txt` on the terminal.

Note: The PEDIDOS=pedidos.txt HIST=historico.txt is mandatory part of the activity, but could be removed.

### Controls:

* Enter: Finish order
* Delete: Delete top ingrendient (permanently)
* Mouse (left or right button): Everything else!

## How was the game made?

### Resources info

The game runs on the C programming language with [Raylib](https://www.raylib.com/), a library made for graphical applications. 
Advice from [ChatGPT](https://chatgpt.com) was used.
For the sprites, I used the online pixel art editor [PixilArt](https://www.pixilart.com/).  
The music was made using Ultrabox. You can find the links (and edit them yourself!) at [music.txt](assets/music/music.txt).  
Some sounds effects were made by me, but most of them are made by the people you can find [here](assets/sounds/sounds.txt)
