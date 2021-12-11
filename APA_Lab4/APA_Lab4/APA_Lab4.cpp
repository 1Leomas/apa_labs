#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono> 
#include <windows.h>

using namespace std;

using ull = unsigned long long int;

//culorile in consola
int color_strings = 6;
int color_cost = 2;
int color_idex = 3;
int color_select_menu = 7;
int color_cin = 9;
int color_alb = 15;

int minim_pentru_afisare = 1; //nr minim noduri pentru afisarea matricei si grafului

int select_nr_noduri;
int select_graf_type; //pentru afisare tipului
int select_algoritm;

void setcolor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

class Matrix {
public:
    int rows, columns;
    int* matrix;

    Matrix()
    {
        rows = 0;
        columns = 0;
        matrix = nullptr;
    }

    Matrix(int rows, int columns) : rows(rows), columns(columns) {
        matrix = new int[columns * rows]{ 0 };
    }

    ~Matrix() {
        delete[]matrix;
    }

    void initializare(int rows, int columns)
    {
        this->rows = rows;
        this->columns = columns;
        matrix = new int[columns * rows]{ 0 };
    }

    int getElement(int row, int colum) {

        return matrix[colum + columns * row];
    }

    void setElement(int r, int c, int val) {
        matrix[c + columns * r] = val;
    }

    void print_matrix()
    {
        cout << setw(4) << " ";
        for (int i = 0; i < rows; i++)
        {
            setcolor(color_idex);
            cout << setw(3) << i << " ";
            setcolor(15);
        }
        cout << endl;

        for (int i = 0; i < rows; i++)
        {
            setcolor(color_idex);
            cout << setw(3) << i << " ";
            setcolor(15);
            for (int j = 0; j < columns; j++)
            {
                if (getElement(i, j) != 0 && getElement(i, j) != INT_MAX)
                    setcolor(color_cost);
                cout << setw(3);
                if (getElement(i, j) == INT_MAX)
                    cout << "INF";
                else
                    cout << getElement(i, j);
                cout << " ";
                setcolor(15);
            }
            cout << endl;
        }
        cout << endl;
    }
};

class Muchie_struct {
public:
    int first, second, cost;
    void set_data(int first, int second, int cost)
    {
        this->first = first;
        this->second = second;
        this->cost = cost;
    }
    void print_data()
    {
        cout << endl << first << " " << second << " " << cost << endl;
    }
};

class Graph {
    int MAX_MUCHII;
    int cost_minim{ 0 };
public:
    int NODURI, MUCHII;
    Muchie_struct* arr_muchii;
    Matrix matrice;

    Graph() { }
    Graph(int noduri) : matrice()
    {
        this->NODURI = noduri;
    }

    void insert_NODURI(int noduri)
    {
        this->NODURI = noduri;
    }

    int get_MAX_MUCHII()
    {
        MAX_MUCHII = NODURI * (NODURI - 1) / 2;
        return MAX_MUCHII;
    }

    void creare_matrice()
    {
        if (matrice.matrix == nullptr)
        {
            matrice.initializare(NODURI, NODURI);
        }
        for (int i = 0; i < MUCHII; i++)
        {
            matrice.setElement(arr_muchii[i].first, arr_muchii[i].second, arr_muchii[i].cost);
            matrice.setElement(arr_muchii[i].second, arr_muchii[i].first, arr_muchii[i].cost);
        }

    }

    void afisare_lista()
    {
        cout << endl;
        cout << "Graful are " << NODURI << " noduri si " << MUCHII << " muchii" << endl;
        for (int i = 0, m = 0; i < MUCHII; i++)
        {
            /*while (arr_muchii[m].first == index_muchie)
            {
                cout << "(" << arr_muchii[m].first << ", " << arr_muchii[m].second << ", " << arr_muchii[m].cost << ") ";
                m++;
            }*/
            cout << "(" << arr_muchii[i].first << ", " << arr_muchii[i].second << ", " << arr_muchii[i].cost << ") ";
            cout << endl;
        }
    }
    void afisare_matrice()
    {
        if (matrice.matrix == nullptr) creare_matrice();
        setcolor(color_strings);
        cout << "Graful are " << NODURI << " noduri si " << MUCHII << " muchii" << endl;
        setcolor(15);
        matrice.print_matrix();
    }
};

void save_to_file(Graph& graph, string file_name)
{
    ofstream MyFile(file_name);

    for (int i = 0; i < graph.NODURI; i++)
    {
        for (int j = 0; j < graph.NODURI; j++)
        {
            MyFile << graph.matrice.getElement(i, j);

            if (j < (graph.NODURI - 1)) MyFile << ",";
        }
        MyFile << endl;
    }
    MyFile.close();
}

