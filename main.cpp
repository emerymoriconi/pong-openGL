#include <iostream> 
#include <GL/glut.h>
#include "glut_text.h"

// Dimens�es da janela
int windowWidth = 800;
int windowHeight = 600;

static bool canDetectCollision = true;

// Posi��es e velocidades das raquetes e bola
float racket1Y = 0.0f;
float racket2Y = 0.0f;
float racketSpeed = 0.05f;
float racketSpeed2 = 0.05f;

float ballX = 0.0f;
float ballY = 0.0f;
float ballSpeedX = 0.04f;
float ballSpeedY = 0.03f;
float ballSize = 0.05f;
float maxspeedx = 0.08f;
float velinicialy = 0.03f;

// Vari�veis para rastrear o estado das teclas
bool keyWPressed = false;
bool keySPressed = false;
bool keyUpPressed = false;
bool keyDownPressed = false;
bool descendo = false;
bool subindo = false;

int scorePlayer1 = 0;
int scorePlayer2 = 0;

void enableCollisionDetection(int value) {
    canDetectCollision = true;
}

void drawSideWalls() {
    // Desenhar paredes laterais
    
    // Paredes � esquerda
    glPushMatrix();
    glColor3f(0.7843, 0.6353, 0.7843); // Cor lil�s para as paredes laterais
    glTranslatef(-2, 0, 0);
    glScalef(0.2, 3.85, 0.15);
    glutSolidCube(1.0);
    glPopMatrix();

    // Paredes � direita
    glPushMatrix();
    glColor3f(0.7843, 0.6353, 0.7843); // Cor lil�s para as paredes laterais
    glTranslatef(2, 0, 0);
    glScalef(0.2, 3.85, 0.15);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Paredes superiores
    glPushMatrix();
    glColor3f(0.7843, 0.6353, 0.7843); // Cor lil�s para as paredes laterais
    glTranslatef(0, 2, 0);
    glScalef(3.85, 0.2, 0.15);
    glutSolidCube(1.0);
    glPopMatrix();

    // Paredes inferiores
    glPushMatrix();
    glColor3f(0.7843, 0.6353, 0.7843); // Cor lil�s para as paredes laterais
    glTranslatef(0, -2, 0);
    glScalef(3.85, 0.2, 0.15);
    glutSolidCube(1.0);
    glPopMatrix();
}

void init() {
    glClearColor(1, 1, 1, 1);

    // Configura��o geral dos materiais
    GLfloat mat_ambient_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient_diffuse);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // Configura��o da ilumina��o
    GLfloat light_position[] = {-3.0, 5.0, 5.0, 0.0};
    GLfloat light_color[] = {1.0, 1.0, 1.0, 1.0};  // Luz branca
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Configura��o de reflex�o especular nos materiais
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);  // Normaliza��o autom�tica das normais

    // Configura��o do modelo de sombreamento Gouraud.
    glShadeModel(GL_INVALID_ENUM);
}

void render(){
	glClearColor(0.9, 0.9, 0.9, 0.5);
    glClear(GL_COLOR_BUFFER_BIT);

    drawSideWalls();

    // Desenhar raquetes
    glPushMatrix();
    glColor3f(0.6, 0.0, 0.0);
    glTranslatef(-1.7, racket1Y, 0.0);
    glScalef(0.06, 0.7, 0.2);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.6);
    glTranslatef(1.7, racket2Y, 0.0);
    glScalef(0.06, 0.7, 0.2);
    glutSolidCube(1.0);
    glPopMatrix();

    // Desenhar bola
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(ballX, ballY, 0.0);
    glutSolidSphere(ballSize, 20, 20);
    glPopMatrix();

    // Desenhar placar
    draw_score(-0.6, 0.9, scorePlayer1, scorePlayer2);

    glutSwapBuffers();
}

