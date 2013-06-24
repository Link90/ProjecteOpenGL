#include "scene.h"
#include "XML.h"
#include <cmath>

int Scene::CASA1=0;
int Scene::CASA2=1;
int Scene::CASA3=2;
int Scene::OBELISC=3;

MaterialLib Scene::matlib;

Scene::Scene() {
	this->load=false;
}

void Scene::Init()
{
    cout << "----------" << endl;
  firstcharge = true;
  XML xmlloader;
  xmlloader.LoadSceneDescription(this, "escena.xml");
}

// Modifica un punt donat aplicant-li la transformació que es té guardada
// a la matriu MODELVIEW
void Scene::transformPoint(Point &p)
{
  GLfloat m[16];
  GLfloat p1[16];

  int i; 
  for (i=4; i<16; ++i) p1[i]=0.0f;

  p1[0] = p.x; 
  p1[1] = p.y; 
  p1[2] = p.z; 
  p1[3] = 1;

  glPushMatrix();
  glMultMatrixf(p1);
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  p.x = m[0];
  p.y = m[1];
  p.z = m[2];
  glPopMatrix();
}


// Mètode que pinta l'escena: el terra, els objectes i el que calgui
void Scene::Render() {

	//pintem el terra
	ground.Render();
	
	// Cal pintar tots els objectes de l'escena que es troben al vector
	//recorrem tots els objects i fem per cada object un object.Render()
  	int i;
	for (i=0;i<(int)this->lobjectsInstances.size();i++) 
	  {
        //    if (i%3 == 0) nomes dibuixava els modul 3 == 0
		this->lobjectsInstances[i].Render(lmodels);
		
         // }
            }
}

//----------------------------------------------------------------------------------------


int Scene::AddModel(Model &o)
{
  lmodels.push_back(o);
  return lmodels.size()-1;
}

int Scene::AddModel2(Model &o)
{
    lmodels[lmodels.size()-1] = o;
  return lmodels.size()-1;
}

void Scene::AddObject(ObjectInstance &oref)
{
  lobjectsInstances.push_back(oref);
}

void Scene::AddObject2(ObjectInstance &oref)
{
  lobjectsInstances[lobjectsInstances.size()-1] = oref;
}

// Calcula les caixes contenidores dels objectes de l'escena
// un cop s'han fet les transformacions de model
void Scene::computeTransformedBoxes()
{
  std::vector<ObjectInstance>::iterator ito;
  for (ito=lobjectsInstances.begin(); ito!=lobjectsInstances.end(); ito++)
  {
    (*ito).computeTransformedBox(lmodels[(*ito).getObjectId()]);
  }
}

void Scene::lmodel(string mod, const char *path) {

        cout << "enter" << endl;
        Model newmod(mod);
        newmod.readObj(path, Scene::matlib);
        newmod.updateBoundingBox();
        cout << "read obj" << endl;
        int idadded;
        if(firstcharge) {
            idadded = AddModel(newmod);
        }
        else idadded = AddModel2(newmod);



        Box modelbox = newmod.boundingBox();
        Vector dobject = (modelbox.maxb - modelbox.minb);
        Vector dground (Point (ground.MidaX(), 0.0, ground.MidaZ()));
        float groundlength = dground.length();
        dobject.normalize();
        dobject = dobject*(groundlength/20);
        ObjectInstance newobjinstance(idadded, Point(ground.MidaX()/2, 0.0, ground.MidaZ()/2),dobject, 0.0);
        if(firstcharge) {
            firstcharge = false;
            AddObject(newobjinstance);
         }else AddObject2(newobjinstance);

}

bool Scene::scenecolision(Box bx) {
    return bx.maxb.x > ground.MidaX() or  bx.minb.x < 0 or bx.maxb.z > ground.MidaZ() or bx.minb.z < 0;

}

bool Scene::buildingcolision(){
    Point posbuilding;
    float rtank, rbuilding;
    Box baux = lobjectsInstances[lobjectsInstances.size()-1].getTransformedBox();
    Vector vaux(baux.maxb.x - baux.minb.x, 0, baux.maxb.z - baux.minb.z);
    rtank = vaux.length() / 2;

    Point postank = lobjectsInstances[lobjectsInstances.size()-1].getposition();

    for(int i = 0; i <= lobjectsInstances.size()-2; ++i){
        Box baux2 = lobjectsInstances[i].getTransformedBox();
        Vector vobject(baux2.maxb.x - baux2.minb.x, 0, baux2.maxb.z - baux2.minb.z);
        rbuilding = vobject.length() / 2;
        posbuilding = lobjectsInstances[i].getposition();
        Vector distance(posbuilding - postank);
        if(distance.length() < (rtank + rbuilding)) return true;
    }
    return false;
}

void Scene::movement(int i, int width, int height, cameraIDI fpscamera) {
    if(!firstcharge) {
        lobjectsInstances[lobjectsInstances.size()-1].movement(i);
        lobjectsInstances[lobjectsInstances.size()-1].computeTransformedBox(lmodels[lmodels.size()-1]);
        Box auxtank = lobjectsInstances[lobjectsInstances.size()-1].getTransformedBox();
        if(scenecolision(auxtank) or buildingcolision()){
           lobjectsInstances[lobjectsInstances.size()-1].movement(-i);
           //lobjectsInstances[lobjectsInstances.size()-1].computeTransformedBox(lmodels[lmodels.size()-1]);
        }
        fpscamera.fps(lobjectsInstances[lobjectsInstances.size()-1].getposition(), lobjectsInstances[lobjectsInstances.size()-1].getorientation(), width, height);
    }
}

