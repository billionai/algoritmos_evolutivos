#include "robot.h"

void wait(int ms){
    usleep(ms*1000);
}

robo::robo(const robo& parent):gene(parent.gene),path(parent.path),t(parent.t){}

robo::robo(map& model){
        //coordenadas das chaves presentes no mapa
    std::vector<coord> keys=model.get_keys();
    //escolhe uma ordem aleatoria para coletar essas chaves
    std::random_shuffle(keys.begin(),keys.end());
	gene=keys;
	gene.push_back(coord(0,0));
	t.gen_random();
}

void robo::simulate(map& m){
    unsigned int i;
	//a path is comprised of many legs (in portuguese, at least, an when talking about flights)
    std::vector<coord> leg; 
    coord start(0,0);
    path.push_back(start);
    leg=m.BFS(start,gene[0]);
    path.insert(path.end(),leg.begin(),leg.end()); //concatenates the leg after the path
    m.unlock(gene[0]);
    for(i=1;i<gene.size();i++){
        leg=m.BFS(gene[i-1],gene[i]);
        path.insert(path.end(),leg.begin(),leg.end());
        m.unlock(gene[i]);
    }
//    leg=m.BFS(gene[i-1],start);
//    path.insert(path.end(),leg.begin(),leg.end());
}

int robo::avalia(map& m){
    if(path.size()==0) simulate(m);
    return -path.size();
}

void robo::debug(){
    for(unsigned int i=0;i<gene.size();i++){
        gene[i].debug('\t');
    }
    printf("\n");
}

int robo::decision(int obs){
	return t[obs];
}

void robo::random(){
        //coordenadas das chaves presentes no mapa
    //escolhe uma ordem aleatoria para coletar essas chaves
    std::random_shuffle(gene.begin(),gene.end());
}

robo robo::transa(robo& r, int legacy){ //legacy esta aqui para "legacy support", ele nao faz nada nesse caso
    robo filho(r);
    filho.t = r.t.cross_over(t);
//    filho.mutacao();
    return filho;
}

void robo::mutacao(){
    //sem cross-over, eh necessario atualizar o grafo, pois todas as chaves ja foram usadas
    int i,j;
    coord temp;
    //chooses 2 random coordinates to change
    i=rand()%gene.size();
    //-1 and possibly adding 1 to j assures that coordinates mus be different
    j=rand()%(gene.size()-1);
    j+=(j>=i);

    //swaps the 2 positions
    temp=gene[i];
    gene[i]=gene[j];
    gene[j]=temp;

    //clears the path, so a new one can be generated
    path.clear();
}

void robo::animate(map& m, std::vector<coord>& mino){
    unsigned int k=0,tmp;
    coord end(0,0);
    for(unsigned int i=0;i<path.size();i++){
        //system("clear");
	path[i].debug();
	mino[i].debug('\n');
		m.animate(path[i],mino[i]);
        if(k<gene.size() && path[i] == gene[k]){
		m.unlock(gene[k]);
            	k++;
		i--;
        }
	printf("Total steps: %d\n",path.size());
        printf("current steps:%d, keys acquired: %d\n",i,k);
	tmp = m.look_around(path[i]);
	tmp &= 0x00ff;
	tmp |= path[i].relative_dir(mino[i]) << MINOTAUR;
	tmp |= path[i].relative_dir(gene[k]) << KEY_DIR;
	printf("(%x,%x,%x,%x,%x,%x): ",tmp & NORTH,(tmp & SOUTH)>>2,(tmp & EAST)>>4,(tmp & WEST)>>6,(tmp>>8)&0x7,(tmp>>11));
	if(t[tmp]==MOVE_UP) printf("move up\n");
	else if(t[tmp] == MOVE_DOWN) printf("move down\n");
	else if(t[tmp] == MOVE_LEFT) printf("move left\n");
	else if(t[tmp] == MOVE_RIGHT) printf("move right\n");
	else printf("something wrong aint right\n");
	gene[k].debug('\n');
        wait(100);
    }
}

void robo::save(std::ofstream& f){
	f << gene.size();
	f << '\n';
	for(unsigned int i=0;i<gene.size();i++){
		gene[i].save(f);
	}
	f << path.size();
	f << '\n';
	for(unsigned int i=0;i<path.size();i++){
		path[i].save(f);
	}
	t.save(f);
}

void robo::read(std::ifstream& f){
	int size;
	coord tmp;
	f >> size;
	gene.clear();
	for(int i=0;i<size;i++){
		tmp.read(f);
		gene.push_back(tmp);
	}
	f >> size;
	for(int i=0; i<size;i++){
		tmp.read(f);
		path.push_back(tmp);
	}
	t.read(f);
}
