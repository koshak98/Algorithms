#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>

template<class SizeType>
SizeType Pow(SizeType base, uint32_t degree) {
    SizeType ans = 1;
    for (int i = 0; i < degree; ++i) {
        ans *= base;
    }
    return ans;
}

std::pair<double, double> CramerRule2D(int32_t a11, int32_t a12, int32_t b1, int32_t a21, int32_t a22, int32_t b2) {
    double delta = a11 * a22 - a21 * a12;
    double delta1 = b1 * a22 - b2 * a12;
    double delta2 = a11 * b2 - a21 * b1;
    return std::make_pair(delta1 / delta, delta2 / delta);
}


double MinDis(const std::vector<double>& distances) {
    double ans = distances[0];
    for (int i = 1; i < distances.size(); ++i) {
        if (ans > distances[i]) {
            ans = distances[i];
        }
    }
    return ans;
}


template<class SizeType, uint32_t dim>
class Vector;
template <class SizeType, uint32_t dim>
Vector<SizeType, dim> operator - (const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2);
template <class SizeType, uint32_t dim>
Vector<SizeType, dim> operator + (const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2);

template<class SizeType, uint32_t dim>
class Vector {
private:
    SizeType coordinates[dim];
public:
    Vector() {
        for (int i = 0; i < dim; ++i) {
            coordinates[i] = 0;
        }
    }
    Vector(SizeType x, SizeType y, SizeType z) {
        coordinates[0] = x;
        coordinates[1] = y;
        coordinates[2] = z;
    }
    Vector(const Vector& other) {
        for (int i = 0; i < dim; ++i) {
            coordinates[i] = other.coordinates[i];
        }
    }
    
    
    Vector& operator = (const Vector& other) {
        for (int i = 0; i < dim; ++i) {
            coordinates[i] = other.coordinates[i];
        }
        return *this;
    }
    Vector operator -= (const Vector& other) {
        for (int i = 0; i < dim; ++i) {
            coordinates[i] -= other.coordinates[i];
        }
        return *this;
    }
    Vector operator *= (SizeType num) {
        for (int i = 0; i < dim; ++i) {
            coordinates[i] *= num;
        }
        return *this;
    }
    Vector operator += (const Vector& other) {
        for (int i = 0; i < dim; ++i) {
            coordinates[i] += other.coordinates[i];
        }
        return *this;
    }
    Vector operator /= (SizeType num) {
        for (int i = 0; i < dim; ++i) {
            coordinates[i] /= num;
        }
        return *this;
    }
    bool operator == (const Vector& other) const {
        for (int i = 0; i < dim; ++i) {
            if (coordinates[i] != other.coordinates[i]) {
                return false;
            }
        }
        return true;
    }
    
    const SizeType* GetCoordinates() const {
        return coordinates;
    }

    friend Vector operator -<> (const Vector& vec1, const Vector& vec2);
    friend Vector operator +<> (const Vector& vec1, const Vector& vec2);
};

template <class SizeType, uint32_t dim>
Vector<SizeType, dim> operator - (const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    Vector<SizeType, dim> ans;
    for (int i = 0; i < dim; ++i) {
        ans.coordinates[i] = vec1.coordinates[i] - vec2.coordinates[i];
    }
    return ans;
}

template <class SizeType, uint32_t dim>
Vector<SizeType, dim> operator + (const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    Vector<SizeType, dim> ans;
    for (int i = 0; i < dim; ++i) {
        ans.coordinates[i] = vec1.coordinates[i] + vec2.coordinates[i];
    }
    return ans;
}

template<class SizeType, uint32_t dim>
class Plane {
private:
    Vector<SizeType, dim> dot;
    Vector<SizeType, dim> guide_vec1;
    Vector<SizeType, dim> guide_vec2;
public:
    Plane(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2, const Vector<SizeType, dim>& vec3) {
        dot = vec1;
        guide_vec1 = vec2 - vec1;
        guide_vec2 = vec3 - vec1;
    }
};

template<class SizeType, uint32_t dim>
class Segment {
private:
    Vector<SizeType, dim> start_dot;
    Vector<SizeType, dim> guide_vec;
public:
    Segment(Vector<SizeType, dim> vec1, Vector<SizeType, dim> vec2) {
        start_dot = vec1;
        guide_vec = vec2 - vec1;
    }
    const Vector<SizeType, dim>& GetGuideVector() const {
        return guide_vec;
    }
    const Vector<SizeType, dim>& GetStartDot() const {
        return start_dot;
    }
};

template<class SizeType, uint32_t dim>
SizeType Module(const Vector<SizeType, dim>& vec) {
    const SizeType* coordinates = vec.GetCoordinates();
    SizeType total_pow = 0;
    for (int i = 0; i < dim; ++i) {
        total_pow += (coordinates[i] * coordinates[i]);
    }
    return std::sqrt(total_pow);
}

