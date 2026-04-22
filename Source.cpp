#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "imageloader.h"

#define M_PI 3.14159265358979323846

// Rotation angles
GLfloat bodyRotation = 0.0;
GLfloat headRotation = 0.0;
GLfloat leftArmRotation = 360.0;
GLfloat rightArmRotation = 360.0;
GLfloat leftLegRotation = 0.0;
GLfloat rightLegRotation = 0.0;

// Animation flag
GLboolean isAnimating = GL_FALSE;

// Mouse interaction variables
GLboolean mouseLeftDown = GL_FALSE;
GLboolean mouseRightDown = GL_FALSE;
GLboolean mouseMiddleDown = GL_FALSE;
GLfloat mouseX, mouseY;
GLfloat cameraAngleX = 0.0;
GLfloat cameraAngleY = 0.0;
GLfloat cameraDistance = 10.0;
GLfloat cameraHeight = 0.0;

// Colors for Doraemon
GLfloat bodyBlue[] = { 0.0, 0.6, 1.0 };
GLfloat faceWhite[] = { 1.0, 1.0, 1.0 };
GLfloat noseRed[] = { 1.0, 0.0, 0.0 };
GLfloat black[] = { 0.0, 0.0, 0.0 };
GLfloat yellow[] = { 1.0, 1.0, 0.0 };
GLfloat eyeBlack[] = { 0.1, 0.1, 0.1 };

// Draw a sphere
void drawSphere(GLfloat radius, GLint slices, GLint stacks) {
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluSphere(quadric, radius, slices, stacks);
    gluDeleteQuadric(quadric);
}

// Draw a cylinder
void drawCylinder(GLfloat baseRadius, GLfloat topRadius, GLfloat height, GLint slices, GLint stacks) {
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluCylinder(quadric, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(quadric);
}

// Draw Doraemon's bell
void drawBell() {
    // Bell base
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    drawSphere(0.25, 20, 20);

    // Bell details
    glColor3fv(black);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.15);
    drawSphere(0.05, 10, 10);
    glPopMatrix();

    // Bell strap
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    drawCylinder(0.25, 0.25, 0.1, 20, 5);
    glPopMatrix();
    glPopMatrix();
}

// Draw Doraemon's head 
void drawHead() {
    // Head sphere
    glPushMatrix();
    glColor3fv(bodyBlue);
    drawSphere(1.0, 30, 30);

    // Face (white oval area)
    glPushMatrix();
    glColor3fv(faceWhite);
    glTranslatef(0.0, -0.05, 0.6);
    glScalef(0.85, 0.9, 0.4);
    drawSphere(1.0, 30, 30);
    glPopMatrix();

    // Eyes 
    // Left eye white background
    glPushMatrix();
    glColor3fv(faceWhite);
    glTranslatef(-0.3, 0.25, 0.88);
    glScalef(1.0, 1.3, 0.15);
    drawSphere(0.22, 20, 20);
    glPopMatrix();

    // Left eye black outline
    glPushMatrix();
    glColor3fv(eyeBlack);
    glTranslatef(-0.3, 0.25, 0.92);
    glScalef(1.0, 1.3, 0.05);
    drawSphere(0.23, 20, 20);
    glPopMatrix();

    // Left eye white inner
    glPushMatrix();
    glColor3fv(faceWhite);
    glTranslatef(-0.3, 0.25, 0.93);
    glScalef(1.0, 1.3, 0.05);
    drawSphere(0.21, 20, 20);
    glPopMatrix();

    // Left eye pupil
    glPushMatrix();
    glColor3fv(black);
    glTranslatef(-0.27, 0.22, 0.95);
    drawSphere(0.08, 15, 15);
    glPopMatrix();

    // Right eye white background
    glPushMatrix();
    glColor3fv(faceWhite);
    glTranslatef(0.3, 0.25, 0.88);
    glScalef(1.0, 1.3, 0.15);
    drawSphere(0.22, 20, 20);
    glPopMatrix();

    // Right eye black outline
    glPushMatrix();
    glColor3fv(eyeBlack);
    glTranslatef(0.3, 0.25, 0.92);
    glScalef(1.0, 1.3, 0.05);
    drawSphere(0.23, 20, 20);
    glPopMatrix();

    // Right eye white inner
    glPushMatrix();
    glColor3fv(faceWhite);
    glTranslatef(0.3, 0.25, 0.93);
    glScalef(1.0, 1.3, 0.05);
    drawSphere(0.21, 20, 20);
    glPopMatrix();

    // Right eye pupil
    glPushMatrix();
    glColor3fv(black);
    glTranslatef(0.27, 0.22, 0.95);
    drawSphere(0.08, 15, 15);
    glPopMatrix();

    // Nose 
    glPushMatrix();
    glColor3fv(noseRed);
    glTranslatef(0.0, 0.05, 1.0);
    drawSphere(0.12, 20, 20);
    glPopMatrix();

    // Nose to mouth connector line
    glPushMatrix();
    glColor3fv(black);
    glTranslatef(0.0, -0.05, 1.0);
    glScalef(0.02, 0.3, 0.02);
    glutSolidCube(1.0);
    glPopMatrix();

    // Simple straight mouth
    glColor3fv(black);
    glPushMatrix();
    glTranslatef(0.0, -0.25, 0.98);
    glScalef(0.3, 0.04, 0.02);
    glutSolidCube(1.0);
    glPopMatrix();

    // Whiskers
    glColor3fv(black);
    // Left whiskers
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(-0.65, 0.1 - i * 0.12, 0.92);
        glRotatef((i == 0) ? 10 : (i == 2) ? -10 : 0, 0.0, 1.0, 0.0);
        glScalef(0.6, 0.015, 0.015);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Right whiskers
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(0.65, 0.1 - i * 0.12, 0.92);
        glRotatef((i == 0) ? -10 : (i == 2) ? 10 : 0, 0.0, 1.0, 0.0);
        glScalef(0.6, 0.015, 0.015);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    glPopMatrix();
}

