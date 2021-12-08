
#pragma once

#include <string>
#include <functional>

#include "Node.h"

namespace p3
{

    class Table : public Node
    {
    public:
        Table();

        // 
        // column sepcification
        class Column : public Synchronizable
        {
        public:
            std::string const& title() const { return _title; }
            void set_title(std::string title){ _title = std::move(title); }

            std::optional<Length> const& width() const { return _width; }
            void set_width(std::optional<Length> width) { _width = std::move(width); }

            float weight() const { return _weight; }
            void set_weight(float weight) { _weight = weight; }

        private:
            std::string _title;
            std::optional<Length> _width;
            float _weight = 1.0f;
        };

        void set_columns(std::vector<std::shared_ptr<Column>>);
        std::vector<std::shared_ptr<Column>> columns() const;

        int freezed_columns() const;
        void set_freezed_columns(int);

        int freezed_rows() const;
        void set_freezed_rows(int);

        bool resizeable() const;
        void set_resizeable(bool);

        bool reorderable() const;
        void set_reorderable(bool);

        //
        // a row ..
        class Row : public Node
        {
        public:
            Row() : Node("Row") {}
            void render_impl(Context&, float width, float height) override;
        };

        void render_impl(Context&, float width, float height) override;
        void update_content() override;

    private:
        std::vector<std::shared_ptr<Column>> _columns;
        int _freezed_columns = 0;
        int _freezed_rows = 1;
        bool _resizeable = false;
        bool _reorderable = false;
    };

}
