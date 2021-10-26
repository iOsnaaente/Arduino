# Projetos em Arduino

Repositório para projetos desenvolvidos em Arduino.

Aqui guardo alguns projetos em arduino :D 

# Sensor Magnetômetro 3 Eixos - GY-271

O Sensor Magnetômetro 3 Eixos Gy-271 mede o campo magnético e funciona como uma Bússola Digital, podendo ser usado por exemplo com o Arduino para indicar o norte geográfico da Terra.

[b] Durante um periodo de 24h a bussola gira 360º em sentido norte, provando que a Terra gira em um periodo de aproximadamente 24h :D 

Sensor adquirido em https://www.eletrogate.com/sensor-magnetometro-3-eixos-gy-271?utm_source=Site&utm_medium=GoogleMerchant&utm_campaign=GoogleMerchant&gclid=CjwKCAjwzt6LBhBeEiwAbPGOgfYN6K4GSSJyBk6vj7sDMx2NiBvCaPNCLMuiu1ufPeATTdM2NBShBhoCQY4QAvD_BwE 

Protocolo de comunicação I2C 

Necessita da biblioteca QMC5883LCompass anexa em ./utils
Criada por [MPrograms](https://github.com/mprograms/QMC5883LCompass/) Página git do projeto [Github Project Page](https://github.com/mprograms/QMC5883LCompass/)

Para mais informações ler o README da biblioteca anexa 

## Ploter.py 
O repositório GY-271 com os códigos arduino possuem também um arquivo Ploter.py que nada mais é do que um plotador dos dados recebidos via Serial. 

Para rodar o ploter, há a necessidade de se ter instala a biblioteca DearPyGui na sua versão 0.8.4 disponível em requirements.txt 



# MMA845x

Sensor adquirido em https://www.baudaeletronica.com.br/acelerometro-3-eixos-mma8452.html?gclid=CjwKCAjwzt6LBhBeEiwAbPGOgWlF5sQ5tTQK_c03PfmdnJXd8mp2HOKK0WSagh1HSzzYuCwhysS3ABoCNEYQAvD_BwE

Protocolo de comunicação I2C 

Necessita da biblioteca SFE_MMA8452Q anexa em ./utils
Biblioteca criada por: [MPrograms](Jim Lindblom @ SparkFun Electronics) 
Disponível em: [Github Project Page](https://github.com/sparkfun/MMA8452_Accelerometer)


## Ploter.py 
O repositório GY-271 com os códigos arduino possuem também um arquivo Ploter.py que nada mais é do que um plotador dos dados recebidos via Serial. 

Para rodar o ploter, há a necessidade de se ter instala a biblioteca DearPyGui na sua versão 0.8.4 disponível em requirements.txt 



# De mais projetos 

O primeiro projeto iniciado (RF_PTC), foi a criação de um protocolo de transmissão de Rádio frequência usando o módulo transmissor FS100A e o módulo receptor MX-RM-5V para ser capaz de transmitir entre dois arduinos como faz a biblioteca VirtualWire. Tendo como intuito o aprendizado pessoal de como o microcontrolador é capaz de fazer esse tipo de transmissão e de como as modulações de sinal funcionam nesses casos.  


O segundo projeto (Sonar_Serial) foi a criação de um Sonar utilizando o módulo hc-sr04. Ele funciona com uma UI criada em python3.6, desenhada com pygame.


O terceiro projeto (Hidrometro_com_TemperaturaLM35) foi a criação de um sistema de monitoramento da umidade e temperatura de uma horta, onde o sistema captava e guardava as informaçes dos sensores em um .TXT para uso em projetos de monitoramento. 


O quarto projeto (p2_mouse) é um teste para ler as informações de um mouse USB que utiliza padronização PS2. A ideia é fazer um carrinho controlado pelo mouse, por isso ele também conta com um sistema de transmissão de informações via RF utilizando os módulos do projeto 1. 


Ainda há os projetos de RF (TransmissõesRF) onde guardo alguns testes utilizando os módulos de comunicação de RF citados no projeto 1.


Enjoy it !!!

https://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM
http://labdegaragem.com/forum/topics/alterar-a-frequencia-do-pwm
