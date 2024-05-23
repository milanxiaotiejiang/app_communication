//
// Created by Looper on 2023/5/30.
//

#ifndef APP_COMMUNICATION_VORONOI_HPP
#define APP_COMMUNICATION_VORONOI_HPP

#include <opencv2/opencv.hpp>
#include <boost/shared_ptr.hpp>
#include "vector"
#include "exploration/voronoi/nanoflann.hpp"

static bool DISPLAY_VORONOI_TRAJECTORY = false;

struct Pos {
    int x_;
    int y_;

    Pos(const int x, const int y) : x_(x), y_(y) {}

    inline bool operator==(const Pos &o) const {
        return x_ == o.x_ && y_ == o.y_;
    }

    inline void operator+=(const Pos &h) {
        x_ += h.x_;
        y_ += h.y_;
    }

    inline void operator-=(const Pos &h) {
        x_ -= h.x_;
        y_ -= h.y_;
    }

    inline Pos operator+(const Pos &h) const {
        Pos c = *this;
        c += h;
        return c;
    }

    inline Pos operator-(const Pos &h) const {
        Pos c = *this;
        c -= h;
        return c;
    }

    inline int dist2(const Pos &h) const {
        return (x_ - h.x_) * (x_ - h.x_) + (y_ - h.y_) * (y_ - h.y_);
    }

    inline int dist2() const {
        return x_ * x_ + y_ * y_;
    }

};

struct Cell {
    Pos pos_;
    int hops_;
    int sx_;
    int sy_;

    typedef boost::shared_ptr<Cell> Ptr;

    Cell() : pos_(0, 0), hops_(0), sx_(0), sy_(0) {}

    Cell(const int x, const int y) : pos_(x, y), hops_(1), sx_(0), sy_(0) {}

    inline void operator+=(const Pos &h) {
        hops_ += 1;
        sx_ += h.x_;
        sy_ += h.y_;
    }

    inline Cell operator+(const Pos &h) const {
        Cell c = *this;
        c += h;
        return c;
    }

    inline void set(const Cell &h) {
        hops_ = h.hops_;
        sx_ = h.sx_;
        sy_ = h.sy_;
    }

    inline void operator+=(const Cell &h) {
        hops_ += h.hops_;
        pos_ += h.pos_;
        assert(sx_ * h.sx_ >= 0);
        assert(sy_ * h.sy_ >= 0);
        sx_ += h.sx_;
        sy_ += h.sy_;
    }

    inline Cell operator+(const Cell &h) const {
        Cell c = *this;
        c += h;
        return c;
    }

    inline int dist2() const {
        return sx_ * sx_ + sy_ * sy_;
    }

    inline bool operator<(const Cell &o) const { return dist2() < o.dist2(); }

    inline explicit operator bool() const { return hops_ > 0; }

};

struct CellMap {
    std::vector<Cell> cells_;
    int w_;
    int h_;

    CellMap(const int w, const int h) : cells_(w * h), w_(w), h_(h) {
        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++)
                (*this)(x, y).pos_ = Pos(x, y);
    }

    int max() const {
        int m = 0;
        for (int x = 0; x < w_; x++)
            for (int y = 0; y < h_; y++)
                m = std::max((*this)(x, y).dist2(), m);
        return m;
    }

    inline bool valid(const Pos &p) const {
        return (p.x_ >= 0 && p.x_ < w_) && (p.y_ >= 0 && p.y_ < h_);
    }

    inline Cell &operator()(const int x, const int y) {
        assert(x >= 0 && x < w_);
        assert(y >= 0 && y < h_);
        return cells_[y * w_ + x];
    }

    inline const Cell &operator()(const int x, const int y) const {
        assert(x >= 0 && x < w_);
        assert(y >= 0 && y < h_);
        return cells_[y * w_ + x];
    }

    inline Cell &operator[](const Pos &p) {
        return (*this)(p.x_, p.y_);
    }

};

