#pragma once

#include "coordinates.h"

#include <cairomm/context.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include <tuple>

using WindowMapping = std::function<Coordinates(Coordinates)>;

class cairo_context_guard final
{
    public:
        explicit cairo_context_guard(const Cairo::RefPtr<Cairo::Context>& ctx):
            ctx{ctx} { ctx->save(); }

        ~cairo_context_guard() { ctx->restore(); }

    private:
        const Cairo::RefPtr<Cairo::Context>& ctx;
};

#if __cplusplus < 201402L
namespace std {

template<class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}
#endif

class Shape
{
    public:
        constexpr Shape() = default;
        virtual ~Shape() = default;

        Shape(const Shape& other): ptr{other.copy()} {}
        Shape(Shape&&) = default;

        template<class T>
        Shape(T data): ptr{std::make_unique<model<T>>(std::move(data))} {}

        Shape& operator=(const Shape& rhs) { ptr = rhs.copy(); return *this; };
        Shape& operator=(Shape&&) = default;

        template<class T>
        Shape& operator=(T data)
        { ptr = std::make_unique<model<T>>(std::move(data)); return *this; }

        bool operator==(const Shape& other) const
        { return *ptr == *other.ptr; }

        bool operator!=(const Shape& other) const
        { return not (*ptr == *other.ptr); }

        std::vector<Coordinates> coordinates() const
        { return ptr ? ptr->coordinates() : std::vector<Coordinates>{}; }

        std::vector<Coordinates> normalized() const
        { return ptr ? ptr->normalized() : std::vector<Coordinates>{}; }

        void normalized(std::vector<Coordinates> normalized)
        { if (ptr) ptr->normalized(normalized); }

        std::vector<Coordinates> vertices() const
        { return ptr ? ptr->vertices() : std::vector<Coordinates>{}; }

        size_t hash() const {
            return std::hash<decltype(ptr)>{}(ptr);
        }

        virtual std::string type() const {
            return ptr ? ptr->type() : "null";
        }

        template<class T>
        const T& as() const { return dynamic_cast<T&>(*ptr); }

    private:
        struct concept {
            virtual ~concept() = default;
            virtual bool operator==(const concept&) const = 0;
            virtual std::unique_ptr<concept> copy() const = 0;
            virtual std::vector<Coordinates> coordinates() const = 0;
            virtual std::vector<Coordinates> normalized() const = 0;
            virtual void normalized(std::vector<Coordinates> normalized) = 0;
            virtual std::vector<Coordinates> vertices() const = 0;
            virtual std::string type() const = 0;
        };

        template<class T>
        struct model final: concept {
            model(T data): data{std::move(data)} {}

            bool operator==(const concept& rhs) const override
            {
                auto cast = dynamic_cast<const model*>(&rhs);
                return cast && data == cast->data;
            }

            std::unique_ptr<concept> copy() const override
            { return std::make_unique<model<T>>(data); }

            std::vector<Coordinates> coordinates() const override
            { return data.coordinates(); }

            std::vector<Coordinates> normalized() const override
            { return data.normalized(); }

            void normalized(std::vector<Coordinates> normalized) override
            { data.normalized(std::move(normalized)); }

            std::vector<Coordinates> vertices() const override
            { return data.vertices(); }

            std::string type() const override
            { return data.type(); }

            T data;
        };

        std::unique_ptr<concept> copy() const { return ptr->copy(); }

        std::unique_ptr<concept> ptr;
};

namespace std {

template<>
struct hash<Shape>
{
    size_t operator()(const Shape& s) const {
        return s.hash();
    }
};

}

struct Point
{
    constexpr Point(double x, double y, double z = 1.):
        coordinates_{x, y, z},
        normalized_{x, y, z} {}

    constexpr Point(Coordinates coordinates):
        coordinates_{coordinates},
        normalized_{std::move(coordinates)} {}

    bool operator==(const Point& rhs) const
    { return coordinates_ == rhs.coordinates_; };

    std::vector<Coordinates> coordinates() const
    { return {coordinates_}; }

