#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define EXPORT   1
#define UNEXPORT 0
#define INPUT	 1
#define OUTPUT	 0
#define HIGH	 1
#define LOW	 0

int UnExport(int pino, int unouex)
{
	char 	buffer[3];
	int 	arquivo;

	arquivo = open( (unouex == 0)? "/sys/class/gpio/unexport":
			"/sys/class/gpio/export", O_WRONLY);
	if( arquivo == -1 )
		return -1;

	snprintf(buffer, 3, "%d", pino);
	if( write(arquivo, buffer, 3) == -1 )
	{
		close(arquivo);
		return -2;
	}

	close(arquivo);
	return 1;
}

int Sentido(int pino, int direcao)
{
	char caminho[35];
	int  arquivo;

	snprintf( caminho, 35, "/sys/class/gpio/gpio%d/direction", pino );
	arquivo = open( caminho, O_WRONLY );
	if( arquivo == -1 )
		return -1;

	if( write( arquivo, (direcao == 0)?"out":"in", 3 ) == -1 )
	{
		close(arquivo);
		return -2;
	}

	close(arquivo);
	return 1;
}

int Gravar(int pino, int estado)
{
	int 	arquivo;
	char 	caminho[33];

	snprintf( caminho, 33, "/sys/class/gpio/gpio%d/value", pino );
	arquivo = open( caminho, O_WRONLY );
	if( arquivo == -1 )
		return -1;

	if( write( arquivo, (estado==0)?"0":"1", 1 ) == -1 )
	{
		close(arquivo);
		return -2;
	}
	close(arquivo);
	return 1;
}

int Ler(int pino)
{
	int 	arquivo;
	char	caminho[33];
	char	retorno[3];

	snprintf( caminho, 33, "/sys/class/gpio/gpio%d/value", pino );
	arquivo = open( caminho, O_RDONLY );
	if( arquivo == -1 )
		return -1;

	if( read( arquivo, retorno, 3 ) == -1 )
	{
		close(arquivo);
		return -2;
	}

	close(arquivo);
	return ( (int)retorno[0] - 48 );
}

void main()
{
	int Pino = 21;
	int contador = 0;

	if(UnExport(Pino, EXPORT))
	{
		if(Sentido(Pino, OUTPUT))
		{
			for(contador=0; contador < 50; contador++)
			{
				printf("saida %d\n",Gravar(Pino, HIGH));
				usleep(500000);
				printf("saida %d\n",Gravar(Pino, LOW));
				usleep(500000);
			}
		}
		UnExport(Pino, UNEXPORT);
	}
	else
	{
		printf("Deu ruim bagarai!\n");
	}
}











