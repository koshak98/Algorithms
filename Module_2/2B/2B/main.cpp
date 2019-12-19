#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <limits>
#include <map>

//////////////////////////////////////////////////////////
template <class SizeType>
struct SubstringInfo {
    SizeType string_index;
    SizeType begin;
    SizeType end;
};

template <class SizeType>
struct Active {
    SizeType node;
    SubstringInfo<SizeType> substring;
};
//////////////////////////////////////////////////////////

template <class SizeType>
class ISuffixTree {
protected:
    std::vector<std::string> strings_;
    SizeType node_count_ = 1;
public:
    template <class Iter>
    void AddString(Iter, Iter) {};
    const std::vector<std::string>& GetAllStrings() const;
    virtual SizeType Root() const = 0;
    SizeType GetNodeCount() const;
    virtual std::pair<SubstringInfo<SizeType>, SizeType> GetTransition(SizeType, char) const = 0;
    virtual std::vector<std::pair<SubstringInfo<SizeType>, SizeType>> GetAllTransitions(SizeType) const = 0;
};
//////////////////////////////////////////////////////////

template <class SizeType = size_t>
class OnlineSuffixTree: public ISuffixTree<SizeType> {
    public:
    static const SizeType SuperRoot = std::numeric_limits<SizeType>::max();
    static const SizeType RootInfinity = std::numeric_limits<SizeType>::max();
protected:
    std::vector<std::map<char, std::pair<SubstringInfo<SizeType>, SizeType>>> edges_;
    std::vector<SizeType> suffix_link_;
    Active<SizeType> active_point_;
    bool start_new_string_ = true;
public:
    OnlineSuffixTree();
    std::pair<bool, SizeType> TestAndSplit(const Active<SizeType>&, char);
    template <class Iter>
    void AddString(Iter, Iter);
    void AddChar(char);
    void TerminateCurrentString();
    SizeType GetSuffixLink(SizeType) const;
    virtual SizeType Root() const override;
    virtual std::pair<SubstringInfo<SizeType>, SizeType> GetTransition(SizeType, char) const override;
    virtual std::vector<std::pair<SubstringInfo<SizeType>, SizeType>> GetAllTransitions(SizeType) const override;
    void Canonize(Active<SizeType>&) const;
};
//////////////////////////////////////////////////////////

template <class SizeType>
const SizeType OnlineSuffixTree<SizeType>::SuperRoot;
template <class SizeType>
const SizeType OnlineSuffixTree<SizeType>::RootInfinity;
//////////////////////////////////////////////////////////

template <class Tree, class SizeType>
void PrintSuffixTree(const Tree&, std::ostream&, std::vector<uint32_t>&);
//////////////////////////////////////////////////////////

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string first_str;
    std::string second_str;
    OnlineSuffixTree<uint32_t> tree;
    tree.AddString(first_str.cbegin(), first_str.cend());
    tree.AddString(second_str.cbegin(), second_str.cend());
    std::cout << tree.GetNodeCount() << std::endl;
    std::vector<uint32_t> nodes(tree.GetNodeCount());
    PrintSuffixTree(tree, std::cout, nodes);
    
    return 0;
    
}
//////////////////////////////////////////////////////////

template <class Tree, class SizeType>
void PrintSuffixTree(const Tree& tree, std::ostream& out, std::vector<SizeType>& new_name) {
    SizeType current_index = 0;
    std::vector<std::pair<SizeType, Active<SizeType>>> stack;
    stack.emplace_back(tree.Root(), Active<SizeType>{tree.Root(), SubstringInfo<SizeType>{0, 0, 0}});
    while (!stack.empty()) {
        auto [current, in_edge] = stack.back();
        stack.pop_back();
        new_name[current] = current_index++;
        in_edge.substring.end = std::min<SizeType>(
            in_edge.substring.end,
            tree.GetAllStrings()[in_edge.substring.string_index].size());
        if (current != tree.Root()) {
            out << new_name[in_edge.node] << ' ' << in_edge.substring.string_index
                << ' ' << in_edge.substring.begin << ' ' << in_edge.substring.end
                << '\n';
        }
        auto edges = tree.GetAllTransitions(current);
        for (auto it = edges.crbegin(); it != edges.crend(); ++it) {
            stack.emplace_back(it->second, Active<SizeType>{current, it->first});
        }
    }
    
}
//////////////////////////////////////////////////////////

template <class SizeType>
const std::vector<std::string>& ISuffixTree<SizeType>::GetAllStrings() const {
  return strings_;
}

template <class SizeType>
SizeType ISuffixTree<SizeType>::GetNodeCount() const {
  return node_count_;
}
//////////////////////////////////////////////////////////

template <class SizeType>
OnlineSuffixTree<SizeType>::OnlineSuffixTree(): edges_(1), suffix_link_(1, SuperRoot), active_point_{Root(), {0, 0, 0}} {}

