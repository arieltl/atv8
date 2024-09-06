#include <algorithm>
#include <chrono> // Para medir o tempo de execução
#include <fstream> // Para manipulação de arquivos
#include <iostream>
#include <random> // Para gerar números aleatórios
#include <vector>

using namespace std;
using namespace std::chrono;

// Estrutura para armazenar o valor e o peso
struct Item {
    int peso;
    int valor;
};
pair<int, int> calculaMochila(vector<Item>& itens, vector<bool>& bitset)
{
    int peso = 0;
    int valor = 0;

    for (int i = 0; i < itens.size(); i++) {
        if (bitset[i]) {
            peso += itens[i].peso;
            valor += itens[i].valor;
        }
    }

    return { peso, valor };
}
void printBitset(const vector<bool>& bitset)
{
    for (const auto& bit : bitset) {
        cout << bit << " ";
    }
    cout << endl;
}

void generateRandomBitset(vector<bool> &bitset,size_t n)
{
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<int> dist(0, 100);
    bitset.clear();
    // 90% de chance de ser 0 para evitar
    // que a mochila fique muito cheia
    // e demore muito para encontrar uma solução
    // inicial aceitável
    for (size_t i = 0; i < n; i++) {
        bitset.push_back(dist(g)>90);
    }

}


// Função para preencher a mochila em ordem aleatória (heurística)
pair<int, int> preencherMochilaAleatoria(int W, vector<Item>& itens)
{
    const size_t n = itens.size(); // Example size

    // Create a bitset of size n, all bits initialized to 0
    vector<bool> bitset;
    cout << "gerando bitset" << endl;
    generateRandomBitset(bitset, n);
    cout << "bitset gerado" << endl;
    const auto result = calculaMochila(itens, bitset);

    int maiorPeso = result.first;
    int maiorValor = result.second;
    while (maiorPeso > W) {
        cout << "regerando" << endl;
        generateRandomBitset(bitset, n);
        const auto result = calculaMochila(itens, bitset);
        maiorPeso = result.first;
        maiorValor = result.second;
    }
    int index = -1;
    printBitset(bitset);
    do {
        index = -1;
        for (int i = 0; i < n; i++) {
            
            auto neighbour = bitset;
            neighbour[i] = !neighbour[i];
            const auto result = calculaMochila(itens, neighbour);
            
            int peso = result.first;
            int valor = result.second;
            if (peso <= W && valor > maiorValor) {
                maiorPeso = peso;
                maiorValor = valor;
                index = i;
            }
        }
        if (index != -1) {
            bitset[index] = !bitset[index];
        }
    } while (index!=-1);
    return { maiorPeso, maiorValor };
}

void salvarResultadoEmArquivo(const string& nomeArquivo, int peso, int valor)
{
    ofstream arquivo(nomeArquivo,
        ios::app); // Abre o arquivo no modo de adicionar (append)
    if (arquivo.is_open()) {
        arquivo << peso << " " << valor << "\n"; // Adiciona o peso e valor ao final do arquivo
        arquivo.close();
    } else {
        cerr << "Erro ao abrir o arquivo!" << endl;
    }
}

int main()
{
    int N, W;
    cin >> N >> W;

    vector<Item> itens(N);

    for (int i = 0; i < N; i++) {
        cin >> itens[i].peso >> itens[i].valor;
    }

    // Capturar o tempo de início para o preenchimento aleatório
    auto inicioAleatorio = high_resolution_clock::now();

    // Executa a função de preenchimento aleatório
    auto resAleatorio = preencherMochilaAleatoria(W, itens);

    // Capturar o tempo de fim para o preenchimento aleatório
    auto fimAleatorio = high_resolution_clock::now();

    // Calcular o tempo de execução do preenchimento aleatório em milissegundos
    auto duracaoAleatorio = duration_cast<milliseconds>(fimAleatorio - inicioAleatorio).count();

    // Imprimir os resultados do preenchimento aleatório
    cout << "Preenchimento aleatório:\n";
    cout << "Peso ocupado: " << resAleatorio.first << " Kg, Valor alcançado: " << resAleatorio.second << " dinheiros\n";
    cout << "Tempo de execução: " << duracaoAleatorio << " ms\n";

    // Salvar o resultado no arquivo "resultados.txt"
    salvarResultadoEmArquivo("resultadosHill.txt", resAleatorio.first, resAleatorio.second);

    return 0;
}
