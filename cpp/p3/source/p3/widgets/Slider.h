#pragma once
#define NOMINMAX

#include <functional>
#include <string>

#include <p3/Node.h>

namespace p3 {

template <typename DataType>
class Slider : public Node {
public:
    using OnChange = std::function<void(DataType)>;

    Slider()
        : Node("Slider")
    {
        set_direction(Direction::Horizontal);
    }

    void render_impl(Context&, float width, float height) override;

    void set_on_change(OnChange);
    OnChange on_change() const;

    void update_content() override;

    void set_value(DataType);
    DataType value() const;

    void set_min(DataType);
    DataType min() const;

    void set_max(DataType);
    DataType max() const;

    void set_format(std::optional<std::string>);
    std::optional<std::string> const& format() const;

    Direction direction() const;
    void set_direction(Direction);

private:
    DataType _value = std::numeric_limits<DataType>::lowest();
    DataType _min = std::numeric_limits<DataType>::min();
    DataType _max = std::numeric_limits<DataType>::max();
    OnChange _on_change;
    Direction _direction = Direction::Horizontal;
    std::optional<std::string> _format = std::nullopt;
};

template <typename DataType>
inline void Slider<DataType>::set_on_change(OnChange on_change)
{
    _on_change = on_change;
}

template <typename DataType>
inline typename Slider<DataType>::OnChange Slider<DataType>::on_change() const
{
    return _on_change;
}

template <typename DataType>
inline void Slider<DataType>::set_value(DataType value)
{
    _value = value;
}

template <typename DataType>
inline DataType Slider<DataType>::value() const
{
    return _value;
}

template <typename DataType>
inline void Slider<DataType>::set_min(DataType value)
{
    _min = value;
}

template <typename DataType>
inline DataType Slider<DataType>::min() const
{
    return _min;
}

template <typename DataType>
inline void Slider<DataType>::set_max(DataType value)
{
    _max = value;
}

template <typename DataType>
inline DataType Slider<DataType>::max() const
{
    return _max;
}

template <typename DataType>
inline void Slider<DataType>::set_format(std::optional<std::string> format)
{
    _format = std::move(format);
}

template <typename DataType>
inline std::optional<std::string> const& Slider<DataType>::format() const
{
    return _format;
}

template <typename DataType>
inline Direction Slider<DataType>::direction() const
{
    return _direction;
}

template <typename DataType>
inline void Slider<DataType>::set_direction(Direction direction)
{
    _direction = direction;
}

}
