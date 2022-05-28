#ifndef MENU_CPP
#define MENU_CPP

#include "menu.h"

template<class T>
void input(T* a, const std::string& msg = "Enter a value:\n")
{
    while (true)
    {
        std::cout << msg;
        std::cin >> *a;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            continue;
        }
        break;
    }
}

void menu()
{
    std::cout << "Welcome to the menu of ternary tree program made by Tarasov Vladimir\n"
                 "Integer TernaryTree is used in this menu\nAll right reserved\n\n";

    TernaryTree<int> tree;

    int t; int elem; std::string path; std::string result;
    printf("Enter 1 to add a node, 2 to delete a node, 3 to set node's value\n4 to get height of the tree, "
           "5 to equate the tree to it's subtree, 6 to convert the tree to string using A variation\n"
           "7 to convert the tree to string using B variation, 8 to find elem, 9 to print the tree\n10 to change traversal of the tree, "
           "11 to exit the program\n");

    while (true)
    {
        try
        {
            while (true)
            {
                t = -1;
                while (t < 1 || t > 12)
                    input(&t, "Enter a command:\n");


                switch (t)
                {
                    case 11:
                        exit(0);
                        break;

                    case 1:
                        input(&elem);
                        if (tree.getHeight() > 0)
                            input(&path, "Enter a path:\n");
                        tree.addElem(path, elem);
                        break;

                    case 2:
                        input(&path, "Enter a path:\n");
                        if (path == "root")
                            path = "";

                        tree.deleteNode(path);
                        break;

                    case 3:
                        input(&elem);
                        input(&path, "Enter a path:\n");
                        if (path == "root")
                            path = "";

                        tree.setElem(path, elem);
                        break;

                    case 9:
                        tree.print();
                        break;

                    case 4:
                        std::cout << "Height of the tree is: " << tree.getHeight() << std::endl;
                        break;

                    case 5:
                        input(&path, "Enter a path:\n");
                        if (path == "root")
                            path = "";

                        tree = tree.getSubTree(path);
                        break;

                    case 6:
                        input(&path, "Enter a traversal:\n");
                        tree.changeTraversal(path);
                        std::cout << "The result string is: " << tree.convertToStringA() << std::endl;
                        break;

                    case 7:
                        std::cout << "The result string is: " << tree.convertToStringB() << std::endl;
                        break;

                    case 8:
                        input(&elem);
                        result = tree.find(elem);
                        if (result.empty())
                            result = "root";

                        std::cout << "Position of the elem in tree is: " << result << std::endl;
                        break;

                    case 10:
                        input(&path, "Enter a traversal:\n");
                        tree.changeTraversal(path);
                        break;
                }
            }
        }
        catch (const Errors& error)
        {
            std::cout << "An error has occurred, it's code is: = " << error << std::endl;
        }
    }
}

#endif