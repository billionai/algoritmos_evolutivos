//ESSE ARQUIVO EH TEMPORARIO, SO PARA TESTAR SE A CLASSE GRAFO ESTA FUNCIONANDO

#include "robot.h"
#include <time.h>

int main(){
    grafo g(10);
    std::vector<coord> keys,doors;
    g.gen_map();
    g.draw();
    robo r(g);
    r.debug();
    return 0;
}
