import pygame
import random
import math

cor = {
	"branco"   : [255,255,255],
	"preto"    : [  0,  0,  0],
	"cinza"    : [ 75, 75, 75],
	"verde"    : [  0,200,  0],
	"vermelho" : [200,  0,  0]
}

screen_dimensions = [20*40,20*20]

barraVermelha = pygame.Surface([18,5])
barraVerde    = pygame.Surface([18,5])
barraPreta    = pygame.Surface([20,7])

barraVermelha.fill(cor["vermelho"])
barraVerde.fill(cor["verde"])
barraPreta.fill(cor["preto"])

center = [screen_dimensions[0]/2, screen_dimensions[1]]

pilhas =[]
for i in range(360):
	pilhas.append(0)

piece_radial = []
for i in range(180):
	piece_radial.append(0)


def plot_barras(width = 0, heigth=0):
	for i in range(0,heigth,1):
		barraPreta.blit(barraVerde, [1,1])
		screen.blit(barraPreta, [width*20,(screen_dimensions[1])-(i*7)])
	barraPreta.blit(barraVermelha, [1,1])	
	screen.blit(barraPreta,[width*20,(screen_dimensions[1])-(heigth*7)])


def catch_Dim():
	for i in range(180):
		pilhas[i] = int(100*math.sin(GtoR(i)))


def GtoR(grau):
	return grau*0.01745


def plot_Piece(raio, angulo=[0,0], cor_=0):

	(x,y) = center
	
	angulo[0] = GtoR(angulo[0])
	angulo[1] = GtoR(angulo[1])

	xo,x1 = raio*math.cos(angulo[1]), raio*math.cos(angulo[0])
	yo,y1 = raio*math.sin(angulo[1]), raio*math.sin(angulo[0])
	xob,x1b = 250*math.cos(angulo[1]), 250*math.cos(angulo[0])
	yob,y1b = 250*math.sin(angulo[1]), 250*math.sin(angulo[0])
	
	pygame.draw.polygon(screen, cor["vermelho"], [[x,y], [x+xob,y-yob],[x+x1b,y-y1b]],0)
	pygame.draw.polygon(screen, cor["verde"]   , [[x,y], [x+xo,y-yo],[x+x1,y-y1]],4)
	pygame.draw.line   (screen, cor["preto"]   , [x+xob,y-yob], [x+x1b,y-y1b],5) 
	pygame.draw.line   (screen, cor["preto"]   , [x+xo,y-yo], [x+x1,y-y1],5) 
 

from SerialShark import *

BAUDRATE = 9600

listaPortas = showSerialAvailable()
comport = initSerialListening(listaPortas[0], BAUDRATE, 1)


pygame.init()

screen = pygame.display.set_mode(screen_dimensions)

pygame.display.set_caption("Teste de sensor de proximidade")

clock = pygame.time.Clock()

x,y = 0,0

while True:

	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			pygame.quit()
		if event.type == pygame.KEYDOWN:
			if event.key == K_ESCAPE:
				pygame.quit()

	screen.fill(cor["branco"])

	try:	
		angulo, raio = getSerialValues(comport)
		angulo = int(angulo)
		raio   = int(raio)/100

		piece_radial[angulo] = raio
		print(angulo, raio)

	except:
		pass

	for i in range(0,180,1):
		plot_Piece(piece_radial[i], [i,i+1], cor["verde"])		
		if i == angulo :
			plot_Piece(piece_radial[i], [i  ,i+1], cor["vermelho"])


	pygame.display.update()
	clock.tick(60)
