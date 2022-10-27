# SemaforoPedestreESP32
Projeto: Semáfaro com passagem de pedestre e display de 7 segmentos implementado usando matrizes e alto-falante.

Descrição:
Inicialmente, o led vermelho do semáforo dos pedestres está ligado.

Para o semáforo dos veículos, o verde fica ativado até que o botão dos pedestres
seja pressionado, tendo um intervalo de 2 segundos para desativar, 
ou fica ativo por 10 segundos para que o amarelo e o vermelho sejam ativados.

Com o vermelho do semáforo dos veículos ativo, o vermelho e o verde dos 
pedrestres são desligado e ligado, respectivamente e um contador regressivo de
10 segundos é iniciado.
Para identificar esse tempo, além da presença do display de 7 segmentos exibindo
os números da contagem, também é reproduzido um bip que ficará mais frequente 
ao chegar ao final do tempo.

Chegando no número 0, o bip para e o vermelho dos pedestres é ativado novamente.
O ciclo se inicia de novo.

Autor: Eduardo Catunda
