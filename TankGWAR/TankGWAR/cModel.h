#pragma once

#include "D3DObject.h"
#include "Timer.h"



class cModel
{
public:
	cModel(void);
	~cModel(void);
	void Paint(void);
	void SetXYZ(float x, float y, float z) { m_xPos = x; m_yPos=y; m_zPos = z; }
private:
    void MakeWorldMatrix(int x);

	LPD3DXMESH m_tank;
    DWORD m_nMat;
    LPDIRECT3DTEXTURE9* m_tanktex;
    D3DMATERIAL9* m_tankmat;
	CTimer m_time, m_time2;

	float m_xPos;
	float m_yPos;
	float m_zPos;
};