template <class SizeType>
std::pair<bool, SizeType> OnlineSuffixTree<SizeType>::TestAndSplit(const Active<SizeType>& ref, char ch){
  if (ref.substring.begin == ref.substring.end)
    return {GetTransition(ref.node, ch).second != SuperRoot, ref.node};

  char first_char =
      OnlineSuffixTree::strings_[ref.substring.string_index]
                                         [ref.substring.begin];
  auto [substring, node] = GetTransition(ref.node, first_char);
  char middle_char =
      OnlineSuffixTree::strings_[substring.string_index]
                                         [substring.begin + ref.substring.end -
                                          ref.substring.begin];
  if (middle_char == ch)
    return {true, ref.node};

  ++OnlineSuffixTree::node_count_;
  edges_.emplace_back();
  suffix_link_.emplace_back();
  edges_[ref.node][first_char] = {
      {substring.string_index, substring.begin,
       substring.begin + ref.substring.end - ref.substring.begin},
      edges_.size() - 1};
  edges_.back()[middle_char] = {
      {substring.string_index,
       substring.begin + ref.substring.end - ref.substring.begin,
       substring.end},
      node};
  return {false, edges_.size() - 1};
}

template <class SizeType>
template <class Iter>
void OnlineSuffixTree<SizeType>::AddString(Iter begin, Iter end) {
    for (; begin != end; ++begin) {
        AddChar(*begin);
    }
  TerminateCurrentString();
}

template <class SizeType>
void OnlineSuffixTree<SizeType>::AddChar(char ch) {
  if (start_new_string_) {
      OnlineSuffixTree::strings_.emplace_back();
      start_new_string_ = false;
      active_point_ = {Root(), {static_cast<unsigned int>(OnlineSuffixTree::strings_.size() - 1), 0, 0}};
  }
  OnlineSuffixTree::strings_.back().push_back(ch);

  SizeType prev_node = Root();
  auto [end_point, new_node] = TestAndSplit(active_point_, ch);
  while (!end_point) {
      ++OnlineSuffixTree::node_count_;
      edges_.emplace_back();
      suffix_link_.emplace_back();
      edges_[new_node][ch] = {{static_cast<unsigned int>(OnlineSuffixTree::strings_.size() - 1), static_cast<unsigned int>(OnlineSuffixTree::strings_.back().size() - 1), RootInfinity}, OnlineSuffixTree::node_count_ - 1};
      if (prev_node != Root())
      suffix_link_[prev_node] = new_node;
      prev_node = new_node;
      active_point_.node = suffix_link_[active_point_.node];
      Canonize(active_point_);
      std::tie(end_point, new_node) = TestAndSplit(active_point_, ch);
  }
  if (prev_node != Root())
      suffix_link_[prev_node] = active_point_.node;
      ++active_point_.substring.end;
      Canonize(active_point_);
}

template <class SizeType>
void OnlineSuffixTree<SizeType>::TerminateCurrentString() {
  start_new_string_ = true;
}

template <class SizeType>
SizeType OnlineSuffixTree<SizeType>::GetSuffixLink(SizeType node) const {
    if (node == SuperRoot) {
      return SuperRoot;
    }
    return suffix_link_[node];
}

template <class SizeType>
SizeType OnlineSuffixTree<SizeType>::Root() const {
    return 0;
}

template <class SizeType>
std::pair<SubstringInfo<SizeType>, SizeType> OnlineSuffixTree<SizeType>::GetTransition(SizeType node, char ch) const {
    if (node == SuperRoot) {
        return {{0, 0, 1}, Root()};
    }
    auto it = edges_[node].find(ch);
    if (it == edges_[node].end()) {
        return {{0, 0, 0}, SuperRoot};
    }
    return it->second;
}

template <class SizeType>
std::vector<std::pair<SubstringInfo<SizeType>, SizeType>> OnlineSuffixTree<SizeType>::GetAllTransitions(SizeType node) const {
    std::vector<std::pair<SubstringInfo<SizeType>, SizeType>> wrapper;
    for (const auto& pair : edges_[node]) {
        wrapper.push_back(pair.second);
    }
    return wrapper;
}

template <class SizeType>
void OnlineSuffixTree<SizeType>::Canonize(Active<SizeType>& ref) const {
  if (ref.substring.begin == ref.substring.end) return;

  auto [substring, node] = GetTransition(
      ref.node, OnlineSuffixTree::strings_[ref.substring.string_index]
                                                   [ref.substring.begin]);
  while (ref.substring.end - ref.substring.begin >= substring.end - substring.begin) {
    ref.substring.begin += substring.end - substring.begin;
    ref.node = node;
    if (ref.substring.begin < ref.substring.end)
      std::tie(substring, node) = GetTransition(ref.node, OnlineSuffixTree::strings_[ref.substring.string_index][ref.substring.begin]);
  }
}

