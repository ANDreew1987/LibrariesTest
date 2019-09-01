#include "Node.hpp"

#include <random>
#include <codecvt>

using std::string;
using std::u32string;
using std::uniform_int_distribution;
using std::random_device;
using std::mt19937;
using std::move;
using std::wstring_convert;
using std::codecvt_utf8;

class AlphabetUChar
{
public:
    using value_type = typename string::value_type;

    AlphabetUChar()
    {
        m_Alphabet = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
        };
    }

    string::size_type size() const
    {
        return m_Alphabet.size();
    }

    const value_type at(const string::size_type index) const
    {
        return m_Alphabet.at(index);
    }
private:
    string m_Alphabet;
};

class AlphabetLChar
{
public:
    using value_type = typename string::value_type;

    AlphabetLChar()
    {
        m_Alphabet = {
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
        };
    }

    string::size_type size() const
    {
        return m_Alphabet.size();
    }

    const value_type at(const string::size_type index) const
    {
        return m_Alphabet.at(index);
    }
private:
    string m_Alphabet;
};

class AlphabetEmojiChar
{
public:
    using alphabet_type = u32string;
    using value_type = typename alphabet_type::value_type;

    AlphabetEmojiChar()
    {
        m_Alphabet = {
            U'\U0001F600', // 😀 GRINNING FACE
            U'\U0001F601', // 😁 GRINNING FACE WITH SMILING EYES
            U'\U0001F602', // 😂 FACE WITH TEARS OF JOY
            U'\U0001F603', // 😃 SMILING FACE WITH OPEN MOUTH
            U'\U0001F604', // 😄 SMILING FACE WITH OPEN MOUTH AND SMILING EYES
            U'\U0001F605', // 😅 SMILING FACE WITH OPEN MOUTH AND COLD SWEAT
            U'\U0001F606', // 😆 SMILING FACE WITH OPEN MOUTH AND TIGHTLY-CLOSED EYES
            U'\U0001F607', // 😇 SMILING FACE WITH HALO
            U'\U0001F608', // 😈 SMILING FACE WITH HORNS
            U'\U0001F609', // 😉 WINKING FACE
            U'\U0001F60A', // 😊 SMILING FACE WITH SMILING EYES
            U'\U0001F60B', // 😋 FACE SAVOURING DELICIOUS FOOD
            U'\U0001F60C', // 😌 RELIEVED FACE
            U'\U0001F60D', // 😍 SMILING FACE WITH HEART-SHAPED EYES
            U'\U0001F60E', // 😎 SMILING FACE WITH SUNGLASSES
            U'\U0001F60F', // 😏 SMIRKING FACE
            U'\U0001F610', // 😐 NEUTRAL FACE
            U'\U0001F611', // 😑 EXPRESSIONLESS FACE
            U'\U0001F612', // 😒 UNAMUSED FACE
            U'\U0001F613', // 😓 FACE WITH COLD SWEAT
            U'\U0001F614', // 😔 PENSIVE FACE
            U'\U0001F615', // 😕 CONFUSED FACE
            U'\U0001F616', // 😖 CONFOUNDED FACE
            U'\U0001F617', // 😗 KISSING FACE
            U'\U0001F618', // 😘 FACE THROWING A KISS
            U'\U0001F619', // 😙 KISSING FACE WITH SMILING EYES
            U'\U0001F61A', // 😚 KISSING FACE WITH CLOSED EYES
            U'\U0001F61B', // 😛 FACE WITH STUCK-OUT TONGUE
            U'\U0001F61C', // 😜 FACE WITH STUCK-OUT TONGUE AND WINKING EYE
            U'\U0001F61D', // 😝 FACE WITH STUCK-OUT TONGUE AND TIGHTLY-CLOSED EYES
            U'\U0001F61E', // 😞 DISAPPOINTED FACE
            U'\U0001F61F', // 😟 WORRIED FACE
            U'\U0001F620', // 😠 ANGRY FACE
            U'\U0001F621', // 😡 POUTING FACE
            U'\U0001F622', // 😢 CRYING FACE
            U'\U0001F623', // 😣 PERSEVERING FACE
            U'\U0001F624', // 😤 FACE WITH LOOK OF TRIUMPH
            U'\U0001F625', // 😥 DISAPPOINTED BUT RELIEVED FACE
            U'\U0001F626', // 😦 FROWNING FACE WITH OPEN MOUTH
            U'\U0001F627', // 😧 ANGUISHED FACE
            U'\U0001F628', // 😨 FEARFUL FACE
            U'\U0001F629', // 😩 WEARY FACE
            U'\U0001F62A', // 😪 SLEEPY FACE
            U'\U0001F62B', // 😫 TIRED FACE
            U'\U0001F62C', // 😬 GRIMACING FACE
            U'\U0001F62D', // 😭 LOUDLY CRYING FACE
            U'\U0001F62E', // 😮 FACE WITH OPEN MOUTH
            U'\U0001F62F', // 😯 HUSHED FACE
            U'\U0001F630', // 😰 FACE WITH OPEN MOUTH AND COLD SWEAT
            U'\U0001F631', // 😱 FACE SCREAMING IN FEAR
            U'\U0001F632', // 😲 ASTONISHED FACE
            U'\U0001F633', // 😳 FLUSHED FACE
            U'\U0001F634', // 😴 SLEEPING FACE
            U'\U0001F635', // 😵 DIZZY FACE
            U'\U0001F636', // 😶 FACE WITHOUT MOUTH
            U'\U0001F637', // 😷 FACE WITH MEDICAL MASK
            U'\U0001F638', // 😸 GRINNING CAT FACE WITH SMILING EYES
            U'\U0001F639', // 😹 CAT FACE WITH TEARS OF JOY
            U'\U0001F63A', // 😺 SMILING CAT FACE WITH OPEN MOUTH
            U'\U0001F63B', // 😻 SMILING CAT FACE WITH HEART-SHAPED EYES
            U'\U0001F63C', // 😼 CAT FACE WITH WRY SMILE
            U'\U0001F63D', // 😽 KISSING CAT FACE WITH CLOSED EYES
            U'\U0001F63E', // 😾 POUTING CAT FACE
            U'\U0001F63F', // 😿 CRYING CAT FACE
            U'\U0001F640', // 🙀 WEARY CAT FACE
            U'\U0001F641', // 🙁 SLIGHTLY FROWNING FACE
            U'\U0001F642', // 🙂 SLIGHTLY SMILING FACE
            U'\U0001F643', // 🙃 UPSIDE-DOWN FACE
            U'\U0001F644', // 🙄 FACE WITH ROLLING EYES
            U'\U0001F645', // 🙅 FACE WITH NO GOOD GESTURE
            U'\U0001F646', // 🙆 FACE WITH OK GESTURE
            U'\U0001F647', // 🙇 PERSON BOWING DEEPLY
            U'\U0001F648', // 🙈 SEE-NO-EVIL MONKEY
            U'\U0001F649', // 🙉 HEAR-NO-EVIL MONKEY
            U'\U0001F64A', // 🙊 SPEAK-NO-EVIL MONKEY
            U'\U0001F64B', // 🙋 HAPPY PERSON RAISING ONE HAND
            U'\U0001F64C', // 🙌 PERSON RAISING BOTH HANDS IN CELEBRATION
            U'\U0001F64D', // 🙍 PERSON FROWNING
            U'\U0001F64E', // 🙎 PERSON WITH POUTING FACE
            U'\U0001F64F'  // 🙏 PERSON WITH FOLDED HANDS
        };
    }

