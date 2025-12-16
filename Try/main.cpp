#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include<mmsystem.h>
#include<windows.h>
#define MAX_DROPS 100
#define MAX_BUBBLES 100
#include <vector>
#include "lodepng.h"
#include "lodepng.cpp"



using namespace std;

#define PI 3.14159265358979323846
float twicePi = 2.0f * PI;

float birdX = -1.2f;
float birdY = 0.9f;
float birdWingY = -1.0f;
float birdDirection = 1.0f;

float airplaneX = -1.2f;
bool airplaneMove = false;
GLuint airplaneTexture;

GLuint sharkTexture;
float sharkX[3] = { 0.9f, 0.65f, 0.4f };//For x
float sharkY[3] = { -0.4f, -0.6f, -0.38f };//For Y
bool sharkDirection[3] = { true, true, true }; // true = right, false = left


GLuint balloonTexture;
int balloonDirection = 1;
bool balloonMove = false;
float balloonY = 0.0f;


const float groundY = -0.8f;

float dropX[MAX_DROPS], dropY[MAX_DROPS], dropSpeed[MAX_DROPS];
int dropCount = 0;

float bubbleX[MAX_BUBBLES], bubbleY[MAX_BUBBLES];
float bubbleRadius[MAX_BUBBLES], bubbleAlpha[MAX_BUBBLES];
int bubbleActive[MAX_BUBBLES], bubbleCount = 0;




// Missing global variables needed across multiple functions
float radiusm1 = 5.0f;
float radiusm3 = 5.0f;
float radiusc = 5.0f, radiusc3 = 5.0f, radiusc4 = 5.0f, radiusc5 = 5.0f;
float radiusw1 = 5.0f;
float radiusf1 = 5.0f;
float xc = 0.0f, yc = 0.0f;
float c12 = 0.0f, c13 = 0.0f, c14 = 0.0f, c15 = 0.0f, c16 = 0.0f, c17 = 0.0f;


int triangleAmount = 1000;
float vehicleSpeed = .003f;
bool night = false;
bool on = false;
bool cover = false;
bool vehicleStop = false;
bool rain = false;
bool waveUp = true;
bool handup= false;
float _crashB2 = 0.0f;
float _crashB1 = 0.0f;

int i = 0;
float _run1 = 0.0;
float _run2 = 0.0;
float _run3 = 0.0;
float _run4 = 0.0;
float _run5 = 0.0;  ///green car
float _run6 = 0.0;
float _run7 = 0.0;  ///red bus
float _run8 = 0.0;
float _run9 = 0.0;
float _run10 = 0.0;
float _run11 = 0.0;
float c1 = 0.00;
float c2 = 0.00;
float c3 = 0.00;
float c4 = 0.00;
float c5 = 0.00;
float c6 = 0.00;
float c7 = 0.00;
float c8 = 0.00;
float c9 = 0.00;
float c10 = 0.00;
float c11 = 0.00;


void clearColor(float r, float g, float b,float bb) {
    glClearColor(r, g, b, bb);
}

void renderBitmapString(float x, float y, float z, void *font, char *string) {
    char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

//S001
void draw_sky_() {

    glBegin(GL_QUADS);

    glColor3f(0.4f,0.69f,1.0f);
    if(rain && !night)
    {
        glColor3f(0.18, 0.36, 0.56);
    }
    glVertex2f(-1.0f, 0.5f);
    glVertex2f(1.0f, 0.5f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);

    if(night)
    {
        glColor3f(0.2, 0.2, 0.40);
        glVertex2f(-1.0f, 0.5f);
        glVertex2f(-1.0f, 1.0f);
        glColor3f(0.0, 0.01, 0.02);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(1.0f, 0.5f);

    }
    glEnd();


}

void loadTexture(const char* filename, GLuint& textureID) {
    vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filename);
    if (error) {
    cout << "PNG load error: " << lodepng_error_text(error) <<endl;
        exit(1);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, *&textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//S0082
void draw_airplane() {
    if (!airplaneMove) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, airplaneTexture);
    glColor4f(1.0, 1.0, 1.0, 1.0); // White to keep the texture colors

    glPushMatrix();
    glTranslatef(airplaneX, 0.9f, 0.0f); // Adjust Y as needed
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(-0.1f, -0.1f);
        glTexCoord2f(1, 1); glVertex2f( 0.1f, -0.1f);
        glTexCoord2f(1, 0); glVertex2f( 0.1f,  0.1f);
        glTexCoord2f(0, 0); glVertex2f(-0.1f,  0.1f);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

//S0083
void draw_shark() {
    for (int i = 0; i < 3; ++i) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, sharkTexture);
        glColor4f(1.0, 1.0, 1.0, 1.0);

        glPushMatrix();
        glTranslatef(sharkX[i], sharkY[i], 0.0f);
           if (sharkDirection[i]) {
           glScalef(-1.0f, 1.0f, 1.0f); // Flip when going right
        }
        glBegin(GL_QUADS);
            glTexCoord2f(0, 1); glVertex2f(-0.04f, -0.04f);
            glTexCoord2f(1, 1); glVertex2f( 0.04f, -0.04f);
            glTexCoord2f(1, 0); glVertex2f( 0.04f,  0.04f);
            glTexCoord2f(0, 0); glVertex2f(-0.04f,  0.04f);
        glEnd();

        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}

//S0084
void draw_balloon() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, balloonTexture);
    glColor4f(1.0, 1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslatef(-0.36f, balloonY, 0.0f); // X position fixed, Y is dynamic
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(-0.05f, -0.1f);
        glTexCoord2f(1, 1); glVertex2f( 0.05f, -0.1f);
        glTexCoord2f(1, 0); glVertex2f( 0.05f,  0.1f);
        glTexCoord2f(0, 0); glVertex2f(-0.05f,  0.1f);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}


void drawCircle(float x, float y, float radius, float alpha) {
    int segments = 32;
    glColor4f(0.5f, 0.7f, 1.0f, alpha);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(x + cosf(angle) * radius, y + sinf(angle) * radius);
    }
    glEnd();
}

void drawGround() {
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(-1, groundY);
    glVertex2f(1, groundY);
    glVertex2f(1, -1);
    glVertex2f(-1, -1);
    glEnd();
}

//S0079
void drawDrops() {
    glColor3f(0.2f, 0.4f, 1.0f);
    for (int i = 0; i < dropCount; ++i) {
        glBegin(GL_LINES);
        glVertex2f(dropX[i], dropY[i]);
        glVertex2f(dropX[i], dropY[i] - 0.05f);
        glEnd();
    }
}

//S0080
void drawBubbles() {
    for (int i = 0; i < bubbleCount; ++i) {
        if (bubbleActive[i]) {
            drawCircle(bubbleX[i], bubbleY[i], bubbleRadius[i], bubbleAlpha[i]);
        }
    }
}

//S0086
void updateRainEffect() {
    // Add new raindrop randomly (if not exceeding MAX_DROPS)
    if (dropCount < MAX_DROPS && (rand() % 5) == 0) {
        dropX[dropCount] = (rand() % 2000 / 1000.0f) - 1.0f;  // random x from -1 to 1
        dropY[dropCount] = 1.0f; // start at top
        dropSpeed[dropCount] = 0.01f + (rand() % 100 / 10000.0f); // speed variation
        dropCount++;
    }

    // Update raindrop positions
    for (int i = 0; i < dropCount; ) {
        dropY[i] -= dropSpeed[i];

        if (dropY[i] <= groundY + 0.01f) {
            // Create bubble if there's space
            if (bubbleCount < MAX_BUBBLES) {
                bubbleX[bubbleCount] = dropX[i];
                bubbleY[bubbleCount] = groundY + 0.01f;
                bubbleRadius[bubbleCount] = 0.01f;
                bubbleAlpha[bubbleCount] = 1.0f;
                bubbleActive[bubbleCount] = 1;
                bubbleCount++;
            }

            // Remove this raindrop (by replacing it with the last drop in the array)
            dropX[i] = dropX[dropCount - 1];
            dropY[i] = dropY[dropCount - 1];
            dropSpeed[i] = dropSpeed[dropCount - 1];
            dropCount--;
        } else {
            i++; // only increment if not removed
        }
    }

    // Update bubble expansion and fading
    for (int i = 0; i < bubbleCount; ++i) {
        if (bubbleActive[i]) {
            bubbleRadius[i] += 0.005f;    // make bubble grow
            bubbleAlpha[i] -= 0.02f;      // make bubble fade
            if (bubbleAlpha[i] <= 0.0f) {
                bubbleActive[i] = 0;      // deactivate bubble
            }
        }
    }
}



void polygon(vector<pair<float, float>> coord, float Tx = 0, float Ty = 0, float s = 1.0f) {
    glBegin(GL_POLYGON);
    for (auto& pt : coord)
        glVertex2f(Tx + s * pt.first, Ty + s * pt.second);
    glEnd();
}


//S0081
void draw_bird(float Tx, float Ty, float direction, float wingY, float s = 0.001f) {
    // Body – red-orange
    glColor3f(0.95f, 0.36f, 0.28f);
    polygon({
        {28.65f * direction, 2.99f}, {31.98f * direction, 1.74f},
        {31.6f * direction, 0.23f}, {31.93f * direction, -1.29f},
        {30.43f * direction, -0.38f}, {28.77f * direction, -1.12f},
        {24.29f * direction, -4.59f}, {16.47f * direction, -5.28f},
        {10.7f * direction, -4.96f}, {2.95f * direction, -10.29f},
        {8.55f * direction, -3.22f}, {0.18f * direction, 1.29f},
        {9.57f * direction, -0.31f}, {11.02f * direction, 0.27f},
        {22.82f * direction, 1.86f}, {28.65f * direction, 2.99f}
    }, Tx, Ty, s);

    // Wing – vivid green-blue
    glColor3f(0.2f, 0.6f, 0.8f);
    polygon({
        {22.82f * direction, wingY * 1.86f}, {23.38f * direction, wingY * 8.57f},
        {9.98f * direction, wingY * 25.45f}, {11.02f * direction, wingY * 0.27f}
    }, Tx, Ty, s);
}

//S0085
void update_bird() {
    birdX += 0.01f * birdDirection;
    birdWingY += 0.05f;
    if (birdWingY > 1.0f) birdWingY = -1.0f;

    if (birdX > 1.5f) birdX = -1.5f;
    if (birdX < -1.5f) birdX = 1.5f;
}

//S002
void draw_sun() {
     if(!night)
    {
        GLfloat xs1=0.2f;
        GLfloat ys1=0.85f;
        GLfloat radiuss1 =0.05f;
        glColor3f(0.95f,0.95f,0.0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(xs1, ys1);
        //GLfloat twicePi = 2.0f * PI;
        for(int i = 0; i <= triangleAmount; i++)
        {
            glVertex2f( xs1 + (radiuss1 * cos(i *  twicePi / triangleAmount)),
                        ys1 + (radiuss1 * sin(i * twicePi / triangleAmount)) );
        }
        glEnd();
    }
}

//S003
void draw_moon() {
    if(night)
    {
        GLfloat xsm1=0.78f;
        GLfloat ysm1=0.9f;
        GLfloat radiussm1 =0.05f;
        glColor3f(1.0f,1.0f,1.0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(xsm1, ysm1);
        //GLfloat twicePi = 2.0f * PI;
        for(int i = 0; i <= triangleAmount; i++)
        {
            glVertex2f( xsm1 + (radiussm1 * cos(i *  twicePi / triangleAmount)),
                        ysm1 + (radiussm1 * sin(i * twicePi / triangleAmount)) );
        }
        glEnd();



        GLfloat xsm2=0.8f;
        GLfloat ysm2=0.9f;
        glColor3f(0.0, 0.01, 0.02);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(xsm2, ysm2);
        for(int i = 0; i <= triangleAmount; i++)
        {
            glVertex2f( xsm2 + (radiussm1 * cos(i *  twicePi / triangleAmount)),
                        ysm2 + (radiussm1 * sin(i * twicePi / triangleAmount)) );
        }
        glEnd();

    }
}

//S004
void draw_star() {
    if(night && !rain)
    {
        glBegin(GL_POINTS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f(-0.9f, 0.9f);

        glVertex2f(-0.7f, 0.9f);

        glVertex2f(-0.5f, 0.9f);

        glVertex2f(-0.3f, 0.9f);

        glVertex2f(-0.1f, 0.9f);

        glVertex2f(0.1f, 0.9f);

        glVertex2f(0.3f, 0.9f);

        glVertex2f(0.5f, 0.9f);

        glVertex2f(0.7f, 0.9f);

        glVertex2f(0.9f, 0.9f);

        glVertex2f(-0.98f, 0.93f);

        glVertex2f(-0.88f, 0.94f);

        glVertex2f(-0.68f, 0.91f);

        glVertex2f(-0.55f, 0.92f);

        glVertex2f(-0.45f, 0.93f);

        glVertex2f(-0.25f, 0.94f);

        glVertex2f(0.0f, 0.9f);

        glVertex2f(0.13f, 0.91f);

        glVertex2f(0.25f, 0.92f);

        glVertex2f(0.37f, 0.93f);

        glVertex2f(0.49f, 0.92f);

        glVertex2f(-0.8f, 0.85f);

        glVertex2f(-0.6f, 0.85f);

        glVertex2f(-0.4f, 0.85f);

        glVertex2f(-0.2f, 0.85f);

        glVertex2f(0.0f, 0.85f);

        glVertex2f(0.2f, 0.85f);

        glVertex2f(0.4f, 0.85f);

        glVertex2f(0.6f, 0.85f);

        glVertex2f(0.8f, 0.85f);

        glVertex2f(1.0f, 0.85f);

        glEnd();
    }
}

//S005
void draw_hill3() {

     glPushMatrix();
    glTranslatef(0.3f,0.0f,0.0f);

    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.2f,0.0f);
    glVertex2f(-1.0f, 0.5f);
    glVertex2f(-1.0f, 0.65f);


    glVertex2f(-1.0f, 0.65f);
    glVertex2f(-0.97f, 0.68f);

    glVertex2f(-0.97f, 0.68f);
    glVertex2f(-0.95f, 0.66f);

    glVertex2f(-0.95f, 0.66f);
    glVertex2f(-0.9f, 0.77f);

    glVertex2f(-0.9f, 0.77f);
    glVertex2f(-0.87f, 0.73f);

    glVertex2f(-0.87f, 0.73f);
    glVertex2f(-0.84f, 0.66f);

    glVertex2f(-0.84f, 0.66f);
    glVertex2f(-0.8f, 0.7f);

    glVertex2f(-0.8f, 0.7f);
    glVertex2f(-0.78f, 0.68f);

    glVertex2f(-0.78f, 0.68f);
    glVertex2f(-0.73f, 0.73f);

    glVertex2f(-0.73f, 0.73f);
    glVertex2f(-0.68f, 0.64f);

    glVertex2f(-0.68f, 0.64f);
    glVertex2f(-0.65f, 0.67f);

    glVertex2f(-0.65f, 0.67f);
    glVertex2f(-0.58f, 0.58f);

    glVertex2f(-0.58f, 0.58f);
    glVertex2f(-0.55f, 0.6f);

    glVertex2f(-0.55f, 0.6f);
    glVertex2f(-0.5f, 0.5f);



    glEnd();


    glPopMatrix();
}

//S006
void draw_hill01() {
     glBegin(GL_POLYGON);
    glColor3f(0.0f,0.4f,0.0f);
    if(night)
    {
        glColor3f(0.09, 0.20, 0.09);
    }
    glVertex2f(-1.0f, 0.5f);
    glVertex2f(-1.0f, 0.65f);


    glVertex2f(-1.0f, 0.65f);
    glVertex2f(-0.97f, 0.68f);

    glVertex2f(-0.97f, 0.68f);
    glVertex2f(-0.95f, 0.66f);

    glVertex2f(-0.95f, 0.66f);
    glVertex2f(-0.9f, 0.75f);

    glVertex2f(-0.9f, 0.75f);
    glVertex2f(-0.87f, 0.73f);

    glVertex2f(-0.87f, 0.73f);
    glVertex2f(-0.84f, 0.66f);

    glVertex2f(-0.84f, 0.66f);
    glVertex2f(-0.8f, 0.7f);

    glVertex2f(-0.8f, 0.7f);
    glVertex2f(-0.78f, 0.68f);

    glVertex2f(-0.78f, 0.68f);
    glVertex2f(-0.73f, 0.73f);

    glVertex2f(-0.73f, 0.73f);
    glVertex2f(-0.68f, 0.64f);

    glVertex2f(-0.68f, 0.64f);
    glVertex2f(-0.65f, 0.67f);

    glVertex2f(-0.65f, 0.67f);
    glVertex2f(-0.58f, 0.58f);

    glVertex2f(-0.58f, 0.58f);
    glVertex2f(-0.55f, 0.6f);

    glVertex2f(-0.55f, 0.6f);
    glVertex2f(-0.5f, 0.5f);



    glEnd();
}

//S007
void draw_hill5() {

     glPushMatrix();
    glTranslatef(0.8f,0.0f,0.0f);

    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.2f,0.0f);
    if(night)
    {
        glColor3f(0.08, 0.19, 0.08);
    }


    glVertex2f(-0.65f, 0.5f);
    glVertex2f(-0.63f, 0.55f);

    glVertex2f(-0.63f, 0.55f);
    glVertex2f(-0.61f, 0.53f);

    glVertex2f(-0.61f, 0.53f);
    glVertex2f(-0.55f, 0.65f);

    glVertex2f(-0.55f, 0.65f);
    glVertex2f(-0.52f, 0.63f);

    glVertex2f(-0.52f, 0.63f);
    glVertex2f(-0.5f, 0.68f);

    glVertex2f(-0.5f, 0.68f);
    glVertex2f(-0.47f, 0.66f);

    glVertex2f(-0.47f, 0.66f);
    glVertex2f(-0.44f, 0.73f);

    glVertex2f(-0.44f, 0.73f);
    glVertex2f(-0.4f, 0.7f);

    glVertex2f(-0.4f, 0.7f);
    glVertex2f(-0.35f, 0.62f);

    glVertex2f(-0.35f, 0.62f);
    glVertex2f(-0.3f, 0.66f);

    glVertex2f(-0.3f, 0.66f);
    glVertex2f(-0.28f, 0.63f);

    glVertex2f(-0.28f, 0.63f);
    glVertex2f(-0.24f, 0.69f);

    glVertex2f(-0.24f, 0.69f);
    glVertex2f(-0.18f, 0.58f);

    glVertex2f(-0.18f, 0.58f);
    glVertex2f(-0.15f, 0.6f);

    glVertex2f(-0.15f, 0.6f);
    glVertex2f(-0.11f, 0.55f);

    glVertex2f(-0.11f, 0.55f);
    glVertex2f(-0.1f, 0.5f);

    glEnd();

    glPopMatrix();

}

//S008
void draw_hill4() {

    glPushMatrix();
    glTranslatef(0.78f,0.0f,0.0f);

    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.3f,0.0f);
    if(night)
    {
        glColor3f(0.12, 0.28, 0.12);
    }
    glVertex2f(-1.0f, 0.5f);
    glVertex2f(-1.0f, 0.65f);


    glVertex2f(-1.0f, 0.65f);
    glVertex2f(-0.97f, 0.68f);

    glVertex2f(-0.97f, 0.68f);
    glVertex2f(-0.95f, 0.66f);

    glVertex2f(-0.95f, 0.66f);
    glVertex2f(-0.9f, 0.75f);

    glVertex2f(-0.9f, 0.75f);
    glVertex2f(-0.87f, 0.73f);

    glVertex2f(-0.87f, 0.73f);
    glVertex2f(-0.84f, 0.66f);

    glVertex2f(-0.84f, 0.66f);
    glVertex2f(-0.8f, 0.7f);

    glVertex2f(-0.8f, 0.7f);
    glVertex2f(-0.78f, 0.68f);

    glVertex2f(-0.78f, 0.68f);
    glVertex2f(-0.73f, 0.73f);

    glVertex2f(-0.73f, 0.73f);
    glVertex2f(-0.68f, 0.64f);

    glVertex2f(-0.68f, 0.64f);
    glVertex2f(-0.65f, 0.67f);

    glVertex2f(-0.65f, 0.67f);
    glVertex2f(-0.58f, 0.58f);

    glVertex2f(-0.58f, 0.58f);
    glVertex2f(-0.55f, 0.6f);

    glVertex2f(-0.55f, 0.6f);
    glVertex2f(-0.5f, 0.5f);



    glEnd();


    glPopMatrix();
}

