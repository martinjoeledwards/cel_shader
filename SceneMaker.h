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
        int samples = 64;
        int indirect_samples = 8;
        bool rayJitter = false;

        auto x_dim = 320u;       //default 320
        auto y_dim = x_dim / aspect_ratio; //240p
        double fov = 140.0;

        int num_bounces = 4;
        int shadowSamples = 8;


        Point cam_from(1.5, .6, 4);
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
        bool rayJitter = false;

        auto x_dim = 320u;       //default 320
        auto y_dim = x_dim / aspect_ratio; //240p
        double fov = 140.0;


        Point cam_from(1.5, 2, 4);
        Point cam_at(0, 0, 0);
        Point cam_up(0, 1, 0);

        auto myCamera = new Camera(x_dim, aspect_ratio, fov, cam_from, cam_at, cam_up);
        myCamera->setRayJitter(rayJitter);
        myCamera->setSampleSubdiv(sampleSubdiv);

        return myCamera;
    }

    Scene* scene1(){
//    create objects
        jupiter->setTexture("Textures/jupiter.ppm");
        auto* jupiter_sphere = new Sphere(jupiter, Point(1.7, 1, .6), 1);
        auto* redSphere = new Sphere(white_mat, Point(0, 1, -.5), .9);
        auto* diff_plane = new Plane(red_solid, Point(0, 1, 0), -.1);

//    create lights
        auto sunlight = new SunLight(Color(.4), Point(-1, 1, 1));
        auto boxLight = new BoxLight(*white, Point(-2, 1, 0), Point(-2.4, 1.4, 0.4));
        auto pointLight = new PointLight(*white, Point(-.4, 0, 0));

//        create scene and set rendering values
        auto global_light = Color(.3);
        auto myScene = new Scene(global_light);

// Add objects

        myScene->AddObjects(diff_plane);
        myScene->AddObjects(redSphere);
        myScene->AddObjects(jupiter_sphere);

        // Add lights
//        myScene->AddLights(sunlight);
//        myScene->AddLights(pointLight);
        myScene->AddLights(boxLight);

        myScene->SetCamera(camera1());

//        initialize bounding box hierarchy
//        myScene->initialize();

        return myScene;
    }

    Scene* scene2(){
        int num_bounces = 2;        //TODO: move these two to the camera class? And give scene a camera member?
        int shadowSamples = 4;


//    create objects
        earth->setTexture("Textures/2k_earth.ppm");

        auto* refrSphere = new Sphere(earth, Point(-2, .2, 1), 1);
        auto* refrSphereR = new Sphere(refractive_rough, Point(.51, 0.3, -0.6), .4);
        auto* blueSphere = new Sphere(reflect_smooth, Point(-.5, 0, .6), .3);
        auto* reflSphereRough = new Sphere(reflect_rough, Point(0, 0, .6), .2);
        auto* greenSphere = new Sphere(green_mat, Point(.6, 0, 0), .3);

        auto* blueBox = new AABox(blue_mat, Point(.2, .2, .2), Point(0, 0, 0));
        auto* redBox = new AABox(red_solid, Point(0, 0, -.1), Point(.2, .2, -.03));
        auto* redBoxLong = new AABox(red_solid, Point(-2, 0.1, -1), Point(2, .2, -.9));

        Point p0(100, -.4, -100);
        Point p1(-100, -.4, -100);
        Point p2(0, -.4, 100);
        auto* triangle = new Triangle(green_mat, p0, p2, p1);

        auto* reflect_plane = new Plane(reflect_smooth, Point(1, 1, .3), -.4);
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
//        myScene->AddObjects(refrSphereR);
        myScene->AddObjects(refrSphere);
//        myScene->AddObjects(blueSphere);
//        myScene->AddObjects(greenSphere);
//        myScene->AddObjects(reflSphereRough);
//        myScene->AddObjects(blueBox);
//        myScene->AddObjects(redBox);
//        myScene->AddObjects(redBoxLong);

        // Add lights
        myScene->AddLights(boxLight);
        myScene->AddLights(sunLight);

        myScene->initialize();

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
    Color* blue = new Color(.3, .3, 1);
    Color* white = new Color(1, 1, 1);
    Color* whiteOut = new Color(6, 6, 6);

    //    create materials
    Material* reflect_smooth = new Material(*white, *white, .5, .4, 0, .1, 0.04, 0, 10, 0);
    Material* reflect_rough = new Material(*white, *white, .1, .7, 0, .1, 0.09, 0, 10, 0);
    Material* red_solid = new Material(*red, *white, 1, 0, 0, .2, 0, 0, 6, 0);
    Material* green_mat = new Material(*green, *white, .4, 0, 0, .2, 0, 0, 6, 0);
    Material* blue_mat = new Material(*blue, *white, .2, 0, .6, .2, 0, 0, 6, 1.2);
    Material* white_mat = new Material(*white, *white, .1, 0, 0, .1, 0, 0, 6, 0);
    Material* refractive_smooth = new Material(*white, *white, .005, .1, .8, .2, 0, 0, 16, 1.3);
    Material* refractive_rough = new Material(*white, *white, .005, .1, .8, .2, 0.09, .09, 8, 1.2);

    Material* earth = new Material(*white, *white, .8, 0, 0, .1, 0.0, .0, 5, 1);
    Material* jupiter = new Material(*white, *white, 1, 0, 0, .2, 0.0, .0, 5, 1);

};






#endif //RT_SCENEMAKER_H


