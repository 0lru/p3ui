#pragma once

#include <string>
#include <functional>

#include <p3/Node.h>

namespace p3
{

    template<typename DataType>
    class InputScalar : public Node
    {
    public:
        using OnChange = std::function<void(DataType)>;

        InputScalar() : Node("InputScalar") {};

        void render_impl(Context&, float width, float height) override;

        void set_on_change(OnChange);
        OnChange on_change() const;

        void update_content() override;

        void set_value(DataType);
        DataType value() const;

        void set_step(std::optional<DataType>);
        std::optional<DataType> const& step() const;

        void set_format(std::optional<std::string>);
        std::optional<std::string> const& format() const;
    protected:
        void dispose() override;

    private:
        DataType _value = std::numeric_limits<DataType>::lowest();
        std::optional<DataType> _step = std::nullopt;
        OnChange _on_change;
        std::optional<std::string> _format = std::nullopt;
    };

    template<typename DataType>
    inline void InputScalar<DataType>::set_on_change(OnChange on_change)
    {
        _on_change = on_change;
    }

    template<typename DataType>
    inline typename InputScalar<DataType>::OnChange InputScalar<DataType>::on_change() const
    {
        return _on_change;
    }

    template<typename DataType>
    inline void InputScalar<DataType>::set_value(DataType value)
    {
        _value = value;
    }

    template<typename DataType>
    inline void InputScalar<DataType>::dispose()
    {
        _on_change = nullptr;
        Node::dispose();
    }

    template<typename DataType>
    inline DataType InputScalar<DataType>::value() const
    {
        return _value;
    }

    template<typename DataType>
    inline void InputScalar<DataType>::set_step(std::optional<DataType> step)
    {
        _step = std::move(step);
    }

    template<typename DataType>
    inline std::optional<DataType> const& InputScalar<DataType>::step() const
    {
        return _step;
    }

    template<typename DataType>
    inline void InputScalar<DataType>::set_format(std::optional<std::string> format)
    {
        _format = std::move(format);
    }

    template<typename DataType>
    inline std::optional<std::string> const& InputScalar<DataType>::format() const
    {
        return _format;
    }

}
