#pragma once

#include <memory>
#include <vector>
#include <tuple>

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
    constexpr Point(double x, double y, double z = 1.): coordinates{x, y, z} {}

    bool operator==(const Point& rhs) const
    { return coordinates == rhs.coordinates; };

    std::string type() const { return "point"; }

    const std::tuple<double, double, double> coordinates;
};

struct Line
{
    constexpr Line(Point first, Point second): vertices{first, second} {}

    bool operator==(const Line& rhs) const
    { return vertices == rhs.vertices; };

    std::string type() const { return "line"; }

    const std::pair<Point, Point> vertices;
};

struct Polygon
{
    Polygon(std::vector<Point> vertices): vertices{std::move(vertices)} {}
    Polygon(std::initializer_list<Point> init): vertices{std::move(init)} {}

    bool operator==(const Polygon& rhs) const
    { return vertices == rhs.vertices; };

    std::string type() const { return "polygon"; }

    const std::vector<Point> vertices;
};
