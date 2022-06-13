/*
    * @author Joao Francisco Gomes Targino - 180102991
    * @author Joao Gabriel Ferreira Saraiva - 180103016
    Compilado no Ubuntu 20.04 c++ versao 9.3.0
    codigo de compilacao g++ Trabalho.cpp -o trabalho
*/

#include <bits/stdc++.h>
 
#define vi vector <int>
#define vl vector <long long>
#define ll long long
#define pb push_back
#define mp make_pair
#define ii pair <int,int>
#define endl "\n"
#define qu queue <int>
#define qmax priority_queue <int>
#define qmin priority_queue <int, vector<int>,greater<int>>
using namespace std;

/*
    # Numero maximo de chave
*/
#define MaximoChave 25;

string chave, frase, decifrada, cifrada;
vector <string> blocostring;
vi blocoint;
vi dist;
pair< vector<string>, vector<int> > ocorrencias;
vector<pair<char, int>> guardaponto;
int tamanhofrase = 0;

/*
    # Frequencia de cada letra de 'a' ao 'z' em portugues
*/
vector <double> freq_port = {0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,
                      0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,
                      0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,
                      0.00978, 0.02360, 0.00150, 0.01974, 0.00074};
/*
    # Frequencia de cada letra de 'a' ao 'z' em ingles
*/
vector <double> freq_english = {0.14630, 0.01040, 0.03880, 0.04990, 0.12570, 0.01020, 
                                 0.01300, 0.01280, 0.06180, 0.00400, 0.00020, 0.02780, 
                                 0.04740, 0.05050, 0.10730, 0.02520, 0.01200, 0.06530, 
                                 0.07810, 0.04340, 0.04630, 0.01670, 0.00100, 0.02100, 0.00100, 0.04700};

vector <double> freq_op;

void funcao(int);
void cifrador();
void decifrador();
void tamanho_chave(int);
void distancia(string);
int maiormdc();
void ord();
bool maior(ii, ii);
void analise_frequencia(int);
void menu();
void print(int);
void tratartex(int);

