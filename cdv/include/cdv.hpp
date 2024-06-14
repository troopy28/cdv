#ifndef CDV_HPP
#define CDV_HPP

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <unordered_map>
#include <algorithm>
#include <forward_list>
#include <map>
#include <memory>
#include <sstream>
#include <string_view>
#include <list>
#include <limits>
#include <variant>

namespace cdv
{

    // ----------------------------------- String literal construction utility ---------------------------------- //

    template<typename char_t>
    constexpr const char_t* make_string_literal(const char* /*string_lit*/, const wchar_t* /*wstring_lit*/)
    {
        return nullptr;
    }
    template<>
    constexpr const char* make_string_literal(const char* string_lit, const wchar_t* /*wstring_lit*/)
    {
        return string_lit;
    }
    template<>
    constexpr const wchar_t* make_string_literal(const char* /*string_lit*/, const wchar_t* wstring_lit)
    {
        return wstring_lit;
    }
#define lit(string_type, x) cdv::make_string_literal<typename string_type::value_type>(x, L##x)

    template<typename string_t>
    constexpr const typename string_t::value_type* new_line()
    {
        return lit(string_t, "\n");
    }

    namespace impl
    {
        template<typename value_t>
        uint64_t get_address_as_uint(const value_t* address)
        {
            return reinterpret_cast<uint64_t>(static_cast<const void*>(address));
        }

        template<typename string_t, typename value_t>
        string_t get_address_as_string(value_t* address)
        {
            if constexpr (std::is_same_v < typename string_t::value_type, char>)
            {
                std::stringstream stream;
                stream << std::hex << "0x" << get_address_as_uint(address);
                return string_t{ stream.str() };
            }
            else
            {
                std::wstringstream stream;
                stream << std::hex << L"0x" << get_address_as_uint(address);
                return string_t{ stream.str() };
            }
        }

    }

    // ---------------------------------------------------------------------------------------------------------- //

    // Handy to-string function.
    // - Templatized on the return type to avoid having to call either std::string or std::wstring
    //   depending on string_t in the library code.
    // - Supports the identity function (works on string_t *inputs*).
    template<typename string_t, typename element_t>
    string_t to_string(element_t&& element)
    {
        // When trying to use to_string on a string_t, return the input.
        if constexpr (std::is_same_v<string_t, element_t>)
        {
            return std::forward<string_t>(element);
        }

        // If a string_t can be directly constructed from an element_t, use the constructor.
        else if constexpr (std::is_constructible_v<string_t, element_t>)
        {
            return string_t{ element };
        }

        // If the element is a pointer, return the address as a hex string.
        else if constexpr (std::is_pointer_v<std::remove_reference_t<element_t>>)
        {
            return impl::get_address_as_string<string_t>(element);
        }

        // General case: call std::to_(w)string on the type.
        else if constexpr (std::is_same_v<string_t, std::string>)
        {
            return std::to_string(element);
        }
        else if constexpr (std::is_same_v<string_t, std::wstring>)
        {
            return std::to_wstring(element);
        }
        return string_t{};
    }

    enum class node_shape
    {
        box,
        polygon,
        ellipse,
        oval,
        circle,
        point,
        egg,
        triangle,
        plaintext,
        plain,
        diamond,
        trapezium,
        parallelogram,
        house,
        pentagon,
        hexagon,
        septagon,
        octagon,
        doublecircle,
        doubleoctagon,
        tripleoctagon,
        invtriangle,
        invtrapezium,
        invhouse,
        Mdiamond,
        Msquare,
        Mcircle,
        rect,
        rectangle,
        square,
        star,
        none,
        underline,
        cylinder,
        note,
        tab,
        folder,
        box3d,
        component,
        promoter,
        cds,
        terminator,
        utr,
        primersite,
        restrictionsite,
        fivepoverhang,
        threepoverhang,
        noverhang,
        assembly,
        signature,
        insulator,
        ribosite,
        rnastab,
        proteasesite,
        proteinstab,
        rpromoter,
        rarrow,
        larrow,
        lpromoter
    };

    /**
    * An arrow shape defines the look of the tip of the arrow.
    * Its combination with the edge_style defines the complete look of an arrow.
    */
    enum class arrow_shape
    {
        normal,
        inv,
        dot,
        invdot,
        odot,
         invodot,
        none,
         tee,
        empty,
        invempty,
        diamond,
        odiamond,
        ediamond,
         crow,
        box,
        obox,
        open,
        halfopen,
        vee
    };

    /**
     * An edge style defines the look of the edge of an arrow or an undirected edge.
     * Its combination with the arrow_shape defines the complete look of an arrow.
     */
    enum class edge_style
    {
        normal,
        dashed,
        dotted,
        solid,
        invis,
        bold,
        tapered
    };

    enum class node_style
    {
        normal,
        dashed,
        dotted,
        solid,
        invis,
        bold,
        filled,
        striped,
        wedged,
        diagonals,
        rounded
    };

    enum class cluster_style
    {
        normal,
        filled,
        striped,
        rounded,
    };

    template<typename string_t = std::string>
    class table_node;

    template<typename string_t = std::string>
    struct node_appearance;

    namespace impl
    {
        template<typename string_t>
        struct node_shape_names
        {
            using char_t = typename string_t::value_type;

            static constexpr const char_t* values[] = {
                lit(string_t, "box"),
            lit(string_t, "polygon"),
            lit(string_t, "ellipse"),
            lit(string_t, "oval"),
            lit(string_t, "circle"),
            lit(string_t, "point"),
            lit(string_t, "egg"),
            lit(string_t, "triangle"),
            lit(string_t, "plaintext"),
            lit(string_t, "plain"),
            lit(string_t, "diamond"),
            lit(string_t, "trapezium"),
            lit(string_t, "parallelogram"),
            lit(string_t, "house"),
            lit(string_t, "pentagon"),
            lit(string_t, "hexagon"),
            lit(string_t, "septagon"),
            lit(string_t, "octagon"),
            lit(string_t, "doublecircle"),
            lit(string_t, "doubleoctagon"),
            lit(string_t, "tripleoctagon"),
            lit(string_t, "invtriangle"),
            lit(string_t, "invtrapezium"),
            lit(string_t, "invhouse"),
            lit(string_t, "Mdiamond"),
            lit(string_t, "Msquare"),
            lit(string_t, "Mcircle"),
            lit(string_t, "rect"),
            lit(string_t, "rectangle"),
            lit(string_t, "square"),
            lit(string_t, "star"),
            lit(string_t, "none"),
            lit(string_t, "underline"),
            lit(string_t, "cylinder"),
            lit(string_t, "note"),
            lit(string_t, "tab"),
            lit(string_t, "folder"),
            lit(string_t, "box3d"),
            lit(string_t, "component"),
            lit(string_t, "promoter"),
            lit(string_t, "cds"),
            lit(string_t, "terminator"),
            lit(string_t, "utr"),
            lit(string_t, "primersite"),
            lit(string_t, "restrictionsite"),
            lit(string_t, "fivepoverhang"),
            lit(string_t, "threepoverhang"),
            lit(string_t, "noverhang"),
            lit(string_t, "assembly"),
            lit(string_t, "signature"),
            lit(string_t, "insulator"),
            lit(string_t, "ribosite"),
            lit(string_t, "rnastab"),
            lit(string_t, "proteasesite"),
            lit(string_t, "proteinstab"),
            lit(string_t, "rpromoter"),
            lit(string_t, "rarrow"),
            lit(string_t, "larrow"),
            lit(string_t, "lpromoter")
            };
        };

