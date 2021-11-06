//
// Created by marti on 27-Sep-21.
//

#ifndef RT_SCENEMAKER_H
#define RT_SCENEMAKER_H

class SceneMaker {
public:

    Camera* camera2(){

        double aspect_ratio = 16.0 / 9.0;

        int sampleSubdiv = 1;
        int samples = 1;
        int indirect_samples = 1;
        bool rayJitter = false;

        auto x_dim = 640u;       //default 320
        auto y_dim = x_dim / aspect_ratio; //240p
        double fov = 140.0;

        int num_bounces = 0;
        int shadowSamples = 1;


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

    Scene* scene2(){
//    create objects
        jupiter->setTexture("Textures/jupiter.ppm");
        earth_ref->setTexture("Textures/2k_earth.ppm");
        auto refl_plane = new Plane(white_diffuse, Point(.4, 1, .3), -2);
        auto jupiter_sphere = new Sphere(jupiter, Point(0, 0, -5), 4);
        auto earth_sphere = new Sphere(earth_ref, Point(1, -.8, .4), .6);
        auto moon = new Sphere(red_diffuse, Point(1, 1, -.5), .2);
        auto moon2 = new Sphere(blue_diffuse, Point(-.2, .6, -.3), .25);
        auto moon3 = new Sphere(yellow_diffuse, Point(4.4, .2, -3.6), .3);

//    create lights
//        auto boxLight = new BoxLight(Color(.9), Point(5, 3, 5), Point(6, 4, 6));
        auto pointLight = new PointLight(Color(.9), Point(50, 30, 50));
//        create scene and set rendering values
        auto global_light = Color(.1);
        auto sunLight = new SunLight(Color(.9), Point(1, .5, 2));
        //FIXME: sun light seems to be breaking shadows with jupiter.
        auto myScene = new Scene(global_light);

//          Add objects

        myScene->AddObjects(refl_plane);
        myScene->AddObjects(jupiter_sphere);
        myScene->AddObjects(earth_sphere);
        myScene->AddObjects(moon);
        myScene->AddObjects(moon2);
        myScene->AddObjects(moon3);

        //         Add lights
        myScene->AddLights(pointLight);

        myScene->SetCamera(camera2());

//        initialize bounding box hierarchy
//        myScene->initialize();

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
    Color* yellow = new Color(1, 1, 0);

    //    create materials
    double ds_fac = .7;
    Material* red_diffuse = new Material(*red, *white, 1, 0, 0, ds_fac, 0, 0, 7, 0);
    Material* blue_diffuse = new Material(*blue, *white, 1, 0, 0, ds_fac, 0, 0, 7, 0);
    Material* yellow_diffuse = new Material(*yellow, *white, 1, 0, 0, ds_fac, 0, 0, 7, 0);
    Material* white_diffuse = new Material(*white, *white, 1, 0, 0, ds_fac, 0, 0, 7, 0);

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