/*
    # Funcao centra que organiza o que tem que ser executado no decorrer do programa
    # de acordo com o op, operador que indica o que tem que ser feito
*/
void funcao(int op){
    /*
        # op = 1 e op == 2, necessitao de uma chave e de uma frase
        # op = 1, uma frase e cifrada usando a chave
        # op = 2, uma frase e decifrada usando uma chave
    */
    if(op == 1 || op ==2){
        cout << "Escreva a chave:" << endl;
        cin.ignore();
        getline(cin,chave);
        cout << "Escreva a frase:" << endl;
        getline(cin,frase);
        tamanhofrase = frase.size();
        tratartex(op);
    }
    /*
        # op = 3 e op = 4, necessitao de um texto cifrado, para poder fazer o ataque e em seguida achar a chave para decifrar o texto
        # op = 3, faz o ataque em um texto cifrado em portugues
        # op = 4, faz o ataque em um texto cifrado em ingles 
        # a op = 4, consegue achar a chave e decifrar textos em portugues tambem, porem com uma precisao menor, no caso de nao saber a lingua do texto
    */
    else if(op == 3 || op == 4){
        cout << "Escreva o texto para o ataque de recuperacao de senha:" << endl;
        cin.ignore();
        getline(cin,frase); 
        //guardando o tamanhho da frase para o print com os caracteres especiais no fim da execucao
        tamanhofrase = frase.size();
        /*
            # o texto e tratado a frase, e em seguida, tenta pegar um padrao entre de grupos de letras com 2, 3, 4 ou 5 de tramanho
            # armazena o numero de repeticoes desses grupos ao longo do texto
            # em seguida manda o grupo de letras com maior numero de repeticao para verificar a distancia de cada repeticao deste grupo de letras
        */
        tratartex(op);
        tamanho_chave(2);
        tamanho_chave(3);
        tamanho_chave(4);
        tamanho_chave(5);
        ord();
        distancia(ocorrencias.first[0]);
    }
    
    int auxmdc = maiormdc(); // armazena o maior divisor comum dos intervados de grupos de letras
    if(op == 1){
        /*
            # Usando a frase ja tratada, cifra a frase e em seguida imprime o texto ja cifrado com os caracteres especiais 
        */
        cifrador();   
        frase = cifrada;
        print(op);
        cout << endl << "Frase cifrada: " << cifrada << endl;
    }
    else if(op == 2){
        /*
            # Usando a frase ja tratada, decifra a frase e em seguida imprime o texto ja decifrado com os caracteres especiais 
        */
        decifrador();
        frase = decifrada;
        print(op);
        cout << endl << "Frase decifrada: " << decifrada << endl;
    }
    else if(op == 3){
        /*
            # coloca a frequencia como a frequencia em portugues
            # faz a analise de frequencia e acha a possivel chave, com o possivel tamanho de chave achado
            # em seguida imprime a chave achada e a frase decifrada
        */
        freq_op = freq_port;
        analise_frequencia(auxmdc);
        decifrador();
        frase = decifrada;
        print(2);
        cout << endl << "Chave encontrada no ataque:" << chave << endl << endl;
        cout << "Texto descriptografado: " << decifrada << endl;
    }
    else if(op == 4){
        /*
            # coloca a frequencia como a frequencia em ingles
            # faz a analise de frequencia e acha a possivel chave, com o possivel tamanho de chave achado
            # em seguida imprime a chave achada e a frase decifrada
        */
        freq_op = freq_english;
        analise_frequencia(auxmdc);
        decifrador();
        frase = decifrada;
        print(2);
        cout << endl << "Chave encontrada no ataque:" << chave << endl << endl;
        cout << "Texto descriptografado: " << decifrada << endl;
    }
    /*
        # limpa todas as variaveis globais sempre no final de uma execucao
    */
    decifrada = {};
    cifrada = {};
    frase = {};
    chave = {};
    blocoint = {};
    blocostring = {};
    dist = {};
}
/*
    # A funcao tratartexto faz um tratamento no texto tirando espacoes e caracteres diferentes de letras,
    # a funcao salva esses caracteres e suas posicoes em um vetor, para realocarlos no print
*/
void tratartex(int op){
    /*
        # string auxiliar para armazenar mensagem tratada
        # vetor de par usado para auxiliar no armazenamento dos caracteres especiais 
    */
    string auxs;
    vector< pair<char, int> > auxpair = {};

    if(op == 1 || op == 2 || op == 3 || op == 4){
        /*
            # for usado para separar as letras na string auxs,
            # e as nao letras e suas posicoes no vetor de par auxpair
        */
        for(int i = 0; i < frase.size(); i++){
            if((('a' <= frase[i]) && (frase[i] <= 'z')) || ((frase[i] >= 'A') && (frase[i] <= 'Z'))){
                auxs += frase[i];
            }else{
                auxpair.pb(mp(frase[i],i));
            }
        }
        /*
            # frase recebe o valor de seu axiliar com apenas letras
        */
        frase = auxs;

        /*
            # no caso da pessoa querer cifrar ou decifrar a frase, as letras da chave sao trocadas para minusculo
        */
        if(op == 1 || op == 2){
            for(int i =0; i < chave.size(); i++){
                if(chave[i] >= 'A' && chave[i] <='Z'){
                    chave[i] = chave[i] + 32;
                }
            }
        }
        /*
            # todas as letras da frase sao trocadas para minusculo
        */
        for(int i =0; i < frase.size(); i++){
            if(frase[i] >= 'A' && frase[i] <= 'Z'){
                frase[i] = frase[i] + 32;
            }
        }
        /*
            # so caso de um ataque, as frases recebidas, apos o tratamento sao colocadas como frases cifradas, para o uso em outras funcoes
        */
        if(op == 3 || op == 4){
            cifrada = frase; 
        }
    }
    /*
        # variave global para guardar os caracteres especiais, para serem printados no final
    */
    guardaponto = auxpair;
    
}
/*
    # O for roda ate o tamanho da frase inicial, apos isso, anda pela string tratada e cifrada/decifrada alocando as letras em uma string auxiliar
    # quando chega em uma variavel especial, ela e alocada na string auxiliar, ao final temos a frase decifrada/cifrada, completamente pontuada
    # e com seus espacos nos devidos lugares
*/
void print(int op){
    int aux = 0, aux1 = 0;
    string auxs;
    if(op == 1 || op == 2){
        for(int i = 0; i < tamanhofrase; i++){
            if(i == guardaponto[aux1].second){
                auxs += guardaponto[aux1].first;
                aux1++;
            }else{
                auxs += frase[aux];
                aux++;
            }
        }
        if(op == 1){
            cifrada = auxs;
        }else if(op == 2){
            decifrada = auxs;
        }
    }
}
/*
    # Funcao que faz a analise de frequencia e decifra a frase, recebendo o tamanho da chave(distan)
    # e procurando a frequencia para poder descobrir a chave
*/
void analise_frequencia(int distan){
    string auxchave;
    vector<string> sequencias(distan);
    vector<vector<double>> freqint(distan);
    vector<double> freq;
    /*
        # inicializa e zera o vetor que conta a frequncia 
    */
    for(int i = 0; i < distan; i ++){
        freqint[i].resize(26, 0.0);
    }
    
    /*
        # Separa o texto inicial em n(distan) strings, para poder achar cara letra da chave separadamente 
    */
    for(int i = 0; i < distan; i++){
        for(int j = i; j < cifrada.size(); j+=distan){
            sequencias[i]+=cifrada[j];
        }
    }
    /*
        # For para saber o qual a frequancia geral de cada letra da chave, e alocando essa frequancia em seu vetor
    */
    for(int i = 0; i < distan; i++){
        freq.resize(26, 0.0); 
        /*
            # Anota a frequancia de cada letra no vetor freq
        */
        for(int j = 0; j < sequencias[i].size(); j++){
            freq[sequencias[i][j]-97]++;
        }
        /*
            # Divide a frequancia de cada letra pela total de letras de sua sequancia
        */
        for(int j = 0; j < 26; j++){
            freq[j] = freq[j]/sequencias[i].size();
        }
        /*
            # For que vai passando para a proxima letra do alfabeto "shifitando" o alfabeto
            # auxfreq = 0.0 sempre zera a media de "acerto" do alfabeto
            # auxi = j faz com que o vetor inicie em j
        */
        for(int j = 0; j < 26; j++){
            double auxfreq = 0.0;
            int auxi = j;
            /*
                # Anda pelo vetor e frequencias
                # auxfreq = auxfreq + ((frequancia de letras usadas na lingua) - (frequancia de letra x no texto))
            */
            for(int k = 0; k < 26; k++){
                if(auxi == 26){
                    auxi = 0;
                }
                auxfreq += abs(freq_op[k] - freq[auxi]);
                auxi++;
            }
            /*
                # frewint[letra i da chave][numero j de "shifts" no meu alfabeto] = quao proximo o numero esta da frquancia de letras em ingles
            */
            freqint[i][j] = auxfreq;
        }
        
    }
    /*
        # Para cada letra da chave, procura o numero x de vezes que o alfabeto foi jogado para o lado
        # e assim somando o numero de vez com 97('a') achamos a letra y da chave
    */
    for(int i = 0; i < distan; i++){
        double menor = 100000000000;
        int aux = 0;
        for(int j = 0; j < 26; j++){
            if(menor > freqint[i][j]){
                menor = freqint[i][j];
                aux = j;
            }
        }  
        aux += 97;
        char x = aux;
        auxchave += x;
    }
    /*
        # Mudamos o valor da chave para auxchave(chave que foi achada na funcao)
    */
    chave = auxchave;
}
/*
    # Procura o numero entre 0 e 25 que mais divide os espacos entre blocos de letras
    # Esses espacoes foram calculados na funcao distancias()
*/
int maiormdc(){
    vi maisdiv(25, 0);
    int maiordiv = -1;

    /*
        # Passa pelo vetor com as distancias de cada grupo de letras
        # Divide e verifica de o resto eh 0, e verifica se eh o numero mais recorrente
        # caso seja o valor mais recorrente ele retorna esse valor
    */
    for(int i = 1; i < 26; i++){
        for(int j = 0; j < dist.size(); j++){
            if(dist[j]%i == 0){
                maisdiv[i-1]++;
            }
        }
        if(maisdiv[i-1] > maiordiv){
            maiordiv = i;
        }
    }

    return maiordiv;
}
/*
    # Usado para ordenar caso p1 seja maior que p2, usada na funcao sort() em ord()
*/
bool maior(ii p1, ii p2){
    if(p1.first > p2.first){
        return true;
    }else{
        return false;
    }
}
/*
    # Funcao usada para ordenar o vetor de par, onde tem uma string e o vetor int de ocorrencia de cada uma dessas strings
*/
void ord(){
    vector<string> auxs(blocostring);
    vector<ii> auxi;
    vi aux;
    /*
        # Coloca o vetor blocoint em um vetor de par<int, int> para temos salvo a posicao no numero x de ocorrencias
        # Usado para manter o vetor de agora com a ordem do vetor de string, podendo assim ordenar o vetor e achar sua posicao no vetor e string
    */
    for(int i = 0; i < blocoint.size(); i++){
        auxi.pb(mp(blocoint[i], i));
    }
    /*
        # Ordena o vetor de acordo com o peimeiro elemento do metor de par, chamando a funcao maior() dentro de sort()
    */
    sort(auxi.begin(), auxi.end(), maior);
    /*
        # Ordena o vetor de strings de acordo com a posicao armazenada no vetor auxi anteriormente
    */
    for(int i = 0; i < auxi.size(); i++){
        auxs[i] = blocostring[auxi[i].second];
        aux.pb(auxi[i].first);
    }
    /*
        # Cria um vetor de ocorrencias com um par contendo (vetor com strings ordenadas) e (numero de ocorrencias ordenado)
    */
    ocorrencias = mp(auxs, aux);
}   

