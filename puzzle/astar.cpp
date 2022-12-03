#include <bits/stdc++.h>
#define fore(i,a,b) for(int i = a, T = b; i < T; ++i)
#define sz(s) int(s.size())
#define f first
#define s second
#define pb push_back
#define all(s) s.begin(),s.end()
#define ENDL '\n'

using namespace std;

typedef vector<vector<int>> state;
typedef pair<int,int> position;
typedef vector<pair<position,string>> neighbors;

class Node {
	public:
		state data;
		string dir;
		int level;
		int fvalue;

		// Inicializar nodo con la data, nivel (profundidad) del nodo, el fvalue y la direccion del movimiento
		Node(state data, int level, int fvalue, string dir = ""){
			this->data = data;
			this->level   = level;
			this->fvalue  = fvalue;
			this->dir = dir;
		}

		// Expandir el nodo en las 4 direcciones posibles (up,down,left,right)
		vector<Node> expand_children(){
			position blank = this->find_empty();
			int x = blank.f, y = blank.s;

			// Nodos adyacentes (vecinos)
			neighbors ngbrs = {
				{{x, y-1}, "left"}, {{x, y+1},"right"}, {{x-1, y},"up"}, {{x+1, y},"down"}
			};

			vector<Node> children;

			// Recorrer los nodos adyacentes
			for(const auto& nb: ngbrs){
				state child = shuffle(this->data, x, y, nb.f.f, nb.f.s);
				state nt = {{-1,-1,-1}};

				// Si se pudo desplazar el -1
				if (child != nt){
					// Expandir nodo hijo a un nivel mas de profundidad
					Node child_node(child, this->level + 1, 0, nb.s);
					cout << child_node << ENDL;
					children.pb(child_node);
				}
			}

			return children;
		}

		state shuffle(state puzzle, int x1, int y1, int x2, int y2){
			state temp_puzzle;

			// Mover la pieza en blanco en la posicion especificada si no se sale de los limites
			if (x2 >= 0 && x2 < sz(this->data) && y2 >= 0 && y2 < sz(this->data)){
				// Puzzle temporal
				temp_puzzle = puzzle;

				// Desplazar la posicion del espacio en blanco
				int temp = temp_puzzle[x2][y2];
				temp_puzzle[x2][y2] = temp_puzzle[x1][y1];
            	temp_puzzle[x1][y1] = temp;

				return temp_puzzle;
			}
			else temp_puzzle = {{-1,-1,-1}};
		}

		// Buscar la posición vacia (-1)
		position find_empty(){
			fore(i,0,sz(data)){
				fore(j,0,sz(data)){
					if (data[i][j] == -1) return make_pair(i, j);
				}
			}
		}

		// Imprimir el nodo (data del state)
		friend ostream& operator << (ostream& out, const Node& n){
			out << "-> " << n.dir << ENDL;
			fore(i,0,sz(n.data)){
				fore(j,0,sz(n.data)){
					if (n.data[i][j] != -1) out << n.data[i][j] << " ";
					else out << "  ";
				}
				out << ENDL;
			}
			return out;
		}
};


class Puzzle {
	public:
		state start, end;

		Puzzle(state start, state end){
			this->start = start;
			this->end   = end;
		}

		// Lista abierta
		vector<Node> open;

		// Lista cerrada
		vector<Node> closed;

		// Pasos para llegar a la meta
		vector<Node> steps;

		// Funcion que inicia el algoritmo
		void astar(){
			int movs(0);
			string out = "";
			cout << "Expandiendo con A*" << ENDL;
			cout << "Nodos expandidos: " << ENDL;
			
			// Nodo inicial
			Node start_node(start, 0, 0, "initial");
			// Nodo meta 
			Node goal_node(end, 0, 0);

			// Actualizar f(x) del nodo inicial
			start_node.fvalue = this->f(start_node, goal_node);

			// Meter al nodo inicial en la lista abierta
			open.pb(start_node);

			for(;;){
				// Aumentar los nodos expandidos
				++movs;

				// Actualizar el nodo actual
				Node curr = open[0];

				// Mostrar el nodo actual
				steps.pb(curr);

				// si h(x), h(curr,goal) == 0, entonces llego a la meta
				if (h(curr.data, goal_node.data) == 0) break;

				for(auto child_node: curr.expand_children()){
					// Actualizar f(x) para cada nodo[i]
					child_node.fvalue = this->f(child_node, goal_node);

					// Agregarlo a la lista abierta
					open.pb(child_node);
				}

				// Agregar el actual a la cerrada
				closed.pb(curr);

				// Eliminar el primer elemento de la lista abierta
				open.erase(open.begin());

				// Ordenar la lista abierta en base a f(x) = fvalue
				sort(all(open), [&](Node& a, Node& b){ return a.fvalue < b.fvalue; });
			}

			cout << ENDL << "Movimientos: " << movs - 1 << ENDL;
			for(const auto& node: steps){
				cout << node << ENDL;
			}
		}

		// Función heurística para calcular f(x) = h(x) + g(x)
		// h(x) = curr[i][j] <> goal[i][j], : curr[i][j] <> -1
		int f(Node curr, Node goal){
			return this->h(curr.data, goal.data) + curr.level;
		}

		// Calculo de la distancia entre el puzzle actual y el puzzle meta
		// h(x)
		int h(state curr, state goal){
			int temp = 0;
			// sz(start) = sz(end)
			fore(i,0,sz(start)){
				fore(j,0,sz(start)){
					// h(x) = numero de [i][j] que no coinciden entre curr y goal, curr[i][j]<> -1 (_blank)
					if (curr[i][j] != goal[i][j] && curr[i][j] != -1) ++temp;
				}
			}
			return temp;
		}
};

int main(){

	//? Caso estado inicial aleatorio:
	// srand(time(0));

	// map<int,bool> generated;
	// state start(3, vector<int>(3,-1));
	// pair<int,int> pos_blank;

	// state target = {
	// 				{1,4,3},
	// 				{7,6,2},
	// 				{5,8,-1}
	// 			   };

	// fore(i,0,3){
	// 	fore(j,0,3){
	// 		int n = (rand() % 9) + 1;
	// 		while (generated[n]) n = (rand() % 9) + 1;	
	// 		generated[n] = true;	

	// 		// Ubicar el 9 para poner el blank
	// 		if (n == 9) pos_blank = make_pair(i,j);
	// 		start[i][j] = n;
	// 	}
	// }
	// start[pos_blank.f][pos_blank.s] = -1;

    //? Caso Base:
	state start = {
					{1,4,3},
	                {7,-1,6},
	 				{5,8,2}
				  };
	state target = {
					{1,4,3},
					{7,6,2},
					{5,8,-1}
				   };
	
	//? Caso 2:
	// state start = {
	// 				{1,2,3},
	// 				{8,-1,4},
	// 				{7,6,5}
	// 			  };
	// state target = {
	// 				{1,2,3},
	// 				{8,4,5},
	// 				{7,-1,6}
	// 			   };

	Puzzle puzzle(start, target);
	puzzle.astar();


    return 0;
}