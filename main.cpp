#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

// Posição inicial da esfera
float xInicioDaEsfera = 0.0, yInicioDaEsfera = -15;
float xAtualDaEsfera = xInicioDaEsfera;
float yAtualDaEsfera = yInicioDaEsfera;
float xE = xAtualDaEsfera+5;
float yE = yAtualDaEsfera+5;

float xInicioDaEsfera2 = -15, yInicioDaEsfera2 = 0;
float xAtualDaEsfera2 = xInicioDaEsfera2;
float yAtualDaEsfera2 = yInicioDaEsfera2;

//Posição inicial do cubo1
float xInicioCubo = 20.0;
float yInicioCubo = 20.0;

float xAtualCubo = xInicioCubo;
float yAtualCubo = yInicioCubo;

float dX = xAtualDaEsfera2 - xAtualDaEsfera;
float dY = yAtualDaEsfera2 - yAtualDaEsfera;

float distancia = sqrt(dX*dX + dY*dY);

// Velocidade de deslocamento no eixo x e no eixo y
float velocDeslocEsfera_X = 0.2, velocDeslocEsfera_Y = 0.05;
float velocDeslocEsfera2_X = 0.2, velocDeslocEsfera2_Y = 0.05;

float velocDeslocCubo_X = 0.2, velocDeslocCubo_Y = 0.05;

// Quantidade de quadros a acumular por redesenho
int quadros = 5;

// Indica que é o primeiro redesenho
bool primeiro = true;
// True se animando
bool animado = true;
// True se gerando motion blur
bool blur = true;

// Variáveis para controles de navegação
GLfloat angle, fAspect;
GLfloat rotacaoX, rotacaoY, rotacaoXInicial, rotacaoYInicial;
GLfloat posicaoObservadorX, posicaoObservadorY, posicaoObservadorZ;
GLfloat observadorXInicial, observadorYInicial, observadorZInicial;
int posicaoMouseInicial_X, posicaoMouseInical_Y, botao;

// Função responsável pela especificação dos parâmetros de iluminação
void DefineIluminacao (void)
{
	GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor"
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
	GLint especMaterial = 60;

	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
}

// Desenha a "cerca" ao redor da esfera
#define tamx 30
#define tamy 30
#define tamz 5
void DesenhaLados(void)
{
	glColor3f(0.2,0.8,0.3);

	glBegin(GL_QUADS);
		glNormal3f( 1,0,0);
		glVertex3f(-tamx,-tamy,-tamz);
		glVertex3f(-tamx,-tamy, tamz);
		glVertex3f(-tamx, tamy, tamz);
		glVertex3f(-tamx, tamy,-tamz);

		glNormal3f(-1,0,0);
		glVertex3f( tamx,-tamy,-tamz);
		glVertex3f( tamx,-tamy, tamz);
		glVertex3f( tamx, tamy, tamz);
		glVertex3f( tamx, tamy,-tamz);

		glNormal3f( 0,-1,0);
		glVertex3f(-tamx, tamy,-tamz);
		glVertex3f(-tamx, tamy, tamz);
		glVertex3f( tamx, tamy, tamz);
		glVertex3f( tamx, tamy,-tamz);

		glNormal3f( 0, 1,0);
		glVertex3f(-tamx,-tamy,-tamz);
		glVertex3f(-tamx,-tamy, tamz);
		glVertex3f( tamx,-tamy, tamz);
		glVertex3f( tamx,-tamy,-tamz);
	glEnd();
}