/*
    # A funcao tamanho_chave pega o padrao mais recorrente de um grupo de letras, testamos de 2 a 5 letras 
*/
void tamanho_chave(int bloco){
    string msg = cifrada;
    int freq = 0, band = 0;
    string aux;
    /*
        # o for de dentro vai pegando os grupos de tamanho "bloco"
        # enquanto o for de fora anda pela nossa mensagem
    */
    for(int i = 0; i < msg.size(); i++){
        for(int j = i; j < i+bloco; j++){
            aux += msg[j];
        }
        /*
            # caso band seja 1 significa que o vetor blocostring possui coisas 
            # entao eh feito uma busca do bloco aux dentro do vetor blocostring e somado o vetor de ocorrencia bloco int
            # caso seja a primeira ocorrencia de aux, e feito um push back no vetor e adicionado 1 ao vetor de ocorrencia
            caso band seja 0, sao feitos os mesmo push backs escritos acima e band  vira 1
        */
        if(band == 1){
            if(find(blocostring.begin(),blocostring.end(),aux) != blocostring.end()){
                for(int j = 0; j < 10; j++){
                    if(blocostring[j] == aux){
                        blocoint[j]++;
                        break;
                    }
                }
            }else{
                blocostring.push_back(aux);
                blocoint.pb(1);            
            }
        }else{
            blocostring.push_back(aux);
            blocoint.pb(1);  
            band = 1;
        }
        aux.clear();
    }
}