        template<typename string_t>
        struct node_style_names
        {
            using char_t = typename string_t::value_type;

            static constexpr const char_t* values[] = {
                lit(string_t,""), // Empty string for 'normal'
                lit(string_t,"dashed"),
                lit(string_t,"dotted"),
                lit(string_t,"solid"),
                lit(string_t,"invis"),
                lit(string_t,"bold"),
                lit(string_t,"filled"),
                lit(string_t,"striped"),
                lit(string_t,"wedged"),
                lit(string_t,"diagonals"),
                lit(string_t,"rounded"),
            };
        };

        template<typename string_t>
        struct arrow_shape_names
        {
            using char_t = typename string_t::value_type;

            static constexpr const char_t* values[] = {
                lit(string_t,""),  // Empty string for 'normal'
                lit(string_t,"inv"),
                lit(string_t,"dot"),
                lit(string_t,"invdot"),
                lit(string_t,"odot"),
                lit(string_t,"invodot"),
                lit(string_t,"none"),
                lit(string_t,"tee"),
                lit(string_t,"empty"),
                lit(string_t,"invempty"),
                lit(string_t,"diamond"),
                lit(string_t,"odiamond"),
                lit(string_t,"ediamond"),
                lit(string_t,"crow"),
                lit(string_t,"box"),
                lit(string_t,"obox"),
                lit(string_t,"open"),
                lit(string_t,"halfopen"),
                lit(string_t,"vee")
            };
        };

        template<typename string_t>
        struct edge_style_names
        {
            using char_t = typename string_t::value_type;

            static constexpr const char_t* values[] = {
                lit(string_t,""), // Empty string for 'normal'
                lit(string_t,"dashed"),
                lit(string_t,"dotted"),
                lit(string_t,"solid"),
                lit(string_t,"invis"),
                lit(string_t,"bold"),
                lit(string_t,"tapered")
            };
        };

        template<typename string_t>
        struct cluster_style_names
        {
            using char_t = typename string_t::value_type;

            static constexpr const char_t* values[] = {
                lit(string_t,""), // Empty string for 'normal'
                lit(string_t,"filled"),
                lit(string_t,"striped"),
                lit(string_t,"rounded")
            };
        };

        inline uint64_t hash_combine(uint64_t a, const uint64_t b)
        {
            constexpr std::hash<uint64_t> hasher;
            a ^= hasher(b) + 0x9e3779b9 + (a << 6) + (a >> 2);
            return a;
        }

        template<typename T>
        constexpr auto get_type_name()
        {
            // Adapted from: https://stackoverflow.com/a/56766138/21420361
            std::string_view name, prefix, suffix;
#ifdef __clang__
            name = __PRETTY_FUNCTION__;
            prefix = "auto cdv::impl::get_type_name() [T = ";
            suffix = "]";
#elif defined(__GNUC__)
            name = __PRETTY_FUNCTION__;
            prefix = "constexpr auto cdv::impl::get_type_name() [with T = ";
            suffix = "]";
#elif defined(_MSC_VER)
            name = __FUNCSIG__;
            prefix = "auto __cdecl cdv::impl::get_type_name<";
            suffix = ">(void)";
#endif
            name.remove_prefix(prefix.size());
            name.remove_suffix(suffix.size());
            return name;
        }

        template<typename string_t>
        void replace_all(string_t& str, const string_t& from, const string_t& to)
     {
            // https://stackoverflow.com/a/24315631/21420361
            size_t start_pos = 0;
            while ((start_pos = str.find(from, start_pos)) != string_t::npos)
            {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
            }
        }

        template<typename string_t>
        void make_html_compatible(string_t& str)
        {
            // First and foremost : replace all '&' by '&amp;'.
            // Do this BEFORE any other replacement, as those will be creating '&'s as part of HTML codes.
            replace_all(str, string_t{ lit(string_t, "&") }, string_t{ lit(string_t, "&amp;") });

            // Then replace other problematic characters.
            replace_all(str, string_t{ lit(string_t, "<") }, string_t{ lit(string_t, "&lt;") });
            replace_all(str, string_t{ lit(string_t, ">") }, string_t{ lit(string_t, "&gt;") });
        }

        template<typename string_t>
        void make_bold(string_t& str)
        {
            str.insert(0, lit(string_t, "<b>"));
            str.insert(str.size(), lit(string_t, "</b>&nbsp;&nbsp;&nbsp;"));
        }

        template<typename string_t>
        void remove_class_struct(string_t& str)
        {
            // Important to have the space after "class" and "struct".
            auto it = str.find(lit(string_t, "class "));
            if(it != string_t::npos)
            {
                str.erase(it, 6);
            }
            it = str.find(lit(string_t, "struct "));
            if (it != string_t::npos)
            {
                str.erase(it, 7);
            }
        }


        template <typename DataT, typename string_t>
        string_t get_type_name_string()
        {
            using char_t = typename string_t::value_type;
            if constexpr (std::is_same_v<char_t, char>)
            {
                auto result = string_t{get_type_name<DataT>()};
                remove_class_struct(result);
                make_html_compatible(result);
                make_bold(result);
                return result;
            }
            else
            {
             const auto non_wide_result = get_type_name<DataT>();

                // char -> wchar_t conversion adapted from: https://stackoverflow.com/a/2573845/21420361
                string_t wide_result(non_wide_result.size(), L' '); // Overestimate number of code points.
             size_t out_size;
                mbstowcs_s(&out_size, wide_result.data(), non_wide_result.data(), non_wide_result.size());
                wide_result.resize(out_size); // Shrink to fit.
                remove_class_struct(wide_result);
                make_html_compatible(wide_result);
                make_bold(wide_result);

             return wide_result;
            }
        }

        // Some graphviz generation function templates must be forward-declared here.

        template<typename string_t>
        [[nodiscard]] string_t generate_table_node_cell_html(const typename table_node<string_t>::cell& cell);

        template<typename string_t>
        [[nodiscard]] string_t generate_node_appearance_string(const node_appearance<string_t>& appearance, const node_appearance<string_t>& default_node_appearance);
    }

    template<typename string_t>
    constexpr const typename string_t::value_type* get_node_shape_name(node_shape shape)
    {
        const size_t name_index = static_cast<size_t>(shape);
        return impl::node_shape_names<string_t>::values[name_index];
    }

    template<typename string_t>
    constexpr const typename string_t::value_type* get_node_style_name(node_style style)
    {
        const size_t name_index = static_cast<size_t>(style);
        return impl::node_style_names<string_t>::values[name_index];
    }

    template<typename string_t>
    constexpr const typename string_t::value_type* get_edge_style_name(edge_style style)
    {
        const size_t name_index = static_cast<size_t>(style);
        return impl::edge_style_names<string_t>::values[name_index];
    }

    template<typename string_t>
    constexpr const typename string_t::value_type* get_arrow_shape_name(arrow_shape shape)
    {
        const size_t name_index = static_cast<size_t>(shape);
        return impl::arrow_shape_names<string_t>::values[name_index];
    }

    template<typename string_t>
    constexpr const typename string_t::value_type* get_cluster_style_name(cluster_style style)
    {
        const size_t name_index = static_cast<size_t>(style);
        return impl::cluster_style_names<string_t>::values[name_index];
    }


    // ------------------------------------------------ base_node------------------------------------------------ //