//S009
void draw_hill6() {

     glPushMatrix();
    glTranslatef(1.3f,0.0f,0.0f);

    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.3f,0.0f);
    if(night)
    {
        glColor3f(0.00, 0.20, 0.00);
    }

    glVertex2f(-0.65f, 0.5f);
    glVertex2f(-0.63f, 0.55f);

    glVertex2f(-0.63f, 0.55f);
    glVertex2f(-0.61f, 0.53f);

    glVertex2f(-0.61f, 0.53f);
    glVertex2f(-0.55f, 0.65f);

    glVertex2f(-0.55f, 0.65f);
    glVertex2f(-0.52f, 0.63f);

    glVertex2f(-0.52f, 0.63f);
    glVertex2f(-0.5f, 0.68f);

    glVertex2f(-0.5f, 0.68f);
    glVertex2f(-0.47f, 0.66f);

    glVertex2f(-0.47f, 0.66f);
    glVertex2f(-0.44f, 0.73f);

    glVertex2f(-0.44f, 0.73f);
    glVertex2f(-0.4f, 0.7f);

    glVertex2f(-0.4f, 0.7f);
    glVertex2f(-0.35f, 0.62f);

    glVertex2f(-0.35f, 0.62f);
    glVertex2f(-0.3f, 0.66f);

    glVertex2f(-0.3f, 0.66f);
    glVertex2f(-0.28f, 0.63f);

    glVertex2f(-0.28f, 0.63f);
    glVertex2f(-0.24f, 0.69f);

    glVertex2f(-0.24f, 0.69f);
    glVertex2f(-0.18f, 0.58f);

    glVertex2f(-0.18f, 0.58f);
    glVertex2f(-0.15f, 0.6f);

    glVertex2f(-0.15f, 0.6f);
    glVertex2f(-0.11f, 0.55f);

    glVertex2f(-0.11f, 0.55f);
    glVertex2f(-0.1f, 0.5f);

    glEnd();

    glPopMatrix();
}

//S0010
void draw_hill2() {
     glBegin(GL_POLYGON);
    glColor3f(0.0f,0.5f,0.0f);
    if(night)
    {
        glColor3f(0.13, 0.31, 0.13);
    }

    glVertex2f(-0.65f, 0.5f);
    glVertex2f(-0.63f, 0.55f);

    glVertex2f(-0.63f, 0.55f);
    glVertex2f(-0.61f, 0.53f);

    glVertex2f(-0.61f, 0.53f);
    glVertex2f(-0.55f, 0.65f);

    glVertex2f(-0.55f, 0.65f);
    glVertex2f(-0.52f, 0.63f);

    glVertex2f(-0.52f, 0.63f);
    glVertex2f(-0.5f, 0.68f);

    glVertex2f(-0.5f, 0.68f);
    glVertex2f(-0.47f, 0.66f);

    glVertex2f(-0.47f, 0.66f);
    glVertex2f(-0.44f, 0.73f);

    glVertex2f(-0.44f, 0.73f);
    glVertex2f(-0.4f, 0.7f);

    glVertex2f(-0.4f, 0.7f);
    glVertex2f(-0.35f, 0.62f);

    glVertex2f(-0.35f, 0.62f);
    glVertex2f(-0.3f, 0.66f);

    glVertex2f(-0.3f, 0.66f);
    glVertex2f(-0.28f, 0.63f);

    glVertex2f(-0.28f, 0.63f);
    glVertex2f(-0.24f, 0.69f);

    glVertex2f(-0.24f, 0.69f);
    glVertex2f(-0.18f, 0.58f);

    glVertex2f(-0.18f, 0.58f);
    glVertex2f(-0.15f, 0.6f);

    glVertex2f(-0.15f, 0.6f);
    glVertex2f(-0.11f, 0.55f);

    glVertex2f(-0.11f, 0.55f);
    glVertex2f(-0.1f, 0.5f);

    glEnd();
}

//S0011
void draw_cloud1() {
   glPushMatrix();
    glTranslatef(_run1,0,0);

    glPushMatrix();
    glTranslatef(0.05f, 0.14f, 0.0f);
    glScalef(.7,.8,0);

    GLfloat xm1=-0.8f;
    GLfloat ym1=0.9f;
    GLfloat radiusm1 =0.08f;
    glColor3f(0.95f,0.95f,0.95);
    if(night)
    {
        glColor3f(0.5f,0.5f,0.5);
    }
    if(rain && !night)
    {
        glColor3f(0.43, 0.63, 0.85);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xm1, ym1);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xm1 + (radiusm1 * cos(i *  twicePi / triangleAmount)),
                    ym1 + (radiusm1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xm2=-0.72f;
    GLfloat ym2=0.9f;
    glColor3f(0.95f,0.95f,0.95);
    if(night)
    {
        glColor3f(0.5f,0.5f,0.5);
    }
    if(rain && !night)
    {
        glColor3f(0.43, 0.63, 0.85);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xm2, ym2);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xm2 + (radiusm1 * cos(i *  twicePi / triangleAmount)),
                    ym2 + (radiusm1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xm3=-0.77f;
    GLfloat ym3=0.82f;
    GLfloat radiusm2 =0.05f;
    glColor3f(0.95f,0.95f,0.95);
    if(night)
    {
        glColor3f(0.5f,0.5f,0.5);
    }
    if(rain && !night)
    {
        glColor3f(0.43, 0.63, 0.85);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xm3, ym3);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xm3 + (radiusm2 * cos(i *  twicePi / triangleAmount)),
                    ym3 + (radiusm2 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xm4=-0.85f;
    GLfloat ym4=0.82f;
    glColor3f(0.95f,0.95f,0.95);
    if(night)
    {
        glColor3f(0.5f,0.5f,0.5);
    }
    if(rain && !night)
    {
        glColor3f(0.43, 0.63, 0.85);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xm4, ym4);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xm4 + (radiusm2 * cos(i *  twicePi / triangleAmount)),
                    ym4 + (radiusm2 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


    GLfloat xm5=-0.69f;
    GLfloat ym5=0.82f;
    glColor3f(0.95f,0.95f,0.95);
    if(night)
    {
        glColor3f(0.5f,0.5f,0.5);
    }
    if(rain && !night)
    {
        glColor3f(0.43, 0.63, 0.85);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xm5, ym5);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xm5 + (radiusm2 * cos(i *  twicePi / triangleAmount)),
                    ym5 + (radiusm2 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glLoadIdentity();
    glPopMatrix();

    glPopMatrix();
}

//S0012
void draw_cloud2() {
    glPushMatrix();
    glTranslatef(_run2, 0, 0);

    glPushMatrix();
    glTranslatef(0.3f, 0.4f, 0.0f);
    glScalef(.6f, .6f, 0.0f);

    auto draw_circle = [](GLfloat x, GLfloat y, GLfloat r) {
        if (night)
            glColor3f(0.5f, 0.5f, 0.5f);
        else if (rain)
            glColor3f(0.43f, 0.63f, 0.85f);
        else
            glColor3f(0.95f, 0.95f, 0.95f);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= triangleAmount; i++) {
            glVertex2f(x + (r * cos(i * twicePi / triangleAmount)),
                       y + (r * sin(i * twicePi / triangleAmount)));
        }
        glEnd();
    };

    draw_circle(0.8f, 0.83f, 0.08f);
    draw_circle(0.84f, 0.76f, 0.08f);
    draw_circle(0.725f, 0.79f, 0.092f);
    draw_circle(0.72f, 0.88f, 0.06f);

    glPopMatrix(); // End inner transform
    glPopMatrix(); // End outer move
}

//S0013
void draw_rail_line1() {

    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);

    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.01f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.02f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.03f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.04f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.05f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.06f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.07f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.08f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.09f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.11f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.12f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.13f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.14f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.16f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.17f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.18f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.19f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.21f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.22f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.23f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.24f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.25f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.26f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.27f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.28f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.29f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.3f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.31f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.32f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.33f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.34f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.35f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.36f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.37f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.38f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.39f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.4f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.41f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.42f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.43f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.44f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.45f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.46f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.47f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.48f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.49f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.51f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.52f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.53f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.54f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.55f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.56f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.57f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.58f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.59f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.61f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.62f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.63f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.64f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.65f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.66f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.67f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.68f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.69f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.71f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.72f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.73f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.74f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.75f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.76f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.77f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.78f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.79f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.81f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.82f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.83f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.84f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.85f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.86f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.87f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.88f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.89f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.9f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.91f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.92f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.93f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.94f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.95f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.96f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.97f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.98f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.99f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

     glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.01f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.02f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.03f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.04f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.05f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.06f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.07f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.08f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.09f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.11f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.12f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.13f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.14f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.16f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.17f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.18f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.19f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.21f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.22f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.23f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.24f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.25f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.26f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.27f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.28f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.29f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.3f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.31f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.32f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.33f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.34f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.35f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.36f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.37f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.38f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.39f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.4f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.41f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.42f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.43f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.44f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.45f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.46f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.47f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.48f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.49f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.51f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.52f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.53f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.54f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.55f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.56f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.57f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.58f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.59f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.61f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.62f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.63f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.64f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.65f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.66f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.67f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.68f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.69f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.71f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.72f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.73f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.74f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.75f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.76f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.77f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.78f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.79f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.81f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.82f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.83f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.84f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.85f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.86f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.87f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.88f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.89f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.9f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.91f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.92f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.93f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.94f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.95f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.96f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.97f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.98f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.99f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-1.0f, 0.45f);
    glVertex2f(-0.995f, 0.45f);
    glVertex2f(-0.995f, 0.5f);
    glVertex2f(-1.0f, 0.5f);
    glEnd();
    glPopMatrix();

    glPopMatrix();


    glBegin(GL_QUADS);
    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(-1.0f, 0.467f);
    glVertex2f(1.0f,0.467f);
    glVertex2f(1.0f,0.46f);
    glVertex2f(-1.0f,0.46f);

    glVertex2f(-1.0f, 0.485f);
    glVertex2f(1.0f,0.485f);
    glVertex2f(1.0f, 0.49f);
    glVertex2f(-1.0f, 0.49f);

    glEnd();

}

//S0014
void draw_train() {

   glPushMatrix();
    glTranslatef(_run3,0,0);

    glBegin(GL_QUADS);
    glColor3f(1.0f,0.4f,0.0f);
    glVertex2f(0.02f,0.51f);
    glVertex2f(0.105f,0.51f);
    glVertex2f(0.105f,0.46f);
    glVertex2f(0.0f,0.46f);

    glColor3f(0.0f,0.3f,0.6f);
    glVertex2f(0.02f,0.505f);
    glVertex2f(0.1f,0.505f);
    glVertex2f(0.1f,0.5f);
    glVertex2f(0.02f,0.5f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(0.02f,0.476f);
    glVertex2f(0.1f,0.476f);
    glVertex2f(0.1f,0.465f);
    glVertex2f(0.02f,0.465f);

    glColor3f(0.0f,0.3f,0.6f);
    glVertex2f(0.025f,0.471f);
    glVertex2f(0.1f,0.471f);
    glVertex2f(0.1f,0.465f);
    glVertex2f(0.021f,0.465f);
//..
    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.012f,0.495f);
    glVertex2f(0.019f,0.495f);
    glVertex2f(0.019f,0.48f);
    glVertex2f(0.007f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.022f,0.495f);
    glVertex2f(0.03f,0.495f);
    glVertex2f(0.03f,0.48f);
    glVertex2f(0.022f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.032f,0.495f);
    glVertex2f(0.04f,0.495f);
    glVertex2f(0.04f,0.48f);
    glVertex2f(0.032f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.042f,0.495f);
    glVertex2f(0.05f,0.495f);
    glVertex2f(0.05f,0.48f);
    glVertex2f(0.042f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.052f,0.495f);
    glVertex2f(0.06f,0.495f);
    glVertex2f(0.06f,0.48f);
    glVertex2f(0.052f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.062f,0.495f);
    glVertex2f(0.07f,0.495f);
    glVertex2f(0.07f,0.48f);
    glVertex2f(0.062f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.072f,0.495f);
    glVertex2f(0.08f,0.495f);
    glVertex2f(0.08f,0.48f);
    glVertex2f(0.072f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.082f,0.495f);
    glVertex2f(0.09f,0.495f);
    glVertex2f(0.09f,0.48f);
    glVertex2f(0.082f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.092f,0.495f);
    glVertex2f(0.099f,0.495f);
    glVertex2f(0.099f,0.48f);
    glVertex2f(0.092f,0.48f);

    glEnd();


    glBegin(GL_QUADS);
    glColor3f(1.0f,0.4f,0.0f);
    glVertex2f(0.205f,0.51f);
    glVertex2f(0.102f,0.51f);
    glVertex2f(0.102f,0.46f);
    glVertex2f(0.205f,0.46f);

    glColor3f(0.0f,0.3f,0.6f);
    glVertex2f(0.199f,0.505f);
    glVertex2f(0.102f,0.505f);
    glVertex2f(0.102f,0.5f);
    glVertex2f(0.199f,0.5f);


    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(0.11f,0.465f);
    glVertex2f(0.105f,0.465f);
    glVertex2f(0.105f,0.5f);
    glVertex2f(0.11f,0.5f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(0.199f,0.476f);
    glVertex2f(0.105f,0.476f);
    glVertex2f(0.105f,0.465f);
    glVertex2f(0.199f,0.465f);

    glColor3f(0.0f,0.3f,0.6f);
    glVertex2f(0.11f,0.471f);
    glVertex2f(0.199f,0.471f);
    glVertex2f(0.199f,0.465f);
    glVertex2f(0.107f,0.465f);

//windus
    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.12f,0.495f);
    glVertex2f(0.112f,0.495f);
    glVertex2f(0.112f,0.48f);
    glVertex2f(0.12f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.13f,0.495f);
    glVertex2f(0.122f,0.495f);
    glVertex2f(0.122f,0.48f);
    glVertex2f(0.13f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.14f,0.495f);
    glVertex2f(0.132f,0.495f);
    glVertex2f(0.132f,0.48f);
    glVertex2f(0.14f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.15f,0.495f);
    glVertex2f(0.142f,0.495f);
    glVertex2f(0.142f,0.48f);
    glVertex2f(0.15f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.16f,0.495f);
    glVertex2f(0.152f,0.495f);
    glVertex2f(0.152f,0.48f);
    glVertex2f(0.16f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.17f,0.495f);
    glVertex2f(0.162f,0.495f);
    glVertex2f(0.162f,0.48f);
    glVertex2f(0.17f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.18f,0.495f);
    glVertex2f(0.172f,0.495f);
    glVertex2f(0.172f,0.48f);
    glVertex2f(0.18f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.19f,0.495f);
    glVertex2f(0.182f,0.495f);
    glVertex2f(0.182f,0.48f);
    glVertex2f(0.19f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.1985f,0.495f);
    glVertex2f(0.192f,0.495f);
    glVertex2f(0.192f,0.48f);
    glVertex2f(0.1985f,0.48f);


    glEnd();


    glBegin(GL_QUADS);
    glColor3f(1.0f,0.4f,0.0f);
    glVertex2f(0.305f,0.51f);
    glVertex2f(0.202f,0.51f);
    glVertex2f(0.202f,0.46f);
    glVertex2f(0.305f,0.46f);

    glColor3f(0.0f,0.3f,0.6f);
    glVertex2f(0.3f,0.505f);
    glVertex2f(0.202f,0.505f);
    glVertex2f(0.202f,0.5f);
    glVertex2f(0.3f,0.5f);


    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(0.21f,0.465f);
    glVertex2f(0.205f,0.465f);
    glVertex2f(0.205f,0.5f);
    glVertex2f(0.21f,0.5f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(0.3f,0.476f);
    glVertex2f(0.205f,0.476f);
    glVertex2f(0.205f,0.465f);
    glVertex2f(0.3f,0.465f);

    glColor3f(0.0f,0.3f,0.6f);
    glVertex2f(0.21f,0.471f);
    glVertex2f(0.3f,0.471f);
    glVertex2f(0.3f,0.465f);
    glVertex2f(0.207f,0.465f);

    //..
    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.22f,0.495f);
    glVertex2f(0.212f,0.495f);
    glVertex2f(0.212f,0.48f);
    glVertex2f(0.22f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.23f,0.495f);
    glVertex2f(0.222f,0.495f);
    glVertex2f(0.222f,0.48f);
    glVertex2f(0.23f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.24f,0.495f);
    glVertex2f(0.232f,0.495f);
    glVertex2f(0.232f,0.48f);
    glVertex2f(0.24f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.25f,0.495f);
    glVertex2f(0.242f,0.495f);
    glVertex2f(0.242f,0.48f);
    glVertex2f(0.25f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.26f,0.495f);
    glVertex2f(0.252f,0.495f);
    glVertex2f(0.252f,0.48f);
    glVertex2f(0.26f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.27f,0.495f);
    glVertex2f(0.262f,0.495f);
    glVertex2f(0.262f,0.48f);
    glVertex2f(0.27f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.28f,0.495f);
    glVertex2f(0.272f,0.495f);
    glVertex2f(0.272f,0.48f);
    glVertex2f(0.28f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.29f,0.495f);
    glVertex2f(0.282f,0.495f);
    glVertex2f(0.282f,0.48f);
    glVertex2f(0.29f,0.48f);

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.3f,0.495f);
    glVertex2f(0.292f,0.495f);
    glVertex2f(0.292f,0.48f);
    glVertex2f(0.3f,0.48f);

    glEnd();

    glPopMatrix();
}

//S0015
void draw_tree1() {
     glPushMatrix();
    glTranslatef(1.f, .2f, 0.0f);
    glScalef(.5,.5,0);
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.64f, 0.2f);
    glVertex2f(-0.64f, 0.3f);
    glVertex2f(-0.65f, 0.3f);
    glVertex2f(-0.65f, 0.2f);
    glEnd();


    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPushMatrix();
    glTranslatef(-0.01f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.02f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.01f, 0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    GLfloat xf1=-0.645f;
    GLfloat yf1=0.3f;
    GLfloat radiusf1 =0.039f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf1, yf1);
    GLfloat twice1Pi = 1.0f * PI;
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf1 + (radiusf1 * cos(i *  twice1Pi / triangleAmount)),
                    yf1 + (radiusf1 * sin(i * twice1Pi / triangleAmount)) );
    }
    glEnd();


    GLfloat xf2=-0.645f;
    GLfloat yf2=0.33f;
    GLfloat radiusf2 =0.034f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf2, yf2);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf2 + (radiusf2 * cos(i *  twice1Pi / triangleAmount)),
                    yf2 + (radiusf2 * sin(i * twice1Pi / triangleAmount)) );
    }
    glEnd();

    GLfloat xf3=-0.645f;
    GLfloat yf3=0.36f;
    GLfloat radiusf3 =0.029f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf3, yf3);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf3 + (radiusf3 * cos(i *  twice1Pi / triangleAmount)),
                    yf3 + (radiusf3 * sin(i * twice1Pi / triangleAmount)) );
    }
    glEnd();

    glLoadIdentity();

    glPopMatrix();

}

//S0016
void draw_tree2() {
    glPushMatrix();
    glTranslatef(1.01f, .15f, 0.0f);
    glScalef(.5,.5,0);
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.64f, 0.2f);
    glVertex2f(-0.64f, 0.3f);
    glVertex2f(-0.65f, 0.3f);
    glVertex2f(-0.65f, 0.2f);
    glEnd();


    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPushMatrix();
    glTranslatef(-0.01f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.02f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.01f, 0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    GLfloat xf4=-0.645f;
    GLfloat yf4=0.3f;
    GLfloat radiusf4 =0.039f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf4, yf4);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf4 + (radiusf4 * cos(i *  twicePi / triangleAmount)),
                    yf4 + (radiusf4 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


    GLfloat xf5=-0.645f;
    GLfloat yf5=0.33f;
    GLfloat radiusf5 =0.034f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf5, yf5);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf5 + (radiusf5 * cos(i *  twicePi / triangleAmount)),
                    yf5 + (radiusf5 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xf6=-0.645f;
    GLfloat yf6=0.36f;
    GLfloat radiusf6 =0.029f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf6, yf6);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf6 + (radiusf6 * cos(i *  twicePi / triangleAmount)),
                    yf6 + (radiusf6 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glLoadIdentity();

    glPopMatrix();
}

