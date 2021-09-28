cmake_minimum_required(VERSION 3.20)
project(RT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-pthread")

add_executable(RT raytracer.cpp Color.cpp Color.h DataGrid.cpp DataGrid.h Helpers.cpp Helpers.h Ray.cpp Ray.h Point.cpp Point.h Objects/Object.cpp Objects/Object.h Objects/Sphere.cpp Objects/Sphere.h Camera.cpp Camera.h Material.cpp Material.h Scene.cpp Scene.h Objects/Plane.cpp Objects/Plane.h Objects/Triangle.cpp Objects/Triangle.h UVPoint.cpp UVPoint.h Lights/Light.cpp Lights/Light.h Lights/SunLight.cpp Lights/SunLight.h Lights/PointLight.cpp Lights/PointLight.h Objects/AABox.cpp Objects/AABox.h Lights/BoxLight.cpp Lights/BoxLight.h Random.cpp Random.h Bounding/BoundingBox.cpp Bounding/BoundingBox.h Bounding/Node.cpp Bounding/Node.h SceneMaker.h)
