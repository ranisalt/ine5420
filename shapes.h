#pragma once

#include <cairomm/context.h>

#include <cmath>
#include <functional>
#include <memory>
#include <vector>
#include <tuple>

using Coordinates = std::tuple<double, double, double>;
using WindowMapping = std::function<Coordinates(Coordinates)>;

class cairo_context_guard final
{
    public:
        cairo_context_guard(const Cairo::RefPtr<Cairo::Context>& ctx): ctx{ctx}
        { ctx->save(); }

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

        bool draw(const Cairo::RefPtr<Cairo::Context>& ctx,
                  const WindowMapping& window) const
        { return ptr ? ptr->draw(ctx, window) : true; }

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
            virtual bool draw(const Cairo::RefPtr<Cairo::Context>&,
                              const WindowMapping&) const = 0;
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

            std::vector<Coordinates> coordinates() const
            { return data.coordinates(); }

            bool draw(const Cairo::RefPtr<Cairo::Context>& ctx,
                      const WindowMapping& window) const override
            { return data.draw(ctx, window); }

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
        coordinates_{x, y, z} {}
    constexpr Point(Coordinates coordinates):
        coordinates_{std::move(coordinates)} {}

    bool operator==(const Point& rhs) const
    { return coordinates_ == rhs.coordinates_; };

    std::vector<Coordinates> coordinates() const
    { return {coordinates_}; }

    bool draw(const Cairo::RefPtr<Cairo::Context>& ctx,
              const WindowMapping& window) const
    {
        cairo_context_guard guard{ctx};
        double x, y;
        std::tie(x, y, std::ignore) = window(coordinates_);
        constexpr auto tau = std::atan(1) * 8;
        ctx->arc(x, y, 1., 0, tau);
        ctx->stroke();
        return true;
    }

    std::string type() const { return "point"; }

    const Coordinates coordinates_;
};

struct Line
{
    constexpr Line(Coordinates first, Coordinates second):
        vertices{first, second} {}

    bool operator==(const Line& rhs) const
    { return vertices == rhs.vertices; };

    std::vector<Coordinates> coordinates() const
    { return {vertices.first, vertices.second}; }

    bool draw(const Cairo::RefPtr<Cairo::Context>& ctx,
              const WindowMapping& window) const
    {
        cairo_context_guard guard{ctx};
        double x, y;
        std::tie(x, y, std::ignore) = window(vertices.first);
        ctx->move_to(x, y);
        std::tie(x, y, std::ignore) = window(vertices.second);
        ctx->line_to(x, y);
        ctx->stroke();
        return true;
    }

    std::string type() const { return "line"; }

    const std::pair<Coordinates, Coordinates> vertices;
};

struct Polygon
{
    Polygon(std::vector<Coordinates> vertices):
        vertices{std::move(vertices)} {}
    Polygon(std::initializer_list<Coordinates> init):
        vertices{std::move(init)} {}

    bool operator==(const Polygon& rhs) const
    { return vertices == rhs.vertices; };

    std::vector<Coordinates> coordinates() const
    { return vertices; }

    bool draw(const Cairo::RefPtr<Cairo::Context>& ctx,
              const WindowMapping& window) const
    {
        cairo_context_guard guard{ctx};
        auto begin = std::begin(vertices);
        double x, y;
        std::tie(x, y, std::ignore) = window(*begin);
        ctx->move_to(x, y);
        for (auto it = begin + 1; it != std::end(vertices); ++it) {
            std::tie(x, y, std::ignore) = window(*it);
            ctx->line_to(x, y);
        }
        begin = std::begin(vertices);
        std::tie(x, y, std::ignore) = window(*begin);
        ctx->line_to(x, y);
        ctx->stroke();
        return true;
    }

    std::string type() const { return "polygon"; }

    const std::vector<Coordinates> vertices;
};
