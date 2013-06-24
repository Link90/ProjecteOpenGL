#include "glwidget.h"

#include <stdlib.h>
#define _USE_MATH_DEFINES 1
#include <cmath>

// Constructora amb format per defecte
GLWidget::GLWidget(QWidget * parent)
: QGLWidget (parent)
{

  // Cal activar el timer per generar animacions a l'escena
  connect( &timer, SIGNAL(timeout()), this, SLOT(updateGL()) );
  timer.start(0);
  
  DoingInteractive = NONE;  
}


// initializeGL() - Aqui incloem les inicialitzacions del context grafic.
void GLWidget::initializeGL()
{
  glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);// ponia GL_LINE y hacia que pintase solo linias o poligonos con caras
  glEnable(GL_DEPTH_TEST);
  // inicialitzem també l'escena
  scene.Init();
  scene.CalculaEsfera(r, center);


  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);

  scene.computeTransformedBoxes();

  // dimensions escena i camera inicial
  bt = false;
  cam.computeDefaultCamera(r, center);
}


// paintGL() - callback cridat cada cop que cal refrescar la finestra. 
void GLWidget::paintGL( void ) { 

	// Esborrem els buffers
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        if(!bt) {
	cam.setProjection(width(), height());
	cam.setModelview();
    }

	// dibuixar eixos aplicacio
	glBegin(GL_LINES);
	glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0); // X
	glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0); // Y
	glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20); // Z
	glEnd();

	// pintem l'escena: terra, objectes i el que calgui
        GLfloat ambient[] = { 0.3, 0.3, 0.3, 1.0};
        GLfloat lightpos[] = { center.x, 20.0, center.z, 1.0};
        GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0};

        glLightfv(GL_LIGHT0,GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0,GL_POSITION, lightpos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	scene.Render();
        scene.light();
}

// resizeGL() - Cridat quan es canvia el tamany del viewport.
void GLWidget::resizeGL (int width, int height) {
	glViewport (0, 0, width, height);

}

// help() - Surt per la terminal des de la que hem 
// engegat el programa. En la versio amn interficie 
// hauria de crear una nova finestra amb la informacio...
void GLWidget::help( void ){
  cout<<"Tecles definides: \n";
  cout<<"f         Pinta en filferros\n";
  cout<<"s         Pinta amb omplert de polígons\n";
  cout<<"h,H,?     Imprimir aquesta ajuda\n";
  // Completar amb altres tecles que definiu...
  //
}

/*--------------------
*
*  teclat()
*
*  callback per quan hom prem una tecla.
*
*/
void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch( e->key() )
    {
      case Qt::Key_F: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        updateGL();
        break;
      case Qt::Key_S: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        updateGL();
        break;
    case Qt::Key_Right: this->scene.twisted(-3, width(), height(), newcamera);
        break;
    case Qt::Key_Left: this->scene.twisted(3, width(), height(), newcamera);
        break;
    case Qt::Key_Up: this->scene.movement(-3, width(), height(), newcamera);
        break;
    case Qt::Key_Down: this->scene.movement(3, width(), height(), newcamera);
      break;
    case Qt::Key_P: {
        scene.changecamera(width(), height(), newcamera);
        bt = !bt;
        break;
    }
      case  'h'  : case  'H'  : case  '?'  :  help();
        break;
      default: e->ignore(); // el propaguem cap al pare...
    }
}

/*--------------------
* mousePressEvent()
*/
void GLWidget::mousePressEvent( QMouseEvent *e){
  xClickr = e->x();
  yClickr = e->y();

  if (e->button()&Qt::LeftButton &&
      ! (e->modifiers()&(Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
  else if (e->button()&Qt::LeftButton &&  e->modifiers() &Qt::ShiftModifier)
  {
    DoingInteractive = ZOOM;
  }
}

/*--------------------
*
* mouseReleaseEvent()
*
* Callback Qt cridat quan es deixa anar el botó del
* ratolí.
*
*/
void GLWidget::mouseReleaseEvent( QMouseEvent *) {
  	DoingInteractive = NONE;
}

/*--------------------
*
* mouseMoveEvent()
*
* Callback Qt cridat quan s'arrosega el ratoli amb
* algun boto premut.
*
*/
void GLWidget::mouseMoveEvent(QMouseEvent *e) {
  	// Aquí cal que es calculi i s'apliqui la rotació o el zoom
 	// com s'escaigui...
	
    newxClick = e->x();
    newyClick = e->y();


        if (DoingInteractive==ROTATE) {
          // Rotar la camera
            float posx = (float) e->x() - xClickr;
            float posy = (float) e->y() - yClickr;
            cam.twist(posx, posy);
        }

        if (DoingInteractive==ZOOM) {
          // Fer zoom

            cam.fzoom(e->globalY() - pyClick);
            pyClick = e->globalY();
        }

        xClickr = newxClick;
        yClickr = newyClick;
        updateGL();
}



void GLWidget::resetCam() {	
  cam.resetCam();
  updateGL();
}


void GLWidget::loadmodel() {
    QString path = QFileDialog::getOpenFileName(this, tr("Carrega objecte"), "./models", tr("Objectes (*.obj)"));
    const char *auxpath = path.toAscii().data();

    scene.lmodel("Tank", auxpath);

}