//S0017
void draw_tree3() {
    glPushMatrix();
    glTranslatef(.95f, .35f, 0.0f);
    glScalef(.3,.3,0);
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.64f, 0.2f);
    glVertex2f(-0.64f, 0.3f);
    glVertex2f(-0.65f, 0.3f);
    glVertex2f(-0.65f, 0.2f);
    glEnd();


    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPushMatrix();
    glTranslatef(-0.01f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.02f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.01f, 0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    GLfloat xf7=-0.645f;
    GLfloat yf7=0.3f;
    GLfloat radiusf7 =0.039f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf7, yf7);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf7 + (radiusf7 * cos(i *  twicePi / triangleAmount)),
                    yf7 + (radiusf7 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


    GLfloat xf8=-0.645f;
    GLfloat yf8=0.33f;
    GLfloat radiusf8 =0.034f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf8, yf8);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf8 + (radiusf8 * cos(i *  twicePi / triangleAmount)),
                    yf8 + (radiusf8 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xf9=-0.645f;
    GLfloat yf9=0.36f;
    GLfloat radiusf9 =0.029f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf9, yf9);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf9 + (radiusf9 * cos(i *  twicePi / triangleAmount)),
                    yf9 + (radiusf9 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glLoadIdentity();

    glPopMatrix();
}

//S0018
void draw_tree4() {
    glPushMatrix();
    glTranslatef(1.12f, .35f, 0.0f);
    glScalef(.3,.3,0);
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.64f, 0.2f);
    glVertex2f(-0.64f, 0.3f);
    glVertex2f(-0.65f, 0.3f);
    glVertex2f(-0.65f, 0.2f);
    glEnd();


    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPushMatrix();
    glTranslatef(-0.01f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.02f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.01f, 0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    GLfloat xf10=-0.645f;
    GLfloat yf10=0.3f;
    GLfloat radiusf10 =0.039f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf10, yf10);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf10 + (radiusf10* cos(i *  twicePi / triangleAmount)),
                    yf10 + (radiusf10 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


    GLfloat xf11=-0.645f;
    GLfloat yf11=0.33f;
    GLfloat radiusf11 =0.034f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf11, yf11);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf11 + (radiusf11 * cos(i *  twicePi / triangleAmount)),
                    yf11 + (radiusf11 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xf12=-0.645f;
    GLfloat yf12=0.36f;
    GLfloat radiusf12 =0.029f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf12, yf12);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf12 + (radiusf12 * cos(i *  twicePi / triangleAmount)),
                    yf12 + (radiusf12 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glLoadIdentity();

    glPopMatrix();
}

//S0019
void draw_tree5() {
    glPushMatrix();
    glTranslatef(1.16f, .35f, 0.0f);
    glScalef(.3,.3,0);
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.64f, 0.2f);
    glVertex2f(-0.64f, 0.3f);
    glVertex2f(-0.65f, 0.3f);
    glVertex2f(-0.65f, 0.2f);
    glEnd();


    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPushMatrix();
    glTranslatef(-0.01f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.02f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.01f, 0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    GLfloat xf13=-0.645f;
    GLfloat yf13=0.3f;
    GLfloat radiusf13 =0.039f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf13, yf13);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf13 + (radiusf13* cos(i *  twicePi / triangleAmount)),
                    yf13 + (radiusf13 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


    GLfloat xf14=-0.645f;
    GLfloat yf14=0.33f;
    GLfloat radiusf14 =0.034f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf14, yf14);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf14 + (radiusf14 * cos(i *  twicePi / triangleAmount)),
                    yf14 + (radiusf14 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xf15=-0.645f;
    GLfloat yf15=0.36f;
    GLfloat radiusf15 =0.029f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xf15, yf15);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xf15 + (radiusf15 * cos(i *  twicePi / triangleAmount)),
                    yf15 + (radiusf15 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glLoadIdentity();

    glPopMatrix();
}

//S0020
void draw_house1() {
   glBegin(GL_QUADS);
    glColor3f(0.6f,0.6f,0.6f);

    glVertex2f(-0.86f, 0.48f);
    glVertex2f(-0.88f, 0.48f);
    glVertex2f(-0.88f, 0.3f);
    glVertex2f(-0.86f, 0.3f);

    glColor3f(0.2f,0.0f,0.0f);

    glVertex2f(-0.855f, 0.48f);
    glVertex2f(-0.885f, 0.48f);
    glVertex2f(-0.885f, 0.495f);
    glVertex2f(-0.855f, 0.495f);


    glEnd();

    glBegin(GL_POLYGON);

    glColor3f(0.6f,0.6f,0.6f);
    glVertex2f(-0.9f, 0.2f);
    glVertex2f(-0.9f, 0.4f);
    glVertex2f(-0.8f, 0.5f);
    glVertex2f(-0.7f, 0.4f);
    glVertex2f(-0.7f, 0.2f);


    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.2f,0.0f,0.0f);

    glVertex2f(-0.69f, 0.39f);
    glVertex2f(-0.8f, 0.5f);
    glVertex2f(-0.8f, 0.52f);
    glVertex2f(-0.68f, 0.4f);

    glVertex2f(-0.91f, 0.39f);
    glVertex2f(-0.8f, 0.5f);
    glVertex2f(-0.8f, 0.52f);
    glVertex2f(-0.92f, 0.4f);
    glEnd();

}

//S0021
void draw_house1_windos1() {
     glBegin(GL_QUADS);
    glColor3f(1.0f,1.0f,1.0f);

    glVertex2f(-0.808f, 0.43f);
    glVertex2f(-0.792f, 0.43f);
    glVertex2f(-0.792f, 0.46f);
    glVertex2f(-0.808f, 0.46f);

    glColor3f(0.6f,0.8f,1.0f);

    glVertex2f(-0.805f, 0.435f);
    glVertex2f(-0.795f, 0.435f);
    glVertex2f(-0.795f, 0.455f);
    glVertex2f(-0.805f, 0.455f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f);

    glVertex2f(-0.78f, 0.32f);
    glVertex2f(-0.82f, 0.32f);
    glVertex2f(-0.82f, 0.38f);
    glVertex2f(-0.78f, 0.38f);

    glColor3f(0.2f,0.0f,0.0f);

    glVertex2f(-0.775f, 0.39f);
    glVertex2f(-0.825f, 0.39f);
    glVertex2f(-0.825f, 0.38f);
    glVertex2f(-0.775f, 0.38f);


    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }

    glVertex2f(-0.785f, 0.325f);
    glVertex2f(-0.795f, 0.325f);
    glVertex2f(-0.795f, 0.345f);
    glVertex2f(-0.785f, 0.345f);

    glVertex2f(-0.785f, 0.35f);
    glVertex2f(-0.795f, 0.35f);
    glVertex2f(-0.795f, 0.37f);
    glVertex2f(-0.785f, 0.37f);

    glVertex2f(-0.815f, 0.325f);
    glVertex2f(-0.805f, 0.325f);
    glVertex2f(-0.805f, 0.345f);
    glVertex2f(-0.815f, 0.345f);

    glVertex2f(-0.815f, 0.35f);
    glVertex2f(-0.805f, 0.35f);
    glVertex2f(-0.805f, 0.37f);
    glVertex2f(-0.815f, 0.37f);


    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(-0.80f, 0.32f);
    glVertex2f(-0.80f, 0.38f);
    glEnd();
}

//S0022
void draw_house1_windos2() {
    glPushMatrix();
    glTranslatef(-0.06f, 0.0f, 0.0f);

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f);

    glVertex2f(-0.78f, 0.32f);
    glVertex2f(-0.82f, 0.32f);
    glVertex2f(-0.82f, 0.38f);
    glVertex2f(-0.78f, 0.38f);

    glColor3f(0.2f,0.0f,0.0f);

    glVertex2f(-0.775f, 0.39f);
    glVertex2f(-0.825f, 0.39f);
    glVertex2f(-0.825f, 0.38f);
    glVertex2f(-0.775f, 0.38f);


    glColor3f(0.6f,0.8f,1.0f);

    glVertex2f(-0.785f, 0.325f);
    glVertex2f(-0.795f, 0.325f);
    glVertex2f(-0.795f, 0.345f);
    glVertex2f(-0.785f, 0.345f);

    glVertex2f(-0.785f, 0.35f);
    glVertex2f(-0.795f, 0.35f);
    glVertex2f(-0.795f, 0.37f);
    glVertex2f(-0.785f, 0.37f);

    glVertex2f(-0.815f, 0.325f);
    glVertex2f(-0.805f, 0.325f);
    glVertex2f(-0.805f, 0.345f);
    glVertex2f(-0.815f, 0.345f);

    glVertex2f(-0.815f, 0.35f);
    glVertex2f(-0.805f, 0.35f);
    glVertex2f(-0.805f, 0.37f);
    glVertex2f(-0.815f, 0.37f);


    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(-0.80f, 0.32f);
    glVertex2f(-0.80f, 0.38f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.06f, 0.0f, 0.0f);

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f);

    glVertex2f(-0.78f, 0.32f);
    glVertex2f(-0.82f, 0.32f);
    glVertex2f(-0.82f, 0.38f);
    glVertex2f(-0.78f, 0.38f);

    glColor3f(0.2f,0.0f,0.0f);

    glVertex2f(-0.775f, 0.39f);
    glVertex2f(-0.825f, 0.39f);
    glVertex2f(-0.825f, 0.38f);
    glVertex2f(-0.775f, 0.38f);


    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }

    glVertex2f(-0.785f, 0.325f);
    glVertex2f(-0.795f, 0.325f);
    glVertex2f(-0.795f, 0.345f);
    glVertex2f(-0.785f, 0.345f);

    glVertex2f(-0.785f, 0.35f);
    glVertex2f(-0.795f, 0.35f);
    glVertex2f(-0.795f, 0.37f);
    glVertex2f(-0.785f, 0.37f);

    glVertex2f(-0.815f, 0.325f);
    glVertex2f(-0.805f, 0.325f);
    glVertex2f(-0.805f, 0.345f);
    glVertex2f(-0.815f, 0.345f);

    glVertex2f(-0.815f, 0.35f);
    glVertex2f(-0.805f, 0.35f);
    glVertex2f(-0.805f, 0.37f);
    glVertex2f(-0.815f, 0.37f);


    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(-0.80f, 0.32f);
    glVertex2f(-0.80f, 0.38f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.1f, 0.0f);

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f);

    glVertex2f(-0.78f, 0.32f);
    glVertex2f(-0.82f, 0.32f);
    glVertex2f(-0.82f, 0.38f);
    glVertex2f(-0.78f, 0.38f);

    glColor3f(0.2f,0.0f,0.0f);

    glVertex2f(-0.775f, 0.39f);
    glVertex2f(-0.825f, 0.39f);
    glVertex2f(-0.825f, 0.38f);
    glVertex2f(-0.775f, 0.38f);


    glColor3f(0.6f,0.8f,1.0f);

    glVertex2f(-0.785f, 0.325f);
    glVertex2f(-0.795f, 0.325f);
    glVertex2f(-0.795f, 0.345f);
    glVertex2f(-0.785f, 0.345f);

    glVertex2f(-0.785f, 0.35f);
    glVertex2f(-0.795f, 0.35f);
    glVertex2f(-0.795f, 0.37f);
    glVertex2f(-0.785f, 0.37f);

    glVertex2f(-0.815f, 0.325f);
    glVertex2f(-0.805f, 0.325f);
    glVertex2f(-0.805f, 0.345f);
    glVertex2f(-0.815f, 0.345f);

    glVertex2f(-0.815f, 0.35f);
    glVertex2f(-0.805f, 0.35f);
    glVertex2f(-0.805f, 0.37f);
    glVertex2f(-0.815f, 0.37f);


    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(-0.80f, 0.32f);
    glVertex2f(-0.80f, 0.38f);
    glEnd();

    glPopMatrix();


    glPushMatrix();
    glTranslatef(-0.06f, -0.1f, 0.0f);

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f);

    glVertex2f(-0.78f, 0.32f);
    glVertex2f(-0.82f, 0.32f);
    glVertex2f(-0.82f, 0.38f);
    glVertex2f(-0.78f, 0.38f);

    glColor3f(0.2f,0.0f,0.0f);

    glVertex2f(-0.775f, 0.39f);
    glVertex2f(-0.825f, 0.39f);
    glVertex2f(-0.825f, 0.38f);
    glVertex2f(-0.775f, 0.38f);


    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }

    glVertex2f(-0.785f, 0.325f);
    glVertex2f(-0.795f, 0.325f);
    glVertex2f(-0.795f, 0.345f);
    glVertex2f(-0.785f, 0.345f);

    glVertex2f(-0.785f, 0.35f);
    glVertex2f(-0.795f, 0.35f);
    glVertex2f(-0.795f, 0.37f);
    glVertex2f(-0.785f, 0.37f);

    glVertex2f(-0.815f, 0.325f);
    glVertex2f(-0.805f, 0.325f);
    glVertex2f(-0.805f, 0.345f);
    glVertex2f(-0.815f, 0.345f);

    glVertex2f(-0.815f, 0.35f);
    glVertex2f(-0.805f, 0.35f);
    glVertex2f(-0.805f, 0.37f);
    glVertex2f(-0.815f, 0.37f);


    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(-0.80f, 0.32f);
    glVertex2f(-0.80f, 0.38f);
    glEnd();

    glPopMatrix();

}

//S0023
void draw_house1_door() {

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f);

    glVertex2f(-0.76f, 0.2f);
    glVertex2f(-0.72f, 0.2f);
    glVertex2f(-0.72f, 0.28f);
    glVertex2f(-0.76f, 0.28f);


    glColor3f(0.6f,0.8f,1.0f);

    glVertex2f(-0.755f, 0.21f);
    glVertex2f(-0.742f, 0.21f);
    glVertex2f(-0.742f, 0.27f);
    glVertex2f(-0.755f, 0.27f);

    glVertex2f(-0.738f, 0.21f);
    glVertex2f(-0.725f, 0.21f);
    glVertex2f(-0.725f, 0.27f);
    glVertex2f(-0.738f, 0.27f);


    glEnd();

    glBegin(GL_QUADS);


    glColor3f(0.8f,0.8f,0.8f);

    glVertex2f(-0.76f, 0.2f);
    glVertex2f(-0.72f, 0.2f);
    glVertex2f(-0.72f, 0.19f);
    glVertex2f(-0.76f, 0.19f);

    glVertex2f(-0.755f, 0.17f);
    glVertex2f(-0.725f, 0.17f);
    glVertex2f(-0.725f, 0.19f);
    glVertex2f(-0.755f, 0.19f);
    glEnd();
}

//S0024
void draw_tree2_copy() {
    glPushMatrix();
    glTranslatef(0.06f, -0.01f, 0.0f);
    float _run3 = 0.0;
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.96f, 0.2f);
    glVertex2f(-0.96f, 0.3f);
    glVertex2f(-0.95f, 0.3f);
    glVertex2f(-0.95f, 0.2f);
    glEnd();

    GLfloat xt10=-0.955f;
    GLfloat yt10=0.3f;
    GLfloat radiust10 =0.03f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt10, yt10);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt10 + (radiust10* cos(i *  twicePi / triangleAmount)),
                    yt10 + (radiust10 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xt11=-0.955f;
    GLfloat yt11=0.33f;
    GLfloat radiust11 =0.025f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt11, yt11);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt11 + (radiust11 * cos(i *  twicePi / triangleAmount)),
                    yt11 + (radiust11 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xt12=-0.955f;
    GLfloat yt12=0.35f;
    GLfloat radiust12 =0.02f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt12, yt12);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt12 + (radiust12 * cos(i *  twicePi / triangleAmount)),
                    yt12 + (radiust12 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glPopMatrix();



    glBegin(GL_QUADS);


    glColor3f(0.6f,0.28f,0.0f);

    glVertex2f(-0.93f, 0.23f);
    glVertex2f(-0.77f, 0.23f);
    glVertex2f(-0.77f, 0.19f);
    glVertex2f(-0.93f, 0.19f);

    glVertex2f(-0.71f, 0.23f);
    glVertex2f(-0.67f, 0.23f);
    glVertex2f(-0.67f, 0.19f);
    glVertex2f(-0.71f, 0.19f);


    glColor3f(0.8f,0.4f,0.0f);

    glVertex2f(-0.76f, 0.24f);
    glVertex2f(-0.77f, 0.24f);
    glVertex2f(-0.77f, 0.19f);
    glVertex2f(-0.76f, 0.19f);

    glVertex2f(-0.71f, 0.24f);
    glVertex2f(-0.72f, 0.24f);
    glVertex2f(-0.72f, 0.19f);
    glVertex2f(-0.71f, 0.19f);


    glEnd();
}

//S0025
void draw_house2() {
    glBegin(GL_QUADS);
    glColor3f(0.05, 0.30, 0.50);
    glVertex2f(-0.47f, 0.6f);
    glVertex2f(-0.47f, 0.2f);
    glVertex2f(-0.6f, 0.2f);
    glVertex2f(-0.6f, 0.6f);
    glEnd();



    glBegin(GL_QUADS);
    glColor3f(0.00, 0.18, 0.34);
    glVertex2f(-0.47f, 0.47f);
    glVertex2f(-0.4f, 0.35f);
    glVertex2f(-0.4f, 0.2f);
    glVertex2f(-0.47f, 0.2f);
    glEnd();

    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(0.04, 0.31, 0.52);
    glVertex2f(-.472,.47);
    glVertex2f(-.39,.34);
    glEnd();

    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(0.05, 0.20, 0.34);
    glVertex2f(-.61,.60);
    glVertex2f(-.46,.60);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.95, 0.98, 0.98);
    glVertex2f(-0.53f, 0.56f);
    glVertex2f(-0.53f, 0.5f);
    glVertex2f(-0.57f, 0.5f);
    glVertex2f(-0.57f, 0.56f);
    glEnd();

    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(0.00, 0.20, 0.35);
    glVertex2f(-.521,.57);
    glVertex2f(-.579,.57);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.95, 0.98, 0.98);
    glVertex2f(-0.5f, 0.56f);
    glVertex2f(-0.5f, 0.52f);
    glVertex2f(-0.485f, 0.52f);
    glVertex2f(-0.485f, 0.56f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.38, 0.68, 0.78);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.497f, 0.557f);
    glVertex2f(-0.497f, 0.523f);
    glVertex2f(-0.487f, 0.523);
    glVertex2f(-0.487f, 0.557f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.38, 0.68, 0.78);
    glVertex2f(-0.532f, 0.557f);
    glVertex2f(-0.532f, 0.503f);
    glVertex2f(-0.549f, 0.503);
    glVertex2f(-0.549f, 0.557f);

    glVertex2f(-0.552f, 0.557f);
    glVertex2f(-0.552f, 0.503f);
    glVertex2f(-0.568f, 0.503);
    glVertex2f(-0.568f, 0.557f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.62, 0.67, 0.73);
    glVertex2f(-0.52f, 0.51f);
    glVertex2f(-0.52f, 0.475f);
    glVertex2f(-0.58f, 0.475);
    glVertex2f(-0.58f, 0.51f);
    glEnd();



    glBegin(GL_QUADS);
    glColor3f(0.95, 0.98, 0.98);
    glVertex2f(-0.53f, 0.3f);
    glVertex2f(-0.53f, 0.24f);
    glVertex2f(-0.57f, 0.24f);
    glVertex2f(-0.57f, 0.3f);
    glEnd();

    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(0.00, 0.20, 0.35);
    glVertex2f(-.521,.31);
    glVertex2f(-.579,.31);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.95, 0.98, 0.98);
    glVertex2f(-0.5f, 0.3f);
    glVertex2f(-0.5f, 0.26f);
    glVertex2f(-0.485f, 0.26f);
    glVertex2f(-0.485f, 0.3f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.38, 0.68, 0.78);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.497f, 0.297f);
    glVertex2f(-0.497f, 0.263f);
    glVertex2f(-0.487f, 0.263);
    glVertex2f(-0.487f, 0.297f);
    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.38, 0.68, 0.78);
    glVertex2f(-0.532f, 0.297f);
    glVertex2f(-0.532f, 0.243f);
    glVertex2f(-0.549f, 0.243);
    glVertex2f(-0.549f, 0.297f);

    glVertex2f(-0.552f, 0.297f);
    glVertex2f(-0.552f, 0.243f);
    glVertex2f(-0.568f, 0.243);
    glVertex2f(-0.568f, 0.297f);

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.62, 0.67, 0.73);
    glVertex2f(-0.52f, 0.25f);
    glVertex2f(-0.52f, 0.215f);
    glVertex2f(-0.58f, 0.215);
    glVertex2f(-0.58f, 0.25f);
    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.95, 0.98, 0.98);
    glVertex2f(-0.53f, 0.43f);
    glVertex2f(-0.53f, 0.37f);
    glVertex2f(-0.57f, 0.37f);
    glVertex2f(-0.57f, 0.43f);
    glEnd();

    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(0.00, 0.20, 0.35);
    glVertex2f(-.521,.44);
    glVertex2f(-.579,.44);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.95, 0.98, 0.98);
    glVertex2f(-0.5f, 0.43f);
    glVertex2f(-0.5f, 0.39f);
    glVertex2f(-0.485f, 0.39f);
    glVertex2f(-0.485f, 0.43f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.38, 0.68, 0.78);
    glVertex2f(-0.497f, 0.427f);
    glVertex2f(-0.497f, 0.393f);
    glVertex2f(-0.487f, 0.393);
    glVertex2f(-0.487f, 0.427f);
    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.38, 0.68, 0.78);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.532f, 0.427f);
    glVertex2f(-0.532f, 0.373f);
    glVertex2f(-0.549f, 0.373);
    glVertex2f(-0.549f, 0.427f);

    glVertex2f(-0.552f, 0.427f);
    glVertex2f(-0.552f, 0.373f);
    glVertex2f(-0.568f, 0.373);
    glVertex2f(-0.568f, 0.427f);

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.62, 0.67, 0.73);
    glVertex2f(-0.52f, 0.38f);
    glVertex2f(-0.52f, 0.345f);
    glVertex2f(-0.58f, 0.345);
    glVertex2f(-0.58f, 0.38f);
    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.95, 0.98, 0.98);
    glVertex2f(-0.428f, 0.37f);
    glVertex2f(-0.428f, 0.32f);
    glVertex2f(-0.453f, 0.32f);
    glVertex2f(-0.453f, 0.37f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.38, 0.68, 0.78);
    glVertex2f(-0.431f, 0.365f);
    glVertex2f(-0.431f, 0.323f);
    glVertex2f(-0.450f, 0.323);
    glVertex2f(-0.450f, 0.365f);
    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.95, 0.98, 0.98);
    glVertex2f(-0.425f, 0.3);
    glVertex2f(-0.425f, 0.2f);
    glVertex2f(-0.455f, 0.2);
    glVertex2f(-0.455f, 0.3);
    glEnd();

    glLineWidth(4);
    glBegin(GL_LINES);
    glColor3f(0.62, 0.67, 0.73);
    glVertex2f(-.435,.215);
    glVertex2f(-.435,.245);
    glEnd();

    glLineWidth(4);
    glBegin(GL_LINES);
    glColor3f(0.62, 0.67, 0.73);
    glVertex2f(-.445,.215);
    glVertex2f(-.445,.245);
    glEnd();


    glLineWidth(4);
    glBegin(GL_LINES);
    glColor3f(0.62, 0.67, 0.73);
    glVertex2f(-.435,.255);
    glVertex2f(-.435,.285);
    glEnd();

    glLineWidth(4);
    glBegin(GL_LINES);
    glColor3f(0.62, 0.67, 0.73);
    glVertex2f(-.445,.255);
    glVertex2f(-.445,.285);
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0.80, 0.80, 0.80);
    glVertex2f(-.421,.2);
    glVertex2f(-.458,.2);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.80, 0.80, 0.80);
    glVertex2f(-0.425f, 0.2f);
    glVertex2f(-0.425f, 0.15f);
    glVertex2f(-0.455f, 0.15);
    glVertex2f(-0.455f, 0.2f);
    glEnd();

}

