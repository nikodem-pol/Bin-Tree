#pragma once
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <type_traits>
#include <string>

//Kolory wêz³ów drzewa
enum class Color { RED, BLACK };

//Struktura wêze³
template<typename N>
struct node
{
    N key_value;                  //Wartoœæ przechowywana w wêŸle
    node *parent, *left, *right; //WskaŸniki na rodzica i potomków wêz³a
    Color color;                  //Kolor wêz³a
    node(N value) : key_value(value), parent(nullptr), left(nullptr), right(nullptr), color(Color::RED){}
};

template<typename T>
class binTree
{
    static_assert(std::is_same<T, int>::value || 
        std::is_same<T, char>::value ||
        std::is_same<T, std::string>::value, 
        "Typ musi byæ int, char lub string"); //Szablon dzia³a tylko dla typów int, char, string.
private:
    node<T>* root;  //Korzeñ drzewa
    int quantity;   //Aktualna iloœæ wêz³ów w drzewie

    int level_r(node<T>* leaf, int level);              //Rekurencyjna funkcja wykorzystana do wyliczenia liczby poziomów drzewa
    void print_tree(node<T>* leaf, int level);          //Rekurencyjna funkcja wykorzystana do wypisywania drzewana na wyjscie standardowe
    void destroy(node<T>* &leaf);                       //Rekurencyjna funkcja, która odpowiada za usuwanie drzewa
    void insert_r( node<T>* &new_elem, node<T>* leaf);  //Rekurencyjna funkcja, doadaje element do drzewa
    void removeNode(node<T>* element);                  //Rekurencyjna funkcja usuwaj¹ca wêze³ o podanym wskaŸniku
    node<T>* findNode_r(const T& value, node<T>* leaf);       //Rekurencyjna funkcja, szuka w drzewie elementu o podanej wartoœci i zwaraca wskaŸnik do szukaego elementu
    node<T>* findNode(const T& value);                  //Zwraca wskaŸnik do elementu o podanej wartoœci

protected:
    void rotateLeft(node<T>*& root, node<T>*& pt);      //Obrót podanego wêz³a w lewo
    void rotateRight(node<T>*& root, node<T>*& pt);     //Obrót podanego wêz³a w prawo
    void balanceTree(node<T>*& root, node<T>* pt);      //Balansowanie drzewa po dodaniu nowego elementu
    void fixDoubleBlack(node<T>* &pt);                  //Usuwanie problemu podwójnie czarnych wêz³ów przy usuwaniu elementów

public:
    binTree() : root(nullptr), quantity(0) {}                             //Konstruktor bezargumentowy, tworzy puste drzewo
    binTree(std::initializer_list<T> list) : root(nullptr), quantity(0) //Konstruktor, tworzy drzewo na podstawie listy inicjalizacyjnej
    {
        
        for ( const T* it = begin(list); it != end(list); it++)
        {
            this->insert(*it);
        }
    }
    ~binTree() {destroy_tree(); }                                         //Destruktor, usuwa wszystkie elementy z drzewa

    int count() { return quantity; }                                      //Zwraca liczbê elementów przechowywanych w drzewie
    int level();                                                          //Zwraca liczê poziomów drzewa
    void print_tree();                                                    //Wypisuje drzewo na wyjœci standardowe
    void insert(const T& value);                                          //dodaje do drzewa nowy element
    void remove(const T& value);                                          //Usuwa z drzewa element o podanej wartoœci
    void destroy_tree() { quantity = 0; destroy(root); root = nullptr; }  //Usuwa wszystkie elementy z drzewa
    T* find(const T value) {
        node<T>* pt = this->findNode(value);
        if (pt != nullptr)
        {
            T* n = &(pt->key_value);
            return  n;
        }
        else
            return nullptr;
    }                                                                     //Zwraca wskaŸnik do szukanej wartoœci
};

//Utility
/////////////////////////////////////////////////////////////////////////