    template<typename string_t /*= std::string*/>
    struct node_appearance
    {
        node_shape shape{ node_shape::ellipse };
        node_style style{node_style::normal};
        string_t color{};
    };


    // ------------------------------------------------ base_node------------------------------------------------ //

    template<typename string_t = std::string>
    class base_node
    {
    public:
        base_node() = default;
        base_node(const base_node& other) = default;
        base_node(base_node&& other) = default;
        virtual ~base_node() = default;
        base_node& operator=(const base_node& other) = default;
        base_node& operator=(base_node&& other) = default;

        [[nodiscard]] virtual string_t generate_structure_string(const node_appearance<string_t>& default_node_appearance) const = 0;

    protected:
        node_appearance<string_t> m_appearance{};
    };

    // ------------------------------------------------- table -------------------------------------------------- //

    template<typename string_t /*= std::string*/>
    class table
    {
    public:
        /**
        * @brief A table cell, part of a node row. It holds either a string value,
        * or a table itself (to allow recursive table structures).
        */
        struct cell
        {
            using value_t = string_t; // TODO : support tables inside of cells (recursive tables).

            value_t value{};
            string_t port_name{};
            int column_span{ 1 };
            int row_span{ 1 };

            static constexpr int default_column_span = 1;
            static constexpr int default_row_span = 1;

            cell() = default;

            explicit cell(const string_t& _value)
                : value{ _value }
            {}
            explicit cell(string_t&& _value)
                : value{ std::move(_value) }
            {}
            explicit cell(const table& _value)
                : value{ std::make_unique<table>(*_value) } // Copy the table to newly allocated memory.
            {}
            explicit cell(table&& _value)
                : value{ std::make_unique<table>(std::move(_value)) } // Move the table to newly allocated memory.
            {}

            /**
            *
            * @tparam value_t Type of the value, must be either a string type, or a type std::to_(w)string can be called on, or a table.
            * @return Cell containing a string representation of the value.
            * @note Static creation method to work around the issues that can be introduced by directly adding a perfect
            * forwarding constructor to 'cell': we want cell to be copyable and moveable.
            */
            template<typename value_t>
            static cell make(value_t&& _value)
            {
                cell result{};
                result.value = cdv::to_string<string_t>(std::forward<value_t>(_value));
                return result;
            }

            template<typename value_t>
            cell& with_value(value_t&& _value)
            {
                value = cdv::to_string<string_t>(std::forward<value_t>(_value));
                return *this;
            }

            cell& with_port(const string_t& _port_value)
            {
                // The port acts as an ID. Only accept string types and integral types, to avoid, for instance,
                // rounding errors if someone were to use floats for the port.
                port_name = _port_value;
                return *this;
            }

            cell& with_port(string_t&& _port_value)
            {
                // The port acts as an ID. Only accept string types and integral types, to avoid, for instance,
                // rounding errors if someone were to use floats for the port.
                port_name = std::move(_port_value);
                return *this;
            }

            cell& spanning_rows(const int _row_span)
            {
                row_span = _row_span;
                return *this;
            }

            cell& spanning_columns(const int _col_span)
            {
                column_span = _col_span;
                return *this;
            }
        };

        struct row
        {
            std::vector<cell> cells{};

            template<typename T1, typename... Ts>
            void build(T1&& cell_value)
            {
                if constexpr (std::is_same_v<std::remove_reference_t<T1>, cell>)
                {
                    // The value already is a cell. Use it.
                    cells.emplace_back(std::forward<T1>(cell_value));
                }
                else if constexpr (std::is_same_v<std::remove_reference_t<T1>, row>)
                {
                    // The value is a row cell. Move it into this.
                    cells = std::move(cell_value.cells);
                }
                else
                {
                    // Build a cell with the given value.
                    cells.emplace_back(cdv::to_string<string_t>(std::forward<T1>(cell_value)));
                }
            }

            template<typename T1, typename... Ts>
            void build(T1&& cell_value, Ts&&... other_cell_values)
            {
                if constexpr (std::is_same_v<std::remove_reference_t<T1>, cell>)
                {
                    // The value already is a cell. Use it.
                    cells.emplace_back(std::forward<T1>(cell_value));
                }
                else
                {
                    // Build a cell with the given value.
                    cells.emplace_back(cdv::to_string<string_t>(std::forward<T1>(cell_value)));
                }

                // Add remaining cells for this row.
                build<Ts...>(std::forward<Ts>(other_cell_values)...);
            }
        };

        table() = default;

        void set_cell_spacing(const int cell_spacing_px)
        {
            m_cell_spacing = cell_spacing_px;
        }

        void set_cell_border(const int cell_border_px)
        {
            m_cell_border = cell_border_px;
        }

        void set_table_border(const int table_border_px)
        {
            m_table_border = table_border_px;
        }

        template<typename T1, typename... Ts>
        void add_row(T1&& first_cell_value, Ts&&... other_cell_values)
        {
            // Add a row and build it.
            m_rows.emplace_back();
            // TODO optim: set row capacity from the parameter pack's size + 1
            m_rows[m_rows.size() - 1].template build<T1, Ts...>(std::forward<T1>(first_cell_value), std::forward<Ts>(other_cell_values)...);
        }

        void add_row(const row& row)
        {
            m_rows.emplace_back(row);
        }

        void add_row(row&& row)
        {
            m_rows.emplace_back(std::move(row));
        }

        [[nodiscard]] string_t generate_table_html_string() const
        {
            if(m_rows.empty())
            {
                return {};
            }

            string_t result;
            result += lit(string_t, "<table border=\"");
            result += cdv::to_string<string_t>(m_table_border);
            result += lit(string_t, "\" cellborder=\"");
            result += cdv::to_string<string_t>(m_cell_border);
            result += lit(string_t, "\" cellspacing=\"");
            result += cdv::to_string<string_t>(m_cell_spacing);
            result += lit(string_t, "\">");

            const size_t cell_count_of_longest_row =
            std::max_element(m_rows.begin(), m_rows.end(), [](const row& row1, const row& row2) {
                return row1.cells.size() < row2.cells.size();
            })->cells.size();

            // Each row.
            for (const row& current_row : m_rows)
            {
            result += lit(string_t, "<tr>");

            // Add the cells of this row.
            size_t current_column_position = 0;
            for (const cell& cell : current_row.cells) // TODO : std instead of a loop
            {
                result += impl::generate_table_node_cell_html<string_t>(cell);
                current_column_position += cell.column_span;
            }

            // If this row has fewer cells than the longest row, add empty rows at the end.
            if (current_column_position < cell_count_of_longest_row)
            {
                const size_t empty_cells_at_end_of_row = cell_count_of_longest_row - current_column_position;
                cell empty_cell{};
                for (size_t empty_cell_idx = 0; empty_cell_idx < empty_cells_at_end_of_row; ++empty_cell_idx)
                {
                    result += impl::generate_table_node_cell_html<string_t>(empty_cell);
                }
            }

            result += lit(string_t, "</tr>");
            }

            // Close the HTML table and the node tag.
            result += lit(string_t, "</table>");
            return result;
        }

    protected:
        std::vector<row> m_rows{};
        int m_cell_border{ 1 };
        int m_cell_spacing{ 0 };
        int m_table_border{ 0 };
    };

    // ----------------------------------------------- table_node------------------------------------------------ //