//S0026
void draw_car1() {
   glBegin(GL_QUADS);
    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex2f(-0.34f, 0.295f);
    glVertex2f(-0.33f, 0.2f);
    glVertex2f(-0.4f, 0.2f);
    glVertex2f(-0.39f, 0.295f);

    glVertex2f(-0.325f, 0.17f);
    glVertex2f(-0.33f, 0.2f);
    glVertex2f(-0.4f, 0.2f);
    glVertex2f(-0.405f, 0.17f);

    glEnd();


    glPushMatrix();
    glTranslatef(-0.01f, 0.01f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(-0.34f, 0.28f);
    glVertex2f(-0.33f, 0.25f);
    glVertex2f(-0.38f, 0.25f);
    glVertex2f(-0.37f, 0.28f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.342f, 0.277f);
    glVertex2f(-0.335f, 0.255f);
    glVertex2f(-0.375f, 0.255f);
    glVertex2f(-0.367f, 0.277f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.9f, 0.0f);
    glVertex2f(-0.325f, 0.24f);
    glVertex2f(-0.33f, 0.25f);
    glVertex2f(-0.38f, 0.25f);
    glVertex2f(-0.385f, 0.24f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(-0.325f, 0.24f);
    glVertex2f(-0.325f, 0.21f);
    glVertex2f(-0.385f, 0.21f);
    glVertex2f(-0.385f, 0.24f);
    glEnd();

    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.328f, 0.234f);
    glVertex2f(-0.328f, 0.216f);
    glVertex2f(-0.339f, 0.216f);
    glVertex2f(-0.339f, 0.234f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.372f, 0.234f);
    glVertex2f(-0.372f, 0.216f);
    glVertex2f(-0.382f, 0.216f);
    glVertex2f(-0.382f, 0.234f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.369f, 0.22f);
    glVertex2f(-0.369f, 0.215f);
    glVertex2f(-0.341f, 0.215f);
    glVertex2f(-0.341f, 0.22f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.369f, 0.225f);
    glVertex2f(-0.369f, 0.23f);
    glVertex2f(-0.341f, 0.23f);
    glVertex2f(-0.341f, 0.225f);


    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.375f, 0.195f);
    glVertex2f(-0.375f, 0.21f);
    glVertex2f(-0.385f, 0.21f);
    glVertex2f(-0.385f, 0.195f);

    glVertex2f(-0.325f, 0.195f);
    glVertex2f(-0.325f, 0.21f);
    glVertex2f(-0.335f, 0.21f);
    glVertex2f(-0.335f, 0.195f);
    glEnd();

    glPopMatrix();
}

//S0027
void draw_house3_royal() {
    // Base of the house
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.4f, 0.2f); // Main green wall
    glVertex2f(-0.3f, 0.45f);
    glVertex2f(0.0f, 0.45f);
    glVertex2f(0.0f, 0.19f);
    glVertex2f(-0.3f, 0.19f);

    glColor3f(0.4f, 0.0f, 0.0f); // Right border
    glVertex2f(-0.0065f, 0.45f);
    glVertex2f(0.0f, 0.45f);
    glVertex2f(0.0f, 0.19f);
    glVertex2f(-0.0065f, 0.19f);

    glVertex2f(-0.3f, 0.45f);
    glVertex2f(-0.295f, 0.45f);
    glVertex2f(-0.295f, 0.19f);
    glVertex2f(-0.3f, 0.19f);
    glEnd();

    // Begin shifted part (like door and design)
    glPushMatrix();
    glTranslatef(0.0f, 0.08f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.4f, 0.0f); // Roof or top plate
    glVertex2f(-0.02f, 0.37f);
    glVertex2f(0.0f, 0.33f);
    glVertex2f(-0.3f, 0.33f);
    glVertex2f(-0.28f, 0.37f);

    glColor3f(1.0f, 0.4f, 0.2f); // Wall mid section
    glVertex2f(-0.01f, 0.22f);
    glVertex2f(-0.01f, 0.33f);
    glVertex2f(-0.29f, 0.33f);
    glVertex2f(-0.29f, 0.22f);

    glColor3f(1.0f, 0.8f, 0.6f); // Wall trim
    glVertex2f(-0.01f, 0.325f);
    glVertex2f(-0.01f, 0.33f);
    glVertex2f(-0.29f, 0.33f);
    glVertex2f(-0.29f, 0.325f);

    glColor3f(0.4f, 0.0f, 0.0f); // Bottom trim
    glVertex2f(-0.01f, 0.22f);
    glVertex2f(-0.01f, 0.227f);
    glVertex2f(-0.29f, 0.227f);
    glVertex2f(-0.29f, 0.22f);
    glEnd();

    // Door section
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.4f, 0.0f); // Door top
    glVertex2f(-0.12f, 0.4f);
    glVertex2f(-0.09f, 0.35f);
    glVertex2f(-0.21f, 0.35f);
    glVertex2f(-0.18f, 0.4f);

    glColor3f(1.0f, 0.6f, 0.2f); // Door panel
    glVertex2f(-0.1f, 0.2f);
    glVertex2f(-0.1f, 0.35f);
    glVertex2f(-0.2f, 0.35f);
    glVertex2f(-0.2f, 0.2f);

    glColor3f(1.0f, 0.8f, 0.6f); // Door trim top
    glVertex2f(-0.1f, 0.345f);
    glVertex2f(-0.1f, 0.35f);
    glVertex2f(-0.2f, 0.35f);
    glVertex2f(-0.2f, 0.345f);

    glColor3f(0.4f, 0.0f, 0.0f); // Door trim bottom
    glVertex2f(-0.1f, 0.2f);
    glVertex2f(-0.1f, 0.215f);
    glVertex2f(-0.2f, 0.215f);
    glVertex2f(-0.2f, 0.2f);
    glEnd();

    // Window
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.17f, 0.282f);
    glVertex2f(-0.17f, 0.215f);
    glVertex2f(-0.13f, 0.215f);
    glVertex2f(-0.13f, 0.282f);

    glColor3f(0.6f, 0.8f, 1.0f); // Glass panes
    glVertex2f(-0.151f, 0.277f);
    glVertex2f(-0.151f, 0.215f);
    glVertex2f(-0.165f, 0.215f);
    glVertex2f(-0.165f, 0.277f);

    glVertex2f(-0.135f, 0.277f);
    glVertex2f(-0.135f, 0.215f);
    glVertex2f(-0.149f, 0.215f);
    glVertex2f(-0.149f, 0.277f);
    glEnd();

    // Roof triangle
    glBegin(GL_TRIANGLES);
    glColor3f(0.6f, 0.0f, 0.0f); // Shadow
    glVertex2f(-0.111f, 0.345f);
    glVertex2f(-0.15f, 0.42f);
    glVertex2f(-0.189f, 0.345f);

    glColor3f(1.0f, 0.8f, 0.6f); // Highlight
    glVertex2f(-0.117f, 0.345f);
    glVertex2f(-0.15f, 0.41f);
    glVertex2f(-0.184f, 0.345f);
    glEnd();

    // Label
    glColor3f(0.0f, 0.0f, 1.0f);
    renderBitmapString(-0.168f, 0.35f, 0.0f, GLUT_BITMAP_HELVETICA_10, "Royal");

    glPopMatrix();
}


//S0028
void draw_royal_windows() {

    glPushMatrix();
    glTranslatef(0.0f, 0.08f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.022f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.044f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.066f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();


}

//S0029
void draw_royal_windows2() {
    glPushMatrix();
    glTranslatef(0.0f, 0.03f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.022f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.044f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.066f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

//S0030
void draw_copyrw1() {

    glPushMatrix();
    glTranslatef(0.189f, 0.08f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.022f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.044f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.066f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.286f, 0.283f);
    glVertex2f(-0.286f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();


}

//S0031
void draw_copyrw2() {
    glPushMatrix();
    glTranslatef(0.189f, 0.03f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.022f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.044f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.285f, 0.283f);
    glVertex2f(-0.285f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.066f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.286f, 0.283f);
    glVertex2f(-0.286f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

//S0032
void draw_copyrw3() {

    glPushMatrix();
    glTranslatef(0.094f, 0.09f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.286f, 0.283f);
    glVertex2f(-0.286f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.022f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.286f, 0.283f);
    glVertex2f(-0.286f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.044f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.286f, 0.283f);
    glVertex2f(-0.286f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.066f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.286f, 0.283f);
    glVertex2f(-0.286f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.283f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

//S0033
void draw_copyrw4() {

    glPushMatrix();
    glTranslatef(0.094f, 0.037f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.286f, 0.26f);
    glVertex2f(-0.286f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.26f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();

}

//S0034
void draw_copyrw5() {

    glPushMatrix();
    glTranslatef(0.163f, 0.037f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.286f, 0.26f);
    glVertex2f(-0.286f, 0.32f);
    glVertex2f(-0.27f, 0.32f);
    glVertex2f(-0.27f, 0.26f);

    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.007f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.012f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.007f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.024f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.6f,0.8f,1.0f);
    glVertex2f(-0.284f, 0.309f);
    glVertex2f(-0.284f, 0.317f);
    glVertex2f(-0.278f, 0.317f);
    glVertex2f(-0.278f, 0.309f);
    glEnd();
    glPopMatrix();
    glPopMatrix();

}


//S0035
void draw_marmad() {
     glPushMatrix();
    // glTranslatef(-0.03f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.89f, 0.8f);
    glVertex2f(0.04f, 0.31f);
    glVertex2f(0.04f, 0.3f);
    glVertex2f(0.36f, 0.3f);
    glVertex2f(0.36f, 0.31f);

    glColor3f(1.0f, 0.69f, 0.4f);
    glVertex2f(0.04f, 0.29f);
    glVertex2f(0.04f, 0.3f);
    glVertex2f(0.36f, 0.3f);
    glVertex2f(0.36f, 0.29f);

    glColor3f(0.69f, 0.4f, 1.0f);
    glVertex2f(0.05f, 0.29f);
    glVertex2f(0.05f, 0.19f);
    glVertex2f(0.35f, 0.19f);
    glVertex2f(0.35f, 0.29f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.055f, 0.285f);
    glVertex2f(0.055f, 0.195f);
    glVertex2f(0.105f, 0.195f);
    glVertex2f(0.105f, 0.285f);

    glColor3f(0.6f, 0.8f, 1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.06f, 0.28f);
    glVertex2f(0.06f, 0.2f);
    glVertex2f(0.1f, 0.2f);
    glVertex2f(0.1f, 0.28f);


    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.295f, 0.285f);
    glVertex2f(0.295f, 0.195f);
    glVertex2f(0.345f, 0.195f);
    glVertex2f(0.345f, 0.285f);

    glColor3f(0.6f, 0.8f, 1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.3f, 0.28f);
    glVertex2f(0.3f, 0.2f);
    glVertex2f(0.34f, 0.2f);
    glVertex2f(0.34f, 0.28f);

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.89f, 0.8f);
    glVertex2f(0.1f, 0.385f);
    glVertex2f(0.1f, 0.4f);
    glVertex2f(0.3f, 0.4f);
    glVertex2f(0.3f, 0.385f);

    glColor3f(1.0f, 0.69f, 0.4f);
    glVertex2f(0.1f, 0.385f);
    glVertex2f(0.1f, 0.37f);
    glVertex2f(0.3f, 0.37f);
    glVertex2f(0.3f, 0.385f);

    glColor3f(0.69f, 0.4f, 1.0f);
    glVertex2f(0.11f, 0.37f);
    glVertex2f(0.11f, 0.19f);
    glVertex2f(0.29f, 0.19f);
    glVertex2f(0.29f, 0.37f);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex2f(0.11f, 0.278f);
    glVertex2f(0.11f, 0.29f);
    glVertex2f(0.29f, 0.29f);
    glVertex2f(0.29f, 0.278f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.11f, 0.282f);
    glVertex2f(0.11f, 0.286f);
    glVertex2f(0.29f, 0.286f);
    glVertex2f(0.29f, 0.282f);


    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.12f, 0.36f);
    glVertex2f(0.12f, 0.3f);
    glVertex2f(0.28f, 0.3f);
    glVertex2f(0.28f, 0.36f);

    glColor3f(0.6f, 0.8f, 1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.125f, 0.355f);
    glVertex2f(0.125f, 0.305f);
    glVertex2f(0.275f, 0.305f);
    glVertex2f(0.275f, 0.355f);


    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.12f, 0.278f);
    glVertex2f(0.12f, 0.19f);
    glVertex2f(0.112f, 0.19f);
    glVertex2f(0.112f, 0.278f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.288f, 0.278f);
    glVertex2f(0.288f, 0.19f);
    glVertex2f(0.28f, 0.19f);
    glVertex2f(0.28f, 0.278f);


    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex2f(0.04f, 0.15f);
    glVertex2f(0.05f, 0.19f);
    glVertex2f(0.35f, 0.19f);
    glVertex2f(0.36f, 0.15f);


    glEnd();
}

//S0038
void draw_car1_copy1() {

    glPushMatrix();
    glTranslatef(0.5f, 0.055f, 0.0f);
    glScalef(.7,.7,0);

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.34f, 0.28f);
    glVertex2f(-0.33f, 0.25f);
    glVertex2f(-0.38f, 0.25f);
    glVertex2f(-0.37f, 0.28f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.342f, 0.277f);
    glVertex2f(-0.335f, 0.255f);
    glVertex2f(-0.375f, 0.255f);
    glVertex2f(-0.367f, 0.277f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.325f, 0.24f);
    glVertex2f(-0.33f, 0.25f);
    glVertex2f(-0.38f, 0.25f);
    glVertex2f(-0.385f, 0.24f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.325f, 0.24f);
    glVertex2f(-0.325f, 0.21f);
    glVertex2f(-0.385f, 0.21f);
    glVertex2f(-0.385f, 0.24f);
    glEnd();
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.328f, 0.234f);
    glVertex2f(-0.328f, 0.216f);
    glVertex2f(-0.339f, 0.216f);
    glVertex2f(-0.339f, 0.234f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.372f, 0.234f);
    glVertex2f(-0.372f, 0.216f);
    glVertex2f(-0.382f, 0.216f);
    glVertex2f(-0.382f, 0.234f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.369f, 0.22f);
    glVertex2f(-0.369f, 0.215f);
    glVertex2f(-0.341f, 0.215f);
    glVertex2f(-0.341f, 0.22f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.369f, 0.225f);
    glVertex2f(-0.369f, 0.23f);
    glVertex2f(-0.341f, 0.23f);
    glVertex2f(-0.341f, 0.225f);


    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.375f, 0.195f);
    glVertex2f(-0.375f, 0.21f);
    glVertex2f(-0.385f, 0.21f);
    glVertex2f(-0.385f, 0.195f);

    glVertex2f(-0.325f, 0.195f);
    glVertex2f(-0.325f, 0.21f);
    glVertex2f(-0.335f, 0.21f);
    glVertex2f(-0.335f, 0.195f);
    glEnd();

    glLoadIdentity();

    glPopMatrix();
}

//S0037
void draw_again_car1_copy1() {

    glPushMatrix();
    glTranslatef(0.445f, 0.055f, 0.0f);
    glScalef(.7,.7,0);

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.29f, 0.6f);
    glVertex2f(-0.34f, 0.28f);
    glVertex2f(-0.33f, 0.25f);
    glVertex2f(-0.38f, 0.25f);
    glVertex2f(-0.37f, 0.28f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.342f, 0.277f);
    glVertex2f(-0.335f, 0.255f);
    glVertex2f(-0.375f, 0.255f);
    glVertex2f(-0.367f, 0.277f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.2f, 0.4f);
    glVertex2f(-0.325f, 0.24f);
    glVertex2f(-0.33f, 0.25f);
    glVertex2f(-0.38f, 0.25f);
    glVertex2f(-0.385f, 0.24f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.29f, 0.6f);
    glVertex2f(-0.325f, 0.24f);
    glVertex2f(-0.325f, 0.21f);
    glVertex2f(-0.385f, 0.21f);
    glVertex2f(-0.385f, 0.24f);
    glEnd();
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.328f, 0.234f);
    glVertex2f(-0.328f, 0.216f);
    glVertex2f(-0.339f, 0.216f);
    glVertex2f(-0.339f, 0.234f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.372f, 0.234f);
    glVertex2f(-0.372f, 0.216f);
    glVertex2f(-0.382f, 0.216f);
    glVertex2f(-0.382f, 0.234f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.369f, 0.22f);
    glVertex2f(-0.369f, 0.215f);
    glVertex2f(-0.341f, 0.215f);
    glVertex2f(-0.341f, 0.22f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.369f, 0.225f);
    glVertex2f(-0.369f, 0.23f);
    glVertex2f(-0.341f, 0.23f);
    glVertex2f(-0.341f, 0.225f);


    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.375f, 0.195f);
    glVertex2f(-0.375f, 0.21f);
    glVertex2f(-0.385f, 0.21f);
    glVertex2f(-0.385f, 0.195f);

    glVertex2f(-0.325f, 0.195f);
    glVertex2f(-0.325f, 0.21f);
    glVertex2f(-0.335f, 0.21f);
    glVertex2f(-0.335f, 0.195f);
    glEnd();

    glLoadIdentity();

    glPopMatrix();
}

