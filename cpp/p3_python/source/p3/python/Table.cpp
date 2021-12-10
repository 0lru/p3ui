#include "p3ui.h"

#include <p3/widgets/Table.h>


namespace p3::python
{

    void Definition<Table>::apply(py::module& module)
    {
        auto table = py::class_<Table, Node, std::shared_ptr<Table>>(module, "Table");
        table.def(py::init<>([](py::kwargs kwargs) {
            auto table = std::make_shared<Table>();
            ArgumentParser<Node>()(kwargs, *table);
            assign(kwargs, "columns", *table, &Table::set_columns);
            assign(kwargs, "resizeable", *table, &Table::set_resizeable);
            assign(kwargs, "reorderable", *table, &Table::set_reorderable);
            assign(kwargs, "reorderable", *table, &Table::set_reorderable);
            assign(kwargs, "freezed_columns", *table, &Table::set_freezed_columns);
            assign(kwargs, "freezed_rows", *table, &Table::set_freezed_rows);
            return table;
        }));
        def_method(table, "add", &Table::add);
        def_method(table, "insert", &Table::insert);
        def_method(table, "remove", &Table::remove);
        def_property(table, "columns", &Table::columns, &Table::set_columns);
        def_property(table, "resizeable", &Table::resizeable, &Table::set_resizeable);
        def_property(table, "reorderable", &Table::reorderable, &Table::set_reorderable);
        def_property(table, "freezed_columns", &Table::freezed_columns, &Table::set_freezed_columns);
        def_property(table, "freezed_rows", &Table::freezed_rows, &Table::set_freezed_rows);

        py::class_<Table::Column, std::shared_ptr<Table::Column>> column(table, "Column");
        column.def(py::init<>([](std::string title, py::kwargs kwargs) {
            auto column = std::make_shared<Table::Column>();
            column->set_title(std::move(title));
            assign(kwargs, "width", *column, &Table::Column::set_width);
            assign(kwargs, "weight", *column, &Table::Column::set_weight);
            return column;
        }), py::arg("title"));
        def_property(column, "title", &Table::Column::title, &Table::Column::set_title);
        def_property(column, "width", &Table::Column::width, &Table::Column::set_width);
        def_property(column, "weight", &Table::Column::weight, &Table::Column::set_weight);

        auto row = py::class_<Table::Row, Node, std::shared_ptr<Table::Row>>(table, "Row");
        row.def(py::init<>([](py::kwargs kwargs) {
            auto row = std::make_shared<Table::Row>();
            ArgumentParser<Node>()(kwargs, *row);
            return row;
        }));
        def_method(row, "add", &Table::Row::add);
        def_method(row, "insert", &Table::Row::insert);
        def_method(row, "remove", &Table::Row::remove);
    }

}