//Obrót wêz³a w lewo
template<typename T>
void binTree<T>::rotateLeft(node<T>*& root, node<T>*& pt)
{
    node<T>* pt_right = pt->right;

    //Zmiana prawego potomka pt na lewego potomka pt_right
    pt->right = pt_right->left;

    //Ustawianie nowego rodzica wymienionego potomka 
    if (pt->right != nullptr)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    //Jeœli pt by³ korzeniem to teraz jest nim pt_right
    if (pt->parent == nullptr)
        root = pt_right;

    //Jeœli pt by³ lewym potomkiem swojego rodzica
    //to teraz jest nim pt_right
    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    //Jeœli pt by³ prawym potomkiem swojego rodzica
    //to teraz jest nim pt_right
    else
        pt->parent->right = pt_right;

    //pt jest teraz lewym potomkiem pt_right
    pt_right->left = pt;
    pt->parent = pt_right;
}

//Obrót wêz³a w prawo
template<typename T>
void binTree<T>::rotateRight(node<T>*& root, node<T>*& pt)
{
    node<T>* pt_left = pt->left;

    //Zmiana lewego potomka pt na prawego potomka pt_right
    pt->left = pt_left->right;

    //Ustawianie nowego rodzica wymienionego potomka 
    if (pt->left != nullptr)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    //Jeœli pt by³ korzeniem to teraz jest nim pt_left
    if (pt->parent == nullptr)
        root = pt_left;

    //Jeœli pt by³ lewym potomkiem swojego rodzica
    //to teraz jest nim pt_left
    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    //Jeœli pt by³ prawym potomkiem swojego rodzica
    //to teraz jest nim pt_left
    else
        pt->parent->right = pt_left;

    //pt jest teraz lewym potomkiem pt_right
    pt_left->right = pt;
    pt->parent = pt_left;
}

//Funkcja zwracaj¹ca wujka podanego wêz³a
template<typename T>
node<T>* get_uncle(node<T>* x)
{
    if (x->parent == x->parent->parent->left)
        return x->parent->parent->right;
    else
        return x->parent->parent->left;
}

//Funkcja zwracaj¹ca rodzeñstwo podanego wêz³a
template<typename T>
node<T>* get_sibling(node<T>* x)
{
    if (x == x->parent->left)
        return x->parent->right;
    else
        return x->parent->left;
}

//Zwraca wiêksz¹ liczbê z dwóch podanych
int max(int a, int b)
{
    return a >= b ? a : b;
}

//Destroy
/////////////////////////////////////////////////////////////////////////
template<typename T>
void binTree<T>::destroy(node<T>* &leaf)
{
    if (leaf != nullptr)
    {
        destroy(leaf->left);
        destroy(leaf->right);
        delete leaf;
    }
}

//Insert
/////////////////////////////////////////////////////////////////////////