struct CmpDist2Ref {
    Pos r;

    explicit CmpDist2Ref(const Pos &r) : r(r) {}

    inline bool operator()(const Pos &a, const Pos &b) const { return a.dist2(r) < b.dist2(r); }
};

template<typename Type, typename Compare = std::less<Type> >
struct pless : public std::binary_function<Type *, Type *, bool> {
    bool operator()(const Type *x, const Type *y) const {
        if (x->dist2() == y->dist2()) {
            if (x->pos_.x_ == y->pos_.x_)
                return x->pos_.y_ > y->pos_.y_;
            return x->pos_.x_ > y->pos_.x_;
        }
        return x->dist2() > y->dist2();
    }
};

template<typename T>
struct PointCloud {
    struct Point {
        T x, y;

        Point() = default;

        Point(const T &x, const T &y) : x(x), y(y) {}
    };

    std::vector<Point> pts;

    inline size_t kdtree_get_point_count() const { return pts.size(); }

    inline T kdtree_distance(const T *p1, const size_t idx_p2, size_t /*size*/) const {
        const T d0 = p1[0] - pts[idx_p2].x;
        const T d1 = p1[1] - pts[idx_p2].y;
        return d0 * d0 + d1 * d1;
    }

    inline T kdtree_get_pt(const size_t idx, int dim) const {
        if (dim == 0) return pts[idx].x;
        else return pts[idx].y;
    }

    template<class BBOX>
    bool kdtree_get_bbox(BBOX & /*bb*/) const { return false; }

};

struct LineSegment {
    double length_;
    double effective_length_;
    std::vector<int> inds_;

    inline bool operator<(const LineSegment &o) const {
        return effective_length_ > o.effective_length_;
    }
};

template<class T>
struct TSPItem {
    Pos in_, out_;
    cv::Point2f in_dir_, out_dir_;
    T *ref_;

    TSPItem() : in_(0, 0), out_(0, 0), ref_(NULL) {}

    TSPItem(const Pos &a, const Pos &b, const cv::Point2f &ad, const cv::Point2f &bd, T *ref) :
            in_(a), out_(b), in_dir_(ad), out_dir_(bd), ref_(ref) {}

    TSPItem swap() const {
        return TSPItem(out_, in_, out_dir_, in_dir_, ref_);
    }
};

template<class T>
struct TSPTour {
    Pos start_;
    std::vector<T> items_;

    explicit TSPTour(const Pos &s) : start_(s) {}

    inline size_t size() const { return 2 * items_.size(); }

    inline double costs() const {
        const double relation = (M_PI / 0.2) * (0.05 / 0.3);

        double c = 0;
        Pos last = start_;
        cv::Point2f last_dir(0, 0);
        for (size_t i = 0; i < items_.size(); i++) {
            const double dist = std::sqrt(last.dist2(items_[i].in_));
            double rotation = 0;
            if (dist > 10) {
                Pos t = items_[i].in_ - last;
                cv::Point2f interm(t.x_, t.y_);
                interm *= 1. / dist;

                rotation = (1 + interm.dot(items_[i].in_dir_));
                rotation += (1 - last_dir.dot(interm));
            } else {
                rotation = (1 + last_dir.dot(items_[i].in_dir_));
            }

            c += dist + rotation * relation;
            last = items_[i].out_;
            last_dir = items_[i].out_dir_;
        }

        return c;
    }

    T get(const size_t i) const {
        assert(i / 2 < items_.size());

        if (i % 2 == 0) return items_[i / 2];
        return items_[i / 2].swap();
    }

    void set(const size_t i, const T &d) {
        if (items_.size() <= i / 2) items_.resize(i / 2 + 1);

        if (i % 2 == 0) items_[i / 2] = d;
        else items_[i / 2] = d.swap();
    }
};

template<class T>
struct TSPalgorithm {
    TSPTour<T> tour_;

    explicit TSPalgorithm(const Pos &p) : tour_(p) {}

