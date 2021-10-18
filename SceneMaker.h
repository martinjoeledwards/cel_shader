//
// Created by marti on 27-Sep-21.
//

#ifndef RT_SCENEMAKER_H
#define RT_SCENEMAKER_H

class SceneMaker {
public:

    Camera* camera1(){

        double aspect_ratio = 16.0 / 9.0;

        int sampleSubdiv = 1;
        int samples = 512;
        int indirect_samples = 4;
        bool rayJitter = true;

        auto x_dim = 640u;       //default 320
        auto y_dim = x_dim / aspect_ratio; //240p
        double fov = 140.0;

        int num_bounces = 4;
        int shadowSamples = 8;


        Point cam_from(0, 1, 4);
        Point cam_at(0, 0, 0);
        Point cam_up(0, 1, 0);

        auto myCamera = new Camera(x_dim, aspect_ratio, fov, cam_from, cam_at, cam_up);
        myCamera->setRayJitter(rayJitter);
        myCamera->setSampleSubdiv(sampleSubdiv);
        myCamera->setSamples(samples);
        myCamera->setNumBounces(num_bounces);
        myCamera->setShadowSamples(shadowSamples);
        myCamera->setIndirectSamples(indirect_samples);

        return myCamera;

    }

    Camera* camera2(){

        double aspect_ratio = 16.0 / 9.0;

        int sampleSubdiv = 1;
        int samples = 1024;
        int indirect_samples = 8;
        bool rayJitter = true;

        auto x_dim = 640u;       //default 320
        auto y_dim = x_dim / aspect_ratio; //240p
        double fov = 140.0;

        int num_bounces = 4;
        int shadowSamples = 4;


        Point cam_from(0, .5, 4);
        Point cam_at(0, 0, 0);
        Point cam_up(0, 1, 0);

        auto myCamera = new Camera(x_dim, aspect_ratio, fov, cam_from, cam_at, cam_up);
        myCamera->setRayJitter(rayJitter);
        myCamera->setSampleSubdiv(sampleSubdiv);
        myCamera->setSamples(samples);
        myCamera->setNumBounces(num_bounces);
        myCamera->setShadowSamples(shadowSamples);
        myCamera->setIndirectSamples(indirect_samples);

        return myCamera;
    }

    Scene* scene1(){
//    create objects
        jupiter->setTexture("Textures/jupiter.ppm");
        earth->setTexture("Textures/earth.ppm");
        auto* white_plane = new Plane(white_mat, Point(0, 1, 0), -.2);
        auto* green_plane = new Plane(green_mat, Point(1, 0, .2), -4);
        auto* blue_plane = new Plane(blue_mat, Point(-1, 0, .2), -4);
        auto* red_plane = new Plane(red_mat, Point(0, 0, 1), -.4);
        auto earth_sphere = new Sphere(earth, Point(0, .4, 1), .6);
        auto tran_sphere = new Sphere(refractive_smooth, Point(1.2, .2, 1.2), .4);
        auto refl_sphere = new Sphere(reflect_smooth, Point(-1.2, .2, 1.2), .4);

//    create lights
        auto boxLight = new BoxLight(Color(.7), Point(3, 2, 4), Point(3.4, 2.4, 4.4));

//        create scene and set rendering values
        auto global_light = Color(.3);
        auto myScene = new Scene(global_light);

//          Add objects

        myScene->AddObjects(white_plane);
        myScene->AddObjects(green_plane);
        myScene->AddObjects(red_plane);
        myScene->AddObjects(blue_plane);
        myScene->AddObjects(earth_sphere);
        myScene->AddObjects(tran_sphere);
        myScene->AddObjects(refl_sphere);

 //         Add lights
        myScene->AddLights(boxLight);

        myScene->SetCamera(camera1());

//        initialize bounding box hierarchy
//        myScene->initialize();

        return myScene;
    }

    Scene* scene2(){
//    create objects
        jupiter->setTexture("Textures/jupiter.ppm");
        earth_ref->setTexture("Textures/2k_earth.ppm");
        auto refl_plane = new Plane(reflect_rough, Point(.4, 1, .3), -2);
        auto jupiter_sphere = new Sphere(jupiter, Point(0, 0, -5), 4);
        auto earth_sphere = new Sphere(earth_ref, Point(1, -.8, .4), .6);
        auto moon = new Sphere(moon_mat, Point(1, 1, -.5), .2);
        auto moon2 = new Sphere(blue_mat, Point(-.2, .6, -.3), .25);
        auto moon3 = new Sphere(red_mat, Point(4.4, .2, -3.6), .3);

//    create lights
        auto boxLight = new BoxLight(Color(.9), Point(5, 3, 5), Point(6, 4, 6));

//        create scene and set rendering values
        auto global_light = Color(.1);
        auto myScene = new Scene(global_light);

//          Add objects

        myScene->AddObjects(refl_plane);
        myScene->AddObjects(jupiter_sphere);
        myScene->AddObjects(earth_sphere);
        myScene->AddObjects(moon);
        myScene->AddObjects(moon2);
        myScene->AddObjects(moon3);

        //         Add lights
        myScene->AddLights(boxLight);

        myScene->SetCamera(camera2());

//        initialize bounding box hierarchy
//        myScene->initialize();

        return myScene;
    }

