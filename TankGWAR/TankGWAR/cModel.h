#pragma once

#include "D3DObject.h"
#include "Timer.h"



class cModel
{
public:
	cModel(float x=0, float y=0, float z=0);
	~cModel(void);
	void Paint(void);
	void SetXYZ(float x, float y, float z) { m_xPos = x; m_yPos=y; m_zPos = z; }
private:
    void MakeWorldMatrix(int x);
	void _LoadGraphics(void);
	void _UnloadGraphics();

	static LPD3DXMESH			m_tank;
    static DWORD				m_nMat;
    static LPDIRECT3DTEXTURE9*	m_tanktex;
    static D3DMATERIAL9*		m_tankmat;
	static int					m_graph_init;

	CTimer						m_time, m_time2;

	float						m_xPos, m_yPos, m_zPos;

};