    void optimize() {
        bool improved = true;
        time_t start;

        while (improved) {

            improved = false;
            double best_distance = tour_.costs();

            for (int i = 0; i < tour_.size() - 1; i += 2) {
                for (int k = i + 1; k < tour_.size(); k++) {
                    TSPTour<T> new_tour(tour_.start_);

                    swap2opt(new_tour, i, k);

                    const double new_distance = new_tour.costs();

                    if (new_distance < best_distance) {
                        improved = true;
                        tour_ = new_tour;
                        best_distance = new_distance;

                        i = tour_.size();
                        break;
                    }
                }
            }

        }
    }

    void swap2opt(TSPTour<T> &new_tour, const int &i, const int &k) {
        int c = 0;
        for (; c + 1 <= i - 1; c += 2) {
            new_tour.set(c, tour_.get(c));
        }

        int dec = 0;
        for (; c <= k; c += 2) {
            assert(dec <= k);
            new_tour.set(c, tour_.get(k - dec));
            dec += 2;
        }

        for (; c < tour_.size(); c += 2) {
            new_tour.set(c, tour_.get(c));
        }
    }

};

template<class Points>
void connect(const Points &pts, std::vector<int> &out, const cv::Mat &coverage, const Pos &start,
             const int min_segment_length = 2) {
    if (pts.size() == 0) return;
    else if (pts.size() == 1) {
        out.push_back(0);
        out.push_back(0);
        return;
    }

    std::vector<int> used(pts.size(), 0);

    const int USED_INC = 20;

    ///////////////////////////////////

    PointCloud<int> cloud;
    for (size_t i = 0; i < pts.size(); i++)
        cloud.pts.push_back(PointCloud<int>::Point(pts[i].x_, pts[i].y_));

    typedef
    nanoflann::KDTreeSingleIndexAdaptor<
            nanoflann::L2_Simple_Adaptor<int, PointCloud<int> >,
            PointCloud<int>,
            2
    > my_kd_tree_t;

    my_kd_tree_t index(2, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(10));
    index.buildIndex();

    int query_pt[2];

    std::vector<LineSegment> segs;
    for (size_t j = 0; j < used.size(); j++) {
        if (used[j] > 0) continue;

        LineSegment seg;
        seg.length_ = 0;

        size_t nexts[4] = {j};
        double last_lenghts[10];
        int num = 1;
        int nnn = 0;
        while (num > 0) {
            --num;
            size_t i = nexts[num];
            used[i] += USED_INC;

            {
                if (seg.inds_.size() > 0)
                    seg.length_ += std::sqrt(pts[seg.inds_.back()].dist2(pts[i]));
                last_lenghts[seg.inds_.size() % 10] = seg.length_;
                seg.inds_.push_back(i);
            }
            ++nnn;

            query_pt[0] = pts[nexts[num]].x_;
            query_pt[1] = pts[nexts[num]].y_;

            std::vector<std::pair<size_t, int> > ret_matches;
            nanoflann::SearchParams params;
            params.sorted = true;

            const size_t nMatches = index.radiusSearch(&query_pt[0], 5, ret_matches, params);
            for (int j = (int) nMatches - 1; j >= 0; j--) {
                if (used[ret_matches[j].first] > 0) continue;

                if (num >= 4) {
                    break;
                }

                if (num > 0) {
                    used[ret_matches[j].first] += USED_INC;
                } else {
                    nexts[num] = ret_matches[j].first;
                    ++num;
                }
            }

            if (seg.inds_.size() > 10 + min_segment_length) {
                Pos p1 = pts[seg.inds_[seg.inds_.size() - 1]];
                Pos p2 = pts[seg.inds_[seg.inds_.size() - 6]];
                Pos p3 = pts[seg.inds_[seg.inds_.size() - 10]];
                Pos d1 = p2 - p1;
                Pos d2 = p3 - p2;
                if (d1.x_ * d2.x_ + d1.y_ * d2.y_ < 10) {
                    std::vector<int> inds(seg.inds_.begin() + seg.inds_.size() - 6, seg.inds_.end());
                    double l = seg.length_ - last_lenghts[(seg.inds_.size() - 6) % 10];
                    seg.length_ = last_lenghts[(seg.inds_.size() - 6) % 10];
                    seg.effective_length_ = seg.length_;
                    seg.inds_.erase(seg.inds_.begin() + seg.inds_.size() - 5, seg.inds_.end());

                    if (seg.inds_.size() > 0 && seg.effective_length_ >= min_segment_length) segs.push_back(seg);

                    seg.inds_ = inds;
                    seg.length_ = l;
                }
            } else if (seg.length_ > 80) {
                seg.effective_length_ = seg.length_;
                if (seg.inds_.size() > 0 && seg.effective_length_ >= min_segment_length) segs.push_back(seg);
                seg = LineSegment();
                seg.length_ = 0;
            }
        }

        seg.effective_length_ = seg.length_;
        if (seg.inds_.size() > 0 && seg.effective_length_ >= min_segment_length) segs.push_back(seg);
    }

    TSPalgorithm<TSPItem<LineSegment> > opt(start);
    LineSegment segs_start = {};
    for (size_t i = 0; i < segs.size(); i++) {
        cv::Point2f dir_a(0, 0), dir_b(0, 0);
        if (segs[i].inds_.size() > 1) {
            const size_t off = std::min((size_t) 9, segs[i].inds_.size() - 1);
            dir_a = cv::Point2f(pts[segs[i].inds_.front()].x_ - pts[segs[i].inds_[off]].x_,
                                pts[segs[i].inds_.front()].y_ - pts[segs[i].inds_[off]].y_);
            dir_b = cv::Point2f(pts[segs[i].inds_.back()].x_ - pts[segs[i].inds_[segs[i].inds_.size() - off - 1]].x_,
                                pts[segs[i].inds_.back()].y_ - pts[segs[i].inds_[segs[i].inds_.size() - off - 1]].y_);
            dir_a *= 1. / cv::norm(dir_a);
            dir_b *= 1. / cv::norm(dir_b);

            assert(!(dir_a != dir_a));
            assert(!(dir_b != dir_b));
        }
        opt.tour_.set(2 * i, TSPItem<LineSegment>(pts[segs[i].inds_.front()], pts[segs[i].inds_.back()], dir_a, dir_b,
                                                  &segs[i]));
    }

    opt.optimize();

    Pos ll(start);
    for (auto &item: opt.tour_.items_) {
        if (item.in_ == pts[item.ref_->inds_.back()])
            std::reverse(item.ref_->inds_.begin(), item.ref_->inds_.end());

        ll = item.out_;

        for (int &ind: item.ref_->inds_)
            out.push_back(ind);
    }
}

