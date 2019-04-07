#include <iostream>
#include <algorithm>
#include <random>
#include <stack>
#include <list>

class Tree
{

public:
struct node
{
        int index;
        node* parent;
        std::list<node* > v;
};

Tree();
~Tree();
void add_descendant(int);
void print_bracket_list(node*);
void print_DFS(node*);
void print_DFS();
int get_nodes_count();
node* get_node(int);
node* get_root();

private:
int number_of_nodes;
node* root;

};

Tree::Tree()
{
        root = new node;
        root->parent = nullptr;
        root->index = 0;
        number_of_nodes = 1;
}

Tree::~Tree()
{

}

void Tree::add_descendant(int index)
{
        node* predecessor = get_node(index);
        node* new_node = new node;

        predecessor->v.push_back(new_node);
        new_node->index = number_of_nodes;
        new_node->parent = predecessor;

        number_of_nodes++;
}

Tree::node* Tree::get_node(int index)
{
        if(index == 0)
                return root;

        std::stack<Tree::node*> st;
        st.push(root);

        while(!st.empty())
        {
                node* current = st.top();
                st.pop();

                if(current->index == index)
                        return current;

                for(node* n : current->v)
                        st.push(n);
        }

        return nullptr;
}

void switch_nodes(Tree::node* first_node, Tree::node* second_node)
{
        Tree::node* helper = second_node;

        second_node->v = first_node->v;
        first_node->v = helper->v;

        second_node->parent = first_node->parent;
        first_node->parent = helper->parent;

        second_node->parent->v.remove(first_node);
        first_node->parent->v.remove(second_node);

        first_node->parent->v.push_back(first_node);
        second_node->parent->v.push_back(second_node);
}

void Tree::print_DFS()
{
        std::stack<Tree::node*> st;
        st.push(root);

        while(!st.empty())
        {
                node* current = st.top();
                st.pop();
                std::cout << current->index << " ";
                for(node* n : current->v)
                        st.push(n);
        }
}

void Tree::print_bracket_list(Tree::node* starting_node)
{
        int x = starting_node->index;
        std::cout << x << " ";
        if( !starting_node->v.empty() )
        {
                std::cout << "( ";
                for(node* n : starting_node->v)
                {
                        print_bracket_list(n);
                }
                std::cout << ") ";
        }
}

int Tree::get_nodes_count()
{
        return number_of_nodes;
}

Tree::node* Tree::get_root()
{
        return root;
}

int main()
{
        int trees_count, nodes_count, desc_count;
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> random_generator(0, 1);

        std::cout << "Maximal number of nodes in one tree : ";
        std::cin >> nodes_count;
        std::cout << "Number of trees : ";
        std::cin >> trees_count;
        std::cout << "Number of pairs of crossed trees : ";
        std::cin >> desc_count;

        Tree* tr = new Tree[trees_count];

        for(int i = 0; i < trees_count; i++)
        {
                random_generator.param( std::uniform_int_distribution<>::param_type(nodes_count/2, nodes_count - 1));
                int nodes = random_generator(rng);
                for(int j = 0; j < nodes; j++)
                {
                        int x = tr[i].get_nodes_count();
                        random_generator.param( std::uniform_int_distribution<>::param_type( 0, x - 1 ) );
                        int index = random_generator(rng);
                        tr[i].add_descendant(index);
                }
        }

        std::cout << "-- Generated " << trees_count << " trees --\n";

        for(int i = 0; i < trees_count; i++)
        {
                Tree::node* root = tr[i].get_root();
                std::cout << "\nBracket list of " << i << " tree: \n";
                tr[i].print_bracket_list(root);
                std::cout << "\n";
        }

        std::cout << "\n----Crossing trees in random nodes...----\n";
        for(int i = 0; i < desc_count; i++)
        {
                std::uniform_int_distribution<int> random_generator( 0, trees_count - 1 );

                auto a = random_generator(rng);
                auto b = random_generator(rng);
                while(b == a) b = random_generator(rng);

                std::cout << "\n-- Mixing " << a << " tree and " << b << " tree --\n";
                Tree t;
                t = tr[a];
                Tree first = t;
                t = tr[b];
                Tree second = t;

                a = first.get_nodes_count();
                random_generator.param( std::uniform_int_distribution<>::param_type( 1, a - 1 ) );
                a = random_generator(rng);

                b = second.get_nodes_count();
                random_generator.param( std::uniform_int_distribution<>::param_type( 1, b - 1 ) );
                b = random_generator(rng);

                std::cout << "-- Mixing " << a << " node and " << b << " node --\n";
                switch_nodes( first.get_node(a), second.get_node(b) );

                std::cout << "Bracket list of first mixed tree: \n";
                first.print_bracket_list( first.get_node(0) );

                std::cout << "\nBracket list of second mixed tree: \n";
                second.print_bracket_list( second.get_node(0) );

                std::cout << "\n";
        }

        delete[] tr;
        return 0;
}