int minDistance(int costul[], bool vizitat[], int V)
{
    int min = INT_MAX, min_index = 0;

    for (int v = 0; v < V; v++)
    {
        if (vizitat[v] == false && costul[v] < min)
        {
            min = costul[v], min_index = v;
        }
    }
    return min_index;
}

void dijkstra(Graph& graph)
{
    int V = graph.NODURI;
    int* cost = new int[V];
    bool* vizitat = new bool[V];

    if (V <= minim_pentru_afisare)
    {
        cout << setw(4) << " ";
        for (int i = 0; i < V; i++)
        {
            setcolor(color_idex);
            cout << setw(3) << i << " ";
            setcolor(15);
        }
        cout << endl;
    }

    for (int k = 0; k < V; k++)
    {
        for (int i = 0; i < V; i++)
        {
            cost[i] = INT_MAX;
            vizitat[i] = false;
        }

        cost[k] = 0;

        for (int count = 0; count < V - 1; count++)
        {
            //selectam varful minim nevizitat
            int u = minDistance(cost, vizitat, V);
            //marcam ca vizitat
            vizitat[u] = true;

            for (int v = 0; v < V; v++)
            {
                if (!vizitat[v] &&
                    cost[u] != INT_MAX &&
                    graph.matrice.getElement(u, v) &&
                    cost[u] + graph.matrice.getElement(u, v) < cost[v])
                {
                    cost[v] = cost[u] + graph.matrice.getElement(u, v);
                }
            }

        }

        if (V <= minim_pentru_afisare)
        {
            setcolor(color_idex);
            cout << setw(3) << k << " ";
            setcolor(15);

            for (int i = 0; i < V; i++)
            {
                if (cost[i])
                {
                    setcolor(color_cost);
                }
                cout << setw(3);
                cout << cost[i] << " ";
                setcolor(color_alb);
            }
            cout << endl;
        }

    }

    delete[]cost;
    delete[]vizitat;
}

void floyd(Graph& graph)
{
    int V = graph.NODURI;

    if (V < minim_pentru_afisare)
    {
        setcolor(color_strings);
        cout << "Matricea initiala:" << endl;
        setcolor(color_alb);
        graph.matrice.print_matrix();
    }

    for (int k = 0; k < V; k++)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = i+1; j < V; j++)
            {
                //if (i == j) continue;

                if (graph.matrice.getElement(i, j) > (graph.matrice.getElement(i, k) + graph.matrice.getElement(k, j)) && (graph.matrice.getElement(k, j) != INT_MAX && graph.matrice.getElement(i, k) != INT_MAX))
                {
                    graph.matrice.setElement(i, j, graph.matrice.getElement(i, k) + graph.matrice.getElement(k, j));
                    graph.matrice.setElement(j, i, graph.matrice.getElement(i, k) + graph.matrice.getElement(k, j));
                }
            }
        }
    }

    if (V < minim_pentru_afisare)
    {
        setcolor(color_strings);
        cout << "Distanta minima pentru orice 2 noduri:" << endl;
        setcolor(color_alb);
        graph.matrice.print_matrix();
    }
}

void open_graph()
{
    system("python ./draw_one_graph.py");
}

void select_numar_de_noduri(Graph& graph)
{
    int select = select_nr_noduri;
    while (select < 1 || select > 8)
    {
        cout << endl;
        setcolor(color_strings);
        cout << "Selectati numarul de noduri pentru graf:" << endl;
        setcolor(color_select_menu);
        cout << "1. 5 noduri" << endl;
        cout << "2. 10 noduri" << endl;
        cout << "3. 50 noduri" << endl;
        cout << "4. 100 noduri" << endl;
        cout << "5. 250 noduri" << endl;
        cout << "6. 500 noduri" << endl;
        cout << "7. 1000 noduri" << endl;
        cout << "8. custom size" << endl;
        setcolor(color_cin);
        cout << "Select: "; cin >> select;
        setcolor(color_alb);
    }

    switch (select)
    {
    case 1: graph.insert_NODURI(5); break;
    case 2: graph.insert_NODURI(10); break;
    case 3: graph.insert_NODURI(50); break;
    case 4: graph.insert_NODURI(100); break;
    case 5: graph.insert_NODURI(250); break;
    case 6: graph.insert_NODURI(500); break;
    case 7: graph.insert_NODURI(1000); break;
    case 8:
        setcolor(color_cin);
        cout << "Enter nr noduri: "; cin >> graph.NODURI; break;
        setcolor(color_alb);
    default: break;
    }

    graph.arr_muchii = new Muchie_struct[graph.get_MAX_MUCHII()];
    graph.matrice.initializare(graph.NODURI, graph.NODURI);
}