// Draw Doraemon's hand
void drawHand() {
    glPushMatrix();
    glColor3fv(faceWhite);
    drawSphere(0.3, 20, 20);
    glPopMatrix();
}

// Draw Doraemon's arm
void drawArm(GLfloat rotationAngle) {
    glPushMatrix();
    glRotatef(rotationAngle, 1.0, 0.0, 0.0);

    // Upper arm
    glColor3fv(bodyBlue);
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    drawCylinder(0.25, 0.25, 0.8, 20, 5);
    glPopMatrix();

    // Hand
    glPushMatrix();
    glTranslatef(0.0, -0.8, 0.0);
    drawHand();
    glPopMatrix();
    glPopMatrix();
}

// Draw Doraemon's foot
void drawFoot() {
    glPushMatrix();
    glColor3fv(faceWhite);
    glScalef(1.0, 0.3, 1.5);
    drawSphere(0.35, 15, 20);
    glPopMatrix();
}

// Draw Doraemon's leg
void drawLeg(GLfloat rotationAngle) {
    glPushMatrix();
    glRotatef(rotationAngle, 1.0, 0.0, 0.0);

    // Upper leg
    glColor3fv(bodyBlue);
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    drawCylinder(0.3, 0.3, 0.9, 20, 5);
    glPopMatrix();

    // Foot
    glPushMatrix();
    glTranslatef(0.0, -0.95, 0.0);
    drawFoot();
    glPopMatrix();
    glPopMatrix();
}

// Draw Doraemon's pocket
void drawPocket() {
    glPushMatrix();
    // Pocket base
    glColor3fv(faceWhite);
    glTranslatef(0.0, 0.0, 0.8);
    glScalef(1.0, 1.0, 0.2);
    drawSphere(0.5, 20, 20);

    // Pocket inner
    glPushMatrix();
    glColor3fv(black);
    glTranslatef(0.0, 0.0, 0.3);
    glScalef(0.8, 0.8, 0.1);
    drawSphere(0.5, 20, 20);
    glPopMatrix();
    glPopMatrix();
}

