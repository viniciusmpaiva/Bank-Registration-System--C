//Inclusão das bibliotecas
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//Definição de uma structure para os dados cadastrados
typedef struct 
{
    int id;
    int *idade;
    char *nome;
    float *saldo_atual;
}inf_padrao;

//Função utilizada para limpar a tela ao apertar "enter"
void limpa_tela(){
    system("pause");
    system("cls");
    return;
}

//A função irá atribuir o id a cada novo usuário cadastrado
int atribuicao_id(){
    //Caso não haja nenhum usuário no arquivo ainda, o id do primeiro será 1
    int ultimo_id = 0 ,novo_id;
    char line[100];
    FILE *leitura_arquivo;
    leitura_arquivo = fopen("dados_cadastrados.txt","r");
    if(leitura_arquivo == NULL){
        printf("Erro ao abrir o arquivo!");
    }
    //Aqui o programa irá ler o ultimo id cadastrado no arquivo e atribuirá 1 a mais no id do novo usuário cadastrado.
    while(fgets(line,sizeof(line),leitura_arquivo) != NULL){
        sscanf(line,"id %d",&ultimo_id);
    }
    fclose(leitura_arquivo);
    novo_id = ultimo_id + 1;
    return novo_id;
    
}

//A função irá cadatrar o usuário utilizando alocação dinâmica.
//O parâmetro "qtd_usuarios" vem da função main, onde se for acionada a 1 opção será atribuido automaticamente o valor 1 a esta variável
//e se for selecionada a 2 opção, o valor atribuido a ela sera informado pelo usuário
void cadastro(int qtd_usuarios){
    inf_padrao dados_pessoais;
    FILE *arquivo;
    arquivo = fopen("dados_cadastrados.txt","a");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo!");
        return;
    }
    //Aqui é realizada a alocação dinâmica de memória
    dados_pessoais.id = atribuicao_id();
    dados_pessoais.idade = (int*)calloc(qtd_usuarios,sizeof(int));
    dados_pessoais.nome = (char*)malloc(sizeof(char)*100);
    dados_pessoais.saldo_atual = (float*)calloc(qtd_usuarios,sizeof(float));
    int c;
    printf("\nInsira o nome, idade e o salario atual\nNome: ");
    getchar();
    gets(dados_pessoais.nome);
    for (int i = 0; dados_pessoais.nome[i] != '\0'; i++) {
        if (isdigit(dados_pessoais.nome[i])) {
            printf("Nome invalido!\n");
            free(dados_pessoais.idade);
            free(dados_pessoais.nome);
            free(dados_pessoais.saldo_atual);
            limpa_tela();
            return;
        }
    }
    printf("\nIdade: ");
    while(scanf("%d",dados_pessoais.idade)!=1){
        printf("Idade invalida!\n");
        limpa_tela();
        while ((c = getchar()) != '\n' && c != EOF) {}
        return;
    }

    if((*dados_pessoais.idade)<0 || *(dados_pessoais.idade)>120){
        printf("A idade informada e invalida! Voce sera redirecionado(a) ao menu\n");
        limpa_tela();
        return;
    }

    printf("\nSaldo atual: ");
    while(scanf("%f",dados_pessoais.saldo_atual)!=1){
        printf("Saldo invalido!\n");
        while ((c = getchar()) != '\n' && c != EOF) {}
        limpa_tela();
        return;
    }

    if(*(dados_pessoais.saldo_atual)<0){
        printf("O saldo informado e invalido! Voce sera redirecionado(a) ao menu\n");
        limpa_tela();
        return;
    }

    printf("\nUsuario cadastrado com id %d\n", dados_pessoais.id);
    fprintf(arquivo,"id %d: %s; %d; %.2f\n",dados_pessoais.id,dados_pessoais.nome,*(dados_pessoais.idade),*(dados_pessoais.saldo_atual));
    fclose(arquivo);
    free(dados_pessoais.idade);
    free(dados_pessoais.nome);    
    free(dados_pessoais.saldo_atual);
    limpa_tela();
}


//FUNÇÃO PARA BUSCAR ID


