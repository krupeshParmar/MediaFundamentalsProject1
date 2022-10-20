///*
//==================================================================================================================================================================
//|Copyright 2018 Oscar Lara	- scarlara@hotmail.com																												   |
//|																																								   |
//|See individual libraries separate legal notices																												   |
//|																																								   |
//|Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"),				   |
//|to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,				   |
//|and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :					   |
//|																																								   |
//|The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.									   |
//|																																								   |
//|THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			   |
//|FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,	   |
//|WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.		   |
//==================================================================================================================================================================
//*/
//
#include "camera.h"


Camera::Camera()
{
	cam_near = 0.1f;
	cam_far = 100.0f;
	field_of_view = _math.convert_to_radians(67.0f);
	window_width = 800;
	window_height = 600;
	aspect_ratio = static_cast<float>(window_width / window_height);
	_range = static_cast<float>(tan(field_of_view * 0.5f) * cam_near);
	position = vec3(0.0f, 0.0f, 8.0f);
	speed = 1.0f;

}

Camera::~Camera()
{
}

mat4 Camera::get_projection_matrix()
{
	_sx = (2.0f * cam_near) / (_range * aspect_ratio + _range * aspect_ratio);
	_sy = cam_near / _range;
	_sz = -(cam_far + cam_near) / (cam_far - cam_near);
	_pz = -(2.0f * cam_far * cam_near) / ( cam_far - cam_near );

	mat4 projection = mat4(_sx, 0.0f, 0.0f, 0.0f, 0.0f, _sy, 0.0f, 0.0f, 0.0f, 0.0f, _sz, -1.0f, 0.0f, 0.0f, _pz, 0.0f);

	return projection;
}

mat4 Camera::get_view_matrix()
{
	mat4 translate = _math.translate(mat4().get_identity(), vec3(-position.v[0], -position.v[1], -position.v[2]));

	return translate;
}