//S0036
void draw_again1_car1_copy1() {

    glPushMatrix();
    glTranslatef(0.395f, 0.055f, 0.0f);
    glScalef(.7,.7,0);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.34f, 0.28f);
    glVertex2f(-0.33f, 0.25f);
    glVertex2f(-0.38f, 0.25f);
    glVertex2f(-0.37f, 0.28f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.342f, 0.277f);
    glVertex2f(-0.335f, 0.255f);
    glVertex2f(-0.375f, 0.255f);
    glVertex2f(-0.367f, 0.277f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.8f, 0.0f);
    glVertex2f(-0.325f, 0.24f);
    glVertex2f(-0.33f, 0.25f);
    glVertex2f(-0.38f, 0.25f);
    glVertex2f(-0.385f, 0.24f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.325f, 0.24f);
    glVertex2f(-0.325f, 0.21f);
    glVertex2f(-0.385f, 0.21f);
    glVertex2f(-0.385f, 0.24f);
    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.328f, 0.234f);
    glVertex2f(-0.328f, 0.216f);
    glVertex2f(-0.339f, 0.216f);
    glVertex2f(-0.339f, 0.234f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.372f, 0.234f);
    glVertex2f(-0.372f, 0.216f);
    glVertex2f(-0.382f, 0.216f);
    glVertex2f(-0.382f, 0.234f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.369f, 0.22f);
    glVertex2f(-0.369f, 0.215f);
    glVertex2f(-0.341f, 0.215f);
    glVertex2f(-0.341f, 0.22f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.369f, 0.225f);
    glVertex2f(-0.369f, 0.23f);
    glVertex2f(-0.341f, 0.23f);
    glVertex2f(-0.341f, 0.225f);


    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.375f, 0.195f);
    glVertex2f(-0.375f, 0.21f);
    glVertex2f(-0.385f, 0.21f);
    glVertex2f(-0.385f, 0.195f);

    glVertex2f(-0.325f, 0.195f);
    glVertex2f(-0.325f, 0.21f);
    glVertex2f(-0.335f, 0.21f);
    glVertex2f(-0.335f, 0.195f);
    glEnd();

    glLoadIdentity();

    glPopMatrix();

    glPopMatrix();
}

//S0039
void draw_hospital() {
    glPushMatrix();
    glTranslatef(0.25f, 0.04f, 0.0f);
    glScalef(0.55f,0.85f,0.0f);


    glBegin(GL_QUADS);
    glColor3f(1.00, 1.00, 1.00);
    glVertex2f(0.7f, 0.6f);
    glVertex2f(0.7f, 0.17f);
    glVertex2f(0.48f, 0.17f);
    glVertex2f(0.48f, 0.6f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.00, 1.00, 1.00);
    glVertex2f(0.48f, 0.6f);
    glVertex2f(0.48f, 0.52f);
    glVertex2f(0.465f, 0.52f);
    glVertex2f(0.465f, 0.6f);
    glEnd();

    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(0.23, 0.71, 0.78);
    glVertex2f(0.45f, 0.6f);
    glVertex2f(0.715f, 0.6f);
    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.45, 0.79, 0.89);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.58f, 0.52f);
    glVertex2f(0.58f, 0.18f);
    glVertex2f(0.7f, 0.18f);
    glVertex2f(0.7f, 0.52f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.45, 0.79, 0.89);
    glVertex2f(0.535f, 0.52f);
    glVertex2f(0.535f, 0.18f);
    glVertex2f(0.48f, 0.18f);
    glVertex2f(0.48f, 0.52f);
    glEnd();

    glLineWidth(2.4);
    glBegin(GL_LINES);
    glColor3f(1.00, 1.00, 1.00);
    glVertex2f(0.62f, 0.52f);
    glVertex2f(0.62f, 0.18f);

    glVertex2f(0.66f, 0.52f);
    glVertex2f(0.66f, 0.18f);

    glVertex2f(0.58f, 0.248f);
    glVertex2f(0.7f, 0.248);

    glVertex2f(0.58f, 0.316f);
    glVertex2f(0.7f, 0.316);

    glVertex2f(0.58f, 0.384f);
    glVertex2f(0.7f, 0.384);

    glVertex2f(0.58f, 0.452f);
    glVertex2f(0.7f, 0.452);
    glEnd();

    glLineWidth(2.4);
    glBegin(GL_LINES);
    glColor3f(0.17, 0.53, 0.55);
    glVertex2f(0.535f, 0.248f);
    glVertex2f(0.48f, 0.248);

    glVertex2f(0.535f, 0.316f);
    glVertex2f(0.48f, 0.316);

    glVertex2f(0.535f, 0.384f);
    glVertex2f(0.48f, 0.384);

    glVertex2f(0.535f, 0.452f);
    glVertex2f(0.48f, 0.452);

    glVertex2f(0.535f, 0.452f);
    glVertex2f(0.48f, 0.452);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.28, 0.68, 0.81);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.67f, 0.255f);
    glVertex2f(0.67f, 0.17f);
    glVertex2f(0.61f, 0.17f);
    glVertex2f(0.61f, 0.255f);
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.17, 0.53, 0.55);
    glVertex2f(410.0f, 292.5f);
    glVertex2f(410.0f, 312.5f);
    glEnd();


    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(0.73, 0.89, 0.87);
    glVertex2f(0.67f, 0.255f);
    glVertex2f(0.67f, 0.17);

    glVertex2f(0.61f, 0.255f);
    glVertex2f(0.61f, 0.17);
    glEnd();


    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0.17, 0.53, 0.55);
    glVertex2f(0.6f, 0.255f);
    glVertex2f(0.68f, 0.255);
    glEnd();

    glLineWidth(8);
    glBegin(GL_LINES);
    glColor3f(0.93, 0.95, 0.96);
    glVertex2f(0.7f, 0.29f);
    glVertex2f(0.79f, 0.29);
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0.93, 0.95, 0.96);
    glVertex2f(0.775f, 0.29f);
    glVertex2f(0.775f, 0.17);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.93, 0.95, 0.96);
    glVertex2f(0.48f, 0.4f);
    glVertex2f(0.48f, 0.17f);
    glVertex2f(0.33f, 0.17f);
    glVertex2f(0.33f, 0.4f);
    glEnd();

    glLineWidth(12);
    glBegin(GL_LINES);
    glColor3f(0.25, 0.70, 0.77);
    glVertex2f(0.315f, 0.41f);
    glVertex2f(0.48f, 0.41);
    glEnd();

    glPushMatrix();
    glTranslatef(0,-.02,0);
    glPushMatrix();
    glTranslatef(.025,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.14, 0.47, 0.54);
    glVertex2f(0.319f, 0.381f);
    glVertex2f(0.371f, 0.381f);
    glVertex2f(0.371f, 0.317f);
    glVertex2f(0.319f, 0.317);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.43, 0.77, 0.87);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.32f, 0.38f);
    glVertex2f(0.37f, 0.38f);
    glVertex2f(0.37f, 0.32f);
    glVertex2f(0.32f, 0.32);
    glEnd();

    glLineWidth(1.2);
    glBegin(GL_LINES);
    glColor3f(0.14, 0.47, 0.54);
    glVertex2f(0.345f, 0.38f);
    glVertex2f(0.345f, 0.32);
    glVertex2f(0.32f, 0.35f);
    glVertex2f(0.37f, 0.35);
    glEnd();
    glLoadIdentity();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(.085,0,0);

    glPushMatrix();
    glTranslatef(.01,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.14, 0.47, 0.54);
    glVertex2f(0.319f, 0.381f);
    glVertex2f(0.371f, 0.381f);
    glVertex2f(0.371f, 0.317f);
    glVertex2f(0.319f, 0.317);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.43, 0.77, 0.87);
    glVertex2f(0.32f, 0.38f);
    glVertex2f(0.37f, 0.38f);
    glVertex2f(0.37f, 0.32f);
    glVertex2f(0.32f, 0.32);
    glEnd();

    glLineWidth(1.2);
    glBegin(GL_LINES);
    glColor3f(0.14, 0.47, 0.54);
    glVertex2f(0.345f, 0.38f);
    glVertex2f(0.345f, 0.32);
    glVertex2f(0.32f, 0.35f);
    glVertex2f(0.37f, 0.35);
    glEnd();
    glLoadIdentity();
    glPopMatrix();

    glLoadIdentity();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.085,-.1,0);

    glPushMatrix();
    glTranslatef(.01,0,0);

    glBegin(GL_QUADS);
    glColor3f(0.14, 0.47, 0.54);
    glVertex2f(0.319f, 0.383f);
    glVertex2f(0.371f, 0.383f);
    glVertex2f(0.371f, 0.317f);
    glVertex2f(0.319f, 0.317);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.43, 0.77, 0.87);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.32f, 0.38f);
    glVertex2f(0.37f, 0.38f);
    glVertex2f(0.37f, 0.32f);
    glVertex2f(0.32f, 0.32);
    glEnd();

    glLineWidth(1.2);
    glBegin(GL_LINES);
    glColor3f(0.14, 0.47, 0.54);
    glVertex2f(0.345f, 0.38f);
    glVertex2f(0.345f, 0.32);
    glVertex2f(0.32f, 0.35f);
    glVertex2f(0.37f, 0.35);
    glEnd();
    glLoadIdentity();
    glPopMatrix();
    glLoadIdentity();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.0,-.1,0);

    glPushMatrix();
    glTranslatef(.025,0,0);

    glBegin(GL_QUADS);
    glColor3f(0.14, 0.47, 0.54);
    glVertex2f(0.319f, 0.383f);
    glVertex2f(0.371f, 0.383f);
    glVertex2f(0.371f, 0.317f);
    glVertex2f(0.319f, 0.317);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.43, 0.77, 0.87);
    glVertex2f(0.32f, 0.38f);
    glVertex2f(0.37f, 0.38f);
    glVertex2f(0.37f, 0.32f);
    glVertex2f(0.32f, 0.32);
    glEnd();

    glLineWidth(1.2);
    glBegin(GL_LINES);
    glColor3f(0.14, 0.47, 0.54);
    glVertex2f(0.345f, 0.38f);
    glVertex2f(0.345f, 0.32);
    glVertex2f(0.32f, 0.35f);
    glVertex2f(0.37f, 0.35);
    glEnd();

    glLoadIdentity();
    glPopMatrix();

    glLoadIdentity();
    glPopMatrix();

    glPopMatrix();
    glLoadIdentity();


    glLoadIdentity();
    glPopMatrix();

    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex2f(0.575f, 0.15f);
    glVertex2f(0.575f, 0.19f);
    glVertex2f(0.685f, 0.19f);
    glVertex2f(0.685f, 0.15f);
    glEnd();
}

//S0040
void draw_tree_01_copy1() {
    glPushMatrix();
    glTranslatef(1.61f, -0.03f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.64f, 0.2f);
    glVertex2f(-0.64f, 0.3f);
    glVertex2f(-0.65f, 0.3f);
    glVertex2f(-0.65f, 0.2f);
    glEnd();

    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);

    glEnd();

    glPushMatrix();
    glTranslatef(-0.01f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);

    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.02f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);

    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.01f, 0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);

    glEnd();

    glPopMatrix();
    GLfloat xt1=-0.645f;
    GLfloat yt1=0.3f;
    GLfloat radiust1 =0.039f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt1, yt1);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt1 + (radiust1 * cos(i *  twicePi / triangleAmount)),
                    yt1 + (radiust1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xt2=-0.645f;
    GLfloat yt2=0.33f;
    GLfloat radiust2 =0.034f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt2, yt2);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt2 + (radiust2 * cos(i *  twicePi / triangleAmount)),
                    yt2 + (radiust2 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xt3=-0.645f;
    GLfloat yt3=0.36f;
    GLfloat radiust3 =0.029f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt3, yt3);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt3 + (radiust3 * cos(i *  twicePi / triangleAmount)),
                    yt3 + (radiust3 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glPopMatrix();
}

//S0041
void draw_1st_roof() {

    glPushMatrix();
    glTranslatef(0.5f, 0.00f, 0.0f);
    glScalef(0.65f,0.95f,0.0f);

    glBegin(GL_QUADS);
    glColor3f(0.65, 0.56, 0.51);
    glVertex2f(0.7f, 0.3f);
    glVertex2f(0.7f, 0.2f);
    glVertex2f(0.55f, 0.2f);
    glVertex2f(0.55f, 0.3f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.72, 0.66, 0.62);
    glVertex2f(0.55f, 0.3f);
    glVertex2f(0.55f, 0.2f);
    glVertex2f(0.45f, 0.2f);
    glVertex2f(0.45f, 0.3f);
    glEnd();


     ///1st roof
    glBegin(GL_QUADS);
    glColor3f(0.44, 0.38, 0.39);
    glVertex2f(0.65f, 0.37f);
    glVertex2f(0.71f, 0.305f);
    glVertex2f(0.34f, 0.305f);
    glVertex2f(0.4f, 0.37f);
    glEnd();

    ///1st floor
    glBegin(GL_QUADS);
    glColor3f(0.64, 0.57, 0.51);
    glVertex2f(0.62f, 0.45f);
    glVertex2f(0.62f, 0.37f);
    glVertex2f(0.43f, 0.37f);
    glVertex2f(0.43f, 0.45f);
    glEnd();

    ///2nd roof
    glBegin(GL_QUADS);
    glColor3f(0.44, 0.38, 0.39);
    glVertex2f(0.59f, 0.51f);
    glVertex2f(0.63f, 0.451f);
    glVertex2f(0.42f, 0.451f);
    glVertex2f(0.46f, 0.51f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.85, 0.85, 0.85);
    glVertex2f(0.5f, 0.4f);
    glVertex2f(0.56f, 0.3f);
    glVertex2f(0.44f, 0.3f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.72, 0.66, 0.62);
    glVertex2f(0.5f, 0.385f);
    glVertex2f(0.55f, 0.3f);
    glVertex2f(0.45f, 0.3);
    glEnd();



    glBegin(GL_QUADS);
    glColor3f(0.65, 0.56, 0.51);
    glVertex2f(0.45f, 0.3f);
    glVertex2f(0.45f, 0.2f);
    glVertex2f(0.41f, 0.2f);
    glVertex2f(0.41f, 0.3f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.33, 0.27, 0.28);
    glVertex2f(0.45f, 0.27f);
    glVertex2f(0.45f, 0.2f);
    glVertex2f(0.43f, 0.2f);
    glVertex2f(0.43f, 0.27f);
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0.85, 0.85, 0.84);
    glVertex2f(.34f,.3f);
    glVertex2f(.71f,.3f);

    glVertex2f(.35f,.3f);
    glVertex2f(.35f,.2f);

    glVertex2f(.45f,.3f);
    glVertex2f(.45f,.2f);

    glVertex2f(.55f,.3f);
    glVertex2f(.55f,.2f);


    glEnd();

    glLineWidth(4);
    glBegin(GL_LINES);
    glColor3f(0.85, 0.85, 0.84);
    glVertex2f(.42f,.45f);
    glVertex2f(.63f,.45f);
    glEnd();


    glBegin(GL_POLYGON);
    glColor3f(0.85, 0.85, 0.85);
    glVertex2f(0.565f, 0.52f);
    glVertex2f(0.6f, 0.45f);
    glVertex2f(0.6f, 0.37f);
    glVertex2f(0.53f, 0.37f);
    glVertex2f(0.53f, 0.45f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.72, 0.66, 0.62);
    glVertex2f(0.565f, 0.51f);
    glVertex2f(0.595f, 0.445f);
    glVertex2f(0.595f, 0.37f);
    glVertex2f(0.535f, 0.37f);
    glVertex2f(0.535f, 0.445f);
    glEnd();

    ///window 2
    glBegin(GL_QUADS);
    glColor3f(0.85, 0.85, 0.85);
    glVertex2f(0.58f, 0.44f);
    glVertex2f(0.58f, 0.39f);
    glVertex2f(0.548f, 0.39f);
    glVertex2f(0.548f, 0.44f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.66, 0.71, 0.80);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(0.577f, 0.435f);
    glVertex2f(0.577f, 0.395f);
    glVertex2f(0.551f, 0.395f);
    glVertex2f(0.551f, 0.435f);
    glEnd();

    glLineWidth(1.5);
    glBegin(GL_LINES);
    glColor3f(0.85, 0.85, 0.84);
    glVertex2f(.564f,.435f);
    glVertex2f(.564f,.395f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.80, 0.80, 0.80);
    glVertex2f(0.68f, 0.28f);
    glVertex2f(0.68f, 0.2f);
    glVertex2f(0.58f, 0.2f);
    glVertex2f(0.58f, 0.28f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.85, 0.85, 0.85);
    glVertex2f(0.53f, 0.27f);
    glVertex2f(0.53f, 0.22f);
    glVertex2f(0.47f, 0.22f);
    glVertex2f(0.47f, 0.27f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.66, 0.71, 0.80);
    glVertex2f(0.527f, 0.268f);
    glVertex2f(0.527f, 0.222f);
    glVertex2f(0.5015f, 0.222f);
    glVertex2f(0.5015f, 0.268f);

    glVertex2f(0.473f, 0.268f);
    glVertex2f(0.473f, 0.222f);
    glVertex2f(0.4985f, 0.222f);
    glVertex2f(0.4985f, 0.268f);
    glEnd();

    ///gDore line
    glLineWidth(.7);
    glBegin(GL_LINES);
    glColor3f(0.70, 0.70, 0.70);

    glVertex2f(.678f,.28f);
    glVertex2f(.58f,.28f);

    glVertex2f(.678f,.26f);
    glVertex2f(.58f,.26f);

    glVertex2f(.678f,.24f);
    glVertex2f(.58f,.24f);

    glVertex2f(.678f,.22f);
    glVertex2f(.58f,.22f);

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.80, 0.80, 0.80);
    glVertex2f(0.685f, 0.2f);
    glVertex2f(0.685f, 0.15f);
    glVertex2f(0.575f, 0.15f);
    glVertex2f(0.575f, 0.2f);

    glVertex2f(0.46f, 0.2f);
    glVertex2f(0.46f, 0.15f);
    glVertex2f(0.34f, 0.15f);
    glVertex2f(0.34f, 0.2f);
    glEnd();

    glLoadIdentity();
    glPopMatrix();
}

//S0042
void draw_tree011() {
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.64f, 0.2f);
    glVertex2f(-0.64f, 0.3f);
    glVertex2f(-0.65f, 0.3f);
    glVertex2f(-0.65f, 0.2f);
    glEnd();

    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);

    glEnd();


    glPushMatrix();
    glTranslatef(-0.01f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);

    glEnd();


    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.02f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);

    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.01f, 0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.645f, 0.25f);
    glVertex2f(-0.64f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.655f, 0.26f);
    glVertex2f(-0.658f, 0.2f);


    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.645f, 0.2f);
    glVertex2f(-0.635f, 0.26f);
    glVertex2f(-0.638f, 0.2f);


    glEnd();


    glPopMatrix();


    GLfloat xt01=-0.645f;
    GLfloat yt01=0.3f;
    GLfloat radiust01 =0.039f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt01, yt01);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt01 + (radiust01 * cos(i *  twicePi / triangleAmount)),
                    yt01 + (radiust01 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


    GLfloat xt02=-0.645f;
    GLfloat yt02=0.33f;
    GLfloat radiust02 =0.034f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt02, yt02);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt02 + (radiust02 * cos(i *  twicePi / triangleAmount)),
                    yt02 + (radiust02 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xt03=-0.645f;
    GLfloat yt03=0.36f;
    GLfloat radiust03 =0.029f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt03, yt03);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt03 + (radiust03 * cos(i *  twicePi / triangleAmount)),
                    yt03 + (radiust03 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
}

//S0043
void draw_tree022() {
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.96f, 0.2f);
    glVertex2f(-0.96f, 0.3f);
    glVertex2f(-0.95f, 0.3f);
    glVertex2f(-0.95f, 0.2f);
    glEnd();


    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.95f, 0.2f);
    glVertex2f(-0.945f, 0.25f);
    glVertex2f(-0.938f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.955f, 0.26f);
    glVertex2f(-0.958f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.935f, 0.26f);
    glVertex2f(-0.938f, 0.2f);
    glEnd();

    glPushMatrix();
    glTranslatef(-0.02f, 0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.95f, 0.2f);
    glVertex2f(-0.945f, 0.25f);
    glVertex2f(-0.938f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.955f, 0.26f);
    glVertex2f(-0.958f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.935f, 0.26f);
    glVertex2f(-0.938f, 0.2f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.03f, 0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.95f, 0.2f);
    glVertex2f(-0.945f, 0.25f);
    glVertex2f(-0.938f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.955f, 0.26f);
    glVertex2f(-0.958f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.935f, 0.26f);
    glVertex2f(-0.938f, 0.2f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.04f, 0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.95f, 0.2f);
    glVertex2f(-0.945f, 0.25f);
    glVertex2f(-0.938f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.955f, 0.26f);
    glVertex2f(-0.958f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.935f, 0.26f);
    glVertex2f(-0.938f, 0.2f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.04f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.95f, 0.2f);
    glVertex2f(-0.945f, 0.25f);
    glVertex2f(-0.938f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.955f, 0.26f);
    glVertex2f(-0.958f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.935f, 0.26f);
    glVertex2f(-0.938f, 0.2f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.02f, 0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.95f, 0.2f);
    glVertex2f(-0.945f, 0.25f);
    glVertex2f(-0.938f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.955f, 0.26f);
    glVertex2f(-0.958f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.935f, 0.26f);
    glVertex2f(-0.938f, 0.2f);
    glEnd();
    glPopMatrix();


    GLfloat xt4=-0.955f;
    GLfloat yt4=0.3f;
    GLfloat radiust4 =0.03f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt4, yt4);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt4 + (radiust4 * cos(i *  twicePi / triangleAmount)),
                    yt4 + (radiust4 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xt5=-0.955f;
    GLfloat yt5=0.33f;
    GLfloat radiust5 =0.025f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt5, yt5);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt5 + (radiust5 * cos(i *  twicePi / triangleAmount)),
                    yt5 + (radiust5 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xt6=-0.955f;
    GLfloat yt6=0.35f;
    GLfloat radiust6 =0.02f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt6, yt6);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt6 + (radiust6 * cos(i *  twicePi / triangleAmount)),
                    yt6 + (radiust6 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
}

