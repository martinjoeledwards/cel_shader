//
// Created by marti on 07-Sep-21.
//

#include "Point.h"
#include "Helpers.h"
#include <cmath>

std::ostream& operator<<(std::ostream& os, const Point& p){
    os << "(" << p.x << ',' << p.y << ',' << p.z << ")";
    return os;
}


double magnitude(Point inP){
    double total = pow(inP.x, 2) + pow(inP.y, 2) +pow(inP.z, 2);
    return pow(total, 0.5);
}

Point norm(Point inP){
    Point newP;
    double mag = magnitude(inP);
    newP.x = inP.x / mag;
    newP.y = inP.y / mag;
    newP.z = inP.z / mag;
    return newP;
}

Point sub(Point first, Point second){
    return Point(first.x - second.x, first.y - second.y, first.z - second.z);
}

Point cross(Point fir, Point sec){
    Point newP;
    newP.x = (fir.y * sec.z) - (fir.z * sec.y);
    newP.y = (fir.z * sec.x) - (fir.x * sec.z);
    newP.z = (fir.x * sec.y) - (fir.y * sec.x);
    return newP;
}

Point absVal(Point in){
    return Point{fabs(in.x), fabs(in.y), fabs(in.z)};
}

double dot(Point fir, Point sec){
//    std::cout << "dot is " << (fir.u * sec.u) + (fir.v * sec.v) + (fir.z * sec.z) << std::endl;
    return (fir.x * sec.x) + (fir.y * sec.y) + (fir.z * sec.z);
}

int maxIndex(Point in) {
    if(in.x > in.y && in.x > in.z) return 0;
    if(in.y > in.z) return 1;
    return 2;
}

UVPoint squish(Point in, int index) {
//    in.Print();
    if(index == 0) return UVPoint{in.y, in.z};
    if(index == 1) return UVPoint{in.x, in.z};
    return UVPoint{in.x, in.y};
}

Point reflect_vector(Point direction, Point normal){
    return norm(direction - normal * dot(direction, normal) * 2); //this gives problems with the -, so I had to make another function not pass by reference

    //    return vector - 2 * np.dot(vector, axis) * axis
}

double get_incidence_angle(Point incident, Point normal){
    incident = norm(incident);
    normal = norm(normal);
    if(dot(incident, normal) < 0){
        normal = normal * -1;
    }
//    incident.Print();
//    normal.Print();
    double top = dot(norm(incident), norm(normal));
    double bottom = magnitude(incident) * magnitude(normal);
    return acos(top / bottom);
}

Point refract_vec(double ior, Point incident, Point normal){
    double inc_angle = get_incidence_angle(incident, normal);
    double theta = 1 / ior;
    if (dot(incident, normal) > 0){ //account for exiting refractive media
        theta = ior / 1;
    }

    Point firstTerm = incident * theta;
    double second = cos(inc_angle) * theta;
    double root = sqrt(1 + pow(theta, 2) * (pow(cos(inc_angle), 2) - 1));
    double right = second - root;
    Point large = normal * right;
    Point final = large + firstTerm;
    return norm(final);

}

Point jitter_vector(Point vector, double amount){
    double x_amt = getRandDouble(-amount, amount);
    double y_amt = getRandDouble(-amount, amount);
    double z_amt = getRandDouble(-amount, amount);

    vector = vector + Point(x_amt, y_amt, z_amt);
    return norm(vector);
}


Point rand_vector(){
    double x_val = getRandDouble(-1, 1);
    double y_val = getRandDouble(-1, 1);
    double z_val = getRandDouble(-1, 1);

    return norm(Point(x_val, y_val, z_val));
}


Point randNormalJitter(Point normal){
    Point rand = rand_vector();
    while (dot(rand, normal) < 0.0){
        rand = rand_vector();
    }
    return rand;
}