/*
    # A funcao distancia pega a string maior, que e a string que possui a maior ocorrencia das letras consecutivas
    # e conta a distancia entre cada uma das vezes que essa string aparece no texto completo 
*/
void distancia(string maior){
    string msg = cifrada;
    int primeiro = 0, distaux = 0;
    /*
        # for que faz a contagem da distancia 
    */
    for(int i = 0; i < msg.size(); i++){
        /*
            # caso a primeira ocorrencia ainda nao tenha sido descoberta ele somente procura pela mesma 
        */
        if(primeiro == 0 && msg[i] == maior[0]){
            for(int j = 1; j < maior.size(); j++){
                if(msg[i+j] != maior[j]){
                    break;
                }
                if(j == maior.size()-1){
                    primeiro = 1;
                    i += maior.size();
                }    
            }
        }
        /*
            # caso a primeira ocorrencia ja tenha sido descoberta ele comeca a contar a variavel distaux
        */
        else if(primeiro != 0){
            /*
                # caso o caracter da mensagem seja diferente do primeiro caracter da nossa string
                # caso o primeiro caracter da nossa string seja achado eh feito um for para checar se a string toda esta ali
            */
            if(msg[i] != maior[0]){ 
                distaux++;
            }
            else{
                distaux++;
                for(int j = 1; j < maior.size(); j++){
                    if(msg[i+j] != maior[j]){
                        break;
                    }
                    if(j == (maior.size()-1)){
                        primeiro = 1;
                        i += maior.size();
                        dist.push_back(distaux + maior.size());
                        distaux = 0;
                    }    
                }       
            }    
        }
    }
    sort(dist.begin(), dist.end(), greater<int>());
}

