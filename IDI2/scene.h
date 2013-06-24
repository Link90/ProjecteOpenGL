#ifndef _Scene_H_
#define _Scene_H_
#include "objectInstance.h"
#include "model.h"
#include "ground.h"
#include "cameraIDI.h"
#include <QtOpenGL/qgl.h>


class Scene
{
	friend class XML;
 private:

	// Això és per a millorar la cerca dels objectes dins del vector.
	static int LEAVES;
	static int STEM;
	static int CASA1;
	static int CASA2;
	static int CASA3;
	static int OBELISC;
	static int FAROLA;

	Ground ground;
	bool load;
        bool firstcharge;
	
	// Tindrem un vector amb els models geomètrics dels objectes urbans
	// bàsics (o primitius) i un altre amb instàncies seves (instàncies o 
	// referències a objectes).

	std::vector<Model> lmodels;
	std::vector<ObjectInstance> lobjectsInstances;
        void pintaCub();
	void computeTransformedBox(ObjectInstance &oref);
	void transformPoint(ObjectInstance &oref, GLfloat p[4]);

 public:
	static MaterialLib matlib;	  // col·lecció de materials 

	Scene();

	void Init();
	void Render();
        int AddModel(Model &);
        int AddModel2(Model &);
	void AddObject(ObjectInstance &);
        void AddObject2(ObjectInstance &);
	void computeTransformedBoxes();
	void CalculaEsfera (double &radi, Point &centreEscena);

	void InitGround(double sx, double sz);
        void light();
	static void transformPoint(Point &p);
        void lmodel(string model, const char *path);
        bool scenecolision(Box bx);
        bool buildingcolision();
        void movement (int i, int width, int height, cameraIDI fpscamera);
        void twisted(int i, int width, int height, cameraIDI fpscamera);
        void changecamera(int widht, int height, cameraIDI newcamera);
};

#endif

