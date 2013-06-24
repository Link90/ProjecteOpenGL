#ifndef _CAMERAIDI_H_
#define _CAMERAIDI_H_

class cameraIDI;

#include "point.h"
#include "scene.h"


class cameraIDI{

  float anglecam;
  double anterior, posterior; 
  Point VRP;
  double dist; 
  float angleX, angleY, angleZ;
  float zoom;


 public:  

  void iniciTGModelat();
  void identitat();
  void fiTGModelat();
  void trasllada(float tx, float ty, float tz);
  void escala(float sx, float sy, float sz);
  void rota(float graus, float rx, float ry, float rz);
  void resetCam();
  void computeDefaultCamera(double r, Point center);
  void setModelview();
  void setProjection(int width, int height);
  void twist(float angx, float angy);
  void fzoom(float newang);
  void fps(Point pos, float orientation, int width, int height);



};



#endif