    template<typename string_t /*= std::string*/>
    class table_node : public table<string_t>, public base_node<string_t>
    {
    public:
        table_node()
         : table<string_t>()
            , base_node<string_t>()
        {
            // Tables draw their border themselves, therefore use plaintext mode to avoid
            // drawing it a second time.
            base_node<string_t>::m_appearance.shape = node_shape::plaintext;
        }

        template<typename T1, typename... Ts>
        table_node& with_row(T1&& first_cell_value, Ts&&... other_cell_values)
        {
            // Call add_row and return this for a simple builder-like pattern.
            table<string_t>::add_row(std::forward<T1>(first_cell_value), std::forward<Ts>(other_cell_values)...);
            return *this;
        }


        [[nodiscard]] string_t generate_structure_string(const node_appearance<string_t>& default_node_appearance) const override
        {
            string_t result;

            // Start structure.
            result += lit(string_t, "[");
            result += impl::generate_node_appearance_string<string_t>(base_node<string_t>::m_appearance, default_node_appearance);

         // Label, ie content of the table.
            result += lit(string_t, "label=<\n\t");
            result += table<string_t>::generate_table_html_string();
            result += lit(string_t, ">]");

            //
            return result;
        }
    };


    // ------------------------------------------------- arrow -------------------------------------------------- //

    template<typename string_t = std::string>
    struct arrow
    {
        arrow(const uint64_t _source_node_id, const uint64_t _destination_node_id)
            : source_node_id{ _source_node_id }
         , destination_node_id{_destination_node_id}
        {}

        arrow(const uint64_t _source_node_id, const string_t& _source_port, const uint64_t _destination_node_id, const string_t& _destination_port)
            : source_node_id{ _source_node_id }
            , source_port{ _source_port }
            , destination_node_id{ _destination_node_id }
            , destination_port{ _destination_port }
        {}

        arrow(const uint64_t _source_node_id, string_t&& _source_port, const uint64_t _destination_node_id, string_t&& _destination_port)
            : source_node_id{ _source_node_id }
            , source_port{ std::move(_source_port) }
            , destination_node_id{ _destination_node_id }
            , destination_port{ std::move(_destination_port)}
        {}

        arrow& with_shape(const arrow_shape _shape)
        {
            shape = _shape;
            return *this;
        }

        arrow& with_style(const edge_style _style)
        {
            style = _style;
            return *this;
        }

        bool operator==(const arrow& rhs)
        {
            return true;
        }

        // Visual appearance information.
        arrow_shape shape{ arrow_shape::normal };
        edge_style style{ edge_style::normal };

        // Actual edge data.
        uint64_t source_node_id;
        string_t source_port{};
        uint64_t destination_node_id;
        string_t destination_port{};
    };


    // ------------------------------------------------ cluster ------------------------------------------------- //

    template<typename string_t = std::string>
    struct cluster
    {
        cluster_style style{cluster_style::normal};
        string_t cluster_color{};
        string_t cluster_label{};
        node_appearance<string_t> default_node_appearance{};
    };


    // --------------------------------------------- rank_constraint -------------------------------------------- //

    struct rank_constraint
    {
        std::vector<uint64_t> constrained_node_ids;
        int requested_rank;
    };

    /**
     * Information about how to display a member of a struct / class.
     */
    enum class member_display_type
    {
        /**
        * Display the data directly inside a cell of the containing instance's node.
        * TODO : currently unsupported, falls back on the 'composition_edge' behaviour.
        */
        inside,
        /**
        * Display the data as a separate node, the address of the data inside a cell of the containing instance's node, and a POINTER edge going from the cell to the node.
        */
        pointer_edge,
        /**
        * Display the data as a separate node, the address of the data inside a cell of the containing instance's node, and a COMPOSITION edge going from the cell to the node.
        */
        composition_edge,
    };

    // ------------------------------------------------- traits ------------------------------------------------- //


    namespace traits
    {
        // ---------- contiguous_container ----------

        template<typename container_t>
        struct contiguous_container : std::false_type {};

        template<typename container_t>
        constexpr bool is_contiguous_container_v = contiguous_container<container_t>::value;

        // --------- linked_list_container ----------

        template<typename container_t>
        struct linked_list_container : std::false_type {};

        template<typename container_t>
        constexpr bool is_linked_list_container_v = linked_list_container<container_t>::value;

     template<typename container_t>
        constexpr bool is_singly_linked_list_container_v = linked_list_container<container_t>::value && !linked_list_container<container_t>::doubly_linked;

        template<typename container_t>
        constexpr bool is_doubly_linked_list_container_v = linked_list_container<container_t>::value && linked_list_container<container_t>::doubly_linked;

        // ----------- key_value_container ----------

        template<typename container_t>
        struct key_value_container : std::false_type {};

        template<typename container_t>
        constexpr bool is_key_value_container_v = key_value_container<container_t>::value;

        // ----------- custom user classes ----------

     template<typename adapted_class_t, size_t member_index>
        struct access : std::false_type
        {
            // static auto get_member_name();
            // static auto get_member_value(const adapted_class_t& instance)
            // static
        };

        template<typename adapted_class_t>
        constexpr bool is_adapted_v = access<adapted_class_t, 0>::value; // A class is adapted if it declares at least one member.

        // -------------- global checks -------------

        template<typename data_t>
        constexpr bool is_container_v = is_contiguous_container_v<data_t>
         || is_linked_list_container_v<data_t>
         || is_key_value_container_v<data_t>;

     template<typename data_t>
        constexpr bool is_handled_by_cdv_v = is_container_v<data_t> // TODO: find better name than "is_handled_by_cdv_v"
         || is_adapted_v<data_t>;

    }

    // Todo: put this in a separate file under "cdv_std_tags.h"
    namespace traits
    {
        // ------------------------------------------------------------------------------------------------------------- //
        //                   	DEFAULT IMPLEMENTATION OF REQUIRED TYPE TRAITS FOR STD CONTAINERS                   	//
        // ------------------------------------------------------------------------------------------------------------- //

        // Contiguous storage array-like containers.
        template<typename value_t> struct contiguous_container<std::vector<value_t>> : std::true_type {};
        template<typename value_t, size_t len> struct contiguous_container<std::array<value_t, len>> : std::true_type {};

        // Linked-list containers.
        template<typename value_t>
        struct linked_list_container<std::list<value_t>> : std::true_type
        {
            static constexpr bool doubly_linked{ true };
        };
        template<typename value_t> struct linked_list_container<std::forward_list<value_t>> : std::true_type
        {
            static constexpr bool doubly_linked{ false };
        };

        // Key-Value containers.
        template<typename key_t, typename value_t> struct key_value_container<std::map<key_t, value_t>> : std::true_type {};
        template<typename key_t, typename value_t> struct key_value_container<std::multimap<key_t, value_t>> : std::true_type {};
        template<typename key_t, typename value_t> struct key_value_container<std::unordered_map<key_t, value_t>> : std::true_type {};
        template<typename key_t, typename value_t> struct key_value_container<std::unordered_multimap<key_t, value_t>> : std::true_type {};

        // ------------------------------------------------------------------------------------------------------------- //
     //                                	DEFAULT ADAPTERS FOR COMMON STD CLASSES                                	//
     // ------------------------------------------------------------------------------------------------------------- //

     // std::pair<T1, T2>
        template<typename T1, typename T2>
        struct access<std::pair<T1, T2>, 0> : std::true_type {
            static auto get_member_name() {
                return "first";
            }
            static const auto& get_member_value(const std::pair<T1, T2>& instance) {
                return instance.first;
            }
            static constexpr member_display_type get_member_display_type()
            {
             return member_display_type::inside;
            }
        };

