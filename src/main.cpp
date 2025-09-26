#include "../include/sphere.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <vector>

std::vector<Sphere> planets = {Sphere(0.5, {0.0, 0.0, 0.0}, 0.0, 0.0),
                               Sphere(0.2, {-2.0, 0.0, 0.0}, 0.0, 0.0)};

void init(void) {
  GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLfloat mat_shininess[] = {100.0};

  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  for (Sphere planet : planets) {
    Position pos = planet.getPosition();

    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glutSolidSphere(planet.getRadius(), 100, 100);
    glPopMatrix();
  }
  glFlush();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
    exit(0);
    break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