template<class SizeType, uint32_t dim>
SizeType DotProduct(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    SizeType result_of_dot_product = 0;
    const SizeType* coor1 = vec1.GetCoordinates();
    const SizeType* coor2 = vec2.GetCoordinates();
    for (int i = 0; i < dim; ++i) {
        result_of_dot_product += (coor1[i] * coor2[i]);
    }
    return result_of_dot_product;
}

template<class SizeType, uint32_t dim>
bool VecProduct3D(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2, const Vector<SizeType, dim>& vec3) {
    
    const SizeType* coor1 = vec1.GetCoordinates();
    const SizeType* coor2 = vec2.GetCoordinates();
    const SizeType* coor3 = vec3.GetCoordinates();
    
    return (coor1[0] * coor2[1] * coor3[2] + coor1[1] * coor2[2] * coor3[0] + coor2[0] * coor1[2] * coor3[1] - coor1[2] * coor2[1] * coor3[0] - coor1[1] * coor2[0] * coor3[2] - coor1[0] * coor2[2] * coor3[1]) == 0;
}

template<class SizeType, uint32_t dim>
SizeType GetCosinus(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    const SizeType* coor1 = vec1.GetCoordinates();
    const SizeType* coor2 = vec2.GetCoordinates();

    
    return (DotProduct<SizeType, dim>(vec1, vec2)) / (Module<SizeType, dim>(vec1) * Module<SizeType, dim>(vec2));
    
}

template<class SizeType, uint32_t dim>
bool InOneHalfPlane(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    const SizeType* coor1 = vec1.GetCoordinates();
    const SizeType* coor2 = vec2.GetCoordinates();
    return (coor1[0]*coor2[1] - coor1[0]*coor2[2] - coor1[1]*coor2[0] + coor1[1]*coor2[2] + coor1[2]*coor2[0] - coor1[2]*coor2[1]) >= 0;
}

template<class SizeType, uint32_t dim>
SizeType DistanceFromDottoSegment(const Segment<SizeType, dim>& segment, const Vector<SizeType, dim>& dot) {
    Vector<SizeType, dim> guide_vec(segment.GetGuideVector());
    Vector<SizeType, dim> start_vec(segment.GetStartDot());
    
    const SizeType* guide_coor = guide_vec.GetCoordinates();
    const SizeType* start_coor = start_vec.GetCoordinates();
    const SizeType* dot_coor = dot.GetCoordinates();
    
    SizeType x = guide_coor[0] * dot_coor[0] - guide_coor[0] * start_coor[0];
    SizeType y = guide_coor[1] * dot_coor[1] - guide_coor[1] * start_coor[1];
    SizeType z = guide_coor[2] * dot_coor[2] - guide_coor[2] * start_coor[2];
    SizeType norm = Pow(guide_coor[0], 2) + Pow(guide_coor[1], 2) + Pow(guide_coor[2], 2);
    SizeType t = (x + y + z) / norm;
    
    Vector<SizeType, dim> X = guide_vec;
    X *= t;
    Vector<SizeType, dim> perpendicular(start_vec + X);
    if (t >= 0 && t <= 1) {
        return Module(dot - perpendicular);
    }
    else {
        std::vector<double> distances;
        distances.push_back(Module(dot - start_vec));
        distances.push_back(Module(dot - (start_vec + guide_vec)));
        return MinDis(distances);
    }
}

template<class SizeType, uint32_t dim>
bool InOnePlane(const Segment<SizeType, dim>& first, const Segment<SizeType, dim>& second) {
    if (fabs(GetCosinus(first.GetGuideVector(), second.GetGuideVector())) >= 0.9999) {
        return true;
    }
    
    Vector<SizeType, dim> start_vec1(first.GetStartDot());
    Vector<SizeType, dim> guide_vec1(first.GetGuideVector());
    Vector<SizeType, dim> start_vec2(second.GetStartDot());
    Vector<SizeType, dim> guide_vec2(second.GetGuideVector());

    if (VecProduct3D(guide_vec1, guide_vec2, start_vec2 - start_vec1) && VecProduct3D(guide_vec1, guide_vec2, start_vec1 - start_vec2)) {
        return true;
    }
    return false;
}

template<class SizeType, uint32_t dim>
bool Intersect(const Segment<SizeType, dim>& first, const Segment<SizeType, dim>& second) {
    Vector<SizeType, dim> start_vec1(first.GetStartDot());
    Vector<SizeType, dim> guide_vec1(first.GetGuideVector());
    Vector<SizeType, dim> start_vec2(second.GetStartDot());
    Vector<SizeType, dim> guide_vec2(second.GetGuideVector());
    bool check_first = InOneHalfPlane(guide_vec1, start_vec1 + guide_vec1 - start_vec2);
    bool check_second = InOneHalfPlane(guide_vec1, start_vec1 + guide_vec1 - (start_vec2 + guide_vec2));
    bool check_third = InOneHalfPlane(guide_vec2, start_vec2 + guide_vec2 - start_vec1);
    bool check_fourth = InOneHalfPlane(guide_vec2, start_vec2 + guide_vec2 - (start_vec1 + guide_vec1));
    
    if ((check_first != check_second && check_third != check_fourth) || start_vec1 == start_vec2 || (guide_vec1 + start_vec1) == (guide_vec2 + start_vec2)) {
        return true;
    }
    return false;
}


