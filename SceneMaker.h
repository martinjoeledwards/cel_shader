//
// Created by marti on 27-Sep-21.
//

#ifndef RT_SCENEMAKER_H
#define RT_SCENEMAKER_H

class SceneMaker {
public:


    Camera* camera1(){

        double aspect_ratio = 1.0 / .75;

        int sampleSubdiv = 4;
        bool rayJitter = true;

        auto x_dim = 320u;       //default 320
        auto y_dim = x_dim / aspect_ratio; //240p
        double fov = 100.0;


        Point cam_from(1.5, 1.5, 1.5);
        Point cam_at(0, 0, 0);
        Point cam_up(0, 1, 0);

        auto myCamera = new Camera(x_dim, aspect_ratio, fov, cam_from, cam_at, cam_up);
        myCamera->setRayJitter(rayJitter);
        myCamera->setSampleSubdiv(sampleSubdiv);

        return myCamera;

    }

    Scene* scene1(){
        int num_bounces = 6;        //TODO: move these two to the camera class? And give scene a camera member?
        int shadowSamples = 1;


//    create objects
        auto* refrSphere = new Sphere(refractive_smooth, Point(-.51, 0.3, -0.3), .5);
        auto* refrSphereR = new Sphere(refractive_rough, Point(.51, 0.3, -0.6), .4);
        auto* blueSphere = new Sphere(reflect_smooth, Point(-.5, 0, .6), .3);
        auto* reflSphereRough = new Sphere(reflect_rough, Point(0, 0, .6), .2);
        auto* greenSphere = new Sphere(green_mat, Point(.6, 0, 0), .3);

        auto* blueBox = new AABox(blue_mat, Point(.2, .2, .2), Point(0, 0, 0));
        auto* redBox = new AABox(red_mat, Point(0, 0, -.1), Point(.2,.2, -.03));
        auto* redBoxLong = new AABox(red_mat, Point(-2, 0.1, -1), Point(2,.2, -.9));

        Point p0(100, -.4, -100);
        Point p1(-100, -.4, -100);
        Point p2(0, -.4, 100);
        auto* triangle = new Triangle(green_mat, p0, p2, p1);

        auto* reflect_plane = new Plane(reflect_smooth, Point(1, 1, .3), -.4);
        auto* diff_plane = new Plane(green_mat, Point(0, 1, 0), -.2);

//    create lights
//        SunLight* sunlight(whiteOut, Point(-1, 1, 1));
//        PointLight* pointlight(white, Point(-1, .5, 0));
//        PointLight* pointlightup(blue, Point(0, 2, 0));
        auto* boxLight = new BoxLight(*white, Point(-.5, 2, -.5), Point(.2, 3, .2));


//        create scene and set rendering values
        auto* myScene = new Scene();
        myScene->setNumBounces(num_bounces);
        myScene->setShadowSamples(shadowSamples);

// Add objects
//    myScene.AddObjects(&reflect_plane);
//    myScene.AddObjects(&triangle);

        myScene->AddObjects(diff_plane);
        myScene->AddObjects(refrSphereR);
        myScene->AddObjects(refrSphere);
        myScene->AddObjects(blueSphere);
        myScene->AddObjects(greenSphere);
        myScene->AddObjects(reflSphereRough);
        myScene->AddObjects(blueBox);
        myScene->AddObjects(redBox);
        myScene->AddObjects(redBoxLong);

        // Add lights
//    myScene.AddLights(&pointlight);
//    myScene.AddLights(&sunlight);
//    myScene.AddLights(&pointlightup);
        myScene->AddLights(boxLight);

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
    Material* reflect_smooth = new Material(*white, *white, .1, .1, .1, .7, 0, 0, 0, 10, 0);
    Material* reflect_rough = new Material(*white, *white, .1, .1, .1, .7, 0, 0.09, 0, 10, 0);
    Material* red_mat = new Material(*red, *red, .8, .3, .1, .2, 0, 0, 0, 6, 0);
    Material* green_mat = new Material(*green, *white, .4, .5, .1, 0, 0, 0, 0, 6, 0);
    Material* blue_mat = new Material(*blue, *white, .3, .5, .3, 0, 0, 0, 0, 6, 0);
    Material* refractive_smooth = new Material(*white, *white, .005, .5, .002, .1, .8, 0, 0, 16, 1.3);
    Material* refractive_rough = new Material(*white, *white, .005, .4, .004, .1, .8, 0, .09, 8, 1.2);

};






#endif //RT_SCENEMAKER_H


