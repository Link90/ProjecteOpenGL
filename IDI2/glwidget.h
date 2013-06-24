#ifndef _GLWIDGET_H_
#define _GLWIDGET_H_

#include <QtOpenGL/qgl.h>
#include <iostream>
#include <qstring.h>
#include <qfiledialog.h>
#include <qtimer.h>
#include <QKeyEvent>

#include "material_lib.h"
#include "point.h"
#include "scene.h"
#include "cameraIDI.h"


class GLWidget : public QGLWidget
{
  Q_OBJECT

 public:
  GLWidget(QWidget * parent);

 public slots:

  void loadmodel();

  // help - Ajuda per la terminal des de la que hem  engegat el programa.
  void help(void);
	
  //SLOTS
  // Afegiu aqu� la declaraci� dels slots que necessiteu
  void resetCam();
  
  //SIGNALS
  // Afegiu aqu� la declaraci� dels signals que necessiteu
signals:



 protected:
  // initializeGL() - Aqui incluim les inicialitzacions del contexte grafic.
  virtual void initializeGL();

  // paintGL - M�tode cridat cada cop que cal refrescar la finestra.
  // Tot el que es dibuixa es dibuixa aqui.
  virtual void paintGL( void );
 
  // resizeGL() - Es cridat quan canvi la mida del widget
  virtual void resizeGL (int width, int height);

  // keyPressEvent() - Cridat quan es prem una tecla
  virtual void keyPressEvent(QKeyEvent *e);

  // mousePressEvent() i mouseReleaseEvent()
  virtual void mousePressEvent( QMouseEvent *e);
  virtual void mouseReleaseEvent( QMouseEvent *);
 
  // mouseMoveEvent() - Cridat quan s'arrosega el ratoli amb algun bot� premut.
  virtual void mouseMoveEvent(QMouseEvent *e);
 
  void computeDefaultCamera(); 
 
 private:

  // par�metres de la camera

  cameraIDI cam;
  // interaccio
  typedef  enum {NONE, ROTATE, ZOOM} InteractiveAction;
  InteractiveAction DoingInteractive;
 
  int   xClickr, yClickr, newxClick, newyClick, pyClick;
  double r;
  Point center;
  cameraIDI newcamera;
  bool bt;
  
  Scene scene;  // Escena a representar en el widget

  QTimer timer;
};

#endif
