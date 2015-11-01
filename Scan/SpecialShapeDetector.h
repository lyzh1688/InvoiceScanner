#pragma once
#include "BaseDetector.h"
#include "SpecialShape.h"
#include "ShapeMap.h"

class SpecialShapeDetector:public BaseDetector<SpecialShape>
{
public:
	SpecialShapeDetector(void){};
	~SpecialShapeDetector(void){};

	SpecialShape FindShape(const IplImage* img,ShapeType shapeType);
	void DrawShape(IplImage* img,const SpecialShape& shapes);

};