void buscar_id(){
    FILE *leitura_arquivo;
    leitura_arquivo = fopen("dados_cadastrados.txt","r");
    if(leitura_arquivo == NULL){
        printf("\nErro ao abrir o arquivo!");
    }

    //Aqui o programa lê o primeiro caractere do arquivo para a verificação de existência de dados nele
    int id_buscado,idade_buscada,id_lido;
    int encontrado = 0;
    int c;
    char line[100],nome_buscado[100];
    float saldo_buscado;

    printf("Digite o Id do usuario: ");
    while(scanf("%d",&id_buscado)!=1){
        printf("Id invalido!\n");
        limpa_tela();
        while ((c = getchar()) != '\n' && c != EOF) {}
        return;
    }

    //Aqui há a busca pelo id informado pelo usuário no arquivo por meio de um laço while que se extente até o momento que se encontra esse id
    while (fgets(line, sizeof(line), leitura_arquivo)) {
        sscanf(line, "id %d", &id_lido);

        //No  momento em que o id informado pelo usuário e o id lido são iguais, há a leitura dos dados pertencentes ao id
        if (id_lido == id_buscado) {
            sscanf(line, "id %d: %[^;]; %d; %f", &id_buscado, nome_buscado, &idade_buscada, &saldo_buscado);
            encontrado = 1;
            break;
        }
    }
    //Aqui uma verificação de existência do usuário informado
    if(encontrado == 1){
        printf("Usuario Id %d\nNome: %s\nIdade: %d\nSaldo: R$%.2f\n",id_buscado,nome_buscado, idade_buscada,saldo_buscado);
        limpa_tela();
    }else{
        printf("Erro! usuario nao encontrado\n");
        limpa_tela();
    }
    fclose(leitura_arquivo);
}


//FUNÇÃO PARA A EXCLUSÃO DE USUÁRIO POR ID

void excluir_id(){
    FILE *leitura_arquivo;
    FILE *arq_temporario;
    leitura_arquivo = fopen("dados_cadastrados.txt","r");
    arq_temporario = fopen("temp.txt","w");
    if(leitura_arquivo == NULL || arq_temporario == NULL){
        printf("\nErro ao abrir o arquivo!\n");
        return;
    }
    int id_excluido, id_lido,excluido=0,c;
    char line[100];
    printf("Digite o id do usuario a ser excluido: ");
    while(scanf("%d",&id_excluido) !=1){
        printf("Id invalido!\n");
        while ((c = getchar()) != '\n' && c != EOF) {}
        limpa_tela();
        return;
        remove("temp.txt");
    }

    //O programa irá procurar todos os ids diferentes do digitado e copiara todas as informações destes em um novo arquivo temporário
    while (fgets(line, sizeof(line), leitura_arquivo)) { 
        sscanf(line, "id %d", &id_lido);
        if(id_lido!=id_excluido){
            fputs(line,arq_temporario);
            
        }else{
            excluido = 1;
        }
    }
    if(excluido == 1){
        printf("Usario de id %d excluido com suceso!\n",id_excluido);
    }else{
        printf("Falha ao excluir usuario!\n");
    }
    fclose(arq_temporario);
    fclose(leitura_arquivo);
}


//FUNÇÃO PARA A TRANSFERÊNCIA ENTRE USUÁRIOS


