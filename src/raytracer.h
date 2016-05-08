#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "vector.h"
#include <memory>

class Camera;
class RenderConfiguration;
class Scene;

class RayTracer
{
public:
	void render(RenderConfiguration &configuration, Camera &camera, Scene &scene);
	void render_hotspots(RenderConfiguration &configuration, Camera &camera, Scene &scene);
};

#endif