void complete_graph(Graph& graph)
{
    int select = select_graf_type;
    while (select < 1 || select > 5)
    {
        cout << endl;
        setcolor(color_strings);
        cout << "Select tipul grafului:" << endl;
        setcolor(color_select_menu);
        cout << "1. Rar" << endl;
        cout << "2. Rar++" << endl;
        cout << "3. Mediu" << endl;
        cout << "4. Dens" << endl;
        cout << "5. Test" << endl;
        setcolor(color_cin);
        cout << "Select: ";
        cin >> select;
        setcolor(color_alb);
        cout << endl;
    }
    switch (select)
    {
    case 1:
        for (int i = 0; i < graph.NODURI - 1; i++)
        {
            int cost = rand() % graph.NODURI + 1;
            graph.arr_muchii[i].set_data(i, i + 1, cost);
            graph.MUCHII++;
        }
        break;
    case 2:
        for (int i = 0, m = 0, j = graph.NODURI - 1; i < graph.NODURI - 1; i++, j--)
        {
            int cost = rand() % graph.NODURI + 1 * 2;

            graph.arr_muchii[m].set_data(i, i + 1, cost);

            if (j > i)
            {
                cost = rand() % graph.NODURI + 1 * 2;
                graph.arr_muchii[m + 1].set_data(i, j, cost);
                m++;
                graph.MUCHII++;
            }
            graph.MUCHII++;
            m++;
        }
        break;
    case 3:
        int densitate;

        for (int i = 0, m = 0; i < graph.NODURI; i++)
        {
            for (int j = 0; j < graph.NODURI; j++)
            {
                if (j <= i) continue;
                densitate = rand() % 2 + 1;

                if (densitate != 1) continue;

                int cost = rand() % graph.NODURI + 1;

                graph.arr_muchii[m].set_data(i, j, cost);

                graph.MUCHII++;
                m++;
            }
        }
        break;
    case 4:
        for (int i = 0, m = 0; i < graph.NODURI; i++)
        {
            for (int j = 0; j < graph.NODURI; j++)
            {
                if (j <= i) continue;

                int cost = rand() % 10 + 1;
                graph.arr_muchii[m].set_data(i, j, cost);
                graph.MUCHII++;
                m++;
            }
        }
        break;
    case 5:
        for (int i = 0, m = 0; i < graph.NODURI; i++)
        {
            for (int j = i + 1; j < graph.NODURI; j++)
            {
                if (i != j && i % 2 == 0)
                {
                    graph.arr_muchii[m].set_data(i, j, rand() % graph.NODURI);
                    //matrix[i][j] = matrix[j][i] = rand() % graph.NODURI;
                    graph.MUCHII++;
                    m++;
                }
            }
        }
        break;
    default: break;
    }

    graph.creare_matrice();
}

void algorithm_time_execution(void (*algorithm_func)(Graph&), Graph& graph)
{
    std::chrono::steady_clock::time_point start, end;

    start = std::chrono::steady_clock::now();
    algorithm_func(graph);
    end = std::chrono::steady_clock::now();

    setcolor(color_strings);
    cout << endl;
    cout << "Timpul de executie pentru un graf ";
    switch (select_graf_type)
    {
    case 1: cout << "rar"; break;
    case 2: cout << "rar++"; break;
    case 3: cout << "mediu"; break;
    case 4: cout << "dens"; break;
    default: break;
    }
    cout << " cu " << graph.NODURI << " noduri si " << graph.MUCHII << " muchii:" << endl;

    setcolor(color_cost);
    if (std::chrono::duration_cast<std::chrono::seconds> (end - start).count())
        cout << std::chrono::duration_cast<std::chrono::seconds> (end - start).count() << " seconds ";
    if (std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count())
        cout << std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count() << " milliseconds ";
    if (std::chrono::duration_cast<std::chrono::microseconds> (end - start).count())
        cout << std::chrono::duration_cast<std::chrono::microseconds> (end - start).count() << " microseconds ";
    if (std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count())
        cout << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() << " nanoseconds";
    cout << endl << endl;;
    setcolor(color_alb);
}