        template<typename T1, typename T2>
        struct access<std::pair<T1, T2>, 1> : std::true_type {
            static auto get_member_name() {
                return "second";
            }
            static const auto& get_member_value(const std::pair<T1, T2>& instance) {
                return instance.second;
            }
            static constexpr member_display_type get_member_display_type()
            {
                return member_display_type::inside;
            }
        };

        // std::optional<T>
        // todo: adapter for std::optional<T>
    }

    namespace impl
    {
        template<typename T>
        constexpr bool is_value_type()
        {
            return !std::is_pointer_v<T> && !std::is_reference_v<T>;
        }

        template<typename T>
        constexpr bool is_cstring_type_v =
            std::is_same_v<std::decay<T>, char*>
         || std::is_same_v<std::decay<T>, wchar_t*>;

        template<typename T>
        constexpr bool is_simple_type_v =
            std::is_fundamental_v<T>
            || std::is_constructible_v<std::basic_string<char>, T>
            || std::is_constructible_v<std::basic_string<wchar_t>, T>;

        template<typename value_t>
        uint64_t get_node_id_for_value(const value_t& value)
        {
            // TODO: check if value_t has a custom node ID computation function.
            return get_address_as_uint<value_t>(&value);
        }
    }

    // ---------------------------------------------- visualization --------------------------------------------- //

    template<typename string_t>
    class visualization;

    template<typename string_t>
    string_t generate_visualization_string(const visualization<string_t>&);

    template<typename string_t>
    class visualization : public cluster<string_t>
    {
    public:
        using base_node_t = base_node<string_t>;
        using cell_t = typename table_node<string_t>::cell;

        visualization() = default;

        // Primitive visualization functions.

        template<typename node_t>
        auto add_node(const uint64_t node_id, const node_t& node)
        {
            return m_nodes.insert_or_assign(node_id, std::make_unique<node_t>(node));
        }

        template<typename node_t>
        auto add_node(const uint64_t node_id, node_t&& node)
        {
            // Overload for rvalue references, allows us to forward the node instead of copying it.
            return m_nodes.insert_or_assign(node_id, std::make_unique<node_t>(std::forward<node_t>(node)));
        }

        arrow<string_t>& add_edge(const arrow<string_t>& arrow)
        {
            m_directed_edges.emplace_back(arrow);
            return m_directed_edges[m_directed_edges.size() - 1];
        }

        arrow<string_t>& add_edge(const arrow<string_t>&& arrow)
        {
            m_directed_edges.emplace_back(std::move(arrow));
            return m_directed_edges[m_directed_edges.size() - 1];
        }

        void add_unique_edge(const arrow<string_t>& arrow)
        {
            const auto iterator = std::find(m_directed_edges.cbegin(), m_directed_edges.cend(), arrow);
            if(iterator != m_directed_edges.end())
            {
                return;
            }
            m_directed_edges.emplace_back(arrow);
            // return m_directed_edges[m_directed_edges.size() - 1];
        }

        void add_rank_constraint(const rank_constraint& constraint)
        {
            m_rank_constraints.emplace_back(constraint);
        }

        void add_rank_constraint(rank_constraint&& constraint)
        {
            m_rank_constraints.emplace_back(std::move(constraint));
        }


        // Advanced automatic data structure visualization functions.

        template<typename contiguous_container_t>
        uint64_t add_data_structure(
            const contiguous_container_t& container,
            std::enable_if_t<traits::is_contiguous_container_v<contiguous_container_t>, bool> = true)      	// For known contiguous structures.
        {
            using value_t = typename contiguous_container_t::value_type;

            // |--------------------------------------------|
            // | <Type name> | <Address> | Length: <Length> |
            // |--------------------------------------------|
            // | <Val 1> | <Val 2> | <Val 3> | ............ |
            // |--------------------------------------------|

            const uint64_t container_node_id = impl::get_node_id_for_value(container);

         auto type_name = impl::get_type_name_string<contiguous_container_t, string_t>();
            auto instance_address = impl::get_address_as_string<string_t>(&container);
            auto length_str = string_t{ lit(string_t, "Length: ") } + to_string<string_t>(container.size());
            auto container_node = table_node<string_t>{}
                .with_row(
                    cell_t{ std::move(type_name) }.spanning_columns(4),
                    cell_t{ std::move(instance_address) }.spanning_columns(2),
                    cell_t{ std::move(length_str) }.spanning_columns(2));

            // value_t is a value type: put the value directly in each cell.
            if constexpr (impl::is_value_type<value_t>())
            {
                auto values_row = typename table_node<string_t>::row{};
                values_row.cells.emplace_back(lit(string_t, "Values: "));
                for (const auto& value : container)
                {
                    values_row.cells.emplace_back(cdv::to_string<string_t>(value));
                }
                container_node.add_row(values_row);
            }
            // value_t is a pointer type:
            // - put the ADDRESS in each cell,
            // - add a node for each pointed value,
            // - add an edge to each node
            else if constexpr (std::is_pointer_v<value_t>)
            {
                auto values_row = typename table_node<string_t>::row{};
                values_row.cells.emplace_back(lit(string_t, "Values: "));
                size_t index = 0;
                for (const auto& value : container)
                {
                    // Address in the vector node.
                    const auto port_name = cdv::to_string<string_t>(index);
                    values_row.cells.emplace_back(
                        cell_t{ impl::get_address_as_string<string_t>(value) }.with_port(port_name));

                    // Node for the value.
                    const uint64_t pointed_node_id = add_data_structure(*value);

                    // Edge from the cell to the value.
                    add_edge(arrow<string_t>{ container_node_id, port_name, pointed_node_id, lit(string_t, "") });
                    ++index;
                }
                container_node.add_row(values_row);
            }
            // todo std::reference_wrapper

            add_node(container_node_id, std::move(container_node));
            return container_node_id;
        }