    std::vector<Coordinates> normalized() const
    { return {normalized_}; }

    void normalized(std::vector<Coordinates> normalized)
    { normalized_ = std::move(normalized[0]); }

    std::vector<Coordinates> vertices() const
    { return normalized(); }

    std::string type() const { return "point"; }

    const Coordinates coordinates_;
    Coordinates normalized_;
};

struct Line
{
    constexpr Line(Coordinates first, Coordinates second):
        vertices_{first, second},
        normalized_{first, second} {}

    bool operator==(const Line& rhs) const
    { return vertices_ == rhs.vertices_; };

    std::vector<Coordinates> coordinates() const
    { return {vertices_.first, vertices_.second}; }

    std::vector<Coordinates> normalized() const
    { return {normalized_.first, normalized_.second}; }

    void normalized(std::vector<Coordinates> normalized)
    { normalized_ = std::make_pair(std::move(normalized[0]), std::move(normalized[1])); }

    std::vector<Coordinates> vertices() const
    { return normalized(); }

    std::string type() const { return "line"; }

    const std::pair<Coordinates, Coordinates> vertices_;
    std::pair<Coordinates, Coordinates> normalized_;
};

std::ostream& operator<<(std::ostream& os, const Line& rhs);

struct Polygon
{
    Polygon(std::vector<Coordinates> vertices):
        vertices_{vertices}, normalized_{std::move(vertices)} {}

    Polygon(std::initializer_list<Coordinates> init):
        vertices_{init}, normalized_{std::move(init)} {}

    bool operator==(const Polygon& rhs) const
    { return vertices_ == rhs.vertices_; };

    std::vector<Coordinates> coordinates() const
    { return vertices_; }

    std::vector<Coordinates> normalized() const
    { return normalized_; }

    void normalized(std::vector<Coordinates> normalized)
    { normalized_ = std::move(normalized); }

    std::vector<Coordinates> vertices() const
    { return normalized(); }

    std::string type() const { return "polygon"; }

    const std::vector<Coordinates> vertices_;
    std::vector<Coordinates> normalized_;
};

struct Bezier
{
    constexpr Bezier(std::array<Coordinates, 4> vertices):
        vertices_{vertices}, normalized_{std::move(vertices)} {}

    bool operator==(const Bezier& rhs) const
    { return vertices_ == rhs.vertices_; };

    std::vector<Coordinates> coordinates() const
    { return {vertices_.begin(), vertices_.end()}; }

    std::vector<Coordinates> normalized() const
    { return {normalized_.begin(), normalized_.end()}; }

    void normalized(std::vector<Coordinates> normalized)
    { std::move(normalized.begin(), normalized.end(), normalized_.begin()); }

    std::vector<Coordinates> vertices() const
    {
        constexpr double k = 500;
        std::vector<Coordinates> vertices;
        vertices.reserve(k);

        using Row = std::array<int, 4>;
        constexpr std::array<Row, 4> matrix = {
            Row{-1,  3, -3,  1},
            Row{ 3, -6,  3,  0},
            Row{-3,  3,  0,  0},
            Row{ 1,  0,  0,  0},
        };

        constexpr double inc = 1.0 / k;
        vertices.push_back(normalized_.front());

        for (auto i = 1; i < k - 1; ++i) {
            auto t = inc * i;

            const std::array<double, 4> t_matrix = {t * t * t, t * t, t, 1.0};
            double x = 0.0, y = 0.0, z = 0.0;

            for (auto i = 0; i < 4; ++i) {
                auto sum = 0.0;
                for (auto j = 0; j < 4; ++j) {
                    sum += t_matrix[j] * matrix[j][i];
                }
                x += sum * std::get<X>(normalized_[i]);
                y += sum * std::get<Y>(normalized_[i]);
                z += sum * std::get<Z>(normalized_[i]);
            }

            vertices.emplace_back(x, y, z);
        }

        vertices.push_back(normalized_.back());
        return vertices;
    }

    std::string type() const { return "bezier"; }

    const std::array<Coordinates, 4> vertices_;
    std::array<Coordinates, 4> normalized_;
};
