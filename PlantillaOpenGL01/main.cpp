/*
	Proyecto 3 Simulación de dos olas
	Computación Gráfica I
	Prof. Eduardo Roa
	Autores:
		Andres Navarro 11-10688
		Meggie Sanchez 11-10939
	Fecha de última actualización:
		25/05/2016
*/

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

#define DEF_floorGridScale	1.0f
#define DEF_floorGridXSteps	10.0f
#define DEF_floorGridZSteps	10.0f

// Variables booleanas para saber con cual ola se va a interactuar en el teclado
bool wave1 = false;
bool wave2 = false;
bool activo = false;

float t; // Variable global para la animacion

// Variables para ola 1
float A1 = 0.4;
float S1 = 2.0;
float L1 = 8.0;
float dirX1 = 0.0;
float dirY1 = -1.0;
float W1, phi1, normaliz1, prodesc1;

// Variables para ola 2
float A2 = 0.0;
float S2 = 0.0;
float L2 = 4.0;
float dirX2 = 1.0;
float dirY2 = 1.0;
float W2, phi2, normaliz2, prodesc2;

// Ecuacion para la simulacion
float h;

GLfloat knotsSup[25]; // Arreglo para la superficie knots

GLfloat ctlpointsNurbsSup[21][21][3]; // Matriz para hacer los puntos de control

GLUnurbsObj *theNurbSup; // Objeto Nurb para la superficie

void ejesCoordenada() {
	
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glLineWidth(1.0);
}

void changeViewport(int w, int h) {
	
	t += 0.1;
	float aspectratio;

	if (h==0)
		h=1;

   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 200.0);
   glMatrixMode (GL_MODELVIEW);
}

float simulacion(float x, float z, float t) {

	normaliz1 = sqrt(dirX1*dirX1 + dirY1*dirY1);
	normaliz2 = sqrt(dirX2*dirX1 + dirY2*dirY2);

	dirX1 = dirX1 / normaliz1;
	dirX2 = dirX2 / normaliz2;
	dirY1 = dirY1 / normaliz1;
	dirY2 = dirY2 / normaliz2;

	prodesc1 = dirX1*x + dirY1*z;
	prodesc2 = dirX2*x + dirY2*z;

	W1 = 2 * 3.1416 / L1;
	W2 = 2 * 3.1416 / L2;
	phi1 = S1 * 2 * 3.1416 / L1;
	phi2 = S2 * 2 * 3.1416 / L2;

	h = (A1 * (sin(prodesc1 * W1 + t * phi1) ) ) + (A2 * (sin(prodesc2 * W2 + t * phi2) ) );
	return h;
}

void animacion(int value) {
	
	if (activo == true) {

		t += 0.1;

		// Cambia altura en Y en los correspondientes puntos de control
		for (int i = 0; i < 21; i++) {
			for (int j = 0; j < 21; j++) {
				ctlpointsNurbsSup[i][j][1] = simulacion(ctlpointsNurbsSup[i][j][0], ctlpointsNurbsSup[i][j][2], t);
			}
		}
	}

	glutTimerFunc(10,animacion,1);
	glutPostRedisplay();
}

void init_surface() {
	
	float contx = 10.0;
	float conty = 0.0;
	float contz = 10.0;
	float aux = 0.0;

	// Puntos de control
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 21; j++) {
			ctlpointsNurbsSup[i][j][0] = contx - j;
			ctlpointsNurbsSup[i][j][1] = conty;
			ctlpointsNurbsSup[i][j][2] = contz;
		}
		contz -= 1;
	}

	// Arreglo knots para la superficie
	for (int i = 0; i < 25; i++) {
		if (i < 4) {
			knotsSup[i] = 0.0;
		}
		else if (i > 20) {
			knotsSup[i] = 1.0;
		}
		else {
			aux += 0.058; 
			knotsSup[i] = aux;
		}
	}
}

void init() {

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);

   theNurbSup = gluNewNurbsRenderer();
   gluNurbsProperty(theNurbSup, GLU_SAMPLING_TOLERANCE, 15.0);
   gluNurbsProperty(theNurbSup, GLU_DISPLAY_MODE, GLU_FILL);

   init_surface();

   glutTimerFunc(10, animacion, 1);
   t = 0.0;
}