        template<typename linked_list_container>
        uint64_t add_data_structure(
            const linked_list_container& container,
            const bool add_rank_constraints = false,
            std::enable_if_t<traits::is_linked_list_container_v<linked_list_container>, bool> = true)      	// For known linked-list-like structures.
        {
            using value_t = typename linked_list_container::value_type;
            constexpr auto header_port_name = lit(string_t, "H");
            constexpr auto value_address_port_name = lit(string_t, "V");

            // |--------------------------------------------------------| 	    |--------------------------------------------------------|
            // |            	<Container type name>               	|  -->  |             	<Container type name>              	    |
            // |--------------------------------------------------------|       |--------------------------------------------------------|
            // | Index: <Index> | <Address of value> | <Value at index> | 	    | Index: <Index> | <Address of value> | <Value at index> |
            // |--------------------------------------------------------| 	    |--------------------------------------------------------|

            const auto container_type_name = impl::get_type_name_string<linked_list_container, string_t>();

            // Special case: empty list.
            if(container.empty())
            {
                auto current_listnode_node = table_node<string_t>{}
                    .with_row(cell_t{ container_type_name })
                    .with_row(lit(string_t, "Empty list"));
                const auto node_id = impl::hash_combine(impl::get_node_id_for_value(container), std::numeric_limits<uint64_t>::max());
                add_node(node_id, std::move(current_listnode_node));
                return node_id;
            }

            // value_t is a value type: put the value directly in the cell of each listnode.
            std::vector<uint64_t> inserted_node_ids{};
            inserted_node_ids.reserve(128);
            if constexpr (impl::is_value_type<value_t>())
            {
                for (const auto& value : container)
                {
                    // Add a node for the current value.
                    auto index_str = string_t{ lit(string_t, "Index: ") } + to_string<string_t>(inserted_node_ids.size());
                    auto value_address = impl::get_address_as_string<string_t>(&value);
                    auto current_listnode_node = table_node<string_t>{}
                        .with_row(cell_t{ container_type_name }.spanning_columns(2).with_port(header_port_name))
                        .with_row(std::move(index_str), std::move(value_address))
                        .with_row(cell_t{ std::move(cdv::to_string<string_t>(value)) }.spanning_columns(2));
                    const auto node_id = impl::hash_combine(impl::get_node_id_for_value(value), inserted_node_ids.size());
                    add_node(node_id, std::move(current_listnode_node));

                    // If not the first value, connect the previous node to the new node.
                    if(!inserted_node_ids.empty())
                    {
                        add_edge(arrow<string_t>{inserted_node_ids[inserted_node_ids.size() - 1], header_port_name, node_id, lit(string_t, "")});
                        // If the container is a doubly linked list, add a backward edge.
                        if constexpr (traits::is_doubly_linked_list_container_v<linked_list_container>)
                        {
                            add_edge(arrow<string_t>{node_id, header_port_name, inserted_node_ids[inserted_node_ids.size() - 1], lit(string_t, "") });
                        }
                    }
                    inserted_node_ids.emplace_back(node_id);
                }
            }
            // value_t is a pointer type:
            // - put the ADDRESS in each cell,
            // - add a node for each pointed value,
            // - add an edge to each node
            else if constexpr (std::is_pointer_v<value_t>)
            {
                for (const auto& value : container)
                {
                    // Add a node for the current value.
                    auto index_str = string_t{ lit(string_t, "Index: ") } + to_string<string_t>(inserted_node_ids.size());
                    auto value_address = impl::get_address_as_string<string_t>(&value); // This is the address OF THE POINTER (stuff**, not stuff*).
                    auto current_listnode_node = table_node<string_t>{}
                        .with_row(cell_t{ container_type_name }.spanning_columns(2).with_port(header_port_name))
                        .with_row(std::move(index_str), std::move(value_address))
                     .with_row(cell_t{ impl::get_address_as_string<string_t>(value) }.spanning_columns(2).with_port(value_address_port_name));
                    const auto node_id = impl::hash_combine(impl::get_node_id_for_value(value), inserted_node_ids.size());
                    add_node(node_id, std::move(current_listnode_node));

                    // Node for the value.
                    const uint64_t pointed_node_id = add_data_structure(*value);
                    // Edge from the cell to the value.
                    add_edge(arrow<string_t>{ node_id, value_address_port_name, pointed_node_id, lit(string_t, "") });

                    // If not the first value, connect the previous node to the new node.
                    if (!inserted_node_ids.empty())
                    {
                        add_edge(arrow<string_t>{inserted_node_ids[inserted_node_ids.size() - 1], header_port_name, node_id, header_port_name});
                        // If the container is a doubly linked list, add a backward edge.
                        if constexpr (traits::is_doubly_linked_list_container_v<linked_list_container>)
                        {
                            add_edge(arrow<string_t>{node_id, header_port_name, inserted_node_ids[inserted_node_ids.size() - 1], header_port_name });
                        }
                    }
                    inserted_node_ids.emplace_back(node_id);
                }
            }
            // todo std::reference_wrapper

            if(add_rank_constraints)
            {
                // If asked: rank constraints!
                // All nodes of the list should have the same rank, more or less:
             // every pack of 5 nodes is asked to have the same rank.
                constexpr int initially_requested_rank = 1;
                constexpr int max_nodes_per_rank = 5;
                rank_constraint current_constraint{ {},  initially_requested_rank };
                for (uint64_t node_id : inserted_node_ids)
                {
                    current_constraint.constrained_node_ids.emplace_back(node_id);
                    if (current_constraint.constrained_node_ids.size() > max_nodes_per_rank)
                    {
                        const int previous_rank = current_constraint.requested_rank;
                        add_rank_constraint(std::move(current_constraint));
                        current_constraint = rank_constraint{ {}, previous_rank + 1 };
                    }
                }
                if (!current_constraint.constrained_node_ids.empty())
                {
                    // Add the last constraint if not empty.
                    add_rank_constraint(std::move(current_constraint));
                }
            }

            return inserted_node_ids[0];
        }

        template<typename adapted_class_t, size_t member_index>
        void add_rows_for_members(const adapted_class_t& data_structure, const uint64_t instance_node_id, table_node<string_t>& node_for_data_structure)
        {
            // indexed_member_access_t::get_member_name()  : unit -> string_t
            // indexed_member_access_t::get_member_value() : const adapted_class_t& instance -> auto
            using indexed_member_access_t = traits::access<adapted_class_t, member_index>;

            // 1. Add a new row for the current member.
            auto member_name = indexed_member_access_t::get_member_name();
            const auto& member_value = indexed_member_access_t::get_member_value(data_structure);
            constexpr member_display_type member_display_method = indexed_member_access_t::get_member_display_type();
            if constexpr (
                member_display_method == member_display_type::inside
                || member_display_method == member_display_type::composition_edge
                )
            {
                // TODO add_rows_for_members | For now, 'inside' is unsupported. Work needed to properly support it, will be the goal
                // TODO add_rows_for_members | of a future commit.

             // Address in the vector node.
                const auto port_name = cdv::to_string<string_t>(member_index);
                node_for_data_structure.add_row(
                    std::move(member_name),
                    cell_t{ impl::get_address_as_string<string_t>(&member_value) }.with_port(port_name));

                // Node for the value.
                const uint64_t pointed_node_id = add_data_structure(member_value);

                // Edge from the cell to the value.
                // TODO add_rows_for_members | proper arrow shape for 'composition_edge'
                add_edge(
                    arrow<string_t>{ instance_node_id, port_name, pointed_node_id, lit(string_t, "") }
                 .with_style(edge_style::dashed));
            }
            else // member_display_method == member_display_type::pointer_edge
            {
                // Address in the vector node.
                const auto port_name = cdv::to_string<string_t>(member_index);
                node_for_data_structure.add_row(
                    std::move(member_name),
                    cell_t{ impl::get_address_as_string<string_t>(&member_value) }.with_port(port_name));

                // Node for the value.
                const uint64_t pointed_node_id = add_data_structure(*member_value);

                // Edge from the cell to the value.
                // TODO add_rows_for_members | proper arrow shape for 'pointer_edge'
                add_edge(arrow<string_t>{ instance_node_id, port_name, pointed_node_id, lit(string_t, "") });
            }

            // 2. Recursion to display the next adapted member, if there's one.
            if constexpr (traits::access<adapted_class_t, member_index + 1>::value)
            {
                add_rows_for_members<adapted_class_t, member_index + 1>(data_structure, instance_node_id, node_for_data_structure);
            }
        }

    // std::string pas géré ... faut se débrouiller pour avoir un add_data_structure qui fasse un to_string quand
    // rien d'autre ne marche

