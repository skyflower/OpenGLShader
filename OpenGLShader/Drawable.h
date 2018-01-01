#pragma once
class CDrawable
{
public:
	enum AttribType
	{
		VERTEXATTRIB,
		ATTRIBPOINTER
	};

	CDrawable();
	virtual ~CDrawable();
	virtual void Draw() = 0;
	virtual void Update(float duration) = 0;
protected:
	
};