void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
	case 27:             
		exit (0);
		break;

	// Caso para cambiar los valores de la ola 1
	case '1':
		wave1 = true;
		wave2 = false;
		break;

	// Caso para cambiar los valores de la ola 2
	case '2':
		wave2 = true;
		wave1 = false;
		break;

	// Empezar la animacion de las olas
	case 'r':
		activo = true;
		break;
  
	// Detener la animacion de las olas
	case 'p':
		activo = false;
		break;

	// L: Distancia entre cada ola
	case 'a':
		if (wave1 == true) {
			L1 = L1 - 0.1;
		}
		else {
			L2 = L2 - 0.1;
		}
		break;

	case 'z':
		if (wave1 == true) {
			L1 = L1 + 0.1;
		}
		else {
			L2 = L2 + 0.1;
		}
		break;

	// A: altura de la ola
	case 's':
		if (wave1 == true) {
			A1 = A1 - 0.1;
		}
		else {
			A2 = A2 - 0.1;
		}
		break;

	case 'x':
		if (wave1 == true) {
			A1 = A1 + 0.1;
		}
		else {
			A2 = A2 + 0.1;
		}
		break;

	// S: velocidad de la ola
	case 'd':
		if (wave1 == true) {
			S1 = S1 - 0.1;
		}
		else {
			S2 = S2 - 0.1;
		}
		break;

	case 'c':
		if (wave1 == true) {
			S1 = S1 + 0.1;
		}
		else {
			S2 = S2 + 0.1;
		}
		break;

	// D: vector de dos coordenadas que determina la dirección de la ola
	case 'f':
		if (wave1 == true) {
			dirX1 = dirX1 - 0.1;
		}
		else {
			dirX2 = dirX2 - 0.1;
		}
		break;

	case 'v':
		if (wave1 == true) {
			dirX1 = dirX1 + 0.1;
		}
		else {
			dirX2 = dirX2 + 0.1;
		}
		break;

	case 'g':
		if (wave1 == true) {
			dirY1 = dirY1 - 0.1;
		}
		else {
			dirY2 = dirY2 - 0.1;
		}
		break;

	case 'b':
		if (wave1 == true) {
			dirY1 = dirY1 + 0.1;
		}
		else {
			dirY2 = dirY2 + 0.1;
		}
		break;
  }
}

void render() {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (25.0, 12.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Luz y material
	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.9, 1.0 };
	GLfloat mat_specular[] = { 0.8, 0.8, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 60.0 };
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat light_ambient[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light_position[] = { -10.0, 5.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   

	// Render Grid 
	/*glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glPushMatrix();
	glRotatef(90,1.0,0.0,0.0);
    glColor3f( 0.0, 0.7, 0.7 );
    glBegin( GL_LINES );
    zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
    for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ )
	{
	xLocal = DEF_floorGridScale * loopX;
	glVertex3f( xLocal, -zExtent, 0.0 );
	glVertex3f( xLocal, zExtent,  0.0 );
	}
    xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
    for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ )
	{
	zLocal = DEF_floorGridScale * loopZ;
	glVertex3f( -xExtent, zLocal, 0.0 );
	glVertex3f(  xExtent, zLocal, 0.0 );
	}
    glEnd();
	//ejesCoordenada();
    glPopMatrix();
	glEnable(GL_LIGHTING);*/
	// Fin Grid
	
	// Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);	
	
	glPushMatrix();

	gluBeginSurface(theNurbSup);
    /* No cambien los numeros de la funcion, solo deben de poner los nombres de las variables correspondiente. */
		gluNurbsSurface(theNurbSup, 
                   25, knotsSup, 25, knotsSup,
                   21 * 3, 3, &ctlpointsNurbsSup[0][0][0], 
                   4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(theNurbSup);
	
	glPopMatrix();
	
	/* Muestra los puntos de control */
	/*
	int i,j;
	glPointSize(5.0);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POINTS);
	for (i = 0; i < 21; i++) {
		for (j = 0; j < 21; j++) {
	        glVertex3f(ctlpointsNurbsSup[i][j][0], 	ctlpointsNurbsSup[i][j][1], ctlpointsNurbsSup[i][j][2]);
		}
	}
	glEnd();
	*/

	glEnable(GL_LIGHTING);
		
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glutSwapBuffers();
}

int main (int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(960,540);
	glutCreateWindow("Nurbs Proyecto - Ola");

	init();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(Keyboard);
		
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	
	glutMainLoop();
	return 0;
}