void transf(){  
    FILE *leitura_arquivo;
    FILE *arq_temporario;
    leitura_arquivo = fopen("dados_cadastrados.txt","r");
    arq_temporario = fopen("temp.txt","w");
    if(leitura_arquivo == NULL || arq_temporario == NULL){
        printf("\nErro ao abrir o arquivo!\n");
        return;
    }
    int id_origem,id_destino,id_lido_origem,encontrado_origem=0,id_lido_destino,encontrado_destino = 0;
    int idade_origem,idade_destino,c;
    char nome_origem[100], nome_destino[100];

    float quantia,saldo_id_origem, saldo_id_destino;
    char line[100]; 
    printf("Digite o Id do usuario de origem: ");
    while(scanf("%d",&id_origem)!=1){
        printf("Id invalido!");
        while ((c = getchar()) != '\n' && c != EOF) {}
        limpa_tela();
        return;
    }

    //O programa irá buscar as informações do id de origem informado e atribuirá estas a novas variáveis
    while (fgets(line, sizeof(line), leitura_arquivo)) {
        sscanf(line, "id %d", &id_lido_origem);
        if (id_lido_origem == id_origem) {
            sscanf(line, "id %d: %[^;]; %d; %f", &id_lido_origem, nome_origem, &idade_origem, &saldo_id_origem);
            encontrado_origem= 1;
            break;
        }

    }
    if (encontrado_origem == 0){
        printf("O Id digitado não esta cadastrado!\n");
        fclose(leitura_arquivo);
        fclose(arq_temporario);
        remove("temp.txt");
        return;
    }

    printf("Digite a quantia: ");
    while(scanf("%f",&quantia) !=1){
        printf("Quantia invalida!\n");
        while ((c = getchar()) != '\n' && c != EOF) {}
        limpa_tela();
        return;
    }

    if(quantia<0){
        printf("Valor invalido!\n");
        fclose(leitura_arquivo);
        fclose(arq_temporario);
        remove("temp.txt");
        return;
    }

    if(quantia>saldo_id_origem){
        printf("O usuario de id %d nao possui saldo disponivel para esta transacao\n",id_origem);
        fclose(leitura_arquivo);
        fclose(arq_temporario);
        remove("temp.txt");
        return;
    }

    //Reprogramação do ponteiro do arquivo para o inicio dele
    rewind(leitura_arquivo);

    //O programa irá buscar as informações do id de destino informado e atribuirá estas a novas variáveis
    printf("Digite o id de destino desta quantia: ");
    while(scanf("%d",&id_destino) != 1){
        printf("Id invalido! Retorne ao menu e tente novamente\n");
        while ((c = getchar()) != '\n' && c != EOF) {}
        limpa_tela();
        return;
    }
    while (fgets(line, sizeof(line), leitura_arquivo)) {
        sscanf(line, "id %d", &id_lido_destino);
        if (id_lido_destino == id_destino) {
            sscanf(line, "id %d: %[^;]; %d; %f", &id_lido_destino, nome_destino, &idade_destino, &saldo_id_destino);
            encontrado_destino = 1;
            break;
        }
    }
    if(encontrado_destino == 0){
        printf("O Id cadastrado nao esta cadastrado no sistema\n");
        fclose(leitura_arquivo);
        fclose(arq_temporario);
        remove("temp.txt");
        return;
    }

    //Realizará as operações de transação
    saldo_id_destino = saldo_id_destino + quantia;
    saldo_id_origem = saldo_id_origem - quantia;

    rewind(leitura_arquivo);

    int transf = 0;

    //O programa aqui lê todas as informações do arquivo original e passa todas elas, com excessão das envolvidas na operação, para um arquivo temporário
    while (fgets(line, sizeof(line), leitura_arquivo)) { 
        sscanf(line, "id %d", &id_lido_origem);
        sscanf(line, "id %d",&id_lido_destino);
        if(id_lido_origem!=id_origem && id_lido_destino != id_destino){
            fputs(line,arq_temporario);
        }
        //Agora o programa passa as informações dos envolvidos com o saldo atualizado para o arquivo temporário
        if(id_lido_origem == id_origem){
            fprintf(arq_temporario,"id %d: %s; %d; %.2f\n",id_origem,nome_origem,idade_origem,saldo_id_origem);
            transf++;     
        }

        if(id_lido_destino==id_destino){
            fprintf(arq_temporario,"id %d: %s; %d; %.2f\n",id_destino,nome_destino,idade_destino,saldo_id_destino);
            transf++; 
        }
    }

    if(transf == 2){
        printf("Transferencia concluida!\nSaldo do usuario de id %d: %.2f\nSaldo do usuario de id %d: %2.f\n",id_origem,saldo_id_origem,id_destino,saldo_id_destino);
    }else{
        printf("Falha ao realizar a transferencia!\n");
        return;
    }

    fclose(leitura_arquivo);
    fclose(arq_temporario);

    //Assim como na opção de excuir id, aqui o arquivo original é excluido e o temporário recebe seu nome
    remove("dados_cadastrados.txt");
    rename("temp.txt","dados_cadastrados.txt");
    limpa_tela();
}


int main(){
    setlocale(LC_ALL ,"Portuguese");
    int opcao,qtd_usuarios,i;
    FILE *arquivo;
    arquivo = fopen("dados_cadastrados.txt","a");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo, tente novamente!");
        return 0;
    }

    while(1){
        printf("\n1 - Insercao de um novo usuario: \n2 - Insercao de varios usuarios: \n3 -Busca de um usuario por id: \n4 - Transferencias entre usuarios: \n5 -Remocao de um usuario por id:  \n6 - Fechar o programa\nInsira a opcao desejada: ");
        scanf("%d",&opcao);
        fflush(stdin);
        limpa_tela();
        int c;
        switch(opcao){
            case 1:
            qtd_usuarios = 1;
            cadastro(qtd_usuarios);
            break;

            case 2:
            printf("\nDigite o numero de usuarios a serem cadastrados: ");
            if(scanf("%d",&qtd_usuarios)!=1){
                printf("Opcao Invalida! Digite novamente\n");
                while ((c = getchar()) != '\n' && c != EOF) {}
                limpa_tela();
                break;
            }
            for(i=0;i<=qtd_usuarios-1;i++){
                cadastro(qtd_usuarios);
            }
            break;

            case 3:
            buscar_id();
            break;
        
            case 4:

            fclose(arquivo);
            transf();
            limpa_tela();
            break;

            case 5:

            excluir_id();
            fclose(arquivo);
            //O arquivo original será excluido e o temporário receberá o nome do original
            remove("dados_cadastrados.txt");
            rename("temp.txt","dados_cadastrados.txt");
            limpa_tela();
            break;
    
            case 6:
            limpa_tela();
            printf("Voce saiu do sistema de cadastro!");
            fclose(arquivo);
            return 0;
                
            default:
            printf("Opcao invalida, digite novamente!\n");
            limpa_tela();
            break;
        }
    }

}