// Draw Doraemon's body
void drawBody() {
    glPushMatrix();
    glRotatef(bodyRotation, 0.0, 1.0, 0.0);

    // Body sphere
    glColor3fv(bodyBlue);
    drawSphere(1.2, 30, 30);

    // Bell
    glPushMatrix();
    glTranslatef(0.0, 0.5, 1.0);
    drawBell();
    glPopMatrix();

    // Pocket
    glPushMatrix();
    glTranslatef(0.0, -0.2, 0.35);
    drawPocket();
    glPopMatrix();

    // Head
    glPushMatrix();
    glTranslatef(0.0, 1.5, 0.0);
    glRotatef(headRotation, 0.0, 1.0, 0.0);
    drawHead();
    glPopMatrix();

    // Left arm
    glPushMatrix();
    glTranslatef(-1.2, 0.5, 0.0);
    drawArm(leftArmRotation);
    glPopMatrix();

    // Right arm
    glPushMatrix();
    glTranslatef(1.2, 0.5, 0.0);
    drawArm(rightArmRotation);
    glPopMatrix();

    // Left leg
    glPushMatrix();
    glTranslatef(-0.5, -1.0, 0.0);
    drawLeg(leftLegRotation);
    glPopMatrix();

    // Right leg
    glPushMatrix();
    glTranslatef(0.5, -1.0, 0.0);
    drawLeg(rightLegRotation);
    glPopMatrix();
    glPopMatrix();
}

// Animation function
void animate() {
    if (isAnimating) {
        bodyRotation = fmod((bodyRotation + 0.5), 360.0);
        headRotation = 15.0 * sin(bodyRotation * M_PI / 180.0);
        leftArmRotation = 30.0 * sin(bodyRotation * M_PI / 90.0);
        rightArmRotation = 30.0 * sin((bodyRotation + 180.0) * M_PI / 90.0);
        leftLegRotation = 20.0 * sin(bodyRotation * M_PI / 90.0);
        rightLegRotation = 20.0 * sin((bodyRotation + 180.0) * M_PI / 90.0);

        glutPostRedisplay();
    }
}

// Mouse button callback
void mouse(int button, int state, int x, int y) {
    mouseX = x;
    mouseY = y;

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseLeftDown = GL_TRUE;
        }
        else if (state == GLUT_UP) {
            mouseLeftDown = GL_FALSE;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseRightDown = GL_TRUE;
        }
        else if (state == GLUT_UP) {
            mouseRightDown = GL_FALSE;
        }
    }
    else if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseMiddleDown = GL_TRUE;
        }
        else if (state == GLUT_UP) {
            mouseMiddleDown = GL_FALSE;
        }
    }
}

// Mouse motion callback
void mouseMotion(int x, int y) {
    GLfloat deltaX = x - mouseX;
    GLfloat deltaY = y - mouseY;

    if (mouseLeftDown) {
        // Left button: rotate camera around the model
        cameraAngleX -= deltaX * 0.5;
        cameraAngleY += deltaY * 0.5;

        // Limit vertical rotation
        if (cameraAngleY > 89.0) cameraAngleY = 89.0;
        if (cameraAngleY < -89.0) cameraAngleY = -89.0;

        glutPostRedisplay();
    }
    else if (mouseRightDown) {
        // Right button: zoom in/out
        cameraDistance += deltaY * 0.1;
        if (cameraDistance < 2.0) cameraDistance = 2.0;
        if (cameraDistance > 50.0) cameraDistance = 50.0;
        glutPostRedisplay();
    }
    else if (mouseMiddleDown) {
        // Middle button: pan camera up/down
        cameraHeight += deltaY * 0.05;
        glutPostRedisplay();
    }

    mouseX = x;
    mouseY = y;
}

// Keyboard function
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:  // ESC key
        exit(0);
        break;
    case 't':  // Toggle animation
        isAnimating = !isAnimating;
        break;
    case 'r':  // Reset rotations and camera
        bodyRotation = 0.0;
        headRotation = 0.0;
        leftArmRotation = 360.0;
        rightArmRotation = 360.0;
        leftLegRotation = 0.0;
        rightLegRotation = 0.0;
        cameraAngleX = 0.0;
        cameraAngleY = 0.0;
        cameraDistance = 10.0;
        cameraHeight = 0.0;
        glutPostRedisplay();
        break;
    case 'c':  // Reset camera only
        cameraAngleX = 0.0;
        cameraAngleY = 0.0;
        cameraDistance = 10.0;
        cameraHeight = 0.0;
        glutPostRedisplay();
        break;
    case 'p':  //play sound
        PlaySoundW(L"D:\\utm_4\\graphics\\FCG_project(Doraemon)\\Debug\\doraemon_theme.wav", NULL, SND_ASYNC);
        break;
    case '5':  //play sound 'suprise'
        PlaySoundW(L"D:\\utm_4\\graphics\\FCG_project(Doraemon)\\Debug\\yowai-mo.wav", NULL, SND_ASYNC);
        break;
    case 's': //stop sound
        PlaySound(NULL, 0, 0);
        break;
    }
    glFlush();
    glutPostRedisplay();

}

