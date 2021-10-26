
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