// Fun��o de atualiza��o
void update(int value) {
    // Atualizar posi��o da bola
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Verificar colis�o com as paredes laterais
    if (ballX + ballSize > 1.9) {
        // Gol, reiniciar no centro
        ballX = 0.0f;
        ballY = 0.0f;
        ballSpeedX = -0.04f;
        ballSpeedY = velinicialy;
        racket1Y = 0.0f;
        racket2Y = 0.0f;
        scorePlayer1++;
    }
    
    if (ballX - ballSize < -1.9) {
        // Gol, reiniciar no centro
        ballX = 0.0f;
        ballY = 0.0f;
        ballSpeedX = 0.04f;
        ballSpeedY = velinicialy;
        scorePlayer2++;
    }

    // Verificar colis�o com as paredes superiores e inferiores
    if (ballY + ballSize > 1.9 || ballY - ballSize < -1.9) {
        ballSpeedY = -ballSpeedY*1.1;
    }

    // Verificar colis�o com as raquetes
    if (canDetectCollision) {
        if (ballX - ballSize < -1.73 && ballY > racket1Y - 0.35 && ballY < racket1Y + 0.35) {
            ballSpeedX = -ballSpeedX*1.1;
            if (ballSpeedX>maxspeedx){
				ballSpeedX = maxspeedx;
			}
            ballX = -1.73 + ballSize; // Adjust the ball's position
            canDetectCollision = false;
            glutTimerFunc(500, enableCollisionDetection, 0); // Enable collision detection after 500 milliseconds
        }

        if (ballX + ballSize > 1.73 && ballY > racket2Y - 0.35 && ballY < racket2Y + 0.35) {
            ballSpeedX = -ballSpeedX*1.1;
            ballX = 1.73 - ballSize; // Adjust the ball's position
            canDetectCollision = false;
            glutTimerFunc(500, enableCollisionDetection, 0); // Enable collision detection after 500 milliseconds
        }
    }

    // Mover as raquetes com base no estado das teclas pressionadas
    if (keyWPressed && racket1Y + 0.1 < 1.6) {
        racket1Y += racketSpeed;
    }

    if (keySPressed && racket1Y - 0.1 > -1.6) {
        racket1Y -= racketSpeed;
    }

    if (ballY > racket2Y +0.05f && racket2Y + 0.1 < 1.6) {
        racket2Y += racketSpeed2;
    }

    if (ballY < racket2Y - 0.05f && racket2Y - 0.1 > -1.6) {
        racket2Y -= racketSpeed2;
    }

    // Redesenhar
    glutPostRedisplay();

    // Chamar a fun��o de atualiza��o novamente ap�s um intervalo de tempo
    glutTimerFunc(16, update, 0);
}

// Fun��o de redimensionamento da janela
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, -1.5, 5.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Fun��o de entrada do teclado
void keyboard(unsigned char key, int x, int y) {
    if (key == 'w') {
        keyWPressed = true;
    } else if (key == 's') {
        keySPressed = true;
    } else if (key == 27) {  // Tecla ESC
        exit(0);
    }
}

// Fun��o de entrada das setas do teclado
void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        keyUpPressed = true;
    } else if (key == GLUT_KEY_DOWN) {
        keyDownPressed = true;
    }
}

// Fun��o chamada quando uma tecla � liberada
void keyReleased(unsigned char key, int x, int y) {
    if (key == 'w') {
        keyWPressed = false;
    } else if (key == 's') {
        keySPressed = false;
    }
}

// Fun��o chamada quando uma tecla de seta � liberada
void specialKeyReleased(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        keyUpPressed = false;
    } else if (key == GLUT_KEY_DOWN) {
        keyDownPressed = false;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Pong 3D");

    init();
    
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutKeyboardUpFunc(keyReleased);  // Registrar fun��o para tratar teclas liberadas
    glutSpecialUpFunc(specialKeyReleased);  // Registrar fun��o para tratar teclas de setas liberadas
    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}
