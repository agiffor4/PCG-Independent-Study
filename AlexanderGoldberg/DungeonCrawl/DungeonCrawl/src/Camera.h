#pragma once
#include"Vector2.h"
class World;

static World* s_world = nullptr;
class Camera
{
protected:
	static Vector2& getstaticVector();
	static Vector2& getCenter();
	static Vector2& getScreenSize();
	static Camera& instance();
	
public:
	Camera();
	~Camera();
	static void SetDimensions(int _width, int _height);
	static void SetOffset(const Vector2 _offset);
	static void SetCenter(const Vector2 _offset);
	static const Vector2& GetScreenSize();
	static const Vector2& GetCenter();
	static const Vector2& Offset();
	static Vector2 ClampMovement(Vector2 _direction);
	static void SetWorld(World* _world);

};

