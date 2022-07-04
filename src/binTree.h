#pragma once
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <type_traits>
#include <string>

//Kolory w�z��w drzewa
enum class Color { RED, BLACK };

//Struktura w�ze�
template<typename N>
struct node
{
    N key_value;                  //Warto�� przechowywana w w�le
    node *parent, *left, *right; //Wska�niki na rodzica i potomk�w w�z�a
    Color color;                  //Kolor w�z�a
    node(N value) : key_value(value), parent(nullptr), left(nullptr), right(nullptr), color(Color::RED){}
};

template<typename T>
class binTree
{
    static_assert(std::is_same<T, int>::value || 
        std::is_same<T, char>::value ||
        std::is_same<T, std::string>::value, 
        "Typ musi by� int, char lub string"); //Szablon dzia�a tylko dla typ�w int, char, string.
private:
    node<T>* root;  //Korze� drzewa
    int quantity;   //Aktualna ilo�� w�z��w w drzewie

    int level_r(node<T>* leaf, int level);              //Rekurencyjna funkcja wykorzystana do wyliczenia liczby poziom�w drzewa
    void print_tree(node<T>* leaf, int level);          //Rekurencyjna funkcja wykorzystana do wypisywania drzewana na wyjscie standardowe
    void destroy(node<T>* &leaf);                       //Rekurencyjna funkcja, kt�ra odpowiada za usuwanie drzewa
    void insert_r( node<T>* &new_elem, node<T>* leaf);  //Rekurencyjna funkcja, doadaje element do drzewa
    void removeNode(node<T>* element);                  //Rekurencyjna funkcja usuwaj�ca w�ze� o podanym wska�niku
    node<T>* findNode_r(const T& value, node<T>* leaf);       //Rekurencyjna funkcja, szuka w drzewie elementu o podanej warto�ci i zwaraca wska�nik do szukaego elementu
    node<T>* findNode(const T& value);                  //Zwraca wska�nik do elementu o podanej warto�ci

protected:
    void rotateLeft(node<T>*& root, node<T>*& pt);      //Obr�t podanego w�z�a w lewo
    void rotateRight(node<T>*& root, node<T>*& pt);     //Obr�t podanego w�z�a w prawo
    void balanceTree(node<T>*& root, node<T>* pt);      //Balansowanie drzewa po dodaniu nowego elementu
    void fixDoubleBlack(node<T>* &pt);                  //Usuwanie problemu podw�jnie czarnych w�z��w przy usuwaniu element�w

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

    int count() { return quantity; }                                      //Zwraca liczb� element�w przechowywanych w drzewie
    int level();                                                          //Zwraca licz� poziom�w drzewa
    void print_tree();                                                    //Wypisuje drzewo na wyj�ci standardowe
    void insert(const T& value);                                          //dodaje do drzewa nowy element
    void remove(const T& value);                                          //Usuwa z drzewa element o podanej warto�ci
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
    }                                                                     //Zwraca wska�nik do szukanej warto�ci
};

//Utility
/////////////////////////////////////////////////////////////////////////

//Obr�t w�z�a w lewo
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

    //Je�li pt by� korzeniem to teraz jest nim pt_right
    if (pt->parent == nullptr)
        root = pt_right;

    //Je�li pt by� lewym potomkiem swojego rodzica
    //to teraz jest nim pt_right
    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    //Je�li pt by� prawym potomkiem swojego rodzica
    //to teraz jest nim pt_right
    else
        pt->parent->right = pt_right;

    //pt jest teraz lewym potomkiem pt_right
    pt_right->left = pt;
    pt->parent = pt_right;
}

//Obr�t w�z�a w prawo
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

    //Je�li pt by� korzeniem to teraz jest nim pt_left
    if (pt->parent == nullptr)
        root = pt_left;

    //Je�li pt by� lewym potomkiem swojego rodzica
    //to teraz jest nim pt_left
    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    //Je�li pt by� prawym potomkiem swojego rodzica
    //to teraz jest nim pt_left
    else
        pt->parent->right = pt_left;

    //pt jest teraz lewym potomkiem pt_right
    pt_left->right = pt;
    pt->parent = pt_left;
}