// Desenha a esfera na posição atual
void DesenhaCena(void)
{
	glPushMatrix();
		glTranslatef(xAtualDaEsfera, yAtualDaEsfera,0);
		glColor3f(1.0f, 0.5f, 0.0f);
		glutSolidSphere(5.0,20,20);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(xAtualDaEsfera2, yAtualDaEsfera2,0);
		glColor3f(0.0f, 1.0f, 0.0f);
		glutSolidSphere(8.0,20,20);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(xAtualCubo, yAtualCubo, 0);
		glColor3f(1.0, 0.0, 0.0);
		glutSolidCube(10);
	glPopMatrix();

	// Atualiza posição atual
	xAtualDaEsfera += velocDeslocEsfera_X;
	yAtualDaEsfera += velocDeslocEsfera_Y;
	xAtualDaEsfera2 += velocDeslocEsfera2_X;
	yAtualDaEsfera2 += velocDeslocEsfera2_Y;
	xAtualCubo += velocDeslocCubo_X;
	yAtualCubo += velocDeslocCubo_Y;

	// Se "bater" nos lados, inverte o sentido do movimento da esfera
	if(xAtualDaEsfera >= 25 || xAtualDaEsfera <= -25)
		velocDeslocEsfera_X = -velocDeslocEsfera_X;

	if(yAtualDaEsfera >= 25 || yAtualDaEsfera < -25)
		velocDeslocEsfera_Y = -velocDeslocEsfera_Y;

	if(xAtualDaEsfera2 >= 25 || xAtualDaEsfera2 <= -25)
		velocDeslocEsfera2_X = -velocDeslocEsfera2_X;

	if(yAtualDaEsfera2 >= 25 || yAtualDaEsfera2 < -25)
		velocDeslocEsfera2_Y = -velocDeslocEsfera2_Y;

	// Se "bater" nos lados, inverte o sentido do movimento do cubo
	if(xAtualCubo >= 25 || xAtualCubo <= -25)
		velocDeslocCubo_X = -velocDeslocCubo_X;

	if(yAtualCubo >= 25 || yAtualCubo < -25)
		velocDeslocCubo_Y = -velocDeslocCubo_Y;

	if(xE < xAtualDaEsfera2 )
		if(yE < yAtualDaEsfera2 ){
		velocDeslocEsfera_X = -velocDeslocEsfera_X;
		velocDeslocEsfera_Y = -velocDeslocEsfera_Y;
		velocDeslocEsfera2_X = -velocDeslocEsfera2_X;
		velocDeslocEsfera2_Y = -velocDeslocEsfera2_Y;

		}


	glFlush();
}

// Função callback de redesenho da janela de visualização
#define	Q 0.9
void Desenha(void)
{
	// Motion blur ativo ?
	if(blur)
	{
		// Ajusta posição inicial da esfera
		xAtualDaEsfera = xInicioDaEsfera;
		yAtualDaEsfera = yInicioDaEsfera;
		for( int i=0; i < quadros; ++i)
		{
			// Limpa a janela
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Desenha os lados
			DesenhaLados();
			// Se for o primeiro redesenho
			if(primeiro)
			{
				// Copia tela para accum buffer
				glAccum(GL_LOAD,1.0);
				primeiro = false;
			}
			DesenhaCena();
			// Faz "fade out" na imagem acumulada até agora
			glAccum(GL_MULT, Q);
			// E acumula imagem corrente
			glAccum(GL_ACCUM, 1-Q);
		}
		// Retorna resultado na tela
		glAccum(GL_RETURN, 1.0);
	}
	else
	{
		// Modo normal
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DesenhaLados();
		DesenhaCena();
	}

	// Se estiver animando...
	if(animado)
	{
		// Atualiza posição inicial como sendo a atual
		xInicioDaEsfera = xAtualDaEsfera;
		yInicioDaEsfera = yAtualDaEsfera;
		xInicioDaEsfera2 = xAtualDaEsfera2;
		yInicioDaEsfera2 = yAtualDaEsfera2;
		xInicioCubo = xAtualCubo;
		yInicioCubo = yAtualCubo;
	}

	glutSwapBuffers();
}

// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void)
{
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	DefineIluminacao();
	// Posiciona e orienta o observador
	glTranslatef(-posicaoObservadorX,-posicaoObservadorY,-posicaoObservadorZ);
	glRotatef(rotacaoX,1,0,0);
	glRotatef(rotacaoY,0,1,0);
}

// Função usada para especificar o volume de visualizaçãoo
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	gluPerspective(angle, fAspect, 0.5, 500);

	PosicionaObservador();
}

// Função callback para realizar a animação
void Anima(void)
{
	glutPostRedisplay();
}


// Função callback chamada para gerenciar eventos de teclas
void Teclado (unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);

	switch(key)
	{
		// Controla total de quadros gerados a cada redesenho
		case '+':
			quadros--;
			break;
		case '-':
			quadros++;
			break;
		// Ativa/desativa motion blur
		case 'b':
			blur=!blur;
			primeiro = true;
			break;
		// Ativa/desativa animação
		case 'a':
			animado = !animado;
			if(animado)
				glutIdleFunc(Anima);
			else
				glutIdleFunc(NULL);
			break;
	}

	glutPostRedisplay();
}