        template<typename adapted_class_t>
        uint64_t add_data_structure(
            const adapted_class_t& data_structure,
            std::enable_if_t<traits::is_adapted_v<adapted_class_t>, bool> = true) // For custom user types.
        {
            // Add a table node referencing all adapted members :

         // |-------------------------------------|
            // |   <Type name>	| 	<Address>	|
            // |-------------------------------------|
            // | <Member Name 1>  | <Member Value 1> |
            // |-------------------------------------|
            // | <Member Name 2>  | <Member Value 2> |
            // |-------------------------------------|
            // |             	...             	|
            // |-------------------------------------|

            const uint64_t node_id = impl::get_node_id_for_value(data_structure);
            auto type_name = impl::get_type_name_string<adapted_class_t, string_t>();
            auto instance_address = impl::get_address_as_string<string_t>(&data_structure);

            // Base node with heading row.
         auto node_for_instance = table_node<string_t>{}
                .with_row(std::move(type_name), std::move(instance_address));

            // Loop over adapted members.
            add_rows_for_members<adapted_class_t, 0>(data_structure, node_id, node_for_instance);
            add_node(node_id, std::move(node_for_instance));
            return node_id;
        }

        template<typename simple_type_t>
        uint64_t add_data_structure(
            const simple_type_t& data_structure,
            std::enable_if_t<impl::is_simple_type_v<simple_type_t>, bool> = true) // For simple types.
        {
            // Add a simple table node:

            // |-------------------------|
            // | <Type name> | <Address> |
            // |-------------------------|
            // |     	<Value>     	|
            // |-------------------------|

            const uint64_t node_id = impl::get_node_id_for_value(data_structure);
            auto type_name = impl::get_type_name_string<simple_type_t, string_t>();
            auto instance_address = impl::get_address_as_string<string_t>(&data_structure);
            auto node_for_instance = table_node<string_t>{}
                .with_row(std::move(type_name), std::move(instance_address))
                .with_row(table_node<string_t>::cell::make(data_structure).spanning_columns(2));
            add_node(node_id, std::move(node_for_instance));
            return node_id;
        }

        template<typename pointer_type_t>
        uint64_t add_data_structure(
            const pointer_type_t& data_structure,
            std::enable_if_t<std::is_pointer_v<pointer_type_t>, bool> = true) // For raw pointers.
        {
            // First add a node for the pointed value (recursion).
            const uint64_t pointed_value_node_id = add_data_structure(*data_structure);

            // Then add a simple table node for the pointer:
            // |---------------------------|
            // |  <Type name> |  <Address> |
            // |---------------------------|
            // | <Value = Pointed address> |  <--- port = "ptr"
            // |---------------------------|
            string_t ptr_port_name { lit(string_t, "ptr") };
            const uint64_t pointer_node_id = impl::get_node_id_for_value(data_structure);
            auto pointer_type_name = impl::get_type_name_string<pointer_type_t, string_t>();
            auto address_of_pointer = impl::get_address_as_string<string_t>(&data_structure);
            auto node_for_pointer = table_node<string_t>{}
                .with_row(std::move(pointer_type_name), std::move(address_of_pointer))
                .with_row(table_node<string_t>::cell::make(data_structure).spanning_columns(2).with_port(ptr_port_name));
            add_node(pointer_node_id, std::move(node_for_pointer));

            // Finally, add an edge between the two.
            add_unique_edge(arrow<string_t>{ pointer_node_id, std::move(ptr_port_name), pointed_value_node_id, lit(string_t, "")});

            return pointer_node_id;
        }

        template<typename cstring_type>
        uint64_t add_data_structure(
            const cstring_type& data_structure,
            std::enable_if_t<impl::is_cstring_type_v<cstring_type>, bool> = true) // Special case for C-style strings.
        {
            // Add a simple table node:

            // |-------------------------|
            // | <Type name> | <Address> |
            // |-------------------------|
            // |          <Value>        |
            // |-------------------------|

            const uint64_t node_id = impl::get_node_id_for_value(data_structure);
            auto type_name = impl::get_type_name_string<cstring_type, string_t>();
            auto instance_address = impl::get_address_as_string<string_t>(&data_structure);
            auto node_for_instance = table_node<string_t>{}
                .with_row(std::move(type_name), std::move(instance_address))
                .with_row(table_node<string_t>::cell::make(data_structure).spanning_columns(2));
            add_node(node_id, std::move(node_for_instance));
            return node_id;
        }


    private:
        /**
        * Key   = node ID.
        * Value = node instance.
        */
        std::unordered_map<uint64_t, std::unique_ptr<base_node<string_t>>> m_nodes;
        /**
        * Key   = hash of the pair source_node_id -> destination_node_id in the connection.
        * Value = description of the edge.
        */
        std::vector<arrow<string_t>> m_directed_edges;
        /**
        *
        */
        std::vector<cluster<string_t>> m_clusters; // TODO: not handled yet.

        std::vector<rank_constraint> m_rank_constraints;

        friend string_t generate_visualization_string<string_t>(const visualization<string_t>&);
    };

    // ------------------------------------------- graphviz generation ------------------------------------------ //

    namespace impl
    {
        template<typename string_t>
        [[nodiscard]] string_t generate_cluster_style_string(const cluster<string_t> cluster)
        {
            string_t result;
            if (cluster.style != cluster_style::normal)
            {
                constexpr size_t max_result_length = 15; // "style=striped;\0"  => 15 chars
                result.reserve(max_result_length);
                result += lit(string_t, "style=");
                result += get_cluster_style_name<string_t>(cluster.style);
                result += lit(string_t, ";\n");
            }
            return result;
        }

        template<typename string_t>
        [[nodiscard]] string_t generate_cluster_color_string(const cluster<string_t> cluster)
        {
            string_t result;
            if (!cluster.cluster_color.empty())
            {
                const size_t result_length = 8 + cluster.cluster_color.size(); // "color=<color>;\0"  => 8 + len(<color>) chars
                result.reserve(result_length);
                result += lit(string_t, "color=");
                result += cluster.cluster_color;
                result += lit(string_t, ";\n");
            }
            return result;
        }

        template<typename string_t>
        [[nodiscard]] string_t generate_cluster_label_string(const cluster<string_t> cluster)
        {
            string_t result;
            if (!cluster.cluster_label.empty())
            {
                const size_t result_length = 8 + cluster.cluster_label.size(); // "label=<label>;\0"  => 8 + len(<label>) chars
                result.reserve(result_length);
                result += lit(string_t, "label=");
                result += cluster.cluster_label;
                result += lit(string_t, ";\n");
            }
            return result;
        }

        template<typename string_t>
        [[nodiscard]] string_t generate_default_node_appearance_string(const cluster<string_t> cluster)
        {
            string_t result;
            result += lit(string_t, "node[");
            result += generate_node_appearance_string(cluster.default_node_appearance, node_appearance{}); // Default-constructed values won't generate any appearance information.
            result += lit(string_t, "]\n");
            return result;
        }

        template<typename string_t>
        [[nodiscard]] string_t generate_node_appearance_string(const node_appearance<string_t>& appearance, const node_appearance<string_t>& default_node_appearance)
        {
            // Override default appearance for the different visual parameters.
            // Don't write anything if the settings are the same as the current default,
            // to reduce output size as much as possible.

            string_t result; // TODO: optim: reserve

            if (appearance.shape != default_node_appearance.shape)
            {
                result += lit(string_t, "shape=");
                result += get_node_shape_name<string_t>(appearance.shape);
                result += lit(string_t, ",");
            }

            if (appearance.style != default_node_appearance.style)
            {
                result += lit(string_t, "style=");
                result += get_node_style_name<string_t>(appearance.style);
                result += lit(string_t, ",");
            }

            if (appearance.color != default_node_appearance.color)
            {
                result += lit(string_t, "color=");
                result += appearance.color;
                result += lit(string_t, ",");
            }

            return result;
        }

