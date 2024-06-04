#include "../cdv/include/cdv.hpp"


class MyClass
{
public:
	const std::string& get_string() const
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

	const char* MyPublicMember { "charptr member" };

private:
	std::string my_string{ "some text" };
	int my_int{ 24 };
	double my_double{ 3.14 };
};

CDV_DECLARE_PUBLIC_MEMBER(MyClass, 0, MyPublicMember)
CDV_DECLARE_CUSTOM_MEMBER(MyClass, 1, "my_string", get_string())
CDV_DECLARE_CUSTOM_MEMBER(MyClass, 2, "my_int", get_int())
CDV_DECLARE_CUSTOM_MEMBER(MyClass, 3, "my_double", get_double())

int main()
{
	using cell = cdv::table_node<std::string>::cell;
	using arrow = cdv::arrow<std::string>;

	cdv::visualization<std::string> my_viz; // Un vecteur de valeurs.

	// -------------------------------------------- Example table nodes --------------------------------------------- //

	{
    	auto my_table_node = cdv::table_node{}
        	.with_row(cell{ "First Node" }.spanning_columns(3))
        	.with_row(cell{ "Row 1" }.with_port("mySourcePort"), cell::make(3.14), "Note: longer row than others")
        	.with_row("Row 2", 6.71)
        	.with_row("Row 3", "Types can change from one row to the other!");

    	auto my_second_table_node = cdv::table_node{}
        	.with_row(cell{ "Second Node" }.spanning_columns(4))
        	.with_row("Baby", "tiny", cell{ "node" }.with_port("myDestinationPort"), "on one row only!");

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
    	//my_viz.add_data_structure(my_int_vec);

    	std::vector<double*> my_double_ptr_vec; // Un vecteur de pointeurs sur des doubles.
    	my_double_ptr_vec.reserve(10);
    	for (int i = 0; i < 10; ++i)
        	my_double_ptr_vec.emplace_back(new double(i * 2.0 + 0.4));
    	//my_viz.add_data_structure(my_double_ptr_vec);

    	{
        	std::vector<std::vector<int*>*> test;
        	for (int vecidx = 0; vecidx < 5; vecidx++)
        	{
            	std::vector<int*>* vec = new std::vector<int*>;
            	for (int k = 0; k < 5; ++k)
            	{
                	vec->emplace_back(new int(k * 2 + 1));
            	}
            	test.emplace_back(vec);
        	}
        	//my_viz.add_data_structure(test);
    	}

    	//my_viz.add_data_structure(3); // Un bête entier.

    	std::list<int> my_int_list; // Une liste d'entiers.
    	for (int i = 0; i < 10; ++i)
        	my_int_list.emplace_back(i);
    	//my_viz.add_data_structure(my_int_list, true);

    	std::forward_list<int*> my_intptr_list; // Une liste de pointeurs sur des entiers.
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

	const std::string my_graphviz_text = cdv::generate_visualization_string(my_viz);
	std::cout << my_graphviz_text << std::endl;

	return 0;
}