// Função callback para tratar eventos de teclas especiais
void TeclasEspeciais (int tecla, int x, int y)
{
	switch (tecla)
	{
		case GLUT_KEY_HOME:
			if(angle >= 10)
				angle -= 5;
			break;
			
		case GLUT_KEY_END:
			if(angle <= 150)
				angle += 5;
			break;
	}

	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

// Função callback para eventos de botões do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		// Salva os parâmetros atuais
		posicaoMouseInicial_X  = x;
		posicaoMouseInical_Y = y;
		observadorXInicial = posicaoObservadorX;
		observadorYInicial = posicaoObservadorY;
		observadorZInicial = posicaoObservadorZ;
		rotacaoXInicial = rotacaoX;
		rotacaoYInicial = rotacaoY;
		botao = button;
	}
	else
		botao = -1;
}

// Função callback para eventos de movimento do mouse
#define SENS_ROT	5.0
#define SENS_OBS	10.0
#define SENS_TRANSL	10.0
void GerenciaMovim(int x, int y)
{
	// Botões esquerdo ?
	if(botao == GLUT_LEFT_BUTTON)
	{
		// Calcula diferenças
		int deltax = posicaoMouseInicial_X  - x;
		int deltay = posicaoMouseInical_Y - y;
		// E modifica ângulos
		rotacaoY = rotacaoYInicial - deltax/SENS_ROT;
		rotacaoX = rotacaoXInicial - deltay/SENS_ROT;
	}
	// Botão direito ?
	else if(botao == GLUT_RIGHT_BUTTON)
	{
		// Calcula diferença
		int deltaz = posicaoMouseInical_Y - y;
		// E modifica distância do observador
		posicaoObservadorZ = observadorZInicial + deltaz/SENS_OBS;
	}
	// Botão do meio ?
	else if(botao == GLUT_MIDDLE_BUTTON)
	{
		// Calcula diferenças
		int deltax = posicaoMouseInicial_X  - x;
		int deltay = posicaoMouseInical_Y - y;
		// E modifica posições
		posicaoObservadorX = observadorXInicial + deltax/SENS_TRANSL;
		posicaoObservadorY = observadorYInicial - deltay/SENS_TRANSL;
	}
	PosicionaObservador();
	glutPostRedisplay();
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei largura, GLsizei altura)
{
	// Para previnir uma divisão por zero
	if ( altura == 0 )
		altura = 1;

	// Especifica as dimensções da viewport
	glViewport(0, 0, largura, altura);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)largura/(GLfloat)altura;

	// Se o tamanho da janela mudar, é necessário recomeçar o processo
	primeiro = true;

	EspecificaParametrosVisualizacao();
}

// Função responsável por inicializar parâmetros e variáveis
void Inicializa (void)
{
	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Limpa accum buffer
	glClear(GL_ACCUM_BUFFER_BIT);

	// Inicializa a variável que especifica o ângulo da projeção
	// perspectiva
	angle = 80;

	// Inicializa as variáveis usadas para alterar a posição do
	// observador virtual
	rotacaoX = 0;
	rotacaoY = 0;
	posicaoObservadorX = posicaoObservadorY = 0;
	posicaoObservadorZ = 55;
}

// Programa Principal
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	// Define o modo de operação da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ACCUM);

	// Especifica a posição inicial da janela GLUT
	glutInitWindowPosition(5,5);

	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(450,450);

	// Cria a janela passando como argumento o título da mesma
	glutCreateWindow("Exemplo de motion blur");

	// Registra a função callback de redesenho da janela de visualização
	glutDisplayFunc(Desenha);

	// Registra a função callback de redimensionamento da janela de visualização
	glutReshapeFunc(AlteraTamanhoJanela);

	// Registra a função callback para tratamento das teclas normais
	glutKeyboardFunc(Teclado);

	// Registra a função callback para tratamento das teclas especiais
	glutSpecialFunc(TeclasEspeciais);

	// Registra a função callback para eventos de botões do mouse
	glutMouseFunc(GerenciaMouse);

	// Registra a função callback para eventos de movimento do mouse
	glutMotionFunc(GerenciaMovim);

	// Registra a função callback para quando o sistema estiver ocioso
	glutIdleFunc(Anima);

	// Chama a função responsável por fazer as inicializações
	Inicializa();

	// Inicia o processamento e aguarda interações do usuário
	glutMainLoop();

	return 0;
}