//Balansowanie drzewa
template<typename T>
void binTree<T>::balanceTree(node<T>*& root, node<T>* pt)
{
    node<T>* parent_pt = nullptr;
    node<T>* grand_parent_pt = nullptr;

    while ((pt != nullptr) && (pt != root) && (pt->color != Color::BLACK) && (pt->parent->color == Color::RED))
    {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;
    //Przypadek A: Rodzic pt jest lewym potomkiem dziadka pt
        if (parent_pt == grand_parent_pt->left)
        {
            node<T>* uncle_pt = grand_parent_pt->right;

        //Przypadek 1: Wujek pt jest czerwony, wêz³y zostaj¹ tylko przekolorowane
            if (uncle_pt != nullptr && uncle_pt->color == Color::RED)
            {
                grand_parent_pt->color = Color::RED;
                parent_pt->color = Color::BLACK;
                uncle_pt->color = Color::BLACK;
                pt = grand_parent_pt;
            }
            else
            {
            //Przypadek 2: pt jest lewym dzieckiem swojego rodzica
                if (pt == parent_pt->right)
                {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                
            //Przypadek 3: pt jest prawym dzieckiem swojego rodzica

                rotateRight(root, grand_parent_pt);
                std::swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
        //Przypadek B: Rodzic pt jest prawym potomkiem dziadka pt
        else
        {
            node<T>* uncle_pt = grand_parent_pt->left;

            //Przypadek 1:  Wujek pt jest czerwony, wêz³y zostaj¹ tylko przekolorowane
            if ((uncle_pt != nullptr) && (uncle_pt->color == Color::RED))
            {
                grand_parent_pt->color = Color::RED;
                parent_pt->color = Color::BLACK;
                uncle_pt->color = Color::BLACK;
                pt = grand_parent_pt;

            }
            else
            {
                //Przypadek 2: pt jest lewym dzieckiem swojego rodzica
                if (pt == parent_pt->left)
                {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                //Przypadek 3: pt jest prawym dzieckiem swojego rodzica
                rotateLeft(root, grand_parent_pt);
                std::swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    root->color = Color::BLACK;
}

//Rekurencyjne wstawianie nowego elementu
template<typename T>
void binTree<T>::insert_r(node<T>*& new_node, node<T>* leaf)
{
    if (new_node->key_value < leaf->key_value)
    {
        if (leaf->left != nullptr)
        {
            //Dalsze poszukiwanie miejsca
            insert_r(new_node, leaf->left);
        }
        else
        {
            //Dodawanie nowego wêz³a
            leaf->left = new_node;
            new_node->parent = leaf;
        }
    }
    else if (new_node->key_value > leaf->key_value)
    {
        if (leaf->right != nullptr)
        {
            //Dalsze poszukiwanie miejsca
            insert_r(new_node, leaf->right);
        }
        else
        {
            //Dodawanie nowego wêz³a
            leaf->right = new_node;
            new_node->parent = leaf;
        }
    }
    else if (new_node->key_value == leaf->key_value)
    {
        //Zapobieganie wyciekom pamiêci w przypadku duplikatów
        delete new_node;
        new_node = nullptr;
    }
}

//Wstawianie elementu o podanej wartoœci
template<typename T>
void binTree<T>::insert(const T& value)
{
    
    node<T>* new_element = new node<T>(value);

    if (root == nullptr)
        root = new_element;
    else
        insert_r(new_element, root);

    //W przypadku próby dodania duplikatu do drzewa 
    //wartoœæ new_element zostanie zmieniona na nullptr w funkcji insert_r
    if (new_element != nullptr)
    {
        quantity++;
        balanceTree(root, new_element);
    }
}

//Remove
/////////////////////////////////////////////////////////////////////////

//Usuwanie problemu podwójnie czarnych wêz³ów
template<typename T>
void binTree<T>::fixDoubleBlack(node<T>*& pt)
{
    //p jest korzeniem
    if (pt == root)
        return;

    node<T>* sibling_pt = get_sibling(pt);
    node<T>* parent_pt = pt->parent;

    //Jeœli rodzeñstwo nie istnieje, 
    //rzowi¹zujemy problrm dla rodzica pt
    if (sibling_pt == nullptr)
    {
        fixDoubleBlack(parent_pt);

    }
    else
    {
        //Rodzeñstwo pt jest czerwone
        if (sibling_pt->color == Color::RED)
        {
            //Przekolorywanie rodzica oraz rodzeñstwa
            parent_pt->color = Color::RED;
            sibling_pt->color = Color::BLACK;

            //pt jest prawym potomkiem
            if (sibling_pt == parent_pt->left)
            {
                rotateRight(root, parent_pt);
            }
            //pt jest lewym potomkiem
            else
            {
                rotateLeft(root, parent_pt);
            }
            fixDoubleBlack(pt);
        }
        //Rodzeñstwo pt jest czarne
        else
        {
            //Rodzeñstwo ma conajmniej jednego czerwonego potomka
            if (((sibling_pt->left != nullptr) && sibling_pt->left->color == Color::RED) ||
                ((sibling_pt->right != nullptr) && (sibling_pt->right->color == Color::RED)))
            {
                //Lewy potomek rodezñstwa pt jest czerwony
                if ((sibling_pt->left != nullptr) && (sibling_pt->left->color == Color::RED))
                {
                    //Rodzeñstwo pt jest lewym potomkirm rodzica pt
                    if (sibling_pt == parent_pt->left)
                    {
                        //sibling_pt->left przekolorowany na czarny
                        sibling_pt->left->color = sibling_pt->color;
                        sibling_pt->color = parent_pt->color;
                        rotateRight(root, sibling_pt);
                    }
                    //Rodzeñstwo pt jest prawym potomkiem rodzica pt
                    else
                    {
                        sibling_pt->left->color = parent_pt->color;
                        rotateRight(root, sibling_pt);
                        rotateLeft(root, parent_pt);
                    }
                }
                //Prawy potomek rodzeñstwa pt jest czerwony
                else
                {
                    //Rodzeñstwo pt jest lewym potomkirm rodzica pt
                    if (sibling_pt == parent_pt->left)
                    {
                        sibling_pt->right->color = parent_pt->color;
                        rotateLeft(root, sibling_pt);
                        rotateRight(root, parent_pt);
                    }
                    //Rodzeñstwo pt jest prawym potomkirm rodzica pt
                    else
                    {
                        //sibling_pt->right przekolorowany na czarny
                        sibling_pt->right->color = sibling_pt->color;
                        sibling_pt->color = parent_pt->color;
                        rotateLeft(root, parent_pt);
                    }
                }
                parent_pt->color = Color::BLACK;
            }
            //Rodzeñstwo pt ma czarnych potomków
            else
            {
                sibling_pt->color = Color::RED;
                //Rodzic pt jest czarny
                if (parent_pt->color == Color::BLACK)
                    fixDoubleBlack(parent_pt);
                else
                    parent_pt->color = Color::BLACK;
            }
        }
    }
   
}

//Znajduje wêze³ który niema lewego potomka w podanym poddrzewie
template<typename T>
node<T>* successor(node<T>* pt)
{
    node<T>* temp = pt;

    while (temp->left != nullptr)
        temp = temp->left;

    return temp;
}

//Znajduje wêze³ który zajmie miejsce usuniêtego wêz³a
template<typename T>
node<T>* replace(node<T>* pt)
{
    //pt ma obu potomków
    if (pt->left != nullptr && pt->right != nullptr)
        return successor(pt->right);

    //pt nie ma potomków, jest liœciem
    if (pt->left == nullptr && pt->right == nullptr)
        return nullptr;

    //Wêze³ ma jedneg potomka
    if (pt->left == nullptr)
        return pt->right;
    else
        return pt->left;
}

//Rekurencyjene usuwanie wêz³a
template<typename T>
void binTree<T>::removeNode(node<T>* element)
{
    node<T>* x = replace(element);

    //Prawda gdy x i element s¹ czarne
    bool elemxBlack = ((x == nullptr || x->color == Color::BLACK) && (element->color == Color::BLACK));
    node<T>* parent = element->parent;
       
    //x = nullptr, element jest liœciem
    if (x == nullptr)
    {
        //element jest korzeniem, drzewo sk³ada siê z jednego elementu
        if (element == root)
        {
            root = nullptr;
        }
        else
        {
            //element i x s¹ czarne
            if (elemxBlack)
            {
                //Naprawa problemu podwójnie czarnych wêz³ów
                fixDoubleBlack(element);
            }
            else
            {
                node<T>* sibling_elem = get_sibling(element);

                //Przekolorowanie rodzeñstwa elementu na czerwony
                if (sibling_elem != nullptr)
                    sibling_elem->color = Color::RED;
            }

            //Od³¹cznnie elementu od drzewa
            if (element == parent->left)
                parent->left = nullptr;
            else
                parent->right = nullptr;
        }
        delete element;
        return;
    }
    
    //element ma jednego potomka
    if (element->left == nullptr || element->right == nullptr)
    {
        //element jest korzeniem
        if (element == root)
        {
            //Wymiana wartoœci miêdzy x i elementem
            //Usuniêcie x
            element->key_value = x->key_value;
            element->left = nullptr;
            element->right = nullptr;
            delete x;
        }
        else
        {
            //Od³¹czanie elemetu od drzewa
            if (element == parent->left)
                parent->left = x;
            else
                parent->right = x;

            delete element;

            x->parent = parent;

            //Jeœli element i x by³y czarne
            //naprawiamy problem podwójnie czarneg wêz³a
            if (elemxBlack)
                fixDoubleBlack(x);
            else
                x->color = Color::BLACK;
        }
        return;
    }

    //Jesli x != nullptr wumieniamy x i element wartoœciami
    //i próbujemy usun¹æ wêze³ wskazywany przez x
    std::swap(element->key_value, x->key_value);
    removeNode(x);
}


//Usuwanie wêz³a o podanej wartoœci
template<typename T>
void binTree<T>::remove(const T &value)
{
    //Drezwo jest puste
    if (root == nullptr)
        return;

    //Poszukiwanie odpowiedniego wskaŸnika
    node<T>* element = findNode(value);

    //Jeœli odpowiedni element zosta³ znaleziony, usuwamy go 
    if (element != nullptr && element->key_value == value)
    {
        removeNode(element);
        quantity--;
    }
}

//Find
/////////////////////////////////////////////////////////////////////////

//Rekurencyjne szukanie wêz³a
template<typename T>
node<T>* binTree<T>::findNode_r(const T& value, node<T>* leaf)
{
    if (leaf != nullptr)
    {
        if (value == leaf->key_value)
            return leaf;
        if (value < leaf->key_value)
            return findNode_r(value, leaf->left);
        if (value > leaf->key_value)
            return findNode_r(value, leaf->right);
    }
    return nullptr;
}

//Szukanie wêz³a o podanej wartoœci
template<typename T>
node<T>* binTree<T>::findNode(const T& value)
{
    return findNode_r(value, root);
}



//Print
/////////////////////////////////////////////////////////////////////////

//Wypisywanie na wyjœcie standardowe tabów
void print_tab(int level)
{
    for (int i = 0; i < level; i++)
        std::cout << "\t\t";
}

//Wyœwietlanie drzewa w konsoli
template<typename T>
void binTree<T>::print_tree()
{
    std::cout << "BINARY TREE\n";
    print_tree(root, 0);
}

//Rekurencyjne wyœwietlanie w konsoli kolejnych wêz³ów drzewa
template<typename T>
void binTree<T>::print_tree(node<T>* leaf, int level)
{

    if (leaf != nullptr)
    {
        std::cout << std::endl;
        print_tab(level);
        std::cout << "NODE: " << leaf->key_value << std::endl;

        print_tab(level);
        if (leaf->right != nullptr)
        {   
            
            std::cout << "right: " << leaf->right->key_value << std::endl;
        }
        else
        {
            
            std::cout << "right: nullptr" << std::endl;
        }

        print_tab(level);
        if (leaf->left != nullptr)
        {
            
            std::cout << "left: " << leaf->left->key_value << std::endl;
        }
        else
        {
            
            std::cout << "left: nullptr" << std::endl;
        }
        print_tab(level);
        std::cout << "Color: ";
        if (leaf->color == Color::RED)
            std::cout << "RED" << std::endl;
        else
            std::cout << "BLACK" << std::endl;
        ++level;
        print_tree(leaf->right, level);
        print_tree(leaf->left, level);
    }

}

//Level
//////////////////////////////////////////////////////////////////////////////////

//Metoda zwraca liczbê poziomów drzewa
template<typename T>
int binTree<T>::level()
{
    if (binTree::root != nullptr)
    {
       return level_r(binTree::root, 1);
    }
    else
    {
        return 0;
    }
}

//Rekurencyjne wyliczanie poziomu drzewa
template<typename T>
int binTree<T>::level_r(node<T>* leaf, int level)
{
    //Aktualny wêze³ to liœæ
    if (leaf->left == nullptr && leaf->right == nullptr)
        return level;

    ++level;
    
    //Wêze³ ma tylko prawego potomka
    if (leaf->left == nullptr)
        return level_r(leaf->right, level);

    //Wêze³ ma tylko prawego potomka
    if (leaf->right == nullptr)
        return level_r(leaf->left, level);

    //Wêze³ ma obu potomków
    return max(level_r(leaf->left, level), level_r(leaf->right, level));
}
