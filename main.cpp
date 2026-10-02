#include "Trie.h"

void imprimir(const std::vector<std::string>& v) {
    std::cout << "{";
    for (size_t i = 0; i < v.size(); i++)
        std::cout << v[i] << (i + 1 < v.size() ? ", " : "");
    std::cout << "}\n";
}

int main() {
    Trie<char> trie;
    trie.insert("cat");
    trie.insert("car");
    trie.insert("cart");
    trie.insert("do");
    trie.insert("dog");
    std::cout << "size: " << trie.size() << "\n";                          

    std::cout << "search(car): "     << trie.search("car") << "\n";        
    std::cout << "search(ca): "      << trie.search("ca") << "\n";           
    std::cout << "startWith(ca): "   << trie.startWith("ca") << "\n";         
    std::cout << "startWith(x): "    << trie.startWith("x") << "\n";           

    auto r = trie.autocomplete("car");
    std::sort(r.begin(), r.end());                                  
    std::cout << "autocomplete(car): "; imprimir(r);     

    trie.remove("car");
    std::cout << "tras remove(car) -> search(car): " << trie.search("car")
              << ", search(cart): " << trie.search("cart")
              << ", size: " << trie.size() << "\n";                     
    return 0;
}