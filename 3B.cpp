#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include <unordered_map>
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
    for (int i = 0; i < distances.size(); ++i) {
        if (ans >= distances[i]) {
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
SizeType temp(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2, const Vector<SizeType, dim>& vec3) {
    
    const SizeType* coor1 = vec1.GetCoordinates();
    const SizeType* coor2 = vec2.GetCoordinates();
    const SizeType* coor3 = vec3.GetCoordinates();
    
    return (coor1[0] * coor2[1] * coor3[2] + coor1[1] * coor2[2] * coor3[0] + coor2[0] * coor1[2] * coor3[1] - coor1[2] * coor2[1] * coor3[0] - coor1[1] * coor2[0] * coor3[2] - coor1[0] * coor2[2] * coor3[1]);
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
    if (fabs(GetCosinus(first.GetGuideVector(), second.GetGuideVector())) >= 0.999) {
        return true;
    }
    
    Vector<SizeType, dim> start_vec1(first.GetStartDot());
    Vector<SizeType, dim> guide_vec1(first.GetGuideVector());
    Vector<SizeType, dim> start_vec2(second.GetStartDot());
    Vector<SizeType, dim> guide_vec2(second.GetGuideVector());

    if (VecProduct3D(guide_vec1, guide_vec2, start_vec2 - start_vec1) && VecProduct3D(guide_vec1, guide_vec2, start_vec1 - start_vec2) && VecProduct3D(guide_vec1, guide_vec2, start_vec2 + guide_vec2 - start_vec1 - guide_vec1) &&
        VecProduct3D(guide_vec1, guide_vec2, start_vec1 + guide_vec1 - start_vec2 - guide_vec2) && VecProduct3D(guide_vec1, start_vec2 - start_vec1, start_vec1 + guide_vec1 - start_vec2 - guide_vec2) && VecProduct3D(guide_vec2, start_vec1 + guide_vec1 - start_vec2 - guide_vec2, start_vec1 - start_vec2)) {
        return true;
    }
    return false;
}


template<class SizeType, uint32_t dim>
struct OppositeVertices {
    Vector<SizeType, dim> first;
    Vector<SizeType, dim> second;
};

template<class SizeType, uint32_t dim>
struct ConvexHull {
//    std::unordered_map<Segment<SizeType, dim>, OppositeVertices<SizeType, dim>> faces;
};


template<class SizeType, uint32_t dim>
Vector<SizeType, dim> FindMinDot(const std::vector<Vector<SizeType, dim>>& dots) {
    Vector<SizeType, dim> A(dots[0]);
    
    for (int i = 1; i < dots.size(); ++i) {
        Vector<SizeType, dim> B(dots[i]);
        const SizeType* coor = B.GetCoordinates();
        if (coor[2] <= coor_ans[2]) {
            if (coor[2] < coor_ans[2]) {
                coor_ans = coor;
            }
            else if (coor[2] == coor_ans[2]) {
                if (coor[0] <= coor_ans[0]) {
                    if (coor[0] < coor_ans[0]) {
                        coor_ans = coor;
                    }
                    else if (coor[0] == coor_ans[0]) {
                        if (coor[1] < coor_ans[1]) {
                            coor_ans = coor;
                        }
                    }
                }
            }
        }
    }
    Vector<SizeType, dim> ans(coor_ans[0], coor_ans[1], coor_ans[2]);
    return ans;
}




template<class SizeType, uint32_t dim>
void FindFirstFace(const std::vector<Vector<SizeType, dim>>& dots, ConvexHull<SizeType, dim>& answer) {
    
}



template<class SizeType, uint32_t dim>
void DoOneTest(std::vector<Vector<SizeType, dim>> dots) {
    ConvexHull<SizeType, dim> answer;
    FindFirstFace(dots, answer);
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
        std::vector<Vector<double, 3>> dots;
    
    Vector<double, 3> a(1, 2, 3);
    Vector<double, 3> b(1, 2, 4);
    Vector<double, 3> c(23, 4, 3);
    dots.push_back(a);
    dots.push_back(b);
    dots.push_back(c);
    Vector<double, 3> d = FindMinDot(dots);
    return 0;
}