void Scene::twisted(int i, int width, int height, cameraIDI fpscamera) {
    if(!firstcharge) {
        lobjectsInstances[lobjectsInstances.size()-1].twisted(i);
        lobjectsInstances[lobjectsInstances.size()-1].computeTransformedBox(lmodels[lmodels.size()-1]);
        Box aux = lobjectsInstances[lobjectsInstances.size()-1].getTransformedBox();
        if(scenecolision(aux) or buildingcolision()){
           lobjectsInstances[lobjectsInstances.size()-1].twisted(-i);
           //lobjectsInstances[lobjectsInstances.size()-1].computeTransformedBox(lmodels[lmodels.size()-1]);
        }
        fpscamera.fps(lobjectsInstances[lobjectsInstances.size()-1].getposition(), lobjectsInstances[lobjectsInstances.size()-1].getorientation(), width, height);
}
    }


void Scene::changecamera(int widht, int height, cameraIDI newcamera) {
    Point pos = lobjectsInstances[lobjectsInstances.size()-1].getposition();
    float orientation = lobjectsInstances[lobjectsInstances.size()-1].getorientation();
    newcamera.fps(pos, orientation, widht, height);
}

void Scene::pintaCub() {
    float mida;
    mida = 1;

    GLfloat colormat[] = { 0.0, 1.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT,  GL_AMBIENT_AND_DIFFUSE,  colormat);
    glNormal3f(0, 1, 0);

    glBegin(GL_QUADS);

    glNormal3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, mida);
    glVertex3f(mida, 0, mida);
    glVertex3f(mida, 0, 0);

    glNormal3f(0, -1, 0);
    glVertex3f(0, mida, 0);
    glVertex3f(0, mida, mida);
    glVertex3f(mida, mida, mida);
    glVertex3f(mida, mida, 0);

    glNormal3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, mida, 0);
    glVertex3f(0, mida, mida);
    glVertex3f(0, 0, mida);

    glNormal3f(-1, 0, 0);
    glVertex3f(mida, 0, 0);
    glVertex3f(mida, mida, 0);
    glVertex3f(mida, mida, mida);
    glVertex3f(mida, 0, mida);

    glNormal3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(mida, 0, 0);
    glVertex3f(mida, mida, 0);
    glVertex3f(0, mida, 0);

    glNormal3f(0, 0, -1);
    glVertex3f(0, 0, mida);
    glVertex3f(mida, 0, mida);
    glVertex3f(mida, mida, mida);
    glVertex3f(0, mida, mida);

    glEnd();
}

void Scene::light(){
    Point pos = lobjectsInstances[lobjectsInstances.size()-1].getposition();
    GLfloat ambient[] = { 0.50, 0.50, 0.50, 1.0 };
    GLfloat position[] = { pos.x, 20.0, pos.z, 1.0 };
    GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat direction[] = { 0.0, -1.0, 0.0 };
    int ang = 20;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(pos.x, 20, pos.z);
    pintaCub();
    glPopMatrix();
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, ang);
}




void Scene::CalculaEsfera (double &radi, Point &centreEscena)
{
  // creem la capsa inicialment amb els vèrtexs mínim i màxim del terra
  Box capsaEscena (Point (0.0, -ground.MidaY(), 0.0),
		   Point (ground.MidaX(), 0.0, ground.MidaZ()));

  // actualitzem la capsa amb tots els objectes de l'escena
  std::vector<ObjectInstance>::iterator ito;
  for (ito=lobjectsInstances.begin(); ito!=lobjectsInstances.end(); ito++)
  {
    Point punts[8];
    punts[0] = (*ito).getTransformedBox().minb;
    punts[7] = (*ito).getTransformedBox().maxb;

    punts[1].x = punts[7].x;
    punts[1].y = punts[0].y;
    punts[1].z = punts[0].z;
    
    punts[2].x = punts[0].x;
    punts[2].y = punts[7].y;
    punts[2].z = punts[0].z;
    
    punts[3].x = punts[0].x;
    punts[3].y = punts[0].y;
    punts[3].z = punts[7].z;
    
    punts[4].x = punts[7].x;
    punts[4].y = punts[7].y;
    punts[4].z = punts[0].z;

    punts[5].x = punts[0].x;
    punts[5].y = punts[7].y;
    punts[5].z = punts[7].z;
    
    punts[6].x = punts[7].x;
    punts[6].y = punts[0].y;
    punts[6].z = punts[7].z;

    for (int i=0; i<8; ++i)
      capsaEscena.update(punts[i]);
  }
  
  centreEscena =  (capsaEscena.minb + capsaEscena.maxb)/2;
  radi = (capsaEscena.maxb-capsaEscena.minb).length()/2.f;
}




//--------------------------------------------------------------------------------------------


void Scene::InitGround(double sx, double sz)
{
	ground.Init(sx,sz);
}