//S0044
void draw_tree022_copy() {
    glPushMatrix();
    glTranslatef(-0.02f, 0.02f, 0.0f);

    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.0f);
    glVertex2f(-0.96f, 0.2f);
    glVertex2f(-0.96f, 0.3f);
    glVertex2f(-0.95f, 0.3f);
    glVertex2f(-0.95f, 0.2f);
    glEnd();

    GLfloat xt7=-0.955f;
    GLfloat yt7=0.3f;
    GLfloat radiust7 =0.03f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt7, yt7);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt7 + (radiust7 * cos(i *  twicePi / triangleAmount)),
                    yt7 + (radiust7 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xt8=-0.955f;
    GLfloat yt8=0.33f;
    GLfloat radiust8 =0.025f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt8, yt8);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt8 + (radiust8 * cos(i *  twicePi / triangleAmount)),
                    yt8 + (radiust8 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    GLfloat xt9=-0.955f;
    GLfloat yt9=0.35f;
    GLfloat radiust9 =0.02f;
    glColor3f(0.0f,0.2f,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xt9, yt9);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xt9 + (radiust9 * cos(i *  twicePi / triangleAmount)),
                    yt9 + (radiust9 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glPopMatrix();


    glPushMatrix();
    glTranslatef(-0.04f, -0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.95f, 0.2f);
    glVertex2f(-0.945f, 0.25f);
    glVertex2f(-0.938f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.955f, 0.26f);
    glVertex2f(-0.958f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.935f, 0.26f);
    glVertex2f(-0.938f, 0.2f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.02f, -0.02f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.95f, 0.2f);
    glVertex2f(-0.945f, 0.25f);
    glVertex2f(-0.938f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.955f, 0.26f);
    glVertex2f(-0.958f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.935f, 0.26f);
    glVertex2f(-0.938f, 0.2f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.05f, -0.01f, 0.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.95f, 0.2f);
    glVertex2f(-0.945f, 0.25f);
    glVertex2f(-0.938f, 0.2f);

    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.955f, 0.26f);
    glVertex2f(-0.958f, 0.2f);

    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.945f, 0.2f);
    glVertex2f(-0.935f, 0.26f);
    glVertex2f(-0.938f, 0.2f);
    glEnd();
    glPopMatrix();
}

//S0045
void draw_wake_way() {
    glBegin(GL_QUADS);
    glColor3f(0.8f,0.8f,0.8f);
    glVertex2f(-1.0f, -0.17f);
    glVertex2f(1.0f, -0.17f);
    glVertex2f(1.0f, -0.12f);
    glVertex2f(-1.0f, -0.12f);


    glColor3f(0.8f,0.8f,0.8f);
    glVertex2f(-1.0f, 0.17f);
    glVertex2f(1.0f, 0.17f);
    glVertex2f(1.0f, 0.12f);
    glVertex2f(-1.0f, 0.12f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(-1.0f, -0.1f);
    glVertex2f(1.0f, -0.1f);
    glVertex2f(1.0f, -0.12f);
    glVertex2f(-1.0f, -0.12f);


    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(-1.0f, 0.1f);
    glVertex2f(1.0f, 0.1f);
    glVertex2f(1.0f, 0.12f);
    glVertex2f(-1.0f, 0.12f);

    glEnd();
}

//S0046
void draw_lamp1() {
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);


    glEnd();

    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.85f, 0.12f);

        glEnd();
    }
}

//S0047
void draw_lamp2() {

    glPushMatrix();
    glTranslatef(0.2f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.85f, 0.12f);

        glEnd();
    }
    glPopMatrix();
}

//S0048
void draw_lamp3() {

    glPushMatrix();
    glTranslatef(0.4f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.85f, 0.12f);
        glEnd();
    }
    glPopMatrix();
}

//S0049
void draw_lamp4() {

    glPushMatrix();
    glTranslatef(0.6f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.85f, 0.12f);

        glEnd();
    }
    glPopMatrix();
}

//S0050
void draw_lamp5() {

    glPushMatrix();
    glTranslatef(0.8f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.85f, 0.12f);
        glEnd();
    }
    glPopMatrix();
}

//S0051
void draw_lamp6() {

    glPushMatrix();
    glTranslatef(1.0f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.85f, 0.12f);
        glEnd();
    }
    glPopMatrix();
}

//S0052
void draw_lamp7() {

    glPushMatrix();
    glTranslatef(1.2f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.85f, 0.12f);

        glEnd();
    }
    glPopMatrix();
}

//S0053
void draw_lamp8() {

    glPushMatrix();
    glTranslatef(1.4f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.85f, 0.12f);

        glEnd();
    }
    glPopMatrix();
}

//S0054
void draw_lamp9() {

    glPushMatrix();
    glTranslatef(1.6f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.85f, 0.12f);

        glEnd();
    }
    glPopMatrix();
}

//S0055
void draw_lamp10() {

    glPushMatrix();
    glTranslatef(1.8f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.8f,0.8f,0.8f);
        glVertex2f(-0.85f, 0.12f);
        glEnd();
    }
    glPopMatrix();
}

//S0056
void draw_police() {

    GLfloat xp=0.4f;
    GLfloat yp=0.195f;
    GLfloat radiusp =0.007f;

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xp, yp); // center of circle
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xp + (radiusp * cos(i *  twicePi / triangleAmount)),
                    yp+ (radiusp * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.8f,0.8f,0.0f);
    glVertex2f(0.394f, 0.185f);
    glVertex2f(0.407f, 0.185f);
    glVertex2f(0.407f, 0.155f);
    glVertex2f(0.394f, 0.155f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(0.396f, 0.185f);
    glVertex2f(0.399f, 0.185f);
    glVertex2f(0.399f, 0.155f);
    glVertex2f(0.396f, 0.155f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(0.405f, 0.185f);
    glVertex2f(0.402f, 0.185f);
    glVertex2f(0.402f, 0.155f);
    glVertex2f(0.405f, 0.155f);

    glEnd();

    glBegin(GL_QUADS);

//right hand
    glColor3f(0.0f,0.0f,1.0f);
    glVertex2f(0.411f, 0.185f);
    glVertex2f(0.407f, 0.185f);
    glVertex2f(0.407f, 0.145f);
    glVertex2f(0.411f, 0.145f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(0.411f, 0.155f);
    glVertex2f(0.407f, 0.155f);
    glVertex2f(0.407f, 0.145f);
    glVertex2f(0.411f, 0.145f);

//left hand
    if(!handup)
    {
        glColor3f(0.0f,0.0f,1.0f);
        glVertex2f(0.394f, 0.185f);
        glVertex2f(0.39f, 0.185f);
        glVertex2f(0.39f, 0.145f);
        glVertex2f(0.394f, 0.145f);

        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f(0.394f, 0.155f);
        glVertex2f(0.39f, 0.155f);
        glVertex2f(0.39f, 0.145f);
        glVertex2f(0.394f, 0.145f);
    }

    //left hand up
    if(handup)
    {
        glColor3f(0.0f,0.0f,1.0f);
        glVertex2f(0.394f, 0.175f);
        glVertex2f(0.39f, 0.175f);
        glVertex2f(0.39f, 0.215f);
        glVertex2f(0.394f, 0.215f);

        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f(0.394f, 0.205f);
        glVertex2f(0.39f, 0.205f);
        glVertex2f(0.39f, 0.215f);
        glVertex2f(0.394f, 0.215f);


    }
// left lag

    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(0.406f, 0.12f);
    glVertex2f(0.401f, 0.12f);
    glVertex2f(0.401f, 0.155f);
    glVertex2f(0.406f, 0.155f);

// right lag

    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(0.395f, 0.12f);
    glVertex2f(0.4f, 0.12f);
    glVertex2f(0.4f, 0.155f);
    glVertex2f(0.395f, 0.155f);

     glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(0.415f, 0.12f);
    glVertex2f(0.47f, 0.12f);
    glVertex2f(0.46f, 0.158f);
    glVertex2f(0.42f, 0.158f);

    glColor3f(0.9f,0.9f,0.0f);
    glVertex2f(0.414f, 0.12f);
    glVertex2f(0.455f, 0.12f);
    glVertex2f(0.46f, 0.158f);
    glVertex2f(0.42f, 0.158f);


    glEnd();
}

//S0057
void draw_road2() {
    glBegin(GL_QUADS);

    glColor3f(0.2f,0.2f,0.2f);
    glVertex2f(-1.0f, -0.1f);
    glVertex2f(1.0f, -0.1f);
    glVertex2f(1.0f, 0.1f);
    glVertex2f(-1.0f, 0.1f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();

    glPushMatrix();
    glTranslatef(-0.2f,0.0f, 0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.4f,0.0f, 0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.6f,0.0f, 0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.8f,0.0f, 0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f,0.0f, 0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.2f,0.0f, 0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.4f,0.0f, 0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6f,0.0f, 0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8f,0.0f, 0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f,0.0f, 0.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.05f,-0.01f);
    glVertex2f(-0.05f,-0.01f);
    glEnd();
    glPopMatrix();
}

//S0058
void draw_bus011() {

    glPushMatrix();
    glTranslatef(_run4,0,0);
    GLfloat xb1=-0.455f;
    GLfloat yb1=0.095f;
    GLfloat radiusb1 =.01f;
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xb1, yb1);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xb1 + (radiusb1 * cos(i *  twicePi / triangleAmount)),
                    yb1 + (radiusb1 * sin(i * twicePi / triangleAmount)));
    }
    glEnd();

    if(night)
    {
        glBegin(GL_QUADS);
        glColor3f(1.0f,0.6f,0.2f);
        glVertex2f(-0.445f, 0.097f);
        glVertex2f(-0.455f, 0.095f);
        glColor3f(1.0f,1.0f,0.79f);
        glVertex2f(-0.435f, 0.06f);
        glVertex2f(-0.39f, 0.06f);

        glEnd();
    }

    glBegin(GL_QUADS);
    glColor3f(1.0, 0.5, 0.0);
    glVertex2f(-.45,.188);
    glVertex2f(-.45,.06);
    glColor3f(0.98, 0.88, 0.02);
    glVertex2f(-.6,.06);
    glVertex2f(-.6,.188);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.00, 0.00, 0.00);
    glVertex2f(-.45,.077);
    glVertex2f(-.45,.069);
    glVertex2f(-.46,.069);
    glVertex2f(-.46,.077);
    glEnd();

    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.455f,.17f);
    glVertex2f(-.455f,.12f);
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.59f,.12f);
    glVertex2f(-.47f,.12f);
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.59f,.1f);
    glVertex2f(-.47f,.1f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();

    glPushMatrix();
    glTranslatef(-.022,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.044,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.066,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.088,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.11,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();
    glPopMatrix();



    GLfloat xb2=-0.49f;
    GLfloat yb2=0.06f;
    GLfloat radiusb2 =.02f;
    glColor3f(0.10, 0.10, 0.10);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xb2, yb2);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xb2 + (radiusb2 * cos(i *  twicePi / triangleAmount)),
                    yb2 + (radiusb2 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


    GLfloat xb3=-.49f;
    GLfloat yb3=0.06f;
    GLfloat radiusb3 =.014f;
    glColor3f(0.47, 0.46, 0.46 );
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xb3, yb3);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xb3 + (radiusb3 * cos(i *  twicePi / triangleAmount)),
                    yb3 + (radiusb3 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();



    GLfloat xb4=-.57f;
    GLfloat yb4=0.06f;
    glColor3f(0.10, 0.10, 0.10 );
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xb4, yb4);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xb4 + (radiusb2 * cos(i *  twicePi / triangleAmount)),
                    yb4 + (radiusb2 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();



    GLfloat xb5=-.57f;
    GLfloat yb5=0.06f;
    glColor3f(0.47, 0.46, 0.46 );
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xb5, yb5);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xb5 + (radiusb3 * cos(i *  twicePi / triangleAmount)),
                    yb5 + (radiusb3 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glPopMatrix();

    //CAR4 body<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    glPushMatrix();
    glTranslatef(_run5,0,0);
    glPushMatrix();
    glTranslatef(0.5f,0.1f,0.0f);
    glRotatef(180,0,1,0),
              glBegin(GL_QUADS);
    glColor3f(0.0f,0.5f,0.0f);
    glVertex2f(0.42f, 0.0175f);
    glVertex2f(0.42f, -0.02f);
    glVertex2f(0.32f, -0.02f);
    glVertex2f(0.33f, 0.0175f);

    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.5f,0.0f);
    glVertex2f(0.42f, -0.02f);
    glVertex2f(0.42f, -0.067f);
    glVertex2f(0.3f, -0.067f);
    glVertex2f(0.3f, -0.02f);

    glEnd();



//Light....................................................................
    glBegin(GL_QUADS);
    glColor3f(1.0f,0.038f,0.27f);



    glVertex2f(0.306f, -0.02f);
    glVertex2f(0.309f, -0.032f);

    glVertex2f(0.3f, -0.032f);
    glVertex2f(0.3f, -0.02f);




    glEnd();

    if(night)
    {
        glBegin(GL_QUADS);
        glColor3f(1.0f,0.6f,0.2f);
        glVertex2f(0.3f, -0.025f);
        glVertex2f(0.3f, -0.032f);
        glColor3f(1.0f,1.0f,0.79f);
        glVertex2f(0.28f, -0.067f);
        glVertex2f(0.25f, -0.067f);

        glEnd();
    }

//back light.............................................
    glBegin(GL_QUADS);
    glColor3f(1.0f,0.38f,0.27f);

    glVertex2f(0.42f, -0.04f);
    glVertex2f(0.413f, -0.04);
    glVertex2f(0.413f, -0.054f);
    glVertex2f(0.42f, -0.054f);


    glEnd();
//  //BACK SIDE WHELL.......................................................

    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.42f, -0.018f);
    glVertex2f(0.425f, -0.018f);
    glVertex2f(0.425f, -0.05f);
    glVertex2f(0.42f, -0.053f);



    glEnd();


    //window1..............................

    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);


    glVertex2f(0.352f, 0.012f);
    glVertex2f(0.352f, -0.02f);
    glVertex2f(0.325f, -0.02f);
    glVertex2f(0.334f, 0.012f);
    glEnd();

    //window2..................
    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.354f, 0.012f);
    glVertex2f(0.354f, -0.02f);
    glVertex2f(0.374f, -0.02f);
    glVertex2f(0.374f, 0.012f);



    glEnd();


    //window3.................................

    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.41f, 0.012f);
    glVertex2f(0.41f, -0.02f);
    glVertex2f(0.376f, -0.02f);
    glVertex2f(0.376f, 0.012f);



    glEnd();
//
    //Door1..........................................


    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.352f, -0.023f);
    glVertex2f(0.352f, -0.027f);
    glVertex2f(0.344f, -0.027f);
    glVertex2f(0.344f, -0.023f);



    glEnd();

    //Door2..........................................................



    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.373f, -0.023f);
    glVertex2f(0.373f, -0.027f);
    glVertex2f(0.367f, -0.027f);
    glVertex2f(0.367f, -0.023f);



    glEnd();

    //wheel1...................................



    GLfloat xc=0.33f;
    GLfloat yc=-0.067f;
    GLfloat radiusc =0.015f;

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xc, yc); // center of circle
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xc + (radiusc * cos(i *  twicePi / triangleAmount)),
                    yc+ (radiusc * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


    GLfloat xc1=0.33f;
    GLfloat yc1=-0.067f;
    GLfloat radiusc3 =0.008f;

    glColor3f(0.50f, 0.50f, 0.50f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xc1, yc1); // center of circle
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xc1 + (radiusc3 * cos(i *  twicePi / triangleAmount)),
                    yc1 + (radiusc3 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


//wheel2.................................


    GLfloat xc2=0.39f;
    GLfloat yc2=-0.067f;
    GLfloat radiusc4=0.015f;

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xc2, yc2); // center of circle
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xc2 + (radiusc4 * cos(i *  twicePi / triangleAmount)),
                    yc2 + (radiusc4 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();



    GLfloat xc3=0.39f;
    GLfloat yc3=-0.067f;
    GLfloat radiusc5=0.008f;

    glColor3f(0.50f, 0.50f, 0.50f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xc3, yc3); // center of circle
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xc3 + (radiusc5 * cos(i *  twicePi / triangleAmount)),
                    yc3 + (radiusc5 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glLoadIdentity();

    glPopMatrix();


    glPopMatrix();
}

//S0059
void draw_car4_body() {
    glPushMatrix();
    glTranslatef(_run5,0,0);
    glPushMatrix();
    glTranslatef(0.5f,0.1f,0.0f);
    glRotatef(180,0,1,0),
              glBegin(GL_QUADS);
    glColor3f(0.0f,0.5f,0.0f);
    glVertex2f(0.42f, 0.0175f);
    glVertex2f(0.42f, -0.02f);
    glVertex2f(0.32f, -0.02f);
    glVertex2f(0.33f, 0.0175f);

    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.5f,0.0f);
    glVertex2f(0.42f, -0.02f);
    glVertex2f(0.42f, -0.067f);
    glVertex2f(0.3f, -0.067f);
    glVertex2f(0.3f, -0.02f);

    glEnd();



//Light....................................................................
    glBegin(GL_QUADS);
    glColor3f(1.0f,0.038f,0.27f);



    glVertex2f(0.306f, -0.02f);
    glVertex2f(0.309f, -0.032f);

    glVertex2f(0.3f, -0.032f);
    glVertex2f(0.3f, -0.02f);




    glEnd();

    if(night)
    {
        glBegin(GL_QUADS);
        glColor3f(1.0f,0.6f,0.2f);
        glVertex2f(0.3f, -0.025f);
        glVertex2f(0.3f, -0.032f);
        glColor3f(1.0f,1.0f,0.79f);
        glVertex2f(0.28f, -0.067f);
        glVertex2f(0.25f, -0.067f);

        glEnd();
    }

//back light.............................................
    glBegin(GL_QUADS);
    glColor3f(1.0f,0.38f,0.27f);

    glVertex2f(0.42f, -0.04f);
    glVertex2f(0.413f, -0.04);
    glVertex2f(0.413f, -0.054f);
    glVertex2f(0.42f, -0.054f);


    glEnd();
//  //BACK SIDE WHELL.......................................................

    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.42f, -0.018f);
    glVertex2f(0.425f, -0.018f);
    glVertex2f(0.425f, -0.05f);
    glVertex2f(0.42f, -0.053f);



    glEnd();


    //window1..............................

    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);


    glVertex2f(0.352f, 0.012f);
    glVertex2f(0.352f, -0.02f);
    glVertex2f(0.325f, -0.02f);
    glVertex2f(0.334f, 0.012f);
    glEnd();

    //window2..................
    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.354f, 0.012f);
    glVertex2f(0.354f, -0.02f);
    glVertex2f(0.374f, -0.02f);
    glVertex2f(0.374f, 0.012f);



    glEnd();


    //window3.................................

    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.41f, 0.012f);
    glVertex2f(0.41f, -0.02f);
    glVertex2f(0.376f, -0.02f);
    glVertex2f(0.376f, 0.012f);



    glEnd();
