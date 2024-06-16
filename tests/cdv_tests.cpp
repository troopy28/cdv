#include "../include/cdv/cdv.hpp"

class MyClass
{
  public:
    const std::string &get_string() const
    {
        return my_string;
    }

    int get_int() const
    {
        return my_int;
    }

    double get_double() const
    {
        return my_double;
    }

    const char *MyPublicMember{"charptr member"};

  private:
    std::string my_string{"some text"};
    int my_int{24};
    double my_double{3.14};
};
CDV_DECLARE_PUBLIC_MEMBER(MyClass, 0, MyPublicMember)
CDV_DECLARE_MEMBER(MyClass, 1, "my_string", get_string())
CDV_DECLARE_MEMBER(MyClass, 2, "my_int", get_int())
CDV_DECLARE_MEMBER(MyClass, 3, "my_double", get_double())

struct Position
{
    int x{0};
    int y{0};
    int z{0};
};
CDV_DECLARE_PUBLIC_MEMBER(Position, 0, x)
CDV_DECLARE_PUBLIC_MEMBER(Position, 1, y)
CDV_DECLARE_PUBLIC_MEMBER(Position, 2, z)

struct NodeGraph
{
    explicit NodeGraph(std::string _name)
        : name{std::move(_name)}
    {
    }
    std::string name;
    std::vector<NodeGraph *> nodes{};
};
CDV_DECLARE_PUBLIC_MEMBER(NodeGraph, 0, name)
CDV_DECLARE_PUBLIC_MEMBER(NodeGraph, 1, nodes)

struct TreeNode
{
    explicit TreeNode(std::string _name)
        : name{std::move(_name)}
    {
    }
    std::string name;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};
CDV_DECLARE_PUBLIC_MEMBER(TreeNode, 0, name)
CDV_DECLARE_PUBLIC_MEMBER(TreeNode, 1, left)
CDV_DECLARE_PUBLIC_MEMBER(TreeNode, 2, right)


void example_1()
{
    // A visualization object browses your data and builds the graph
    // representing the state of your data.
    cdv::visualization<std::string> visualization;

    int some_int = 12;
    visualization.add_data_structure(some_int); // Just add the integer !

    int *some_int_pointer = &some_int;
    visualization.add_data_structure(some_int_pointer); // Just add the pointer !

    int **some_int_pointer_pointer = &some_int_pointer;
    visualization.add_data_structure(some_int_pointer_pointer);

    // Create a pair, referencing the two previous pointers.
    const std::pair<int *&, int **&> my_pair{some_int_pointer, some_int_pointer_pointer};
    visualization.add_data_structure(my_pair);

    // Export the visualization as a GraphViz string.
    const std::string my_graphviz_text = cdv::generate_dot_visualization_string(visualization);
    std::cout << my_graphviz_text << "\n";
}

void example_2_optional()
{
    cdv::visualization<std::string> visualization;

    const std::optional<int *> empty_optional = std::nullopt;
    visualization.add_data_structure(empty_optional);

    int my_int = 3;
    int *my_int_ptr = &my_int;
    const std::optional<int *> valued_optional = my_int_ptr;
    visualization.add_data_structure(valued_optional);

    std::cout << cdv::generate_dot_visualization_string(visualization) << "\n";
}

void example_3_vector_of_struct()
{
    cdv::visualization<std::string> visualization;

    std::vector<Position> positions;
    positions.reserve(5);
    for (int i = 0; i < 5; ++i)
    {
        positions.emplace_back(Position{i, (i + 5) % 2, (i * 3) % 4});
    }
    visualization.add_data_structure(positions);

    std::cout << cdv::generate_dot_visualization_string(visualization) << "\n";
}

void example_4_user_defined_graph()
{
    cdv::visualization<std::string> visualization;

    NodeGraph start{"start"}, level1_node1{"level1_node1"}, level1_node2{"level1_node2"}, level2_node1{"level2_node1"},
        level3_node1{"level3_node1"}, level4_node1{"level4_node1"}, level4_node2{"level4_node2"}, end{"end"};
    //               start
    //              /     \
    //  level1_node1      level1_node2
    //      |            /     |
    //  level2_node1    /      |
    //      |          /       |
    //  level3_node1--/        |
    //      |                  |
    //  level4_node1        level4_node2
    //              \      /
    //                end

    // Root level.
    start.nodes.emplace_back(&level1_node1);
    start.nodes.emplace_back(&level1_node2);
    // Level 1.
    level1_node1.nodes.emplace_back(&level2_node1);
    level1_node2.nodes.emplace_back(&level3_node1);
    level1_node2.nodes.emplace_back(&level4_node2);
    // Level 2.
    level2_node1.nodes.emplace_back(&level3_node1);
    // Level 3.
    level3_node1.nodes.emplace_back(&level4_node1);
    // Level 4.
    level4_node1.nodes.emplace_back(&end);
    level4_node2.nodes.emplace_back(&end);

    visualization.add_data_structure(start);

    std::cout << cdv::generate_dot_visualization_string(visualization) << "\n";
}