template<class SizeType, uint32_t dim>
double CommonPerpendicular(Segment<SizeType, dim> first, Segment<SizeType, dim> second) {
    
    Vector<SizeType, dim> start_vec1(first.GetStartDot());
    Vector<SizeType, dim> guide_vec1(first.GetGuideVector());
    Vector<SizeType, dim> start_vec2(second.GetStartDot());
    Vector<SizeType, dim> guide_vec2(second.GetGuideVector());
    
    

    const SizeType* start_coor1 = start_vec1.GetCoordinates();
    const SizeType* guide_coor1 = guide_vec1.GetCoordinates();
    const SizeType* start_coor2 = start_vec2.GetCoordinates();
    const SizeType* guide_coor2 = guide_vec2.GetCoordinates();
    
    
    if (InOnePlane(first, second)) {
        std::vector<double> distances;
        
        
        if (Intersect(first, second)) {
            return 0;
        }
        
        distances.push_back(DistanceFromDottoSegment(first, start_vec2));
        distances.push_back(DistanceFromDottoSegment(first, start_vec2 + guide_vec2));
        distances.push_back(DistanceFromDottoSegment(second, start_vec1));
        distances.push_back(DistanceFromDottoSegment(second, start_vec1 + guide_vec1));
        return MinDis(distances);
    }
    
    int32_t a11 = guide_coor1[0] * guide_coor1[0] + guide_coor1[1] * guide_coor1[1] + guide_coor1[2] * guide_coor1[2];
    int32_t a12 = -guide_coor1[0] * guide_coor2[0] - guide_coor1[1] * guide_coor2[1] - guide_coor1[2] * guide_coor2[2];
    int32_t a21 = guide_coor1[0] * guide_coor2[0] + guide_coor1[1] * guide_coor2[1] + guide_coor1[2] * guide_coor2[2];
    int32_t a22 = -guide_coor2[0] * guide_coor2[0] - guide_coor2[1] * guide_coor2[1] - guide_coor2[2] * guide_coor2[2];
    int32_t b1 = guide_coor1[0] * start_coor2[0] - guide_coor1[0] * start_coor1[0] +
                 guide_coor1[1] * start_coor2[1] - guide_coor1[1] * start_coor1[1] +
                 guide_coor1[2] * start_coor2[2] - guide_coor1[2] * start_coor1[2];

    int32_t b2 = guide_coor2[0] * start_coor2[0] - guide_coor2[0] * start_coor1[0] +
                 guide_coor2[1] * start_coor2[1] - guide_coor2[1] * start_coor1[1] +
                 guide_coor2[2] * start_coor2[2] - guide_coor2[2] * start_coor1[2];
    
    std::pair<double, double> coef(CramerRule2D(a11, a12, b1, a21, a22, b2));
    
    double t = coef.first;
    double s = coef.second;
    
    Vector<SizeType, dim> A(guide_coor1[0] * t + start_coor1[0], guide_coor1[1] * t + start_coor1[1], guide_coor1[2] * t + start_coor1[2]);
    Vector<SizeType, dim> B(guide_coor2[0] * s + start_coor2[0], guide_coor2[1] * s + start_coor2[1], guide_coor2[2] * s + start_coor2[2]);
    if (t >= 0 && t <= 1 && s >= 0 && s <= 1) {
        return Module(A - B);
    }
    else {
        std::vector<double> distances;
        distances.push_back(Module(start_vec1 - start_vec2));
        distances.push_back(Module(start_vec1 - (guide_vec2 + start_vec2)));
        distances.push_back(Module(start_vec1 + guide_vec1 - start_vec2));
        distances.push_back(Module(start_vec1 + guide_vec1 - (guide_vec2 + start_vec2)));
        return MinDis(distances);
    }
}



int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int32_t x1, y1, z1;
    std::cin >> x1 >> y1 >> z1;

    int32_t x2, y2, z2;
    std::cin >> x2 >> y2 >> z2;

    int32_t x3, y3, z3;
    std::cin >> x3 >> y3 >> z3;

    int32_t x4, y4, z4;
    std::cin >> x4 >> y4 >> z4;

    Vector<double, 3> vec1(x1, y1, z1);
    Vector<double, 3> vec2(x2, y2, z2);
    Vector<double, 3> vec3(x3, y3, z3);
    Vector<double, 3> vec4(x4, y4, z4);

    Segment<double, 3> first(vec1, vec2);
    Segment<double, 3> second(vec3, vec4);
    std::cout << std::setprecision(12) << CommonPerpendicular(first, second) << std::endl;

    
    return 0;
}
