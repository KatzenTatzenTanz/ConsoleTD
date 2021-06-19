#include <iostream>

std::string Map =
    "                    "
    "                    "
    "                    "
    "                    "
    "      ... #...      "
    "      . .... .      "
    "      ..  #  ...S   "
    "       ......       "
    "     ~  ### .#      "
    "   ~~~~......       "
    "   ~~~ . # #        "
    "       .#           "
    "       .            "
    "       .            "
    "       E            "
    "                    "
    "                    "
    "                    "
    "                    "
    "                    ";

int width = 20;
int height = Map.length() / width;
std::string *drawnMap;

/*
1000 + 1
0100 + 2
1100 ┘ 3
0010 + 4
1010 ─ 5
0110 └ 6
1110 ┴ 7
0001 + 8
1001 ┐ 9
0101 │ 10
1101 ┤ 11
0011 ┌ 12
1011 ┬ 13
0111 ├ 14
1111 ┼ 15
*/
std::string* paths = new std::string[17] {"$","$","$","╯","$","─","╰","┴","$","╮","│","┤","╭","┬","├","┼"};

void TextureMap()
{
    drawnMap = new std::string[height];
    for (int y = 0, i = 0; y < height; y++)
    {
        char last = -1;
        drawnMap[y] = "";
        for (int x = 0; x < width; x++, i++)
        {
            if (last != Map.at(i))
            {
                switch (Map.at(i))
                {
                case '#':
                    drawnMap[y] += "\033[0m\033[32m";
                    break;
                case '~':
                    drawnMap[y] += "\033[0m\033[34m";
                    break;
                case ' ':
                    drawnMap[y] += "\033[2m\033[37m";
                    break;
                case '.':
                    drawnMap[y] += "\033[0m\033[31m";
                    break;
                case 'S':
                    drawnMap[y] += "\033[0m\033[33m";
                    break;
                case 'E':
                    drawnMap[y] += "\033[0m\033[35m";
                    break;
                default:
                    break;
                }
            }
            switch (Map.at(i))
            {
            case ' ': {
                drawnMap[y] += '.';
                break;
            }
            case '.': {
                int connections = 0;
                std::string s = ".ES";
                char c = Map.at(i - 1);
                if (x > 0 && (c == '.' || c == 'E' || c == 'S'))
                    connections += 1;
                c = Map.at(i - width);
                if (y > 0 && (c == '.' || c == 'E' || c == 'S'))
                    connections += 2;
                c = Map.at(i + 1);
                if (x < width - 1 && (c == '.' || c == 'E' || c == 'S'))
                    connections += 4;
                c = Map.at(i + width);
                if (y < height - 1 && (c == '.' || c == 'E' || c == 'S'))
                    connections += 8;
                drawnMap[y] += paths[connections];
                break;
            }
            default:
                drawnMap[y] += Map.at(i);
                break;
            }
            last = Map.at(i);
        }
    }
}

int selected_x = 10,selected_y = 10;
bool running = true;
int money = 1000, wave = 1, health = 100;
char input;

int mode;

void drawMap()
{
    std::cout << "\033[2J";
    for (int i = 0; i < height; i++)
        std::cout << "\033[" + std::to_string(i+6) + "f\033[2G\033[0m" + drawnMap[i];
    std::cout << "\033[5f\033[1G\033[0m\033[35m╭";
    for(int i = 0; i < width; i++) {
        std::cout << "─";
    }
    std::cout << "╮";
    for(int i = 0; i < height; i++) {
        std::cout << "\033[" + std::to_string(6 + i) + "f\033[1G\033[0m\033[35m│";
        std::cout << "\033[" + std::to_string(width+2) + "G\033[0m\033[35m│";
    }
    std::cout << "\033[" + std::to_string(height + 6) + "f\033[1G\033[0m\033[35m╰";
    for(int i = 0; i < width; i++) {
        std::cout << "─";
    }
    std::cout << "╯";
    std::cout << "\033[1f\033[0G\033[0mWASD/ARROWS to move";
    std::cout << "\033[2f\033[0G\033[0mSPACE to select";
    std::cout << "\033[3f\033[0G\033[0mENTER to start wave";
    std::cout << "\033[4f\033[0G\033[0mE/ESCAPE to leave";
    std::cout << "\033[1f\033[25G\033[1m\033[33mMONEY: " + std::to_string(money);
    std::cout << "\033[2f\033[25G\033[1m\033[31mWAVE: " + std::to_string(wave);
    std::cout << "\033[3f\033[25G\033[1m\033[35mHEALTH: " + std::to_string(health);
    std::cout << "\033[23f\033[25G\033[1m\033[35mLAST INPUT: " + std::to_string(input);
    std::cout << "\033[" + std::to_string(selected_y + 6) + "f\033[" + std::to_string(selected_x + 2) + "G";
}

void DrawShop() {

}

void controlls() {
    if(std::cin.peek() != EOF) {
        char c;
        std::cin.read(&c,1);
        if(c == 'e' || c == 27) {
            running = false;
            return;
        }
        if(c == 'w' || c == 65)
            if(selected_y > 0)
                selected_y-=1;
        if(c == 's'|| c == 66)
            if(selected_y < height-1)
                selected_y+=1;
        if(c == 'a'|| c == 68)
            if(selected_x > 0)
            selected_x-=1;
        if(c == 'd'|| c == 67)
            if(selected_x < width-1)
            selected_x+=1;
        input = c;
    }
}

int main()
{
    TextureMap();
    system("stty raw");
    std::cout << "\033[2 q";
    while(running) {
        drawMap();
        controlls();
    }
    drawMap();
    std::cout << "\033[" + std::to_string(height + 10) + "f\033[0G";
    system("stty -raw");
    return 0;
}