//Funkcja zwracaj�ca wujka podanego w�z�a
template<typename T>
node<T>* get_uncle(node<T>* x)
{
    if (x->parent == x->parent->parent->left)
        return x->parent->parent->right;
    else
        return x->parent->parent->left;
}

//Funkcja zwracaj�ca rodze�stwo podanego w�z�a
template<typename T>
node<T>* get_sibling(node<T>* x)
{
    if (x == x->parent->left)
        return x->parent->right;
    else
        return x->parent->left;
}

//Zwraca wi�ksz� liczb� z dw�ch podanych
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

        //Przypadek 1: Wujek pt jest czerwony, w�z�y zostaj� tylko przekolorowane
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

            //Przypadek 1:  Wujek pt jest czerwony, w�z�y zostaj� tylko przekolorowane
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
            //Dodawanie nowego w�z�a
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
            //Dodawanie nowego w�z�a
            leaf->right = new_node;
            new_node->parent = leaf;
        }
    }
    else if (new_node->key_value == leaf->key_value)
    {
        //Zapobieganie wyciekom pami�ci w przypadku duplikat�w
        delete new_node;
        new_node = nullptr;
    }
}

//Wstawianie elementu o podanej warto�ci
template<typename T>
void binTree<T>::insert(const T& value)
{
    
    node<T>* new_element = new node<T>(value);

    if (root == nullptr)
        root = new_element;
    else
        insert_r(new_element, root);

    //W przypadku pr�by dodania duplikatu do drzewa 
    //warto�� new_element zostanie zmieniona na nullptr w funkcji insert_r
    if (new_element != nullptr)
    {
        quantity++;
        balanceTree(root, new_element);
    }
}

//Remove
/////////////////////////////////////////////////////////////////////////

