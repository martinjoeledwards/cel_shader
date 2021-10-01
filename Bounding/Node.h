//
// Created by marti on 27-Sep-21.
//

#ifndef RT_NODE_H
#define RT_NODE_H

//TODO: switch to Median Split. Don't worry about until more objects are in the scene.

#include <vector>
#include "BoundingBox.h"
#include "../Objects/Object.h"

class Node {
public:

    bool rootNode = false;
    bool leafNode = false;
    int depth = 0;

    void setDepth(int d) { depth = d;}

    Node(){
        rootNode = true;
//        std::cout << "I am root\n";
    }

    explicit Node(Object* object, int depth){
        myBB = object->getBB();
        objects.push_back(object);
        leafNode = true;
        this->depth = depth;
//        std::cout << "node depth is " << this->depth << std::endl;
    }

    void initialize(){      //magic here: create child bounding box

    }

    void addObject(Object* object){
        objects.push_back(object);
//        if(object->getBB().getSurfArea() == 0.0){       //add planes to root level.
//            objects.push_back(object);
//            return;
//        }       //magic here.
//
//        double surf_increase = this->getSurfIncrease(object);
//        Node* most_efficient = this;
//
//        for(auto child : children){
//            if (child->getSurfIncrease(object) < surf_increase){
//                surf_increase = child->getSurfIncrease(object);
//                most_efficient = child;
//            }
//        }
//
//        most_efficient->addNode(object);
    }

    bool rayHits(Point rayStart, Point ray_dir){
        if (myBB.inside(rayStart)) return true;     // ray is inside the bounding box
        Ray myRay(rayStart, ray_dir);
        return myBB.rayHit(myRay);
    }

    std::vector<Object*> getObjs(Point ray_start, Point ray_dir){   //determines which objects are potentially hit by the ray.
        std::vector<Object*> hitObjects;
        for(auto obj : objects){        //return object from this node. Root and others.
            hitObjects.push_back(obj);
        }

        for(auto node : children) {
            if (node->rayHits(ray_start, ray_dir)) {
                auto childObjs = node->getObjs(ray_start, ray_dir);
                for (auto obj: childObjs) {
                    hitObjects.push_back(obj);
                }
            }
        }


        return hitObjects;
    }




private:
    BoundingBox myBB;       //Must be updated when object is added...?
    std::vector<Node*> children;
    std::vector<Object*> objects;

    void addNode(Object* object){
        // if root node
        if (rootNode){
            Node* node = new Node(object, depth +1);
            children.push_back(node);
            // my bounding box is not changed
        }  else if (leafNode) {                 // leaf node: make not leaf node, then add both objects as children
            leafNode = false;
            children.push_back(new Node(object, depth +1));
            children.push_back(new Node(objects[0], depth +1));
            objects.clear();
            //  Bounding box of current node becomes the combination of the boxes.
            myBB = children[0]->myBB.combine(children[1]->myBB);

        } else {                                // node, not leaf node: recursively search for best place. could this get messy?
            std::cout << "adding recursively.\n";
            this->addObject(object);
        }
    }

    double getSurfIncrease(Object* obj){
        if(rootNode)
            return obj->getBB().getSurfArea();

        double orig_surf = myBB.getSurfArea();
        double next_surf = myBB.combine(obj->getBB()).getSurfArea();
        return next_surf - orig_surf;

    }

};


#endif //RT_NODE_H