class VoronoiMap {
    CellMap map_;
    std::vector<Cell *> centers_;
    int max_track_width_;
    int wall_offset_;

    typedef std::priority_queue<Cell *, std::vector<Cell *>, pless<Cell, std::greater<Cell> > > T_WAVE;

    enum {
        OCC = 100,
        FREE = 0,
        UNK = -1,
        SET = 3
    };

    inline int8_t &operator()(int8_t *occ, const int w, const int h, const Pos &c) {
        return occ[c.y_ * w + c.x_];
    }

    inline int add(int8_t *occ, const int w, const int h, const Pos &org, T_WAVE &wave) {
        const int NUM = 4;
        static const Pos dirs[NUM] = {Pos(-1, 0), Pos(0, -1), Pos(1, 0), Pos(0, 1)};

        const Cell &cur = map_[org];

        int n = 0;
        for (auto dir: dirs) {
            Pos p = org + dir;
            Cell t = cur;
            t += dir;
            if (map_.valid(p) && (*this)(occ, w, h, p) == FREE) {
                Cell *c = &map_[p];
                c->set(cur);
                *c += dir;
                wave.push(c);
                (*this)(occ, w, h, p) = SET;
                ++n;
            }
        }

        return n;
    }

public:

    VoronoiMap(int8_t *occ, const int w, const int h, const int max_track_width) : map_(w, h),
                                                                                   max_track_width_(max_track_width),
                                                                                   wall_offset_(0) {
        T_WAVE wave;

        if (max_track_width_ % 2 == 1) --max_track_width_;

        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++) {
                if (occ[y * w + x] == OCC) {
                    const Pos p(x, y);
                    wave.push(&map_[p]);
                    (*this)(occ, w, h, p) = SET;
                } else if (occ[y * w + x] != FREE && occ[y * w + x] != UNK)
                    std::cout << occ[y * w + x] << std::endl;

            }

