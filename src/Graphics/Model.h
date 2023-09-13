#pragma once

#include "Direct3D11\Direct3D11Drawable.h"

class Model : public Direct3D11Drawable
{
public:
	Model(Direct3D11Renderer& pd3dRenderer, std::shared_ptr<Mesh>& mesh, std::shared_ptr<Material>& material);

	DirectX::XMMATRIX GetModelMatrix() override;
	
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
private:
	float pos_x;
	float pos_y;
	float pos_z;

	float rot_x;
	float rot_y;
	float rot_z;

	float scl_x;
	float scl_y;
	float scl_z;
};