void example_5_nullptr()
{
    cdv::visualization<std::string> visualization;

    visualization.add_data_structure(nullptr);

    int *null_int = nullptr;
    visualization.add_data_structure(null_int);

    char **null_charptr = nullptr;
    visualization.add_data_structure(null_charptr);

    std::cout << cdv::generate_dot_visualization_string(visualization) << "\n";
}


void example_6_user_defined_tree()
{
    cdv::visualization<std::string> visualization;

    TreeNode root{"root"};

    //              root
    //             /    \
    //            a      b
    //           / \    / \
    //          c   d  e   f

    root.left = std::make_unique<TreeNode>("a");
    root.right = std::make_unique<TreeNode>("b");
    root.left->left = std::make_unique<TreeNode>("c");
    root.left->right = std::make_unique<TreeNode>("c");
    root.right->left = std::make_unique<TreeNode>("e");
    root.right->right = std::make_unique<TreeNode>("f");

    visualization.add_data_structure(root);

    std::cout << cdv::generate_dot_visualization_string(visualization) << "\n";
}


void big_example()
{
    using cell = cdv::table_node<std::string>::cell;
    using arrow = cdv::arrow<std::string>;

    cdv::visualization<std::string> my_viz; // Un vecteur de valeurs.

    // -------------------------------------------- Example table nodes --------------------------------------------- //

    {
        auto my_table_node =
            cdv::table_node{}
                .with_row(cell{"First Node"}.spanning_columns(3))
                .with_row(cell{"Row 1"}.with_port("mySourcePort"), cell::make(3.14), "Note: longer row than others")
                .with_row("Row 2", 6.71)
                .with_row("Row 3", "Types can change from one row to the other!");

        auto my_second_table_node =
            cdv::table_node{}
                .with_row(cell{"Second Node"}.spanning_columns(4))
                .with_row("Baby", "tiny", cell{"node"}.with_port("myDestinationPort"), "on one row only!");
    }
    // ------------------------------------------- Visualization creation ------------------------------------------- //

    {
#if 0
        my_viz.add_node(0, std::move(my_table_node));
        my_viz.add_node(1, std::move(my_second_table_node));
        // Simple edge between nodes, no ports.
        my_viz.add_edge(arrow{ 0, 1 }.with_style(cdv::edge_style::normal));
        // Edge between two nodes, with specific source and destination ports.
        my_viz.add_edge(arrow{ 0, "mySourcePort", 1, "myDestinationPort" }.with_style(cdv::edge_style::normal));

#endif
    }

    // ----------------------------------- Automatic data structure visualization ----------------------------------- //

    {
        std::vector<int> my_int_vec;
        my_int_vec.reserve(10);
        for (int i = 0; i < 10; ++i)
            my_int_vec.emplace_back(i);
        // my_viz.add_data_structure(my_int_vec);

        std::vector<double *> my_double_ptr_vec; // Un vecteur de pointeurs sur des doubles.
        my_double_ptr_vec.reserve(10);
        for (int i = 0; i < 10; ++i)
            my_double_ptr_vec.emplace_back(new double(i * 2.0 + 0.4));
        // my_viz.add_data_structure(my_double_ptr_vec);

        {
            std::vector<std::vector<int *> *> test;
            for (int vecidx = 0; vecidx < 5; vecidx++)
            {
                std::vector<int *> *vec = new std::vector<int *>;
                for (int k = 0; k < 5; ++k)
                {
                    vec->emplace_back(new int(k * 2 + 1));
                }
                test.emplace_back(vec);
            }
            // my_viz.add_data_structure(test);
        }

        // my_viz.add_data_structure(3); // Un bête entier.

        std::list<int> my_int_list; // Une liste d'entiers.
        for (int i = 0; i < 10; ++i)
            my_int_list.emplace_back(i);
        // my_viz.add_data_structure(my_int_list, true);

        std::forward_list<int *> my_intptr_list; // Une liste de pointeurs sur des entiers.
        for (int i = 0; i < 10; ++i)
            my_intptr_list.emplace_front(new int(i * 3 % 5));
        my_viz.add_data_structure(my_intptr_list);
    }

    // -------------------------------------------- Custom user classes --------------------------------------------- //

    {
        MyClass some_class;
        my_viz.add_data_structure(some_class);

        // std::pair<std::string, double> my_pair{ "MyStringValue", 42.1 };
        // my_viz.add_data_structure(my_pair);
    }

    // ------------------------------------------------ Text export ------------------------------------------------- //

    const std::string my_graphviz_text = cdv::generate_dot_visualization_string(my_viz);
    std::cout << my_graphviz_text << std::endl;
}

int main()
{
    // example_1();
    // example_2_optional();
    // example_3_vector_of_struct();
    // example_4_user_defined_graph();
    // example_5_nullptr();
    example_6_user_defined_tree();
    return 0;
}