//Usuwanie problemu podw�jnie czarnych w�z��w
template<typename T>
void binTree<T>::fixDoubleBlack(node<T>*& pt)
{
    //p jest korzeniem
    if (pt == root)
        return;

    node<T>* sibling_pt = get_sibling(pt);
    node<T>* parent_pt = pt->parent;

    //Je�li rodze�stwo nie istnieje, 
    //rzowi�zujemy problrm dla rodzica pt
    if (sibling_pt == nullptr)
    {
        fixDoubleBlack(parent_pt);

    }
    else
    {
        //Rodze�stwo pt jest czerwone
        if (sibling_pt->color == Color::RED)
        {
            //Przekolorywanie rodzica oraz rodze�stwa
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
        //Rodze�stwo pt jest czarne
        else
        {
            //Rodze�stwo ma conajmniej jednego czerwonego potomka
            if (((sibling_pt->left != nullptr) && sibling_pt->left->color == Color::RED) ||
                ((sibling_pt->right != nullptr) && (sibling_pt->right->color == Color::RED)))
            {
                //Lewy potomek rodez�stwa pt jest czerwony
                if ((sibling_pt->left != nullptr) && (sibling_pt->left->color == Color::RED))
                {
                    //Rodze�stwo pt jest lewym potomkirm rodzica pt
                    if (sibling_pt == parent_pt->left)
                    {
                        //sibling_pt->left przekolorowany na czarny
                        sibling_pt->left->color = sibling_pt->color;
                        sibling_pt->color = parent_pt->color;
                        rotateRight(root, sibling_pt);
                    }
                    //Rodze�stwo pt jest prawym potomkiem rodzica pt
                    else
                    {
                        sibling_pt->left->color = parent_pt->color;
                        rotateRight(root, sibling_pt);
                        rotateLeft(root, parent_pt);
                    }
                }
                //Prawy potomek rodze�stwa pt jest czerwony
                else
                {
                    //Rodze�stwo pt jest lewym potomkirm rodzica pt
                    if (sibling_pt == parent_pt->left)
                    {
                        sibling_pt->right->color = parent_pt->color;
                        rotateLeft(root, sibling_pt);
                        rotateRight(root, parent_pt);
                    }
                    //Rodze�stwo pt jest prawym potomkirm rodzica pt
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
            //Rodze�stwo pt ma czarnych potomk�w
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

//Znajduje w�ze� kt�ry niema lewego potomka w podanym poddrzewie
template<typename T>
node<T>* successor(node<T>* pt)
{
    node<T>* temp = pt;

    while (temp->left != nullptr)
        temp = temp->left;

    return temp;
}

//Znajduje w�ze� kt�ry zajmie miejsce usuni�tego w�z�a
template<typename T>
node<T>* replace(node<T>* pt)
{
    //pt ma obu potomk�w
    if (pt->left != nullptr && pt->right != nullptr)
        return successor(pt->right);

    //pt nie ma potomk�w, jest li�ciem
    if (pt->left == nullptr && pt->right == nullptr)
        return nullptr;

    //W�ze� ma jedneg potomka
    if (pt->left == nullptr)
        return pt->right;
    else
        return pt->left;
}

//Rekurencyjene usuwanie w�z�a
template<typename T>
void binTree<T>::removeNode(node<T>* element)
{
    node<T>* x = replace(element);

    //Prawda gdy x i element s� czarne
    bool elemxBlack = ((x == nullptr || x->color == Color::BLACK) && (element->color == Color::BLACK));
    node<T>* parent = element->parent;
       
    //x = nullptr, element jest li�ciem
    if (x == nullptr)
    {
        //element jest korzeniem, drzewo sk�ada si� z jednego elementu
        if (element == root)
        {
            root = nullptr;
        }
        else
        {
            //element i x s� czarne
            if (elemxBlack)
            {
                //Naprawa problemu podw�jnie czarnych w�z��w
                fixDoubleBlack(element);
            }
            else
            {
                node<T>* sibling_elem = get_sibling(element);

                //Przekolorowanie rodze�stwa elementu na czerwony
                if (sibling_elem != nullptr)
                    sibling_elem->color = Color::RED;
            }

            //Od��cznnie elementu od drzewa
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
            //Wymiana warto�ci mi�dzy x i elementem
            //Usuni�cie x
            element->key_value = x->key_value;
            element->left = nullptr;
            element->right = nullptr;
            delete x;
        }
        else
        {
            //Od��czanie elemetu od drzewa
            if (element == parent->left)
                parent->left = x;
            else
                parent->right = x;

            delete element;

            x->parent = parent;

            //Je�li element i x by�y czarne
            //naprawiamy problem podw�jnie czarneg w�z�a
            if (elemxBlack)
                fixDoubleBlack(x);
            else
                x->color = Color::BLACK;
        }
        return;
    }

    //Jesli x != nullptr wumieniamy x i element warto�ciami
    //i pr�bujemy usun�� w�ze� wskazywany przez x
    std::swap(element->key_value, x->key_value);
    removeNode(x);
}


//Usuwanie w�z�a o podanej warto�ci
template<typename T>
void binTree<T>::remove(const T &value)
{
    //Drezwo jest puste
    if (root == nullptr)
        return;

    //Poszukiwanie odpowiedniego wska�nika
    node<T>* element = findNode(value);

    //Je�li odpowiedni element zosta� znaleziony, usuwamy go 
    if (element != nullptr && element->key_value == value)
    {
        removeNode(element);
        quantity--;
    }
}

//Find
/////////////////////////////////////////////////////////////////////////

//Rekurencyjne szukanie w�z�a
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

//Szukanie w�z�a o podanej warto�ci
template<typename T>
node<T>* binTree<T>::findNode(const T& value)
{
    return findNode_r(value, root);
}



//Print
/////////////////////////////////////////////////////////////////////////

//Wypisywanie na wyj�cie standardowe tab�w
void print_tab(int level)
{
    for (int i = 0; i < level; i++)
        std::cout << "\t\t";
}

//Wy�wietlanie drzewa w konsoli
template<typename T>
void binTree<T>::print_tree()
{
    std::cout << "BINARY TREE\n";
    print_tree(root, 0);
}

//Rekurencyjne wy�wietlanie w konsoli kolejnych w�z��w drzewa
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

//Metoda zwraca liczb� poziom�w drzewa
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
    //Aktualny w�ze� to li��
    if (leaf->left == nullptr && leaf->right == nullptr)
        return level;

    ++level;
    
    //W�ze� ma tylko prawego potomka
    if (leaf->left == nullptr)
        return level_r(leaf->right, level);

    //W�ze� ma tylko prawego potomka
    if (leaf->right == nullptr)
        return level_r(leaf->left, level);

    //W�ze� ma obu potomk�w
    return max(level_r(leaf->left, level), level_r(leaf->right, level));
}