//
    //Door1..........................................


    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.352f, -0.023f);
    glVertex2f(0.352f, -0.027f);
    glVertex2f(0.344f, -0.027f);
    glVertex2f(0.344f, -0.023f);



    glEnd();

    //Door2..........................................................



    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.373f, -0.023f);
    glVertex2f(0.373f, -0.027f);
    glVertex2f(0.367f, -0.027f);
    glVertex2f(0.367f, -0.023f);



    glEnd();

    //wheel1...................................



    GLfloat xc=0.33f;
    GLfloat yc=-0.067f;
    GLfloat radiusc =0.015f;

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xc, yc); // center of circle
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xc + (radiusc * cos(i *  twicePi / triangleAmount)),
                    yc+ (radiusc * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


    GLfloat xc1=0.33f;
    GLfloat yc1=-0.067f;
    GLfloat radiusc3 =0.008f;

    glColor3f(0.50f, 0.50f, 0.50f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xc1, yc1); // center of circle
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xc1 + (radiusc3 * cos(i *  twicePi / triangleAmount)),
                    yc1 + (radiusc3 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


//wheel2.................................


    GLfloat xc2=0.39f;
    GLfloat yc2=-0.067f;
    GLfloat radiusc4=0.015f;

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xc2, yc2); // center of circle
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xc2 + (radiusc4 * cos(i *  twicePi / triangleAmount)),
                    yc2 + (radiusc4 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();



    GLfloat xc3=0.39f;
    GLfloat yc3=-0.067f;
    GLfloat radiusc5=0.008f;

    glColor3f(0.50f, 0.50f, 0.50f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xc3, yc3); // center of circle
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xc3 + (radiusc5 * cos(i *  twicePi / triangleAmount)),
                    yc3 + (radiusc5 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glLoadIdentity();

    glPopMatrix();


    glPopMatrix();
}

//S0060
void draw_car3_body() {
    glPushMatrix();
    glTranslatef(_run6,0,0);
    glPushMatrix();
    glTranslatef(-0.45f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,1.0f);
    glVertex2f(0.4f, 0.0175f);
    glVertex2f(0.42f, -0.02f);
    glVertex2f(0.32f, -0.02f);
    glVertex2f(0.33f, 0.0175f);

    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,1.0f);
    glVertex2f(0.42f, -0.02f);
    glVertex2f(0.42f, -0.067f);
    glVertex2f(0.3f, -0.067f);
    glVertex2f(0.3f, -0.02f);

    glEnd();



//Light....................................................................
    glBegin(GL_QUADS);
    glColor3f(1.0f,0.038f,0.27f);



    glVertex2f(0.306f, -0.02f);
    glVertex2f(0.309f, -0.032f);

    glVertex2f(0.3f, -0.032f);
    glVertex2f(0.3f, -0.02f);


    glEnd();
    if(night)
    {
        glBegin(GL_QUADS);
        glColor3f(1.0f,0.6f,0.2f);
        glVertex2f(0.3f, -0.025f);
        glVertex2f(0.3f, -0.032f);
        glColor3f(1.0f,1.0f,0.79f);
        glVertex2f(0.28f, -0.067f);
        glVertex2f(0.25f, -0.067f);

        glEnd();
    }
//back light.............................................
    glBegin(GL_QUADS);
    glColor3f(1.0f,0.38f,0.27f);

    glVertex2f(0.42f, -0.04f);
    glVertex2f(0.413f, -0.04);
    glVertex2f(0.413f, -0.054f);
    glVertex2f(0.42f, -0.054f);


    glEnd();


    //window1..............................

    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);


    glVertex2f(0.352f, 0.012f);
    glVertex2f(0.352f, -0.02f);
    glVertex2f(0.325f, -0.02f);
    glVertex2f(0.334f, 0.012f);
    glEnd();

    //window2..................
    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.354f, 0.012f);
    glVertex2f(0.354f, -0.02f);
    glVertex2f(0.374f, -0.02f);
    glVertex2f(0.374f, 0.012f);



    glEnd();


    //window3.................................

    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.394f, 0.012f);
    glVertex2f(0.41f, -0.02f);
    glVertex2f(0.376f, -0.02f);
    glVertex2f(0.376f, 0.012f);



    glEnd();

    //Door1..........................................


    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.352f, -0.023f);
    glVertex2f(0.352f, -0.027f);
    glVertex2f(0.344f, -0.027f);
    glVertex2f(0.344f, -0.023f);


    glEnd();

    //Door2..........................................................

    glBegin(GL_QUADS);
    glColor3f(0.8f,1.0f,1.0f);

    glVertex2f(0.373f, -0.023f);
    glVertex2f(0.373f, -0.027f);
    glVertex2f(0.367f, -0.027f);
    glVertex2f(0.367f, -0.023f);

// Wheels
float radius = 0.012f;
float cx1 = 0.31f, cy1 = -0.07f;
float cx2 = 0.4f, cy2 = -0.07f;

glColor3f(0.0f, 0.0f, 0.0f);

// Left wheel using GL_TRIANGLE_FAN
glBegin(GL_TRIANGLE_FAN);
glVertex2f(cx1, cy1); // center
for (int i = 0; i <= 100; i++) {
    float angle = 2.0f * M_PI * i / 100;
    glVertex2f(cx1 + radius * cos(angle), cy1 + radius * sin(angle));
}
glEnd();

// Right wheel using GL_TRIANGLE_FAN
glBegin(GL_TRIANGLE_FAN);
glVertex2f(cx2, cy2); // center
for (int i = 0; i <= 100; i++) {
    float angle = 2.0f * M_PI * i / 100;
    glVertex2f(cx2 + radius * cos(angle), cy2 + radius * sin(angle));
}

    glEnd();



    glPopMatrix();
    glPopMatrix();
}


//S0061
void draw_bus02() {

     glPushMatrix();

    glTranslatef(_run7,0,0);

    glPushMatrix();
    glTranslatef(0.09f,-0.125f, 0.0f);
    glRotatef(180,0,1,0);



    GLfloat xb6=-0.455f;
    GLfloat yb6=0.095f;
    GLfloat radiusb6 =.01f;
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xb6, yb6);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xb6 + (radiusb6 * cos(i *  twicePi / triangleAmount)),
                    yb6 + (radiusb6 * sin(i * twicePi / triangleAmount)));
    }
    glEnd();

    if(night)
    {
        glBegin(GL_QUADS);
        glColor3f(1.0f,0.6f,0.2f);
        glVertex2f(-0.445f, 0.097f);
        glVertex2f(-0.455f, 0.095f);
        glColor3f(1.0f,1.0f,0.79f);
        glVertex2f(-0.435f, 0.06f);
        glVertex2f(-0.39f, 0.06f);

        glEnd();
    }

    glBegin(GL_QUADS);
    glColor3f(0.0, 0.4, 0.0);
    glVertex2f(-.45,.188);
    glVertex2f(-.45,.06);
    glColor3f(1.0, 0.2, 0.2);
    glVertex2f(-.6,.06);
    glVertex2f(-.6,.188);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.00, 0.00, 0.00);
    glVertex2f(-.45,.077);
    glVertex2f(-.45,.069);
    glVertex2f(-.46,.069);
    glVertex2f(-.46,.077);
    glEnd();

    glLineWidth(6);
    glBegin(GL_LINES);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.455f,.17f);
    glVertex2f(-.455f,.12f);
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.59f,.12f);
    glVertex2f(-.47f,.12f);
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.59f,.1f);
    glVertex2f(-.47f,.1f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();

    glPushMatrix();
    glTranslatef(-.022,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.044,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.066,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.088,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.11,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.76, 0.77, 0.78);
    glVertex2f(-.465,.17);
    glVertex2f(-.465,.14);
    glVertex2f(-.48,.14);
    glVertex2f(-.48,.17);
    glEnd();
    glPopMatrix();



    GLfloat xb7=-0.49f;
    GLfloat yb7=0.06f;
    GLfloat radiusb7 =.02f;
    glColor3f(0.10, 0.10, 0.10);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xb7, yb7);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xb7 + (radiusb7 * cos(i *  twicePi / triangleAmount)),
                    yb7 + (radiusb7 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();


    GLfloat xb8=-.49f;
    GLfloat yb8=0.06f;
    GLfloat radiusb8 =.014f;
    glColor3f(0.47, 0.46, 0.46 );
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xb8, yb8);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xb8 + (radiusb8 * cos(i *  twicePi / triangleAmount)),
                    yb8 + (radiusb8 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();



    GLfloat xb9=-.57f;
    GLfloat yb9=0.06f;
    GLfloat radiusb9 =.02f;
    glColor3f(0.10, 0.10, 0.10 );
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xb9, yb9);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xb9 + (radiusb9 * cos(i *  twicePi / triangleAmount)),
                    yb9 + (radiusb9 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();



    GLfloat xb10=-.57f;
    GLfloat yb10=0.06f;
    GLfloat radiusb10 =.014f;
    glColor3f(0.47, 0.46, 0.46);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xb10, yb10);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xb10 + (radiusb10 * cos(i *  twicePi / triangleAmount)),
                    yb10 + (radiusb10* sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glLoadIdentity();
    glPopMatrix();
    glPopMatrix();
}

//S0062
void draw_lamp11() {

    glPushMatrix();
    glTranslatef(-0.1f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);


        glEnd();
    }
    glPopMatrix();
}

//S0063
void draw_lamp12() {

    glPushMatrix();
    glTranslatef(0.1f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);


        glEnd();
    }
    glPopMatrix();
}

//S0064
void draw_lamp13() {

    glPushMatrix();
    glTranslatef(0.3f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);


        glEnd();
    }
    glPopMatrix();
}

//S0065
void draw_lamp14() {

    glPushMatrix();
    glTranslatef(0.5f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);


        glEnd();
    }
    glPopMatrix();
}

//S0066
void draw_lamp15() {

    glPushMatrix();
    glTranslatef(0.7f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);


        glEnd();
    }
    glPopMatrix();
}

//S0067
void draw_lamp16() {

    glPushMatrix();
    glTranslatef(0.9f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);


        glEnd();
    }
    glPopMatrix();
}

//S0068
void draw_lamp17() {

    glPushMatrix();
    glTranslatef(1.1f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);


        glEnd();
    }
    glPopMatrix();


    //18

    glPushMatrix();
    glTranslatef(1.3f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);


        glEnd();
    }
    glPopMatrix();
}

//S0069
void draw_lamp19() {

    glPushMatrix();
    glTranslatef(1.5f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);


        glEnd();
    }
    glPopMatrix();
}

//S0070
void draw_lamp20() {

    glPushMatrix();
    glTranslatef(1.7f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);

        glEnd();
    }
    glPopMatrix();
}

//S0071
void draw_lamp21() {

    glPushMatrix();
    glTranslatef(1.9f,-0.22f,0.0f);
    glBegin(GL_QUADS);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.9025f, 0.21f);
    glVertex2f(-0.8975f, 0.21f);
    glVertex2f(-0.8975f, 0.11f);
    glVertex2f(-0.9025f, 0.11f);

    glColor3f(0.8f,0.9f,1.0f);
    glVertex2f(-0.915f, 0.21f);
    glVertex2f(-0.885f, 0.21f);
    glVertex2f(-0.885f, 0.216f);
    glVertex2f(-0.915f, 0.216f);

    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);

    glColor3f(0.0f,0.0f,0.0f);
    if(night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glVertex2f(-0.915f, 0.213f);

    glVertex2f(-0.885f, 0.213f);
    glEnd();
    if(night)
    {
        //night light
        glBegin(GL_TRIANGLES);
        glBegin(GL_TRIANGLES);
        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.915f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.905f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.95f, 0.12f);

        glColor3f(0.9f,0.9f,0.0f);
        glVertex2f(-0.885f, 0.213f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.895f, 0.12f);
        glColor3f(0.2f,0.2f,0.2f);
        glVertex2f(-0.85f, 0.12f);

        glEnd();
    }
    glPopMatrix();

    //vod bech
    glBegin(GL_QUADS);
    glColor3f(1.0f,0.71f,0.43f);
    if(night)
    {
        glColor3f(0.8, 0.51, 0.23);
    }

    glVertex2f(-1.0f, -0.45f);
    glVertex2f(1.0f, -0.45f);
    glVertex2f(1.0f, -0.17f);
    glVertex2f(-1.0f, -0.17f);
    glEnd();
}

//S0072
void draw_sea() {
    glPushMatrix();
    glTranslatef(0,-.1,0);
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }

    glVertex2f(-1.0f, -0.35f);
    glVertex2f(1.0f, -0.35f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();
    glPopMatrix();
}

//S0073
void draw_c() {
     glPushMatrix();
    glTranslatef(0,c1,0);
    GLfloat xw1=-0.9f;
    GLfloat yw1=-0.61f;
    GLfloat radiusw1 =0.3f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw1, yw1);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw1 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw1 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c2..............................
    glPushMatrix();
    glTranslatef(0,c2,0);
    GLfloat xw2=0.3f;
    GLfloat yw2=-0.61f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw2, yw2);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw2 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw2 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();


    //c3..............................
    glPushMatrix();
    glTranslatef(0,c3,0);
    GLfloat xw3=0.2f;
    GLfloat yw3=-0.62f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw3, yw3);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw3 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw3 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c4..............................

    glPushMatrix();
    glTranslatef(0,c4,0);
    GLfloat xw4=0.1f;
    GLfloat yw4=-0.63f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw4, yw4);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw4 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw4 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();


    //c5..............................
    glPushMatrix();
    glTranslatef(0,c5,0);
    GLfloat xw5=-0.2f;
    GLfloat yw5=-0.62f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw5, yw5);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw5 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw5 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c6..............................
    glPushMatrix();
    glTranslatef(0,c6,0);
    GLfloat xw6=-0.8f;
    GLfloat yw6=-0.62f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw6, yw6);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw6 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw6 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c7..............................
    glPushMatrix();
    glTranslatef(0,c7,0);
    GLfloat xw7=-0.7f;
    GLfloat yw7=-0.63f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw7, yw7);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw7 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw7 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c8..............................
    glPushMatrix();
    glTranslatef(0,c8,0);
    GLfloat xw8=-0.6f;
    GLfloat yw8=-0.62f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw8, yw8);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw8 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw8 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c9..............................
    glPushMatrix();
    glTranslatef(0,c9,0);
    GLfloat xw9=-0.5f;
    GLfloat yw9=-0.61f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw9, yw9);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw9 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw9 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c10..............................
    glPushMatrix();
    glTranslatef(0,c10,0);
    GLfloat xw10=-0.4f;
    GLfloat yw10=-0.62f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw10, yw10);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw10 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw10 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c11..............................
    glPushMatrix();
    glTranslatef(0,c11,0);
    GLfloat xw11=-0.05f;
    GLfloat yw11=-0.63f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw11, yw11);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw11 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw11 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c12..............................
    glPushMatrix();
    glTranslatef(0,c12,0);
    GLfloat xw12=0.4f;
    GLfloat yw12=-0.61f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw12, yw12);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw12 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw12 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c13..............................
    glPushMatrix();
    glTranslatef(0,c13,0);
    GLfloat xw13=0.5f;
    GLfloat yw13=-0.62f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw13, yw13);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw13 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw13 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c14..............................
    glPushMatrix();
    glTranslatef(0,c14,0);
    GLfloat xw14=0.6f;
    GLfloat yw14=-0.63f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw14, yw14);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw14 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw14 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c15..............................
    glPushMatrix();
    glTranslatef(0,c15,0);
    GLfloat xw15=0.7f;
    GLfloat yw15=-0.61f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw15, yw15);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw15 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw15 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();


    //c16..............................
    glPushMatrix();
    glTranslatef(0,c16,0);
    GLfloat xw16=0.8f;
    GLfloat yw16=-0.62f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw16, yw16);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw16 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw16 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    //c17..............................
    glPushMatrix();
    glTranslatef(0,c17,0);
    GLfloat xw17=0.9f;
    GLfloat yw17=-0.63f;
    glColor3f(0.0f,0.29f,0.6f);
    if(night)
    {
        glColor3f(0.0f, 0.09f, 0.4f);
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xw17, yw17);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xw17 + (radiusw1 * cos(i *  twicePi / triangleAmount)),
                    yw17 + (radiusw1 * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();
    glPopMatrix();

    glPopMatrix();

}

//S0074
void draw_small_boat() {
    glPushMatrix();
    glTranslatef(0,-.1,0);
    if(!night && ! rain)
    {
        glPushMatrix();
        glTranslatef(_run8,0,0);

        glPushMatrix();
        glTranslatef(0.01f, -0.2f, 0.0f);
        glScalef(.5,.5,0);


        glBegin(GL_POLYGON);

        glColor3f(0.4f,0.0f,0.0f);

        glVertex2f(-0.75f, -0.47f);
        glVertex2f(-0.65f, -0.47f);

        glVertex2f(-0.6f, -0.4f);
        glVertex2f(-0.65f, -0.47f);

        glVertex2f(-0.8f, -0.4f);
        glVertex2f(-0.75f, -0.47f);

        glVertex2f(-0.6f, -0.4f);
        glVertex2f(-0.65f, -0.44f);

        glVertex2f(-0.75f, -0.44f);
        glVertex2f(-0.8f, -0.4f);

        glVertex2f(-0.65f, -0.44f);
        glVertex2f(-0.75f, -0.44f);

        glEnd();

        glBegin(GL_QUADS);

        glColor3f(0.0f,0.6f,0.0f);

        glVertex2f(-0.72f, -0.39f);
        glVertex2f(-0.67f, -0.39f);
        glVertex2f(-0.65f, -0.435f);
        glVertex2f(-0.7f, -0.435f);


        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.0f,0.0f,0.0f);

        glVertex2f(-0.72f, -0.39f);
        glVertex2f(-0.7f, -0.43f);
        glVertex2f(-0.75f, -0.418f);


        glEnd();
        glLoadIdentity();
        glPopMatrix();

        glPopMatrix();
    }
    glPopMatrix();

    //smallboat night
    if(night || rain)
    {
        glPushMatrix();
        glTranslatef(1.1,-.05,0);

        glPushMatrix();
        glTranslatef(0.01f, -0.2f, 0.0f);
        glScalef(.5,.5,0);


        glBegin(GL_POLYGON);

        glColor3f(0.4f,0.0f,0.0f);

        glVertex2f(-0.75f, -0.47f);
        glVertex2f(-0.65f, -0.47f);

        glVertex2f(-0.6f, -0.4f);
        glVertex2f(-0.65f, -0.47f);

        glVertex2f(-0.8f, -0.4f);
        glVertex2f(-0.75f, -0.47f);

        glVertex2f(-0.6f, -0.4f);
        glVertex2f(-0.65f, -0.44f);

        glVertex2f(-0.75f, -0.44f);
        glVertex2f(-0.8f, -0.4f);

        glVertex2f(-0.65f, -0.44f);
        glVertex2f(-0.75f, -0.44f);

        glEnd();

        glBegin(GL_QUADS);

        glColor3f(0.0f,0.6f,0.0f);

        glVertex2f(-0.72f, -0.39f);
        glVertex2f(-0.67f, -0.39f);
        glVertex2f(-0.65f, -0.435f);
        glVertex2f(-0.7f, -0.435f);


        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.0f,0.0f,0.0f);

        glVertex2f(-0.72f, -0.39f);
        glVertex2f(-0.7f, -0.43f);
        glVertex2f(-0.75f, -0.418f);


        glEnd();
        glLoadIdentity();
        glPopMatrix();

        glPopMatrix();
    }
}

//S0075
void draw_small_boat2() {
    glPushMatrix();
    glTranslatef(0,-.1,0);
    if(!night && !rain)
    {
        glPushMatrix();
        glTranslatef(_run9,0,0);

        glPushMatrix();
        glTranslatef(0.09f,-0.3f, 0.0f);
        glRotatef(180,0,1,0);

        glScalef(.5,.5,0);


        glBegin(GL_POLYGON);

        glColor3f(0.2f,0.1f,0.0f);

        glVertex2f(-0.75f, -0.47f);
        glVertex2f(-0.65f, -0.47f);

        glVertex2f(-0.6f, -0.4f);
        glVertex2f(-0.65f, -0.47f);

        glVertex2f(-0.8f, -0.4f);
        glVertex2f(-0.75f, -0.47f);

        glVertex2f(-0.6f, -0.4f);
        glVertex2f(-0.65f, -0.44f);

        glVertex2f(-0.75f, -0.44f);
        glVertex2f(-0.8f, -0.4f);

        glVertex2f(-0.65f, -0.44f);
        glVertex2f(-0.75f, -0.44f);

        glEnd();

        glBegin(GL_QUADS);

        glColor3f(0.9f,0.5f,0.0f);

        glVertex2f(-0.72f, -0.39f);
        glVertex2f(-0.67f, -0.39f);
        glVertex2f(-0.65f, -0.435f);
        glVertex2f(-0.7f, -0.435f);


        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.0f,0.0f,0.0f);

        glVertex2f(-0.72f, -0.39f);
        glVertex2f(-0.7f, -0.43f);
        glVertex2f(-0.75f, -0.418f);

        glEnd();
        glLoadIdentity();
        glPopMatrix();

        glPopMatrix();

    }
    glPopMatrix();
