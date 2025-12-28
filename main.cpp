#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>

using namespace std;

// ---------------- STRUCT DA QUESTÃO ----------------
struct Questao {
    string pergunta;
    string opA, opB, opC, opD;
    char correta;
};

// ---------------- STRUCT DO RANKING ----------------
struct Jogador {
    string nome;
    int pontos;
};

// ---------------- LEITURA DAS PERGUNTAS ----------------
bool carregarPerguntas(vector<Questao>& perguntas) {
    ifstream arq("perguntas.txt");

    if (!arq.is_open()) {
        cout << "Erro: Arquivo 'perguntas.txt' nao encontrado ou invalido." << endl;
        return false;
    }

    while (true) {
        Questao q;
        string linha;

        if (!getline(arq, q.pergunta)) break;
        if (q.pergunta == "") break;

        getline(arq, q.opA);
        getline(arq, q.opB);
        getline(arq, q.opC);
        getline(arq, q.opD);
        getline(arq, linha);

        if (q.pergunta == "" || q.opA == "" || q.opB == "" || q.opC == "" || q.opD == "" || linha == "") {
            cout << "Erro: Formato invalido no arquivo perguntas.txt" << endl;
            return false;
        }

        q.correta = toupper(linha[0]);
        perguntas.push_back(q);

        getline(arq, linha);
    }

    if (perguntas.empty()) {
        cout << "Erro: Nenhuma pergunta valida encontrada." << endl;
        return false;
    }

    return true;
}

// ---------------- CARREGAR RANKING ----------------
void carregarRanking(vector<Jogador>& ranking) {
    ifstream arq("ranking.txt");

    if (!arq.is_open()) return;

    Jogador j;
    while (arq >> j.nome >> j.pontos)
        ranking.push_back(j);
}

// ---------------- SALVAR RANKING ----------------
void salvarRanking(vector<Jogador>& ranking) {
    ofstream arq("ranking.txt");

    for (auto& j : ranking)
        arq << j.nome << " " << j.pontos << endl;
}

// ---------------- ATUALIZAR RANKING ----------------
void atualizarRanking(vector<Jogador>& ranking, string nome, int pontos) {
    bool existe = false;

    for (auto& j : ranking) {
        if (j.nome == nome) {
            existe = true;
            if (pontos > j.pontos)
                j.pontos = pontos;
            break;
        }
    }

    if (!existe) {
        ranking.push_back({nome, pontos});
    }

    sort(ranking.begin(), ranking.end(), [](Jogador a, Jogador b) {
        return a.pontos > b.pontos;
    });
}

// ---------------- PROGRAMA PRINCIPAL ----------------
int main() {
    vector<Questao> perguntas;

    if (!carregarPerguntas(perguntas)) {
        return 0;
    }

    int pontos = 0;
    char resposta;

    for (auto& q : perguntas) {
        cout << q.pergunta << endl;
        cout << q.opA << endl;
        cout << q.opB << endl;
        cout << q.opC << endl;
        cout << q.opD << endl;

        cout << "Digite sua resposta (A-D): ";
        
        while (true) {
            cin >> resposta;
            resposta = toupper(resposta);

            if (resposta == 'A' || resposta == 'B' || resposta == 'C' || resposta == 'D')
                break;

            cout << "Opcao invalida. Digite novamente (A-D): ";
        }

        if (resposta == q.correta)
            pontos++;
        
        cout << endl;
    }

    cout << "Quiz finalizado!" << endl;
    cout << "Voce acertou " << pontos << " de " << perguntas.size() << " perguntas." << endl;

    string nome;
    cout << "Digite seu primeiro nome: ";
    cin >> nome;

    vector<Jogador> ranking;
    carregarRanking(ranking);
    atualizarRanking(ranking, nome, pontos);
    salvarRanking(ranking);

    cout << "\n--- Ranking ---" << endl;
    for (auto& j : ranking) {
        cout << j.nome << " " << j.pontos << endl;
    }

    return 0;
}