    alphabet_type::size_type size() const
    {
        return m_Alphabet.size();
    }

    const value_type at(const alphabet_type::size_type index) const
    {
        return m_Alphabet.at(index);
    }
private:
    alphabet_type m_Alphabet;
};

template<class A>
class CharGenerator
{
public:
    using value_type = typename A::value_type;

    CharGenerator(
        const A &alphabet):
        m_Alphabet(alphabet)
    {
    }

    template<class Engine>
    value_type GetChar(Engine &engine) const
    {
        uniform_int_distribution<Engine::result_type> dis(0, m_Alphabet.size() - 1);
        return m_Alphabet.at(dis(engine));
    }
private:
    A m_Alphabet;
};

template<class A>
class StringGenerator
{
public:
    using value_type = basic_string<
        typename A::value_type,
        char_traits<typename A::value_type>,
        allocator<typename A::value_type>>;

    StringGenerator(
        const int size,
        const CharGenerator<A> &chGen):
        m_Size(size),
        m_ChGen(chGen)
    {
    }

    template<class Engine>
    value_type GetString(Engine &engine) const
    {
        value_type result(m_Size, '0');
        for (auto &ch : result) {
            ch = m_ChGen.GetChar(engine);
        }
        return result;
    }
private:
    int m_Size;
    CharGenerator<A> m_ChGen;
};