        template<typename string_t>
        [[nodiscard]] string_t generate_table_node_cell_html(const typename table_node<string_t>::cell& cell)
        {
            string_t result; // TODO: optim: reserve

            // 1. Open the cell and fill its parameters.
            result += lit(string_t, "<td ");

            // Generate ' rowspan="<value>"' if non-default span.
            if (cell.row_span != cell.default_row_span)
            {
                result += lit(string_t, " rowspan=\"");
                result += cdv::to_string<string_t>(cell.row_span);
                result += lit(string_t, "\"");
            }
            // Generate ' colspan="<value>"' if non-default span.
            if (cell.column_span != cell.default_column_span)
            {
                result += lit(string_t, " colspan=\"");
                result += cdv::to_string<string_t>(cell.column_span);
                result += lit(string_t, "\"");
            }
            // Generate ' port="<value>"' if a port is specified for this cell.
            if (!cell.port_name.empty())
            {
                result += lit(string_t, " port=\"");
                result += cell.port_name;
                result += lit(string_t, "\"");
            }

            result += lit(string_t, ">");

            // 2. The value in the cell.
            result += cell.value;

            // 3. Close the cell.
            result += lit(string_t, "</td>");
            return result;
        }
    }

    template<typename string_t>
    [[nodiscard]] string_t generate_visualization_string(const visualization<string_t>& visualization)
    {
        string_t result;
        result.reserve(50000);

        // 1. Graph setup, with all the cluster information of the global graph
        // (the global graph itself is a cluster).
        result += lit(string_t, "digraph G {\n");
        result += impl::generate_cluster_color_string(visualization);
        result += impl::generate_cluster_label_string(visualization);
        result += impl::generate_cluster_style_string(visualization);
        result += impl::generate_default_node_appearance_string(visualization);

        // 2. Print each node's structure, ie actual node content.
        for (const auto& [node_id, node] : visualization.m_nodes)
        {
            // Each graphviz node is uniquely identified by this ID. Used later for edges.
            result += cdv::to_string<string_t>(node_id);
            // Actual content of the nodes.
            result += node->generate_structure_string(visualization.default_node_appearance);
            // One node per line.
            result += new_line<string_t>();
        }

        // 3. Print each arrow / directed edge between nodes.
        for (const auto& arrow : visualization.m_directed_edges)
        {
            // Source node with port if specified.
            result += cdv::to_string<string_t>(arrow.source_node_id);
            if (!arrow.source_port.empty())
            {
                result += lit(string_t, ":");
                result += cdv::to_string<string_t>(arrow.source_port);
            }

            // The arrow.
            result += lit(string_t, " -> ");

            // Destination node with port if specified.
            result += cdv::to_string<string_t>(arrow.destination_node_id);
            if (!arrow.destination_port.empty())
            {
                result += lit(string_t, ":");
                result += cdv::to_string<string_t>(arrow.destination_port);
            }

            // Arrow shape / style.
            result += lit(string_t, "[");
            if (arrow.shape != arrow_shape::normal)
            {
                result += lit(string_t, "shape");
                result += lit(string_t, "=");
                result += cdv::get_arrow_shape_name<string_t>(arrow.shape);
                result += lit(string_t, ","); // Trailing commas are allowed in the graphviz grammar.
            }
            if (arrow.style != edge_style::normal)
            {
                result += lit(string_t, "style");
                result += lit(string_t, "=");
                result += cdv::get_edge_style_name<string_t>(arrow.style);
                result += lit(string_t, ","); // Trailing commas are allowed in the graphviz grammar.
            }
            result += lit(string_t, "]\n");
        }

        // 4. TODO: print each undirected edge.

        // 5. Rank constraints.
        for(const rank_constraint& constraint : visualization.m_rank_constraints)
        {
            if(!constraint.constrained_node_ids.empty())
            {
                result += lit(string_t, "{rank=same;");
                for(const auto node_id : constraint.constrained_node_ids)
                {
                    result += cdv::to_string<string_t>(node_id);
                    result += lit(string_t, ";");
                }
                result += lit(string_t, "}");
            }
        }


        // 6. Close the graph !
        result += lit(string_t, "}\n");

        return result;
    }

#undef lit

}

#define CDV_DECLARE_MEMBER(ClassName, MemberIndex, MemberNameRetrievalExpression, MemberValueGetterExpression)   	\
namespace cdv::traits                                                                                                   	\
{                                                                                                                       	\
    template<>                                                                                                          	\
    struct access<ClassName, MemberIndex> : std::true_type                                                              	\
    {                                                                                                                   	\
     static auto get_member_name()                                                                                   	\
     {                                                                                                               	\
        return (MemberNameRetrievalExpression);                                                                     	\
     }                                                                                                               	\
     static decltype(auto) get_member_value(const MyClass& instance)                                                 	\
     {                                                                                                               	\
        return instance.MemberValueGetterExpression;                                                                	\
     }                                                                                                               	\
     static constexpr member_display_type get_member_display_type()                                                  	\
     {                                                                                                               	\
        using member_t = decltype(get_member_value(std::declval<MyClass>()));                                       	\
                                                                                                                         \
        if constexpr (std::is_pointer_v<member_t>)                                                                  	\
        {                                                                                                           	\
            return member_display_type::pointer_edge;                                                               	\
        }                                                                                                           	\
        else if constexpr (cdv::traits::is_handled_by_cdv_v<member_t>)                                              	\
        {                                                                                                           	\
            return member_display_type::composition_edge;                                                           	\
        }                                                                                                           	\
        else                                                                                                        	\
        {                                                                                                           	\
            return member_display_type::inside;                                                                     	\
        }                                                                                                           	\
     }                                                                                                               	\
    };                                                                                                                  	\
}

#define CDV_DECLARE_PUBLIC_MEMBER(ClassName, MemberIndex, MemberName)                                                   	\
namespace cdv::traits                                                                                                   	\
{                                                                                                                       	\
    template<>                                                                                                          	\
    struct access<ClassName, MemberIndex> : std::true_type                                                              	\
    {                                                                                                                   	\
     static auto get_member_name()                                                                                   	\
     {                                                                                                               	\
        return #MemberName;                                                                                         	\
     }                                                                                                               	\
     static decltype(auto) get_member_value(const MyClass& instance)                                                 	\
     {                                                                                                               	\
        return instance.MemberName;                                                                                 	\
     }                                                                                                               	\
     static constexpr member_display_type get_member_display_type()                                                  	\
     {                                                                                                               	\
        using member_t = decltype(get_member_value(std::declval<MyClass>()));                                       	\
                                                                                                                         \
        if constexpr (std::is_pointer_v<member_t>)                                                                  	\
        {                                                                                                           	\
            return member_display_type::pointer_edge;                                                               	\
        }                                                                                                           	\
        else if constexpr (cdv::traits::is_handled_by_cdv_v<member_t>)                                              	\
        {                                                                                                           	\
            return member_display_type::composition_edge;                                                           	\
        }                                                                                                           	\
        else                                                                                                        	\
        {                                                                                                           	\
            return member_display_type::inside;                                                                     	\
        }                                                                                                           	\
     }                                                                                                               	\
    };                                                                                                                  	\
}

// TODO : DECLARE_CUSTOM_MEMBER pour déclarer un membre sur lequel on n'appelle pas un getter mais une méthode perso prenant l'instance en paramètre directement.

#endif // CDV_HPP