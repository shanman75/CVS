/***************************************************************

d3dmat.h

Header file for d3dmat.cpp

Last updated: march 29,2004
***************************************************************/

#include <d3d9.h>
#include <d3dx9.h>

void SetWorldMatrix(float);
void SetViewMatrix(float,float,bool);
void SetProjectionMatrix();
void SetAmbientLighting();
void setupd3d();
BOOL LoadTextures();
void LoadSB();
BOOL PageFlip();
void DrawBackground();
BOOL ComposeFrame();
void ProcessFrame();