template<class A>
class StringVarLenGenerator
{
public:
    using value_type = basic_string<
        typename A::value_type,
        char_traits<typename A::value_type>,
        allocator<typename A::value_type>>;

    StringVarLenGenerator(
        const int minLen,
        const int maxLen,
        const CharGenerator<A> &chGen):
        m_MinLen(minLen),
        m_MaxLen(maxLen),
        m_ChGen(chGen)
    {
    }

    template<class Engine>
    value_type GetString(Engine &engine) const
    {
        uniform_int_distribution<Engine::result_type> dis(m_MinLen, m_MaxLen);
        StringGenerator strGen(dis(engine), m_ChGen);
        return strGen.GetString(engine);
    }
private:
    int m_MinLen;
    int m_MaxLen;
    CharGenerator<A> m_ChGen;
};

constexpr int subNodesCount = 10;
constexpr int subSubNodesCount = 50;
constexpr int subSubSubNodesCount = 10;

constexpr int subNodeAttributesCount = 3;
constexpr int subSubNodeAttributesCount = 3;
constexpr int subSubSubNodeAttributesCount = 5;

string to_utf8(const u32string &s)
{
    wstring_convert<codecvt_utf8<int32_t>, int32_t> convert;
    auto p = reinterpret_cast<const int32_t *>(s.data());
    return convert.to_bytes(p, p + s.size());
}

Node *GenMainNode()
{
    random_device rd;
    mt19937 gen(rd());

    AlphabetUChar alphabetUChar;
    AlphabetLChar alphabetLChar;
    AlphabetEmojiChar alphabetEmoji;

    CharGenerator chGenTags(alphabetUChar);
    StringVarLenGenerator strGenTags(3, 10, chGenTags);

    CharGenerator chGenAttrs(alphabetLChar);
    StringVarLenGenerator strGenAttrs(3, 15, chGenAttrs);

    CharGenerator chGenAttrValues(alphabetEmoji);
    StringVarLenGenerator strGenAttrValues(3, 20, chGenAttrValues);

    Node *mainNode = new Node(strGenTags.GetString(gen));

    for (int i = 0; i < subNodesCount; i++) {
        unique_ptr<Node> subNode(
            new Node(strGenTags.GetString(gen)));
        for (int i = 0; i < subSubNodesCount; i++) {
            unique_ptr<Node> subSubNode(
                new Node(strGenTags.GetString(gen)));
            for (int i = 0; i < subSubSubNodesCount; i++) {
                unique_ptr<Node> subSubSubNode(
                    new Node(strGenTags.GetString(gen)));
                for (int i = 0; i < subSubSubNodeAttributesCount; i++) {
                    subSubSubNode->attributes.emplace_back(
                        strGenAttrs.GetString(gen), to_utf8(strGenAttrValues.GetString(gen)));
                }
                subSubNode->childNodes.push_back(move(subSubSubNode));
            }
            for (int i = 0; i < subSubNodeAttributesCount; i++) {
                subSubNode->attributes.emplace_back(
                    strGenAttrs.GetString(gen), to_utf8(strGenAttrValues.GetString(gen)));
            }
            subNode->childNodes.push_back(move(subSubNode));
        }
        for (int i = 0; i < subNodeAttributesCount; i++) {
            subNode->attributes.emplace_back(
                strGenAttrs.GetString(gen), to_utf8(strGenAttrValues.GetString(gen)));
        }
        mainNode->childNodes.push_back(move(subNode));
    }
    return mainNode;
}
