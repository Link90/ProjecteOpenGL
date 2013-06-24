#ifndef _objectInstance_H_
#define _objectInstance_H_

class ObjectInstance;

#include <vector>
#include "point.h"
#include "model.h"
#include "box.h"
#include "cameraIDI.h"

using namespace std;

class ObjectInstance
{
 private:
  int object;   // identificador de l'objecte
  Point pos;    // posicio sobre el terra
  Vector size;	   // mida en unitats
  float orientation;  //orientació respecte Y
  Box _transBox;      // caixa contenidora de l'objecte transformat.

 public:
  ObjectInstance(int idob, Point p, Vector sz, float ori);
  ~ObjectInstance(void);

  void Render(std::vector<Model> &);

  int getObjectId();
  Point getposition();
  float getorientation();
  void computeTransformedBox(Model &obj);

  Box & getTransformedBox ();
  void movement(int i);
  void twisted(int i);
};

#endif