void select_algorithm(Graph& graph)
{
    //acest graf trebuie pentru alg floyd
    Graph graf_INT_MAX(graph.NODURI);

    graf_INT_MAX.matrice.initializare(graph.NODURI, graph.NODURI);

    for (int i = 0; i < graph.NODURI; i++)
    {
        for (int j = 0; j < graph.NODURI; j++)
        {
            if (i == j) continue;
            if (graph.matrice.getElement(i, j))
                graf_INT_MAX.matrice.setElement(i, j, graph.matrice.getElement(i, j));
            else
                graf_INT_MAX.matrice.setElement(i, j, INT_MAX);
        }
    }

    int select = select_algoritm;
    while (select < 1 || select > 3)
    {
        cout << endl;
        setcolor(color_strings);
        cout << "Algoritmul pentru aflarea drumului minim dintr-un anumit punct:" << endl;
        setcolor(color_select_menu);
        cout << "1. Dijkstra" << endl;
        cout << "2. Floyd" << endl;
        cout << "3. Ambele" << endl;

        setcolor(color_cin);
        cout << "Select: "; cin >> select;
        setcolor(15);
        cout << endl;
    }
    switch (select)
    {
    case 1:
        setcolor(12);
        cout << "Algoritmul Dijkstra:" << endl << endl;
        setcolor(15);
        algorithm_time_execution(&dijkstra, graph);
        break;
    case 2:
    {

        setcolor(12);
        cout << "Algoritmul Floyd:" << endl << endl;
        setcolor(15);
        algorithm_time_execution(&floyd, graf_INT_MAX);
        break;
    }

    case 3:
        setcolor(12);
        cout << "Algoritmul Dijkstra:" << endl << endl;
        setcolor(15);
        algorithm_time_execution(&dijkstra, graph);
        setcolor(12);
        cout << "Algoritmul Floyd: " << endl << endl;
        setcolor(15);
        algorithm_time_execution(&floyd, graf_INT_MAX);
    default: break;
    }
}

void savefile_time_execution(ofstream& MyFile)
{
    std::chrono::steady_clock::time_point start, end;
    Graph graph;
    select_numar_de_noduri(graph);
    complete_graph(graph);

    Graph graf_INT_MAX(graph.NODURI);
    graf_INT_MAX.matrice.initializare(graph.NODURI, graph.NODURI);
    for (int i = 0; i < graph.NODURI; i++)
    {
        for (int j = 0; j < graph.NODURI; j++)
        {
            if (i == j) continue;
            if (graph.matrice.getElement(i, j))
                graf_INT_MAX.matrice.setElement(i, j, graph.matrice.getElement(i, j));
            else
                graf_INT_MAX.matrice.setElement(i, j, INT_MAX);
        }
    }

    start = std::chrono::steady_clock::now();
    dijkstra(graph);
    end = std::chrono::steady_clock::now();

    MyFile << std::chrono::duration_cast<std::chrono::microseconds> (end - start).count();
    MyFile << '\t';

    start = std::chrono::steady_clock::now();
    floyd(graf_INT_MAX);
    end = std::chrono::steady_clock::now();
    MyFile << std::chrono::duration_cast<std::chrono::microseconds> (end - start).count();
    MyFile << endl;
}

void raport_execution() //executa metodele de mai multe si exporteaza datele intr-un fisier
{
    //                      1  2  3   4   5   6    7
    select_nr_noduri = 7; //5 10 50 100 250 500 1000
    select_graf_type = 1; //rar rar++ mediu dens
    minim_pentru_afisare = 1;

    ofstream MyFile("time_execution2.txt");
    switch (select_nr_noduri)
    {
    case 1: MyFile << "5"; break;
    case 2: MyFile << "10"; break;
    case 3: MyFile << "50"; break;
    case 4: MyFile << "100"; break;
    case 5: MyFile << "250"; break;
    case 6: MyFile << "500"; break;
    default: break;
    }
    MyFile << " n" << '\t';
    switch (select_graf_type)
    {
    case 1: MyFile << "rar"; break;
    case 2: MyFile << "rar++"; break;
    case 3: MyFile << "mediu"; break;
    case 4: MyFile << "dens"; break;
    default: break;
    }
    MyFile << endl << endl;
    MyFile << "=AVERAGE(A5:A1004)" << '\t' << "=AVERAGE(B5:B1004)" << endl;
    MyFile << "Dijkstra" << '\t' << "Floyd" << endl;

    for (int i = 0; i < 250; i++)
    {
        cout << i << endl;
        savefile_time_execution(MyFile);
    }
    MyFile.close();
}

void standart_execution()
{
    //1  2  3   4   5    6      7
    select_nr_noduri = 0; //5 10 50 100 500 1000 custom
    select_graf_type = 0; //rar rar++ mediu dens test
    select_algoritm = 0;  //krusal prim ambele
    minim_pentru_afisare = 20;

    Graph graph;

    select_numar_de_noduri(graph);
    complete_graph(graph);

    if (graph.NODURI <= minim_pentru_afisare)
    {
        graph.afisare_matrice();
        save_to_file(graph, "graph.csv");
    }

    select_algorithm(graph);

    if (graph.NODURI <= minim_pentru_afisare)
        open_graph();
}

int main()
{
    srand(time(NULL));
    
    standart_execution();
    //raport_execution();

    system("pause");
    return 0;
}