/*
    # A funcao cifrador pega a chave e a frase passadas por argumento e atualiza a string cifrada
*/
void cifrador(){
    int aux = 0;
    vi vetoraux;
    /*
        # for que pega a frase que foi passada e filtrada na funcao de tratar texto
        # caso a chave seja uma letra ele criptografa essa letra com o caracter correspondente na chave
        # com a formula (x + n)%26, diminuindo 26 se necessario, sendo x o caracter da frase e n o caracter da chave
    */
    for(int i = 0; i < frase.length(); i++){
        if(frase[i] >= 'a' && frase[i] <= 'z'){
            vetoraux.pb(((frase[i]-97) + (chave[aux] - 97))%26);
            if((frase[i]-97) - (chave[aux] - 97) > 25){
                vetoraux[i]-=26;
            }
            if(aux == chave.length()-1){
                aux = 0;
            }else{
                aux++;
            }
        }
    }
    for(int i = 0; i < vetoraux.size(); i++){
        cifrada += (vetoraux[i]+97);
    }
}

/*
    # A funcao decifrador pega a chave e a cifra passadas por argumento e atualiza a string cifrada
*/
void decifrador(){
    vi vetoraux; 
    int aux = 0;
    /*
        # for que pega a frase que foi passada e filtrada na funcao de tratar texto
        # caso a chave seja uma letra ele descriptografa essa letra com o caracter correspondente na chave
        # com a formula (x - n)%26, somando 26 se necesario, sendo x o caracter da frase e n o caracter da chave
        # caso a chave seja nao seja letra, ele somente adiciona ela no vetor
    */
    for(int i = 0; i < frase.size();i++){
        if(frase[i] >= 97 && frase[i] <= 122){
            vetoraux.pb(((frase[i]-97) - (chave[aux] - 97))%26);
            if((frase[i]-97) - (chave[aux] - 97) < 0){
                vetoraux[i]+=26;
            }
            if(aux == chave.length()-1){
                aux = 0;
            }else{
                aux++;
            }
        }
        else{
            vetoraux.pb(frase[i]);
        }
    }
    /*
        # for que vai colocar o vetor auxiliar que eh nossa frase decifrada e coloca na variavel que será mostrada na tela 
    */
    for(int i = 0; i < vetoraux.size(); i++){
        decifrada += (vetoraux[i]+97);
    }
}
/*
    # Menu interativo com o usuario
*/
void menu(){
    int op = 0;

    while(op != 5){
        cout << "Escolha uma das opcoes abaixo: " << endl;
        cout << "1) Criptografar um texto com uma chave. " << endl;
        cout << "2) Descriptografar um criptograma com uma chave. " << endl;
        cout << "3) Ataque de recuperacao de senha, texto em portugues. " << endl;
        cout << "4) Ataque de recuperacao de senha, texto em ingles. " << endl;
        cout << "5) Sair. " << endl;
        
        cin >> op;
        switch(op){
            case 1:
                funcao(op);
                break;
            case 2:
                funcao(op);
                break;
            case 3:
                funcao(op);
                break;
            case 4:
                funcao(op);
                break;
            default:
                break;
        }
    }
}

