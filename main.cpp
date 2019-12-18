#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

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
    Vector(SizeType x, SizeType y) {
        coordinates[0] = x;
        coordinates[1] = y;
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
    
    const SizeType GetFirstCoordinate() const {
        return coordinates[0];
    }
    const SizeType GetSecondCoordinate() const {
        return coordinates[1];
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
        result_of_dot_product += coor1[i] * coor2[i];
    }
    return result_of_dot_product;
}

template<class SizeType, uint32_t dim>
SizeType VectorProduct(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    const SizeType* coor1 = vec1.GetCoordinates();
    const SizeType* coor2 = vec2.GetCoordinates();
    return coor1[0] * coor2[1] - coor1[1] * coor2[0];
}

template<class SizeType, uint32_t dim>
SizeType GetCosinus(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    Vector<SizeType, dim> zero(0, 0);
    if (vec1 == zero || vec2 == zero) {
        return 0;
        
    }
    return (DotProduct<SizeType, dim>(vec1, vec2)) / (Module<SizeType, dim>(vec1) * Module<SizeType, dim>(vec2));
}

template<class SizeType, uint32_t dim>
SizeType GetSinus(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    Vector<SizeType, dim> zero(0, 0);
    if (vec1 == zero || vec2 == zero) {
        return 0;
    }
    return (Vector<SizeType, dim>(vec1, vec2)) / (Module<SizeType, dim>(vec1) * Module<SizeType, dim>(vec2));
}

template<class SizeType, uint32_t dim>
bool InOneHalfPlane(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    const SizeType* coor1 = vec1.GetCoordinates();
    const SizeType* coor2 = vec2.GetCoordinates();
    return (coor1[0] * coor2[1] - coor1[1] * coor2[0]) >= 0;
}

template<class SizeType, uint32_t dim>
SizeType GetCosinusWithOrientation (const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    return GetCosinus(vec1, vec2) + InOneHalfPlane(vec1, vec2);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





template<class SizeType, uint32_t dim>
void Sort(std::vector<std::pair<SizeType, Vector<SizeType, dim>>>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        for (int j = vec.size() - 1; j > i; --j) {
            if (vec[j].first > vec[j - 1].first) {
                std::pair<SizeType, Vector<SizeType, dim>> temp;
                temp = vec[j];
                vec[j] = vec[j - 1];
                vec[j - 1] = temp;
            }
        }
    }
}


template<class SizeType, uint32_t dim>
std::pair<Vector<SizeType, dim>, uint32_t> GetMin(const std::vector<Vector<SizeType, dim>>& A) {
    Vector<SizeType, dim> min(A[0].GetFirstCoordinate(), A[0].GetSecondCoordinate());
    uint32_t index = 0;
    for (int i = 1; i < A.size(); ++i) {
        if (min.GetSecondCoordinate() >= A[i].GetSecondCoordinate()) {
            if (min.GetSecondCoordinate() == A[i].GetSecondCoordinate()) {
                if (min.GetFirstCoordinate() > A[i].GetFirstCoordinate()) {
                    min = A[i];
                    index = i;
                }
            }
            else {
                min = A[i];
                index = i;
            }
        }
    }
    std::pair<Vector<SizeType, dim>, SizeType> ans = std::make_pair(min, index);
    return ans;
}

template<class SizeType, uint32_t dim>
std::vector<Vector<SizeType, dim>> SortByAngle(const std::vector<Vector<SizeType, dim>>& A) {
    std::vector<Vector<SizeType, dim>> sorted;
    std::pair<Vector<SizeType, dim>, uint32_t> first_dot(GetMin(A));
    sorted.push_back(first_dot.first);
    uint32_t count = first_dot.second;
    while (sorted.size() != A.size()) {
        if (count > 0) {
            sorted.push_back(A[count - 1]);
            --count;
        }
        else {
            count += A.size() - 1;
            sorted.push_back(A[count]);
            
        }
    }
    return sorted;
}


template<class SizeType, uint32_t dim>
SizeType PolarAngle(const Vector<SizeType, dim>& vec) {
    const SizeType* coor = vec.GetCoordinates();
    SizeType x = coor[0];
    SizeType y = coor[1];
    if (x == 0) {
        if (y > 0) return M_PI / 2;
        if (y < 0) return 3 * M_PI / 2;
    } else if (x > 0) {
        if (y >= 0) return atan(y / x);
        return atan(y / x) + 2 * M_PI;
    } else {
        return atan(y / x) + M_PI;
    }
    return 0;
}

template<class SizeType, uint32_t dim>
std::vector<Vector<SizeType, dim>> BuildMinkowskiSum (std::vector<Vector<SizeType, dim>> A, std::vector<Vector<SizeType, dim>> B) {
    std::vector<Vector<SizeType, dim>> sortedA(SortByAngle(A));
    std::vector<Vector<SizeType, dim>> sortedB(SortByAngle(B));
    
    uint32_t iterA = 0;
    uint32_t iterB = 0;
    std::vector<Vector<SizeType, dim>> result;
    while (iterA < A.size() || iterB < B.size()) {
        
        result.push_back(sortedA[iterA % A.size()] + sortedB[iterB % B.size()]);
        SizeType angle1 = PolarAngle(sortedA[(iterA + 1) % A.size()] - sortedA[iterA % A.size()]);
        SizeType angle2 = PolarAngle(sortedB[(iterB + 1) % B.size()] - sortedB[iterB % B.size()]);
    
        iterA == A.size() ? angle1 += 2 * M_PI : angle1;
        iterB == B.size() ? angle2 += 2 * M_PI : angle2;
    
        if (angle1 < angle2) {
            ++iterA;
        } else if (angle2 < angle1) {
            ++iterB;
        } else {
            ++iterA;
            ++iterB;
        }
    }
    
    return result;
}

template<class SizeType, uint32_t dim>
bool IncludeZero(const std::vector<Vector<SizeType, dim>>& A) {
    Vector<SizeType, dim> zero(0, 0);
    for (int i = 0; i < A.size(); ++i) {
        if (A[i] == zero) {
            return true;
        }
    }
    
    bool flag = true;
    for (int i = 1; i <= A.size(); ++i) {
        if (!InOneHalfPlane(A[i - 1], A[i % A.size()] - A[i - 1])){
            flag = false;
            break;
        }
    }
    return flag;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint32_t n;
    std::cin >> n;

    std::vector<Vector<double, 2>> A;
    std::vector<Vector<double, 2>> reverse_A;
    for (int i = 0; i < n; ++i) {
        int32_t x, y;
        std::cin >> x >> y;

        A.push_back(Vector<double, 2>(x, y));
        reverse_A.push_back(Vector<double, 2>(-x, -y));
    }
    
    uint32_t m;
    std::cin >> m;
    std::vector<Vector<double, 2>> B;
    std::vector<Vector<double, 2>> reverse_B;
    for (int i = 0; i < m; ++i) {
        int32_t x, y;
        std::cin >> x >> y;
        
        B.push_back(Vector<double, 2>(x, y));
        reverse_B.push_back(Vector<double, 2>(-x, -y));
    }
    std::vector<Vector<double, 2>> C(BuildMinkowskiSum(A, reverse_B));
    std::vector<Vector<double, 2>> reverse_C(BuildMinkowskiSum(reverse_A, B));
    if (IncludeZero(C) && IncludeZero(reverse_C)) {
        std::cout << "YES" << std::endl;
    }
    else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}




