#include "Camera.h"
#include "World.h"
Camera::Camera() 
{

}
Camera::~Camera() 
{

}

void Camera::SetDimensions(int _width, int _height)
{
	getScreenSize() = Vector2(_width, _height);

}

void Camera::SetOffset(const Vector2 _offset)
{
	getstaticVector() = _offset;
}

void Camera::SetCenter(const Vector2 _offset)
{
	getCenter() = _offset;
}



const Vector2& Camera::GetScreenSize()
{
	return getScreenSize();
}

const Vector2& Camera::GetCenter()
{
	return getCenter();
}

const Vector2& Camera::Offset()
{
	
	return getstaticVector();
}

Vector2 Camera::ClampMovement(const Vector2 _direction)
{
	if (s_world == nullptr)
		return _direction;
	return s_world->CheckIfCameraShouldMove(_direction);
}

void Camera::SetWorld(World* _world)
{
	s_world = _world;
}


Vector2& Camera::getstaticVector()
{

	static Vector2 s_myPosition;
	return s_myPosition;
}

Vector2& Camera::getCenter()
{
	static Vector2 s_center;
	return s_center;
}

Vector2& Camera::getScreenSize()
{
	static Vector2 s_size;
	return s_size;
}

Camera& Camera::instance()
{
	static Camera c;
	return c;
}


