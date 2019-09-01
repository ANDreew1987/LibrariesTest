#include <string>
#include <vector>
#include <memory>

using std::basic_string;
using std::char_traits;
using std::allocator;
using std::pair;
using std::vector;
using std::unique_ptr;

class AlphabetChar;

template<class T>
struct BasicNode
{
public:
    using basic_data_type = basic_string<T, char_traits<T>, allocator<T>>;
    using name_value_type = basic_data_type;
    using attr_value_type = basic_data_type;
    using attr = pair<attr_value_type, attr_value_type>;

    name_value_type name;
    vector<attr> attributes;
    vector<unique_ptr<BasicNode<T>>> childNodes;

    BasicNode() = default;

    BasicNode(
        const name_value_type &_name):
        name(_name) {}
};

using Node = BasicNode<char>;

Node *GenMainNode();