int main(){
    menu();

    return 0;
}

/*
    Advances in microscopy also had a profound impact on biological thinking. In the early 19th century, a number of biologists pointed to the central importance of the cell. Then, in 1838, Schleiden and Schwann began promoting the now universal ideas that (1) the basic unit of organisms is the cell and (2) that individual cells have all the characteristics of life, although they opposed the idea that (3) all cells come from the division of other cells. Thanks to the work of Robert Remak and Rudolf Virchow, however, by the 1860s most biologists accepted all three tenets of what came to be known as cell theAdvances in microscopy also had a profound impact on biological thinking. In the early 19th century, a number of biologists pointed to the central importance of the cell. Then, in 1838, Schleiden and Schwann began promoting the now universal ideas that (1) the basic unit of organisms is the cell and (2) that individual cells have all the characteristics of life, although they opposed the idea that (3) all cells come from the division of other cells. Thanks to the work of Robert Remak and Rudolf Virchow, however, by the 1860s most biologists accepted all three tenets of what came to be known as cell theory.
*/

/*
    regulating the circulation.  whenever ifind myself growing grim about the mouth; whenever it is a damp,drizzly november in my soul; whenever i find myself involuntarilypausing before coffin warehouses, and bringing up the rear of everyfuneral i meet; and especially whenever my hypos get such an upperhand of me, that it requires a strong moral principle to prevent mefrom deliberately stepping into the street, and methodically knockingpeople's hats off--then, i account it high time to get to sea as soonas i can.  this is my substitute for pistol and ball.  with aphilosophical flourish cato throws himself upon his sword; i quietlytake to the ship.  there is nothing surprising in this.  if they butknew it, almost all men in their degree, some time or other, cherish very nearly the same feelings towards the ocean with me.
*/

/*
rvgllakieg tye tirtucatzoe.  whvnvvei i winu mpsecf xronieg giid abfuk thv mfuty; wyenvvvr ik ij a drmg, drzzqly eomemsei in dy jouc; wyenvvvr i wied mpsvlf znmollnkarzlp palszng seworv cfffzn narvhfusvs, rnd srzngznx up khv rerr ff emeiy flnvrac i deek; aed ejpvcirlcy wyeeevvr dy hppfs gvt jucy ae upgei haed ff mv, tyat zt ieqliies r skroeg dorrl grieczplv tf prvvvnt de wrod dvliseiatvlp stvpginx ieto khv stievt, aed detyouicrlcy keotkieg geoglv's hrtj ofw--tyen, z atcolnk it yixh tzmv to xek to jer as jofn aj i tan.  khzs ij mp susskitltv foi pzstfl rnd sacl.  wzty a pyicosfpyicrl wlolrzsh tako tyrfws yidsecf lpoe hzs snoid; i huzetcy kakv tf thv syip.  khvre zs eotyieg slrgrijieg ie tyis.  zf khep blt keen it, rldosk acl mvn zn tyezr dvgiee, jode tzmv or ftyer, thvrijh merp nvarcy khe jade fvecinxs kowrrus tye fcern nity mv.
*/

/*
    
*/