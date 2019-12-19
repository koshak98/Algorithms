#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include <unordered_map>
#include <queue>
#include <unordered_set>



constexpr double epsilon = 1e-9;

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
    
    bool operator < (const Vector& rhs) const {
        if (coordinates[2] == rhs.coordinates[2]) {
            if (coordinates[1] == rhs.coordinates[1]) {
                return coordinates[0] < rhs.coordinates[0];
            }
            return coordinates[1] < rhs.coordinates[1];
        }
        return coordinates[2] < rhs.coordinates[2];
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
Vector<SizeType, dim> CrossProduct(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2) {
    const SizeType* coor1 = vec1.GetCoordinates();
    const SizeType* coor2 = vec2.GetCoordinates();
    Vector<SizeType,dim> ans(coor1[1] * coor2[2] - coor1[2] * coor2[1], coor2[0] * coor1[2] - coor1[0] * coor2[2], coor1[0] * coor2[1] - coor1[1] * coor2[0]);
    return ans;
}

template<class SizeType, uint32_t dim>
bool VecProduct3D(const Vector<SizeType, dim>& vec1, const Vector<SizeType, dim>& vec2, const Vector<SizeType, dim>& vec3) {
    
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
struct OppositeVertices {
    Vector<SizeType, dim> first;
    Vector<SizeType, dim> second;
};



template<class SizeType, uint32_t dim>
Vector<SizeType, dim> FindMinDot(const std::vector<Vector<SizeType, dim>>& dots) {
    Vector<SizeType, dim> A(dots[0]);
    const SizeType* coor_ans = A.GetCoordinates();
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
//The same as in Delaunay triangulation
struct Edge {
    int32_t first_;
    int32_t second_;
    Edge(): first_(-1), second_(-1) {}
    Edge(int32_t index1, int32_t index2) : first_(index1), second_(index2) {}


    bool operator == (const Edge& other) const {
        return (first_ == other.first_) && (second_ == other.second_);
    }
 
};
 
template<> struct std::hash<Edge> {
    size_t operator()(const Edge& edge) const {
        return (std::hash<int32_t>()(edge.first_)) ^ (std::hash<int32_t>()(edge.second_));
    }
};


 
struct ConvexHull {
    ConvexHull(int32_t first_i, int32_t second_i, int32_t third_i): first_(first_i), second_(second_i), third_(third_i) {}
    ConvexHull() : ConvexHull(-1, -1, -1) {}
 
    bool operator<(const ConvexHull& other) const noexcept {
        if (first_ == other.first_) {
            if (second_ == other.second_) {
                return third_ < other.third_;
            }
            return second_ < other.second_;
        }
        return first_ < other.first_;
    }
    bool operator==(const ConvexHull& other) const noexcept {
        return first_ == other.first_ && second_ == other.second_ && third_ == other.third_;
    }
    template<class SizeType>
    void InitializeConvexHull(const std::vector<Vector<SizeType, 3>>& points, int32_t first_i, int32_t second_i, int32_t third_i);
 
    Edge GetFirstEdge() const noexcept { return Edge(first_, second_); }
    Edge GetSecondEdge() const  noexcept { return Edge(second_, third_); }
    Edge GetThirdEdge() const noexcept { return Edge(third_, first_); }
 
    void Sort() noexcept {
        if (second_ < third_ && second_ < first_) {
            std::swap(first_, second_);
            std::swap(second_, third_);
        } else if (third_ < second_ && third_ < first_) {
            std::swap(first_, third_);
            std::swap(second_, third_);
        }
    }
 
    int32_t first_;
    int32_t second_;
    int32_t third_;
 
    friend std::hash<ConvexHull>;
};
 
template<class SizeType>
void ConvexHull::InitializeConvexHull(const std::vector<Vector<SizeType, 3>>& points, int32_t first_i, int32_t second_i, int32_t third_i) {
    ConvexHull tmp_ConvexHull(first_i, second_i, third_i);
    tmp_ConvexHull.Sort();
    first_i = tmp_ConvexHull.first_;
    second_i = tmp_ConvexHull.second_;
    third_i = tmp_ConvexHull.third_;

    Vector<SizeType, 3> basis1(points[second_i] - points[first_i]);
    Vector<SizeType, 3> basis2(points[third_i] - points[second_i]);
    Vector<SizeType, 3> normal_vector = CrossProduct(basis2, basis1);
    normal_vector /= Module(normal_vector);
    int32_t different_point_i = -1;
    int32_t size = points.size();
    for (int32_t i = 0; i < size; ++i) {
        if (i != first_i && i != second_i && i != third_i) {
            different_point_i = i;
            break;
        }
    }
 
    bool counter_clockwise = true;
    if (DotProduct(normal_vector, points[different_point_i] - points[first_i]) > -epsilon) {
        normal_vector -= normal_vector;
        normal_vector -= normal_vector;
        counter_clockwise = false;
    }

    if (!counter_clockwise) {
        first_ = first_i;
        second_ = third_i;
        third_ = second_i;
    } else {
        first_ = first_i;
        second_ = second_i;
        third_ = third_i;
    }
}
 
template<>
struct std::hash<ConvexHull> {
    size_t operator() (const ConvexHull& ConvexHull) const {
        return (std::hash<int32_t>()(ConvexHull.first_)) ^
               (std::hash<int32_t>()(ConvexHull.second_) ^
                (std::hash<int32_t>()(ConvexHull.third_)));
    }
};

template<class SizeType>
int32_t find_first_point(const std::vector<Vector<SizeType, 3>>& points) {
    int32_t min_i = 0, size = points.size();
 
    for (int32_t i = 1; i < size; ++i) {
        if (points[i] < points[min_i]) {
            min_i = i;
        }
    }
 
    return min_i;
}
 template<class SizeType>
int32_t find_second_point(const std::vector<Vector<SizeType, 3>>& points, int32_t first_i) {
    int32_t size = points.size();
    int32_t second_i = -1;
    double max_cos = -2.0;
 
    for (int32_t i = 0; i < size; ++i) {
        if (i != first_i) {
            Vector<SizeType, 3> diff(points[i] - points[first_i]);
            const SizeType* coordinates = diff.GetCoordinates();
            SizeType cur_cos = sqrt(coordinates[0] * coordinates[0] + coordinates[1] * coordinates[1]) / Module(diff);
            if (max_cos < cur_cos) {
                max_cos = cur_cos;
                second_i = i;
            }
        }
    }
 
    return second_i;
}
 template<class SizeType>
int32_t find_point(const std::vector<Vector<SizeType, 3>>& points, int32_t first_i, int32_t second_i, int32_t third_i = -1) {
    int32_t size = points.size();
    int32_t new_point = -1;
    Vector<SizeType, 3> edge_vector(points[second_i] - points[first_i]);
 
    for (int32_t i = 0; i < size; ++i) {
        if (i != first_i && i != second_i && i != third_i) {
            if (new_point == -1) {
                new_point = i;
                continue;
            }
 
            Vector<SizeType, 3> cur_outer(points[new_point] - points[first_i]);
            Vector<SizeType, 3> cur_test(points[i] - points[first_i]);
 
            Vector<SizeType, 3> normal_vector = CrossProduct(cur_outer, cur_test);
            if (DotProduct(normal_vector, edge_vector) > -epsilon) {
                new_point = i;
            }
        }
    }
 
    assert(new_point != -1);
 
    return new_point;
}
 
void insert_edges(std::unordered_set<Edge>& processed_edges, const ConvexHull& ConvexHull) {
    processed_edges.insert(ConvexHull.GetFirstEdge());
    processed_edges.insert(ConvexHull.GetSecondEdge());
    processed_edges.insert(ConvexHull.GetThirdEdge());
}
 
 template<class SizeType>
ConvexHull TryToAddNewConvexHull(std::unordered_set<Edge>& processed_edges,
        const std::vector<Vector<SizeType, 3>>& points,
        Edge edge,
        int32_t third_i) {
        std::swap(edge.first_, edge.second_);
        ConvexHull result_ConvexHull;
        if (!processed_edges.count(edge)) {
            int32_t point_to_add_i = find_point(points, edge.first_, edge.second_, third_i);
            result_ConvexHull = ConvexHull(edge.first_, edge.second_, point_to_add_i);
            result_ConvexHull.Sort();
    }
 
    return result_ConvexHull;
}
template<class SizeType, uint32_t dim>
void DoOneTest(std::vector<Vector<SizeType, dim>> dots) {
    ConvexHull answer;
    FindFirstConvexHull(dots, answer);
}

template<class SizeType>
std::vector<ConvexHull> FindConvexHull(const std::vector<Vector<SizeType, 3>>& points) {
    int32_t first_i = find_first_point(points);
    int32_t second_i = find_second_point(points, first_i);
    int32_t third_i = find_point(points, first_i, second_i);
 
    ConvexHull initial_ConvexHull;
    initial_ConvexHull.InitializeConvexHull(points, first_i, second_i, third_i);
 
    std::queue<ConvexHull> queue;
    queue.push(initial_ConvexHull);
    std::unordered_set<Edge> processed_edges;
    initial_ConvexHull.Sort();
    insert_edges(processed_edges, initial_ConvexHull);
    std::vector<ConvexHull> ConvexHulls;
 
    while(!queue.empty()) {
        ConvexHull cur_ConvexHull = queue.front();
        queue.pop();
        ConvexHulls.push_back(cur_ConvexHull);
 
        Edge edge1 = cur_ConvexHull.GetFirstEdge();
        Edge edge2 = cur_ConvexHull.GetSecondEdge();
        Edge edge3 = cur_ConvexHull.GetThirdEdge();
 
        ConvexHull tmp_ConvexHull;
        ConvexHull tmp_result;
 
        tmp_result = TryToAddNewConvexHull(processed_edges, points, edge1, edge2.second_);
        if (!(tmp_result == tmp_ConvexHull)) {
            queue.push(tmp_result);
            insert_edges(processed_edges, tmp_result);
        }
        tmp_result = TryToAddNewConvexHull(processed_edges, points, edge2, edge3.second_);
        if (!(tmp_result == tmp_ConvexHull)) {
            queue.push(tmp_result);
            insert_edges(processed_edges, tmp_result);
        }
        tmp_result = TryToAddNewConvexHull(processed_edges, points, edge3, edge1.second_);
        if (!(tmp_result == tmp_ConvexHull)) {
            queue.push(tmp_result);
            insert_edges(processed_edges, tmp_result);
        }
    }

    std::sort(ConvexHulls.begin(), ConvexHulls.end(), std::less<ConvexHull>());
 
    return ConvexHulls;
}



int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int32_t m;
    std::cin >> m;
 
    for (int32_t i = 0; i < m; ++i) {
        int32_t n = 0;
        std::cin >> n;
 
        std::vector<Vector<int32_t, 3>> points(n);
        for (int32_t j = 0; j < n; ++j) {
            double x, y, z;
            std::cin >> x >> y >> z;
            points[j] = Vector<int32_t, 3>(x, y, z);
        }
 
        std::vector<ConvexHull> ConvexHulls = FindConvexHull(points);
 
        int32_t size = ConvexHulls.size();
        std::cout << size << std::endl;
 

        const int32_t number_of_points_in_ConvexHull = 3;
        for (const auto& item : ConvexHulls) {
            std::cout << number_of_points_in_ConvexHull << ' '
                      << item.first_ << ' ' << item.second_ << ' ' << item.third_ << std::endl;
        }
    }
    return 0;
}