    Scene* scene3(){
        int num_bounces = 3;        //TODO: move these two to the camera class? And give scene a camera member?
        int shadowSamples = 32;


//    create objects
        earth->setTexture("Textures/2k_earth.ppm");
        jupiter->setTexture("Textures/jupiter.ppm");

        auto* refrSphere = new Sphere(earth, Point(-1, .2, .3), 1);

        Point p0(0, 0, 0);
        Point p1(3, 0, 0);
        Point p2(1.5, .6, -2);
        auto* tri = new Triangle(jupiter, p0, p1, p2);
        auto* diff_plane = new Plane(reflect_rough, Point(0, 1, 0), -.8);

//    create lights
        auto boxLight = new BoxLight(*white, Point(.8, 1.4, 0), Point(1, 1.6, .2));
        auto sunLight = new SunLight(*white, Point(.1, .3, 1));

//        create scene and set rendering values
        auto* myScene = new Scene();
//        myScene->setNumBounces(num_bounces);
//        myScene->setShadowSamples(shadowSamples);

// Add objects

        myScene->AddObjects(diff_plane);
        myScene->AddObjects(refrSphere);
        myScene->AddObjects(tri);

        // Add lights
        myScene->AddLights(boxLight);
        myScene->AddLights(sunLight);

        myScene->initialize();

        return myScene;
    }

    Scene* scene4(){
        int num_bounces = 3;        //TODO: move these two to the camera class? And give scene a camera member?
        int shadowSamples = 32;


//    create objects
        earth->setTexture("Textures/2k_earth.ppm");
        jupiter->setTexture("Textures/jupiter.ppm");

        auto* refrSphere = new Sphere(jupiter, Point(-1, .2, .3), 1);

        Point p0(0, 0, 0);
        Point p1(3, 0, 0);
        Point p2(1.5, .6, -2);
        auto* tri = new Triangle(earth, p0, p1, p2);
        auto* diff_plane = new Plane(reflect_rough, Point(0, 1, 0), -.8);

//    create lights
        auto boxLight = new BoxLight(*white, Point(.8, 1.4, 0), Point(1, 1.6, .2));
        auto sunLight = new SunLight(*white, Point(.1, .3, 1));

//        create scene and set rendering values
        auto* myScene = new Scene();
//        myScene->setNumBounces(num_bounces);
//        myScene->setShadowSamples(shadowSamples);

// Add objects

        myScene->AddObjects(diff_plane);
        myScene->AddObjects(refrSphere);
        myScene->AddObjects(tri);

        // Add lights
        myScene->AddLights(boxLight);
        myScene->AddLights(sunLight);

        myScene->initialize();

        return myScene;
    }

private:
    //    create colors
    Color* red = new Color(1, 0, 0);
    Color* green = new Color(0, 1, 0);
    Color* blue = new Color(0, 0, 1);
    Color* white = new Color(1, 1, 1);
    Color* superWhite = new Color(5, 5, 5);
    Color* moon_col = new Color(.5, .5, .6);

    //    create materials
    Material* reflect_smooth = new Material(*white, *white, .3, .7, 0, 0, 0.0, 0, 10, 0);
    Material* reflect_rough = new Material(*white, *white, .2, .6, 0, 0, 0.05, 0, 10, 0);
    Material* red_mat = new Material(*red, *white, 1, 0, 0, 0, 0, 0, 6, 0);
    Material* green_mat = new Material(*green, *white, 1, 0, 0, 0, 0, 0, 6, 0);
    Material* blue_mat = new Material(*blue, *white, 1, 0, 0, 0, 0, 0, 6, 1.2);
    Material* white_mat = new Material(*white, *white, 1, 0, 0, 0, 0, 0, 6, 0);
    Material* refractive_smooth = new Material(*white, *white, .005, .2, .7, 0, 0, 0, 16, 1.3);
    Material* refractive_rough = new Material(*white, *white, .005, .1, .8, .2, 0.09, .09, 8, 1.2);

    Material* earth = new Material(*white, *white, .8, 0, 0, .1, 0.0, .0, 5, 1);
    Material* earth_ref = new Material(*white, *white, .5, .5, 0, 0, 0.1, .0, 5, 1);
    Material* jupiter = new Material(*white, *white, 1, 0, 0, 0, 0.0, .0, 5, 1);
    Material* moon_mat = new Material(*moon_col, *white, 1, 0, 0, .2, 0.0, 0.0, 5, 1);
};






#endif //RT_SCENEMAKER_H