// Special keys function
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        if (leftArmRotation > 180) {
            leftArmRotation -= 5.0;
            rightArmRotation -= 5.0;
        }
        break;
    case GLUT_KEY_DOWN:
        if (leftArmRotation < 360) {
            leftArmRotation += 5.0;
            rightArmRotation += 5.0;
        }
        break;
    case GLUT_KEY_LEFT:
        bodyRotation -= 5.0;
        break;
    case GLUT_KEY_RIGHT:
        bodyRotation += 5.0;
        break;
    case GLUT_KEY_PAGE_UP:
        if (headRotation < 90) {
            headRotation += 5.0;
        }
        break;
    case GLUT_KEY_PAGE_DOWN:
        if (headRotation > -90) {
            headRotation -= 5.0;
        }
        break;
    }
    glutPostRedisplay();
}

// Reshape function
void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
}

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    return textureId;
}

GLuint _textureId1;

void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    Image* image1 = loadBMP("D:\\utm_4\\graphics\\FCG_project(Doraemon)\\Debug\\sky.bmp");
    _textureId1 = loadTexture(image1);
    delete image1;
}

void drawSphericalBackground() {
    // Disable lighting and depth writing for background
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPushMatrix();

    // Create a large sphere around the scene
    glColor3f(1.0f, 1.0f, 1.0f);
    // Make it large enough to surround everything
    glScalef(45.0f, 45.0f, 45.0f); 

    // Reverse the sphere normals by scaling negatively
    glScalef(-1.0f, 1.0f, 1.0f);

    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, 1.0, 32, 32);
    gluDeleteQuadric(quadric);

    glPopMatrix();

    // Re-enable lighting and depth writing
    glDisable(GL_TEXTURE_2D);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Calculate camera position based on spherical coordinates
    GLfloat eyeX = cameraDistance * cos(cameraAngleY * M_PI / 180.0) * sin(cameraAngleX * M_PI / 180.0);
    GLfloat eyeY = cameraDistance * sin(cameraAngleY * M_PI / 180.0) + cameraHeight;
    GLfloat eyeZ = cameraDistance * cos(cameraAngleY * M_PI / 180.0) * cos(cameraAngleX * M_PI / 180.0);

    // Set camera position
    gluLookAt(eyeX, eyeY, eyeZ,     // eye position
        0.0, 0.0, 0.0,        // look at center
        0.0, 1.0, 0.0);       // up vector

    drawSphericalBackground();

    // Draw Doraemon
    drawBody();

    glutSwapBuffers();
}

// Initialize OpenGL settings 
void init() {
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glEnable(GL_DEPTH_TEST);

    initRendering();

    // Lighting setup 
    GLfloat light_position[] = { -10.0, 10.0, -0.5, 0.0 };
    GLfloat light_ambient[] = { 0.15, 0.15, 0.15, 1.0 };
    GLfloat light_diffuse[] = { 0.35, 0.35, 0.35, 1.0 };
    GLfloat light_specular[] = { 0.3, 0.3, 0.3, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Material properties
    GLfloat mat_specular[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat mat_shininess[] = { 5.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    glShadeModel(GL_SMOOTH);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Doraemon 3D Model");

    init();

    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutIdleFunc(animate);

    printf("Controls:\n");
    printf("Mouse Controls:\n");
    printf("  Left click + drag: Rotate camera around model\n");
    printf("  Right click + drag: Zoom in/out\n");
    printf("  Middle click + drag: Pan camera up/down\n\n");

    printf("Keyboard Controls:\n");
    printf("  Arrow keys (left & right): Rotate body\n");
    printf("  Arrow keys (up & down): Rotate arms\n");
    printf("  Page up & down keys: Rotate head\n");
    printf("  't': Toggle animation\n");
    printf("  'r': Reset all rotations and camera\n");
    printf("  'c': Reset camera only\n");
    printf("  'p': Play background music\n");
    printf("  's': Stop background music\n");
    printf("  ESC: Quit\n\n");

    printf("--- Press 5 and you will get a surprise!! ---\n");

    glutMainLoop();
    return 0;
}