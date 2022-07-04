#include <iostream>
#include "binTree.h"
//#include <vld.h>

int main()
{  
    int n = 1;
    
    //Int
    if (n == 1)
    {
        binTree<int> tree;

        for (int i = 1; i < 1000; i++)
            tree.insert(i);

        std::cout << "Liczba elementow: " << tree.count() << "\n\n";
        std::cout << "Liczba poziomow: " << tree.level() << "\n\n";

        tree.insert(5);
        tree.insert(10);

        std::cout << "Liczba elementow pod dodaniu duplikatow: " << tree.count() << "\n\n";
        std::cout << "Liczba poziomow pod dodaniu duplikatow: " << tree.level() << "\n\n";

        int *adress1 = tree.find(15); 
        int *adress2 = tree.find(200);

        std::cout << "Adres 15: ";
        if (adress1 == nullptr)
            std::cout << "nullptr\n\n";
        else
            std::cout << adress1 << ", wartosc: " << *adress1 << "\n\n";
        std::cout << "Adres 200: ";
        if (adress2 == nullptr)
            std::cout << "nullptr\n\n";
        else
            std::cout << adress2 <<", wartosc: " << *adress2 << "\n\n";

        tree.destroy_tree();
    }
    //Char
    else if (n == 2)
    {

        binTree<char> tree = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };

        std::cout << "\nLiczba elementow: " << tree.count() << "\n";
        std::cout << "Liczba poziomow: " << tree.level() << "\n\n";

        tree.print_tree();

        std::cout << "\n===============================================================================" << "\n\n";
        std::cout << "Usuniete i oraz d\n\n";
        tree.remove('i');
        tree.remove('d');

        std::cout << "Liczba elementow po usunieciu: " << tree.count() << "\n";
        std::cout << "Liczba poziomow po usunieciu: " << tree.level() << "\n\n";

        tree.print_tree();
        tree.destroy_tree();

    }
    //String
    else
    {
        std::string napisy[] = { "ostk", "dgmo", "ukycx", "gnzxo", "kpnyx", "qsxil", "wihdj"};
        binTree<std::string> tree;

        for (auto& x : napisy)
            tree.insert(x);

        tree.print_tree();
        
    }
    
    /*VLDEnable();
    VLDReportLeaks();*/
    
}   
