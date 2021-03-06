/*-----------------------------------------------------------------------------+
| File name: av_system.h													   |
| Date:		 13th March 2006				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Algorith Visualization System.     										   |
| It Provides classes, methods, for easy representing way of work of           |
| algorithms. It is a layer between the CAlgorithm class and the graphics      |
| framework...                                                                 |
+-----------------------------------------------------------------------------*/

#ifndef AV_SYSTEM_H
	#define AV_SYSTEM_H

#include <typeinfo>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "clog.h"
#include "tg_math.h"
#include "gl_shapes.h"
#include "irendersystem.h"

// the CAVSystem class --------------------------------------------------------+

// this class provides a basic functionality of drawing chart shapes
class CAVSystem : public IRenderSystem {
public:
	CAVSystem(const CLog& logger);
	~CAVSystem();

	void SetDiagramBlockInfo(BlockType bType, const VECTOR3D &vCol, const VECTOR3D &vColMarked, 
		                     const VECTOR3D &vColHighlighted) override;
	void BeginDrawing(double fMaxData, int nCount) override;
	void DrawDiagramBlock(double fValue, ColorType cType) override;
	void EndDrawing() override;
	void SetMaxSize(float fWidth, float fHeight, float fDepth) override { m_vMaxSize = VECTOR3D(fWidth, fHeight, fDepth); }
	void SetOutlook(const VECTOR3D &vFrameCol, unsigned int iFrameTex) override;
	void SetBlockType(BlockType bType) override { m_bType = bType; }

	// #refactor: some variables have getters/setters some not...
	bool m_bHoriz;
	bool m_bFrame;
	bool m_bTextured;

private:
	BlockType m_bType;
	VECTOR3D m_vCol;
	VECTOR3D m_vColMarked;
	VECTOR3D m_vColHighlighted;
	GLuint m_iTex;
	bool m_bDrawing;
	VECTOR3D m_vFrameCol;
	VECTOR3D m_vMaxSize;
	VECTOR3D m_vSizeAsp;
	int m_iLod;

	const CLog& m_logger;
};

#endif // AV_SYSTEM_HPP

// end of file ----------------------------------------------------------------+