        while (!wave.empty()) {
            int added = add(occ, w, h, wave.top()->pos_, wave);

            if (added == 0 && wave.top()->hops_ > 0 && wave.top()->dist2() >= max_track_width_ * max_track_width_ * 2) {
                centers_.push_back(wave.top());
            }

            wave.pop();
        }

    }

    int operator()(const int x, const int y) const {
        if (x < 1 || y < 1 || x >= map_.w_ - 1 || y >= map_.h_ - 1)
            return -1;

        if (!map_(x, y).sx_ && !map_(x, y).sy_)
            return 0; //obstacle

        if (map_(x, y).hops_ <= 0) return 0;

        const float V = std::abs(
                (float) fmod(std::sqrt((float) map_(x, y).dist2()) - (float) wall_offset_, (float) max_track_width_) -
                (float) max_track_width_ / 2.f);
        if (V < 0.45f) return 2222;

        const int xx = roundf(float(map_(x, y).sx_) / std::max(std::abs(map_(x, y).sx_), std::abs(map_(x, y).sy_)));
        const int yy = roundf(float(map_(x, y).sy_) / std::max(std::abs(map_(x, y).sx_), std::abs(map_(x, y).sy_)));

        const int xx2 = roundf(
                float(map_(x, y).sx_) * 2 / std::max(std::abs(map_(x, y).sx_), std::abs(map_(x, y).sy_)));
        const int yy2 = roundf(
                float(map_(x, y).sy_) * 2 / std::max(std::abs(map_(x, y).sx_), std::abs(map_(x, y).sy_)));

        const float v[5] = {
                std::abs((float) fmod(std::sqrt((float) map_(x + xx, y + yy).dist2()) - (float) wall_offset_,
                                      (float) max_track_width_) - (float) max_track_width_ / 2.f),
                V,
                std::abs((float) fmod(std::sqrt((float) map_(x - xx, y - yy).dist2()) - (float) wall_offset_,
                                      (float) max_track_width_) - (float) max_track_width_ / 2.f),

                std::abs((float) fmod(std::sqrt((float) map_(x - xx2, y - yy2).dist2()) - (float) wall_offset_,
                                      (float) max_track_width_) - (float) max_track_width_ / 2.f),
                std::abs((float) fmod(std::sqrt((float) map_(x + xx2, y + yy2).dist2()) - (float) wall_offset_,
                                      (float) max_track_width_) - (float) max_track_width_ / 2.f)
        };

        if (v[1] < v[3] && v[1] < v[4] && !(v[0] < v[4] && v[0] < v[1]) && !(v[2] < v[1] && v[2] < v[3]))
            return 2222;

        return 1000;

    }

    template<class PathVector>
    void
    generatePath(cv::Mat room_map, PathVector &path, const cv::Mat &coverage, const int start_x, const int start_y) {
        std::vector<Pos> pts;
        size_t num = 0;

        for (int x = 0; x < map_.w_; x++) {
            for (int y = 0; y < map_.h_; y++) {
                if (map_(x, y).dist2() > std::pow(wall_offset_, 2) && (*this)(x, y) == 2222) {
                    pts.emplace_back(x, y);
                    ++num;
                }
            }
        }

        for (const auto &item: pts) {
            cv::circle(room_map, cv::Point(item.x_, item.y_), 1, cv::Scalar(200), cv::FILLED);
        }

        if (DISPLAY_VORONOI_TRAJECTORY) {
            cv::imshow("voronoi_path", room_map);
            cv::waitKey();
        }

        std::sort(pts.begin(), pts.end(), CmpDist2Ref(Pos(start_x, start_y)));

        num = 0;

        std::vector<int> out;
        connect(pts, out, coverage, Pos(start_x, start_y));

        struct OptVal {
            struct OptValCompare {
                bool operator()(const typename std::list<OptVal>::iterator &lhs,
                                const typename std::list<OptVal>::iterator &rhs) const {
                    if (lhs->val == rhs->val) return lhs->val_sec < rhs->val_sec;
                    return lhs->val < rhs->val;
                }
            };

            typedef std::multimap<typename std::list<OptVal>::iterator, char, OptValCompare> MM;

            double val;
            double val_sec;
            int ind;
            typename MM::iterator mm;
        };

        auto ValueFunc = [pts, out, this](typename std::list<OptVal>::iterator cur) {
            typename std::list<OptVal>::iterator b = cur;
            typename std::list<OptVal>::iterator a = cur;
            --b;
            ++a;

            auto dx1 = pts[out[b->ind]].x_ - pts[out[cur->ind]].x_;
            auto dy1 = pts[out[b->ind]].y_ - pts[out[cur->ind]].y_;
            auto dx2 = pts[out[a->ind]].x_ - pts[out[cur->ind]].x_;
            auto dy2 = pts[out[a->ind]].y_ - pts[out[cur->ind]].y_;
            auto dx3 = pts[out[a->ind]].x_ - pts[out[b->ind]].x_;
            auto dy3 = pts[out[a->ind]].y_ - pts[out[b->ind]].y_;

            auto Dist = [](int dx, int dy) { return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2)); };
            auto l = Dist(dx1, dy1) + Dist(dx2, dy2);
            cur->val = (l - Dist(dx3, dy3)) * Dist(dx3, dy3);
            cur->val_sec = Dist(dx3, dy3);
        };

        std::list<OptVal> vals;
        for (size_t i = 0; i < out.size(); i++) {
            OptVal v;
            v.ind = i - 1;
            v.val = 0;
            v.val_sec = 0;
            vals.push_back(v);
        }

        typename OptVal::MM vals_sorted;
        for (typename std::list<OptVal>::iterator it = ++vals.begin(); it != --vals.end(); ++it) {
            ValueFunc(it);
            it->mm = vals_sorted.insert(std::pair<typename std::list<OptVal>::iterator, char>(it, 0));
        }

        const double THR = 8;
        while (!vals_sorted.empty()) {
            typename std::list<OptVal>::iterator it = vals_sorted.begin()->first;
            if (it->val >= THR) break;

            typename std::list<OptVal>::iterator a = it, b = it;
            ++a;
            --b;

            vals_sorted.erase(it->mm);
            if (a != --vals.end()) vals_sorted.erase(a->mm);
            if (b != vals.begin()) vals_sorted.erase(b->mm);
            vals.erase(it);

            if (a != --vals.end()) {
                ValueFunc(a);
                a->mm = vals_sorted.insert(std::pair<typename std::list<OptVal>::iterator, char>(a, 0));
            }
            if (b != vals.begin()) {
                ValueFunc(b);
                b->mm = vals_sorted.insert(std::pair<typename std::list<OptVal>::iterator, char>(b, 0));
            }
        }

        path.resize(vals.size());
        size_t i = 0;
        for (typename std::list<OptVal>::iterator it = vals.begin(); it != vals.end(); ++it, ++i) {
            path[i].x = pts[out[it->ind]].x_;
            path[i].y = pts[out[it->ind]].y_;
        }

    }

};


#endif //APP_COMMUNICATION_VORONOI_HPP
