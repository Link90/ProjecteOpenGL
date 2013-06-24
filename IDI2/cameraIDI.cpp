#include "cameraIDI.h"
#include <cmath>


void cameraIDI::computeDefaultCamera(double r, Point center)
{

    VRP = center;

    dist = r;

  anterior = 3.;
  posterior = 150.;
  anglecam = 60;

  // inicialitzem els angles per a veure-ho des d'un cert punt de vista   
  angleX = 45;
  angleY = 45;
  angleZ = 0;

  zoom=0;
}


void cameraIDI::setModelview() 
{
  glMatrixMode(GL_MODELVIEW); //Modifiquem matriu ModelView (Camera)      
  glLoadIdentity(); //Coloquem camera a l'origent (punt per defecte)      

  trasllada(0, 0, -80);
  rota(angleX, 1, 0, 0);
  rota(angleY, 0, -1, 0);
  trasllada(-VRP.x, -VRP.y, -VRP.z);
}


void cameraIDI::setProjection(int width, int height) 
{
  float aspect=float(width)/float(height); //relació d'aspecte = amplada / alçada   

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(anglecam,aspect,anterior,posterior);

}


void cameraIDI::resetCam() 
{
  angleX = 45;
  angleY = 45;
  angleZ = 0;
  anglecam=60;
}



void cameraIDI::iniciTGModelat()
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
}

void cameraIDI::identitat()
{
  glLoadIdentity();
}

void cameraIDI::fiTGModelat()
{
  glPopMatrix();
}

void cameraIDI::trasllada(float tx, float ty, float tz)
{
  glTranslatef(tx, ty, tz);
}

void cameraIDI::escala(float sx, float sy, float sz)
{
  glScalef(sx, sy, sz);
}

void cameraIDI::rota(float graus, float rx, float ry, float rz)
{
  glRotatef(graus, rx, ry, rz);
}

void cameraIDI::fzoom(float newang) {
    int auxang = newang + anglecam;
    if(auxang < 180 and auxang > 0) anglecam += newang;
}

void cameraIDI::twist(float angx, float angy) {
    angleX = angleX + angy;
    angleY = angleY - angx;
}

void cameraIDI::fps(Point pos, float orientation, int width, int height){
    cout << "enter" << endl;
    float z =pos.z - 1*cos(orientation*M_PI/180);
    float x =pos.x - 1*sin(orientation*M_PI/180);
    Point vrp(x, 1, z);
    VRP = vrp;
    dist = 1;
    anterior = 1;
    posterior = 150;
    anglecam = 60;
    angleX = 0;
    angleY = 0;
    angleZ = 0;
    zoom = 0;
    setProjection(width, height);
    glMatrixMode(GL_MODELVIEW); //Modifiquem matriu ModelView (Camera)
    glLoadIdentity();
    //altura del tanque con su caja en lugar d 1
    cout << "Look at: " << vrp.x << " " << vrp.y << " " << vrp.z << endl;
    gluLookAt(pos.x, 2, pos.z, vrp.x, 2, vrp.z, 0, 1, 0);
}