///night
    if(night || rain)
    {
        glPushMatrix();
        glTranslatef(.17,.05,0);

        glPushMatrix();
        glTranslatef(0.09f,-0.3f, 0.0f);
        glRotatef(180,0,1,0);

        glScalef(.5,.5,0);


        glBegin(GL_POLYGON);

        glColor3f(0.2f,0.1f,0.0f);

        glVertex2f(-0.75f, -0.47f);
        glVertex2f(-0.65f, -0.47f);

        glVertex2f(-0.6f, -0.4f);
        glVertex2f(-0.65f, -0.47f);

        glVertex2f(-0.8f, -0.4f);
        glVertex2f(-0.75f, -0.47f);

        glVertex2f(-0.6f, -0.4f);
        glVertex2f(-0.65f, -0.44f);

        glVertex2f(-0.75f, -0.44f);
        glVertex2f(-0.8f, -0.4f);

        glVertex2f(-0.65f, -0.44f);
        glVertex2f(-0.75f, -0.44f);

        glEnd();

        glBegin(GL_QUADS);

        glColor3f(0.9f,0.5f,0.0f);

        glVertex2f(-0.72f, -0.39f);
        glVertex2f(-0.67f, -0.39f);
        glVertex2f(-0.65f, -0.435f);
        glVertex2f(-0.7f, -0.435f);


        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.0f,0.0f,0.0f);

        glVertex2f(-0.72f, -0.39f);
        glVertex2f(-0.7f, -0.43f);
        glVertex2f(-0.75f, -0.418f);

        glEnd();
        glLoadIdentity();
        glPopMatrix();

        glPopMatrix();

    }
}

//S0076
void draw_big_boat() {

    glPushMatrix();
    glTranslatef(_run10,0,0);

    glPushMatrix();
    glTranslatef(0.01f,-0.3f, 0.0f);
    glScalef(.7f,0.6f,0.0f);

    glBegin(GL_QUADS);
    glColor3f(0.6f,0.29f,0.0f);

    glVertex2f(0.1f, -0.8f);
    glVertex2f(0.5f, -0.8f);
    glVertex2f(0.4f, -0.9f);
    glVertex2f(0.2f, -0.9f);

    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(0.6f, -0.8f);
    glVertex2f(0.5f, -0.8f);
    glVertex2f(0.4f, -0.9f);
    glVertex2f(0.5f, -0.9f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(0.18f, -0.88f);
    glVertex2f(0.2f, -0.9f);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(0.4f, -0.9f);
    glVertex2f(0.42f, -0.88f);

    glColor3f(0.5f,0.5f,0.5f);
    glVertex2f(0.15f, -0.75f);
    glVertex2f(0.45f, -0.75f);
    glVertex2f(0.45f, -0.8f);
    glVertex2f(0.15f, -0.8f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(0.55f, -0.75f);
    glVertex2f(0.45f, -0.75f);
    glVertex2f(0.45f, -0.8f);
    glVertex2f(0.55f, -0.8f);

    glColor3f(0.8f,0.6f,1.0f);
    glVertex2f(0.15f, -0.75f);
    glVertex2f(0.45f, -0.75f);
    glVertex2f(0.4f, -0.7f);
    glVertex2f(0.15f, -0.7f);

    glColor3f(0.6f,0.6f,1.0f);
    glVertex2f(0.55f, -0.75f);
    glVertex2f(0.45f, -0.75f);
    glVertex2f(0.4f, -0.7f);
    glVertex2f(0.5f, -0.7f);
    glEnd();
}

//S0077
void draw_big_boat_pillar() {
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.18f, -0.65f);
    glVertex2f(0.21f, -0.65f);
    glVertex2f(0.21f, -0.7f);
    glVertex2f(0.18f, -0.7f);
    glEnd();

    glColor3f(0.6f,0.29f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.18f, -0.65f);
    glVertex2f(0.21f, -0.65f);
    glVertex2f(0.21f, -0.63f);
    glVertex2f(0.18f, -0.63f);
    glEnd();

    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.28f, -0.65f);
    glVertex2f(0.31f, -0.65f);
    glVertex2f(0.31f, -0.7f);
    glVertex2f(0.28f, -0.7f);
    glEnd();

    glColor3f(0.6f,0.29f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.28f, -0.65f);
    glVertex2f(0.31f, -0.65f);
    glVertex2f(0.31f, -0.63f);
    glVertex2f(0.28f, -0.63f);
    glEnd();

    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.38f, -0.65f);
    glVertex2f(0.41f, -0.65f);
    glVertex2f(0.41f, -0.7f);
    glVertex2f(0.38f, -0.7f);
    glEnd();

    glColor3f(0.6f,0.29f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.38f, -0.65f);
    glVertex2f(0.41f, -0.65f);
    glVertex2f(0.41f, -0.63f);
    glVertex2f(0.38f, -0.63f);
    glEnd();
}

//S0078
void draw_big_boat_design() {

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glBegin(GL_QUADS);
    glVertex2f(0.16f, -0.79f);
    glVertex2f(0.19f, -0.79f);
    glVertex2f(0.19f, -0.76f);
    glVertex2f(0.16f, -0.76f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.05f, 0.0f, 0.0f);
    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glBegin(GL_QUADS);
    glVertex2f(0.16f, -0.79f);
    glVertex2f(0.19f, -0.79f);
    glVertex2f(0.19f, -0.76f);
    glVertex2f(0.16f, -0.76f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1f, 0.0f, 0.0f);
    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glBegin(GL_QUADS);
    glVertex2f(0.16f, -0.79f);
    glVertex2f(0.19f, -0.79f);
    glVertex2f(0.19f, -0.76f);
    glVertex2f(0.16f, -0.76f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.0f);
    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glBegin(GL_QUADS);
    glVertex2f(0.16f, -0.79f);
    glVertex2f(0.19f, -0.79f);
    glVertex2f(0.19f, -0.76f);
    glVertex2f(0.16f, -0.76f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2f, 0.0f, 0.0f);
    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glBegin(GL_QUADS);
    glVertex2f(0.16f, -0.79f);
    glVertex2f(0.19f, -0.79f);
    glVertex2f(0.19f, -0.76f);
    glVertex2f(0.16f, -0.76f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.25f, 0.0f, 0.0f);
    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glBegin(GL_QUADS);
    glVertex2f(0.16f, -0.79f);
    glVertex2f(0.19f, -0.79f);
    glVertex2f(0.19f, -0.76f);
    glVertex2f(0.16f, -0.76f);
    glEnd();

    glPopMatrix();

    glColor3f(0.6f,0.8f,1.0f);
    if (night)
    {
        glColor3f(0.9f,0.9f,0.0f);
    }
    glBegin(GL_QUADS);
    glVertex2f(0.46f, -0.79f);
    glVertex2f(0.54f, -0.79f);
    glVertex2f(0.54f, -0.76f);
    glVertex2f(0.46f, -0.76f);
    glEnd();


    GLfloat xboat1=0.175f;
    GLfloat yboat1=-0.84f;
    GLfloat radiusb =.015f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xboat1, yboat1);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xboat1 + (radiusb * cos(i *  twicePi / triangleAmount)),
                    yboat1 + (radiusb * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glPushMatrix();
    glTranslatef(0.045f, 0.0f, 0.0f);
    GLfloat xboat2=0.18f;
    GLfloat yboat2=-0.84f;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xboat2, yboat2);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xboat2 + (radiusb * cos(i *  twicePi / triangleAmount)),
                    yboat2 + (radiusb * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.095f, 0.0f, 0.0f);
    GLfloat xboat3=0.18f;
    GLfloat yboat3=-0.84f;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xboat3, yboat3);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xboat3 + (radiusb * cos(i *  twicePi / triangleAmount)),
                    yboat3 + (radiusb * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.145f, 0.0f, 0.0f);
    GLfloat xboat4=0.18f;
    GLfloat yboat4=-0.84f;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xboat4, yboat4);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xboat4 + (radiusb * cos(i *  twicePi / triangleAmount)),
                    yboat4 + (radiusb * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.195f, 0.0f, 0.0f);
    GLfloat xboat5=0.18f;
    GLfloat yboat5=-0.84f;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xboat5, yboat5);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xboat5 + (radiusb * cos(i *  twicePi / triangleAmount)),
                    yboat5 + (radiusb * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.245f, 0.0f, 0.0f);
    GLfloat xboat6=0.18f;
    GLfloat yboat6=-0.84f;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xboat6, yboat6);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( xboat6 + (radiusb * cos(i *  twicePi / triangleAmount)),
                    yboat6 + (radiusb * sin(i * twicePi / triangleAmount)) );
    }
    glEnd();

    glPopMatrix();
    glLoadIdentity();

    glPopMatrix();

    glPopMatrix();


    glColor3f(0.0,0.0,0.0);
    renderBitmapString(0.52f, 0.5f, 0.0f, GLUT_BITMAP_9_BY_15, "Hospital");

    glColor3f(0.0,0.0,0.0);
    renderBitmapString(0.811f, 0.3f, 0.0f, GLUT_BITMAP_HELVETICA_10, "Inani");


    glColor3f(0.6,0.0,0.0);
    renderBitmapString(-0.85f, 0.4f, 0.0f, GLUT_BITMAP_HELVETICA_10, "La Bella Resort");

    glColor3f(1.0,1.0,1.0);
    renderBitmapString(-0.59f, 0.61f, 0.0f, GLUT_BITMAP_HELVETICA_10, "Neeshorgo Hotel");

    glColor3f(0.0,0.2,0.0);
    renderBitmapString(0.11f, 0.375f, 0.0f, GLUT_BITMAP_HELVETICA_10, "Mermaid Cafe Marine Drive");

    glFlush();  // Render now

  }

void update(int value)
{

    if(on)
    {
        if(!vehicleStop)
        {
            _run7 -= vehicleSpeed;
            if(!handup)
            {
                _run4 += vehicleSpeed;
            }
            _run5 += (vehicleSpeed+.001);
            _run6 -= (vehicleSpeed+.001);
            _run3 -= (vehicleSpeed+.002);
        }
        _run1 += .0003f;
        _run2 -= .0002f;
        _run10 += .0009f;


        if(waveUp)
        {
            c1 += .00042;
            c2 += .00042;
            c3 += .00042;
            c4 += .00042;
            c5 += .00042;
            c6 += .00042;
            c7 += .00042;
            c8 += .00042;
            c9 += .00042;
            c10 += .00042;
            c11 += .00042;
            c12 += .00042;
            c13 += .00042;
            c14 += .00042;
            c15 += .00042;
            c16 += .00042;
            c17 += .00042;
        }
        else
        {
            c1 -= .00032;
            c2 -= .00032;
            c3 -= .00032;
            c4 -= .00032;
            c5 -= .00032;
            c6 -= .00032;
            c7 -= .00032;
            c8 -= .00032;
            c9 -= .00032;
            c10 -= .00032;
            c11 -= .00032;
            c12 -= .00032;
            c13 -= .00032;
            c14 -= .00032;
            c15 -= .00032;
            c16 -= .00032;
            c17 -= .00032;
        }

        if(!night && !rain)
        {
            _run8 -= .0005f;
            _run9 += .0005f;
            //_run10 += .0009f;
        }
    }

    if (_run7 < -1.7)
    {
        _run7 = .5;//float _rain2y = 0.0;
    }
    if (_run5 > 1)
    {
        _run5 = -1.2;
    }
    if (_run6 < -1)
    {
        _run6 = 1.2;
    }
    if (_run4 > 1.7)
    {
        _run4 = -.6;
    }
    if (_run3 < -1.7)
    {
        _run3 = 1.3;
    }
    if (_run1 > 1.7)
    {
        _run1= -.6;
    }
    if (_run2 < -1.9)
    {
        _run2= .35;
    }
    if (_run8 < -.8)
    {
        _run8= 1.5;
    }
    if (_run9 > .8)
    {
        _run9= -1.5;
    }
    if (_run10 > .9)
    {
        _run10= -1.5;
    }


    if (c1 >= .08)
    {
        waveUp = false;
    }
    if (c1 < 0.0)
    {
        waveUp = true;
    }
    if (c2 >= .08)
    {
        waveUp = false;
    }
    if (c2 < 0.0)
    {
        waveUp = true;
    }
    if (c3 >= .08)
    {
        waveUp = false;
    }
    if (c3 < 0.0)
    {
        waveUp = true;
    }
    if (c4 >= .08)
    {
        waveUp = false;
    }
    if (c4 < 0.0)
    {
        waveUp = true;
    }
    if (c5 >= .08)
    {
        waveUp = false;
    }
    if (c5 < 0.0)
    {
        waveUp = true;
    }

    if (c6 >= .08)
    {
        waveUp = false;
    }
    if (c6 < 0.0)
    {
        waveUp = true;
    }
    if (c7 >= .08)
    {
        waveUp = false;
    }
    if (c7 < 0.0)
    {
        waveUp = true;
    }
    if (c8 >= .08)
    {
        waveUp = false;
    }
    if (c8 < 0.0)
    {
        waveUp = true;
    }
    if (c9 >= .08)
    {
        waveUp = false;
    }
    if (c9 < 0.0)
    {
        waveUp = true;
    }
    if (c10 >= .08)
    {
        waveUp = false;
    }
    if (c10 < 0.0)
    {
        waveUp = true;
    }

    if (c11 >= .08)
    {
        waveUp = false;
    }
    if (c11 < 0.0)
    {
        waveUp = true;
    }
    if (c12 >= .08)
    {
        waveUp = false;
    }
    if (c12 < 0.0)
    {
        waveUp = true;
    }
    if (c13 >= .08)
    {
        waveUp = false;
    }
    if (c13 < 0.0)
    {
        waveUp = true;
    }
    if (c14 >= .08)
    {
        waveUp = false;
    }
    if (c14 < 0.0)
    {
        waveUp = true;
    }
    if (c15 >= .08)
    {
        waveUp = false;
    }
    if (c15 < 0.0)
    {
        waveUp = true;
    }
    if (c16 >= .08)
    {
        waveUp = false;
    }
    if (c16 < 0.0)
    {
        waveUp = true;
    }
    if (c17 >= .08)
    {
        waveUp = false;
    }
    if (c17 < 0.0)
    {
        waveUp = true;
    }

       if (rain) {
    updateRainEffect();
    }

    if(on){

    update_bird();


     for (int i = 0; i < 3; i++) {
        if (sharkDirection[i]) {
            sharkX[i] += 0.002f;
            if (sharkX[i] >= 1.2f) sharkDirection[i] = false;
        } else {
            sharkX[i] -= 0.002f;
            if (sharkX[i] <= -1.2f) sharkDirection[i] = true;
        }
    }

    draw_shark();  // Make sure this draws all 3 sharks using a loop inside it
}


    if (airplaneMove) {
    airplaneX += 0.005f;
    if (airplaneX > 1.2f) {
        airplaneX = -1.2f;  // Reset
        //airplaneMove = false; // Stop after one loop (optional)
    }
}

if (balloonMove) {
    balloonY += 0.001f * balloonDirection;

    // Optional: auto-stop when reaching limits
    if (balloonY >= 1.0f) {
        balloonY = 1.0f;
        balloonMove = false;
    } else if (balloonY <= -0.14f) {
        balloonY = -0.14f;
        balloonMove = false;
    }
}
    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

void SpecialInput(int key, int x, int y)
{

    switch(key)
    {
    case GLUT_KEY_UP:
        if(vehicleSpeed >= 0)
        {
            vehicleStop = false;
        }
        vehicleSpeed = vehicleSpeed + 0.001;
        break;

    case GLUT_KEY_DOWN:
        if(vehicleSpeed > 0)
        {
            vehicleSpeed = vehicleSpeed - 0.001;
        }
        if(vehicleSpeed <= 0)
        {
            vehicleStop = true;
        }
        break;
    }
    glutPostRedisplay();
}

void Input(unsigned char key, int x, int y)
{

    switch(key)
    {
    case 'n':
    case 'N':
        night = true;
        break;
    case 'd':
    case 'D':
        night = false;
        break;

    case '1':
        on = (!on);
        airplaneMove =(!airplaneMove);
        for (int i = 0; i < 3; i++) {
        sharkDirection[i] = true;
}
        break;

    case '+':
        glutFullScreen();
        break;

    case '-':
        glutReshapeWindow(1024,576);
        glutInitWindowPosition(50,40);
        break;

    case 'x':
        exit(0);
        break;

    case 'r':
    case 'R':
        rain = (!rain);
        break;

    glutPostRedisplay();

    }

}

  void handleMouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            handup = !handup;
        } else if (button == GLUT_RIGHT_BUTTON) {
            birdDirection *= -1.0f;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            balloonMove = true;
            balloonDirection *= -1;
        }
    }
    glutPostRedisplay();
}

void display() {
    clearColor(0.0f, 0.7f, 0.4f, 1.0f);
    if(night) {
        clearColor(0.07f, 0.30f, 0.15f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT);

    draw_sky_();
    draw_sun();
    draw_moon();
    draw_star();

    draw_hill3();
    draw_hill01();
    draw_hill5();
    draw_hill4();
    draw_hill6();
    draw_hill2();

    draw_cloud1();//left
    draw_cloud2();//right

    draw_rail_line1();//Full
    draw_train();

    draw_tree1();//right side
    draw_tree2();//right side
    draw_tree3();//right side
    draw_tree4();//right side
    draw_tree5();//right side

    draw_house1();//left1
    draw_house1_windos1();
    draw_house1_windos2();
    draw_house1_door();
    draw_tree2_copy();//tree and font side of house1

    draw_house2();//left2
    draw_car1();//parking beside house2

    draw_house3_royal();
    draw_royal_windows();//left uper4
    draw_royal_windows2();//left lower4
    draw_copyrw1();//right uper4
    draw_copyrw2();
    draw_copyrw3();
    draw_copyrw4();

    draw_copyrw5();


    draw_marmad();
    draw_again1_car1_copy1();//1st car
    draw_again_car1_copy1();//2nd car
    draw_car1_copy1();//3rd car

    draw_hospital();

    draw_tree_01_copy1();//Irani right

    draw_1st_roof();//irani house

    draw_tree011();//left 3rd
    draw_tree022();//left 1st
    draw_tree022_copy();

    draw_wake_way();//both side walking way

    draw_lamp1();
    draw_lamp2();
    draw_lamp3();
    draw_lamp4();
    draw_lamp5();
    draw_lamp6();
    draw_lamp7();
    draw_lamp8();
    draw_lamp9();
    draw_lamp10();

    draw_police();//police and table

    draw_road2();//Main road

    draw_bus011();//2wheel 1 back light
    draw_car4_body();//green
    draw_car3_body();//blue
    draw_bus02();

    draw_lamp11();
    draw_lamp12();
    draw_lamp13();
    draw_lamp14();
    draw_lamp15();
    draw_lamp16();
    draw_lamp17();
    draw_lamp19();
    draw_lamp20();
    draw_lamp21();

    draw_sea();
    draw_c();

    draw_small_boat();
    draw_small_boat2();
    draw_big_boat();
    draw_big_boat_pillar();
    draw_big_boat_design();

    if (rain) {
    drawDrops();
    drawBubbles();
              }


     float birdSpacingX = 0.06f;   // reduced horizontal gap
     float birdSpacingY = 0.08f;   // reduced vertical gap
     float birdSize[] = {0.002f, 0.0018f, 0.0016f, 0.0014f, 0.0012f};  // size decrease by depth

     for (int i = 0; i < 4; ++i) {
     float offsetX = birdX - i * birdSpacingX;
     float offsetY = birdY - i * birdSpacingY;
     float scale   = birdSize[i];
     float wingY   = (i % 2 == 0) ? birdWingY : -birdWingY;

    draw_bird(offsetX, offsetY, birdDirection, wingY, scale);
    }

    draw_airplane();
    draw_shark();
    draw_balloon();


    glutSwapBuffers();
    glFlush();
}

void init()

    {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Allow transparency
    glClearColor(0.0, 0.0, 0.0, 0.0); // Optional: fully transparent background
    loadTexture("C:/Users/ASUS/OneDrive - American International University-Bangladesh/Documents/Graphics/Try/Airplane.png", airplaneTexture);
    loadTexture("C:/Users/ASUS/OneDrive - American International University-Bangladesh/Documents/Graphics/Try/Shark.png", sharkTexture);
    loadTexture("C:/Users/ASUS/OneDrive - American International University-Bangladesh/Documents/Graphics/Try/Balloon.png", balloonTexture);


}

int main(int argc, char** argv)
{
    glutInitWindowSize(1024,576);
    glutInit(&argc, argv);
    glutCreateWindow("Graphics Final Project");
    glutInitWindowPosition(50,40);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(SpecialInput);
    glutKeyboardFunc(Input);
    glutMouseFunc(handleMouse);
    glutTimerFunc(25,update,0);
    glutMainLoop();

    return 0;
}
