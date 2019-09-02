#include <string>
#include <vector>
#include <memory>

using std::basic_string;
using std::char_traits;
using std::allocator;
using std::pair;
using std::vector;
using std::unique_ptr;

template<class T>
struct BasicNode
{
public:
    using basic_data_type = basic_string<T, char_traits<T>, allocator<T>>;
    using attr_type = pair<basic_data_type, basic_data_type>;

    basic_data_type name;
    vector<attr_type> attributes;
    vector<unique_ptr<BasicNode<T>>> childNodes;

    BasicNode() = default;

    BasicNode(
        const basic_data_type &_name):
        name(_name) {}
};

using Node = BasicNode<char>;

Node *GenMainNode();
