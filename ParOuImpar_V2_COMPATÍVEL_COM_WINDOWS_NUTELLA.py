##########################################################
##														##		
##		SUPER ULTRA MEGA DESAFIO DA FABIS				##
##														##
##		   CARINHOSAMENTE CHAMO DE ...					##
##														##
##  ENSINANDO A FABIS COMO SE JOGA PAR OU Impar         ##
##														##
##														##
##														##
##	       FEITO POR MIM, BRUNO GABRIEL 				##
##  		 CODE MAN DAS GALAXIAS B|					##
##														##
##														##
##########################################################

from random import randint 
import os

# FUNÇÃO ANÔNIMA CHAVOSA QUE COMPLICA A VIDA, MAS LANÇA UM GRAU ;D 
soma = lambda a,b : a+b 


userNum = 0
pcNum = 0

# PLACAR JOGADOR,COMPUTADOR
placar = [0,0]


# PARA LIMPAR O TERMINAL ANTES DE COMEÇAR O JOGO
os.system('cls' if os.name == 'nt' else 'clear')


# LOOPS INFINTIS IMORTAL QUE NUNCA TERMINAL ~A NÃO SER QUE EU MANDE ELE TERMINAR break :O
while True: 

	# Se a soma for par é minha vez de escolher entre Par ou Impar
	if soma(placar[0],placar[1])%2 == 0:

		# Escolha com direito a fugir do jogo 
		choice = input("Escolha par ou impar digitando um valor Par ou um valor Impar: \n(Ou escreva exit para sair)\n")
		
		# Função capitalize padroniza o input para primeira letra maiuscula e o resto minusculo
		# Evita que escreva Exit ou EXIT ou exit ou sla ExIt
		if choice.capitalize() == "Exit":
			break

		# Não pensei que se escrever errado dá ruim no code, mas foda-C  ~Corrigi on code  
		else:
			# PUXA O INT NO INPUT 
			# Se não der para puxar o int no input o usuário digitou errado o Exit e ele é um jumento
			# Seremos obrigados a encerrar o jogo né.... Jumento
			try:
				# Tenta puxar o int
				# Se der, beleza
				choice = int(choice)
			
			# Se não der BREEEEEKA
			except:
				input("Comando inválido.... Encerrando o jogo")
				break

			# Se input for par tasta-lhe "PAR"
			if choice%2 == 0 :
				choice = "PAR"
			# Aqui eu nem comento o que acontece né
			else:
				choice = "IMPAR"

		# A titulo de curiosidade, a sua escolha foi ...... tã nã nã nãaaa
		print("SUA ESCOLHA FOI {}.  \npressione enter para continuar".format(choice))
		input()
		os.system('cls' if os.name == 'nt' else 'clear')
	

	# Se a soma é impar então o computador escolhe aleatóriamente o que ele quer
	# mesmissima coisa de antes
	else:
		exit = input("Caso deseje sair, digite Exit ou então pressione enter para prossegui: ")
		if exit.capitalize() == "Exit":
			break
		else:
			os.system('cls' if os.name == 'nt' else 'clear')

		aux = "PAR" if randint(0,1)==0 else "IMPAR"
		
		print("Você já escolheu {} antes, agora é a vez do computador escolher. Ele escolhe {}".format(choice, aux))
		choice = "PAR" if aux=="IMPAR" else "IMPAR"

		print("\nPORTANTO A SUA CONDIÇÃO É {}.  \npressione enter para continuar".format(choice))
		input()
		os.system('cls' if os.name == 'nt' else 'clear')
	

	# Usuário escolhe um número
	try:
		userNum = int(input("Agora escolha um número: "))
	except:
		input("Comando inválido.... Encerrando o jogo")
		break
		
	# Pc escolhe outro
	pcNum = randint(0,10)

	# Passa as info marota para o usuário
	print("\nO número do jogador é {} e o do computador foi {}. \nA soma dos números é {}. \nEsse número é ".format(userNum, pcNum, userNum+pcNum))	
	
	# Confere a soma
	if soma(userNum, pcNum)%2 == 0 :
		print("PAR")
		print("Você escolheu {}".format(choice)) #Printa a escolha do jogador

		if choice == "PAR":
			print("\nVOCÊ GANHOU")
			placar[0] = placar[0]+1			# Soma o placar do jogador
		else:
			print("\nVOCÊ PERDEU")
			placar[1] = placar[1]+1			# Soma o placar do computador

	# Mesmissima coisa de antes
	else:
		print("IMPAR")
		print("Você escolheu {}".format(choice))

		if choice == "IMPAR":
			print("\nVOCÊ GANHOU")
			placar[0] = placar[0]+1
		else:
			print("\nVOCÊ PERDEU")
			placar[1] = placar[1]+1

	# Novo turno
	input("Pressione enter para uma nova escolha")
	os.system('cls' if os.name == 'nt' else 'clear')


# Aqui a gente já saiu do jogo digitando o Exit ou qualquer outra merda errada
os.system('cls' if os.name == 'nt' else 'clear')

# Sou querido e agradeço pelo jogador ter jogado meu joguinho :3
input("OBRIGADO POR JOGAR. PRESSIONE ENTER PARA VER O PLACAR")
os.system('cls' if os.name == 'nt' else 'clear')


# Tascalhe o placar 
# Vitorioso
if placar[0]>placar[1]:
	print("O placar do jogo foi {} contra {} para o Jogador".format(placar[0], placar[1]))
	print("\n\nVOCÊ VENCEU O JOGO")

# LOOOOOSER
elif placar[0]<placar[1]:
	print("O placar do jogo foi {} contra {} para o Computador".format(placar[1], placar[0]))
	print("\n\nVOCÊ PERDEU O JOGO")

# EMPATE MAROTO
else:
	print("O placar do jogo foi {} contra {} para o Computador".format(placar[1], placar[0]))
	print("\n\nDEU EMPATE")


# VAI TEEMBORA SEU MERDA
input("\n\nPRESSIONE ENTER PARA SAIR")
os.system('cls' if os.name == 'nt' else 'clear')