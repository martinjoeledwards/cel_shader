cmake_minimum_required(VERSION 3.20)
project(RT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-pthread")

add_executable(RT raytracer.cpp Color.cpp Color.h DataGrid.cpp DataGrid.h Helpers.cpp Helpers.h Ray.cpp Ray.h Point.cpp Point.h Object.cpp Object.h Sphere.cpp Sphere.h Camera.cpp Camera.h Material.cpp Material.h Scene.cpp Scene.h Plane.cpp Plane.h Triangle.cpp Triangle.h UVPoint.cpp UVPoint.h Light.cpp Light.h SunLight.cpp SunLight.h PointLight.cpp PointLight.h)
