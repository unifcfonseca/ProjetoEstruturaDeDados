#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tamNome 30
#define tamRG 11
#define tamHeap 20

void limpaBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

typedef struct Data{
  	int Dia;
    int Mes;
    int Ano;
} Data;

typedef struct Registro{
  	struct Data* Entrada;
  	int Idade;
    char Nome[tamNome];
    char RG[tamRG];
} Registro;

typedef struct Vertice{
    int valor;
	Registro *info;
	struct Vertice* esq;
	struct Vertice* dir;
	struct Vertice* pai;
} Vertice;

typedef struct Arvore{
	Vertice* raiz;
	int qtde;
} Arvore;

typedef struct ArvoreABB{
	Arvore *arvoreDia;
    Arvore *arvoreMes;
    Arvore *arvoreAno;
    Arvore *arvoreIdade;
} ArvoreABB;

void in_ordem_abb(Vertice *raiz) {
    if(raiz==NULL){
        return;
    }
    in_ordem_abb(raiz->esq);
    printf("Nome: %s, RG: %s, Idade: %d\n", raiz->info->Nome, raiz->info->RG, raiz->info->Idade);
    printf("Entrada: %d/%d/%d\n\n", raiz->info->Entrada->Dia, raiz->info->Entrada->Mes,raiz->info->Entrada->Ano);
    in_ordem_abb(raiz->dir);
}

Vertice *procura_in_ordem_abb(Vertice *raiz, Registro *info) {
    if(raiz==NULL){
        return NULL;
    }
    if(raiz->info==info){
        return raiz;
    }
    Vertice *x = procura_in_ordem_abb(raiz->esq, info);
    if(x!=NULL){
        return x;
    }

    Vertice *y = procura_in_ordem_abb(raiz->dir, info);
    if(y!=NULL){
        return y;
    }
}

Vertice *cria_vertice(Registro *info, int valor){
	Vertice* novo = malloc(sizeof(Vertice));
	novo->dir = NULL;
	novo->esq = NULL;
	novo->pai = NULL;
	novo->info = info;
    novo->valor = valor;
	return novo;
}

Arvore *cria_arvore(){
	Arvore* arvore = malloc(sizeof(Arvore));
	arvore->raiz = NULL;
	arvore->qtde = 0;

	return arvore;
}

ArvoreABB *cria_arvore_abb(){
	ArvoreABB* arvoreABB = malloc(sizeof(ArvoreABB));

    arvoreABB->arvoreDia   = cria_arvore();
    arvoreABB->arvoreMes   = cria_arvore();
    arvoreABB->arvoreAno   = cria_arvore();
    arvoreABB->arvoreIdade = cria_arvore();

	return arvoreABB;
}

void inserir_vertice(Arvore* arvore, Registro *info, int qual){
    int valor;
    if (qual == 0){
        valor = info->Entrada->Ano;
    }else if (qual == 1){
        valor = info->Entrada->Mes;
    }else if (qual == 2){
        valor = info->Entrada->Dia;
    }else{
        valor = info->Idade;
    } 

    Vertice *novo = cria_vertice(info, valor);
	Vertice *atual = arvore->raiz;
    Vertice *anterior = NULL;
    if(atual==NULL){
        arvore->raiz = novo;
        arvore->qtde++;
        return;
    }
    while(atual!=NULL){
        anterior = atual;
        if(atual->valor>valor){
            atual = atual->esq;
        }else{
            atual = atual->dir;
        }
    }
    novo->pai = anterior;
    if(anterior->valor>novo->valor){
        anterior->esq = novo;
    }else{
        anterior->dir = novo;
    }
    arvore->qtde++;
}

void remover_vertice(Arvore* arvore, Vertice* vertice) {
    if(vertice->esq == NULL && vertice->dir == NULL){
        if(vertice->pai==NULL){
            arvore->raiz=NULL;
        }else if(vertice->pai->esq==vertice){
            vertice->pai->esq=NULL;
        }else{
            vertice->pai->dir=NULL;
        }
        arvore->qtde--;
    }else if(vertice->esq == NULL){
        if(arvore->raiz==vertice){
            arvore->raiz=vertice->dir;
            vertice->dir->pai = NULL;
        }else{
            if(vertice->pai->esq==vertice){
                vertice->pai->esq=vertice->dir;
                vertice->dir->pai = vertice->pai;
                
            }else{
                vertice->pai->dir=vertice->dir;
                vertice->dir->pai = vertice->pai;
            }
        }
        arvore->qtde--;
    }else if(vertice->dir == NULL){
        if(arvore->raiz==vertice){
            arvore->raiz=vertice->esq;
            vertice->esq->pai = NULL;
        }else{
            if(vertice->pai->esq==vertice){
                vertice->pai->esq=vertice->esq;
                vertice->esq->pai = vertice->pai;
            }else{
                vertice->pai->dir=vertice->esq;
                vertice->esq->pai = vertice->pai;
            }
        }
        arvore->qtde--;
    }else{
        Vertice *filhaD = vertice->esq;
        while(filhaD->dir!=NULL){
            filhaD = filhaD->dir;
        }
        int x = vertice->valor;
        vertice->valor = filhaD->valor;
        filhaD->valor = x;
        remover_vertice(arvore,filhaD);
    }
}

void inserirProcurar(ArvoreABB* arvoreabb, Registro *info){
    inserir_vertice(arvoreabb->arvoreAno, info, 0);
    inserir_vertice(arvoreabb->arvoreMes, info, 1);
    inserir_vertice(arvoreabb->arvoreDia, info, 2);
    inserir_vertice(arvoreabb->arvoreIdade, info, 3);
}

void removerProcurar(ArvoreABB* arvoreabb, Registro *info){
    
    Vertice *vAno = procura_in_ordem_abb(arvoreabb->arvoreAno->raiz, info);
    Vertice *vMes = procura_in_ordem_abb(arvoreabb->arvoreMes->raiz, info);
    Vertice *vDia = procura_in_ordem_abb(arvoreabb->arvoreDia->raiz, info);
    Vertice *vIdade = procura_in_ordem_abb(arvoreabb->arvoreIdade->raiz, info);

    if(vAno!=NULL){
        remover_vertice(arvoreabb->arvoreAno, vAno);
    }
    if(vMes!=NULL){
        remover_vertice(arvoreabb->arvoreMes, vMes);
    }
    if(vDia!=NULL){
         remover_vertice(arvoreabb->arvoreDia, vDia);
    }
    if(vIdade!=NULL){
        remover_vertice(arvoreabb->arvoreIdade, vIdade);
    }
}

Data *criar_data(int Dia,int Mes,int Ano){
    Data *novo = malloc(sizeof(Data));
    novo->Dia = Dia;
	novo->Mes = Mes;
    novo->Ano = Ano;
    return novo;
}

Registro *criar_registro(int Dia, int Mes, int Ano, int Idade, char Nome[tamNome], char RG[tamRG]){
    Registro *registro = malloc(sizeof(Registro));
    registro->Entrada = criar_data(Dia, Mes, Ano);
	registro->Idade = Idade;
	strcpy(registro->Nome, Nome);
    strcpy(registro->RG, RG);
    return registro;
}

typedef struct Celula {
    struct Celula *proximo;
    struct Registro *info;
} Celula;

typedef struct LDE {
    Celula *inicio;
    int qtde;
} LDE;

LDE *criar_lista() {
    LDE *lista = malloc(sizeof(LDE));
    lista->inicio = NULL;
    lista->qtde = 0;
    return lista;
}

Celula *criar_celula(Registro *info) {
    Celula *novo = malloc(sizeof(Celula));
    novo->info = info;
    novo->proximo = NULL;
    return novo;
}

void inserir_paciente_LDE(LDE *lista, Registro *info) {
    Celula *nova = criar_celula(info);
    Celula *proximo = lista->inicio;
    Celula *anterior = NULL;
    while(proximo != NULL ){
        anterior = proximo;
        proximo = proximo->proximo;
    }
    if (anterior == NULL) {
        lista->inicio = nova;
        if (proximo != NULL) {
            nova->proximo = proximo;
        }
    } else {
        anterior->proximo = nova;  
    }
    lista->qtde++;
}

void exibir_lista_LDE(LDE *lista) {
    Celula *proximo = lista->inicio;
    if(proximo == NULL){
        printf("Lista vazia!\n");
    }
    while(proximo != NULL){
        printf("Nome: %s, RG: %s, Idade: %d\n", proximo->info->Nome, proximo->info->RG, proximo->info->Idade);
        printf("Entrada: %d/%d/%d\n\n", proximo->info->Entrada->Dia, proximo->info->Entrada->Mes,proximo->info->Entrada->Ano);
        proximo = proximo->proximo;
    }
}

void remover_paciente_LDE(LDE *lista, Registro *info) {
    Celula *atual = lista->inicio;
    Celula *anterior = NULL;
    while(atual != NULL){
        if(atual->info == info){
            if(anterior == NULL){
                lista->inicio = atual->proximo;
            }else{
                anterior->proximo = atual->proximo;
            }
            lista->qtde--;
            free(atual->info->Entrada);
            free(atual->info);
            free(atual);
            break;
        }
        anterior = atual;
        atual = atual->proximo;
    }
}

Registro *acha_paciente_LDE(LDE *lista, char RG[tamRG]) {
    Celula *atual = lista->inicio;
    while(atual != NULL){
        if(strcmp(atual->info->RG, RG) == 0){
            return atual->info;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void novoPaciente(LDE *lista, ArvoreABB* arvoreabb){
    int dia, mes, ano, idade;
    char nome[tamNome];
    char RG[tamRG];

    printf("Digite o RG: ");
    fgets(RG, tamRG, stdin);
    RG[strcspn(RG, "\n")] = 0;
    
    Registro *checaPaciente = acha_paciente_LDE(lista, RG);
    if(checaPaciente != NULL){
        return;
    }

    printf("Digite o Nome: ");
    fgets(nome, tamNome, stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite a idade: \n");
    scanf("%d", &idade);
    limpaBuffer();

    printf("Digite o dia de entrada: \n");
    scanf("%d", &dia);
    limpaBuffer();

    printf("Digite o mes de entrada: \n");
    scanf("%d", &mes);
    limpaBuffer();

    printf("Digite o ano de entrada: \n");
    scanf("%d", &ano);
    limpaBuffer();

    Registro *paciente = criar_registro(dia, mes, ano, idade, nome, RG);
    inserirProcurar(arvoreabb, paciente);
    inserir_paciente_LDE(lista, paciente);
}

void consultaPaciente(LDE *lista){
    char RG[tamRG];

    printf("Digite o RG: ");
    fgets(RG, tamRG, stdin);
    RG[strcspn(RG, "\n")] = 0;

    Registro *checaPaciente = acha_paciente_LDE(lista, RG);
    if(checaPaciente == NULL){
        printf("Paciente não encontrado!\n");
        return;
    }else{
        printf("Nome: %s, RG: %s, Idade: %d\n", checaPaciente->Nome, checaPaciente->RG, checaPaciente->Idade);
        printf("Entrada: %d/%d/%d\n", checaPaciente->Entrada->Dia, checaPaciente->Entrada->Mes, checaPaciente->Entrada->Ano);
    }
}

void atualizaPaciente(LDE *lista){
    int dia, mes, ano, idade;
    char nome[tamNome];

    char RG[tamRG];

    printf("Digite o RG: ");
    fgets(RG, tamRG, stdin);
    RG[strcspn(RG, "\n")] = 0;

    Registro *checaPaciente = acha_paciente_LDE(lista, RG);

    if(checaPaciente == NULL){
        printf("Paciente não encontrado!\n");
        return;
    }

    char opcao;

    do{
        printf("Mudar nome do paciente?\n[S/N]: ");
        scanf("%c", &opcao);
        limpaBuffer();

        printf("\n");
        if(opcao!='S' && opcao!='N'){
            printf("Opção Invalida!\n");
        }else{
            break;
        }
    }while(1);

    if(opcao=='S'){
        printf("Digite o Nome: ");
        fgets(nome, tamNome, stdin);
        nome[strcspn(nome, "\n")] = 0;
        strcpy(checaPaciente->Nome, nome);
    }

    do{
        printf("Mudar idade do paciente?\n[S/N]: ");
        scanf("%c", &opcao);
        limpaBuffer();

        printf("\n");
        if(opcao!='S' && opcao!='N'){
            printf("Opção Invalida!\n");
        }else{
            break;
        }
    }while(1);

    if(opcao=='S'){
        printf("Digite a idade: \n");
        scanf("%d", &idade);
        limpaBuffer();

        checaPaciente->Idade = idade;
    }

    do{
        printf("Mudar data de entrada do paciente?\n[S/N]: ");
        scanf("%c", &opcao);
        limpaBuffer();

        printf("\n");
        if(opcao!='S' && opcao!='N'){
            printf("Opção Invalida!\n");
        }else{
            break;
        }
    }while(1);
        
    if(opcao=='S'){
        printf("Digite o dia de entrada: \n");
        scanf("%d", &dia);
        limpaBuffer();

        checaPaciente->Entrada->Dia = dia;

        printf("Digite o mes de entrada: \n");
        scanf("%d", &mes);
        limpaBuffer();

        checaPaciente->Entrada->Mes = mes;

        printf("Digite o ano de entrada: \n");
        scanf("%d", &ano);
        limpaBuffer();

        checaPaciente->Entrada->Ano = ano;
    }
}

void removePaciente(LDE *lista, ArvoreABB* arvoreabb){
    char RG[tamRG];

    printf("Digite o RG: ");
    fgets(RG, tamRG, stdin);
    RG[strcspn(RG, "\n")] = 0;

    Registro *checaPaciente = acha_paciente_LDE(lista, RG);

    if(checaPaciente == NULL){
        printf("Paciente não encontrado!\n");
        return;
    }

    char opcao;

    do{
        printf("Remover Paciente %s?\n[S/N]: ", checaPaciente->Nome);
        scanf("%c", &opcao);
        limpaBuffer();

        printf("\n");
        if(opcao!='S' && opcao!='N'){
            printf("Opção Invalida!\n");
        }else{
            break;
        }
    }while(1);

    if(opcao=='S'){
        remover_paciente_LDE(lista, checaPaciente);
        removerProcurar(arvoreabb, checaPaciente);
        printf("Paciente Removido!\n");
    }

}

void Cadastrar(LDE *lista, ArvoreABB* arvoreabb){
    int escolha;
    do{
        printf("\n1 - Cadastrar novo paciente\n");
        printf("2 - Consultar cliente cadastrado\n");
        printf("3 - Mostrar lista completa\n");
        printf("4 - Atualizar dados de paciente\n");
        printf("5 - Remover paciente\n");
        printf("0 - Sair\n");
        printf("Digite a opção: ");

        scanf("%d", &escolha);
        limpaBuffer();
        printf("\n");

        if(escolha==1){
            novoPaciente(lista,arvoreabb);
        }else if(escolha==2){
            consultaPaciente(lista);
        }else if(escolha==3){
            exibir_lista_LDE(lista);
        }else if(escolha==4){
             atualizaPaciente(lista);
        }else if(escolha==5){
            removePaciente(lista,arvoreabb);
        }else if(escolha==0){
            printf("Saindo!\n");
            break;
        }else{
            printf("Voltando ao menu!\n");
        }
    }while(1);
}

typedef struct CellDesfazer {
  struct CellDesfazer *anterior;
  struct CellDesfazer *proximo;
  struct Registro *info; 
  int operacao;
} CellDesfazer;

CellDesfazer *criar_cell_desfazer(int operacao, Registro *info) {
  CellDesfazer *nova = malloc(sizeof(CellDesfazer));
  nova->anterior = NULL;
  nova->proximo = NULL;
  nova->info = info;
  nova->operacao = operacao;
  return nova;
}

typedef struct {
  CellDesfazer *topo;
  int qtde;
} Pilha;

typedef struct Fila{
  	struct Celula* head;
	struct Celula* tail;
  	int qtde;
} Fila;

Pilha *criar_pilha() {
    Pilha *pilha = malloc(sizeof(Pilha));
    pilha->qtde = 0;
    pilha->topo = NULL;
    return pilha;
}

Fila *criar_fila(){
    Fila *queue = malloc(sizeof(Fila));
    queue->head = NULL;
    queue->tail = NULL;
    queue->qtde = 0;
    return queue;
}

void push(Pilha *pilha, int operacao, Registro *info) {
  CellDesfazer *nova = criar_cell_desfazer(operacao, info);
  if (pilha->qtde != 0) {
    nova->proximo = pilha->topo;
    pilha->topo->anterior = nova;
  }
  pilha->topo = nova;
  pilha->qtde++;
}

CellDesfazer *pop(Pilha *pilha) {
  if (pilha->qtde > 0) {
    CellDesfazer *topo = pilha->topo;
    pilha->topo = pilha->topo->proximo;
    if (pilha->topo != NULL){
        pilha->topo->anterior = NULL;
    }
    pilha->qtde--;
    CellDesfazer *valor = topo;
    free(topo);
    return valor;
  } else {
    return NULL;
  }
}

void desfazerOperacoes(Pilha *pilha, Fila *queue) {
    CellDesfazer *topo = pilha->topo;
    if(pilha->qtde==0){
        printf("Nenhuma operação feita na fila de atendimento!\n");
        return;
    }else{
        char opcao;
        do{
            printf("Ultima operação feita na fila de atendimento: \n\n");
            if(topo->operacao==0){
                printf("Operação de desenfileirar o paciente: \n");
            }else{
                printf("Operação de enfileirar o paciente: \n");
            }
            printf("Nome: %s, RG: %s, Idade: %d\n", topo->info->Nome, topo->info->RG, topo->info->Idade);
            printf("Entrada: %d/%d/%d\n\n", topo->info->Entrada->Dia, topo->info->Entrada->Mes,topo->info->Entrada->Ano);
            printf("Desfazer operação?\n[S/N]: ");
            scanf("%c", &opcao);
            limpaBuffer();

            printf("\n");
            if(opcao!='S' && opcao!='N'){
                printf("Opção Invalida!\n");
            }else{
                break;
            }
        }while(1);

        if(opcao=='S'){
            CellDesfazer *ultimaOp = pop(pilha);
            if(ultimaOp->operacao==0){
                Celula *novo = criar_celula(ultimaOp->info);
                novo->proximo = queue->head;
                queue->head = novo;
                if(queue->tail==NULL){
                    queue->tail = novo;
                }
                queue->qtde++;
            }else{
                Celula *ultimo = queue->tail;
                Celula *proximo = queue->head;
                Celula *anterior = NULL;
                while(proximo!=ultimo){
                    anterior = proximo;
                    proximo = proximo->proximo;
                }
                if(anterior!=NULL){
                    anterior->proximo = NULL;
                }
                queue->tail = anterior;
                queue->qtde--;
                free(ultimo);
            }
        }

    }

    
}

void enfileirar(Fila *queue, Registro *info){
	Celula *novo = criar_celula(info);
	if(queue->tail==NULL){
		queue->head = novo;
		queue->tail = novo;
	}else{
		queue->tail->proximo = novo;
		queue->tail = novo;
	}
	queue->qtde++;
}

Registro *desenfileirar(Fila *queue, Pilha *pilha){
	if(queue->head==NULL){
		return NULL;
	}
	Celula *resultado = queue->head;
	Registro *newInfo = resultado->info;

	queue->head = resultado->proximo;
	if(queue->head == NULL){
		queue->tail = NULL;
	}
	queue->qtde--;
	free(resultado);
    push(pilha,0, newInfo);
	return newInfo;
}

void enfileirarFila(LDE *lista, Fila *queue, Pilha *pilha){
    char RG[tamRG];

    printf("Digite o RG a ser colocado na fila de atendimento: ");
    fgets(RG, tamRG, stdin);
    RG[strcspn(RG, "\n")] = 0;

    Registro *checaPaciente = acha_paciente_LDE(lista, RG);
    if(checaPaciente == NULL){
        printf("Paciente não encontrado!\n");
        return;
    }

    Celula *proximo = queue->head;
    while(proximo != NULL){
            if(proximo->info==checaPaciente){
                printf("Paciente já está na fila!\n");
                return;
            }            
            proximo = proximo->proximo;
        }

    printf("Paciente enfileirado!\n");
    enfileirar(queue, checaPaciente);
    push(pilha, 1, checaPaciente);
    
}

void desenfileirarFila(Fila *queue, Pilha *pilha){
    Registro *atendePaciente = desenfileirar(queue, pilha);

    if(atendePaciente == NULL){
        printf("Nenhum paciente na fila!\n");
        return;
    }else{
        printf("Proximo paciente a ser atendido: \n");
        printf("Nome: %s, RG: %s, Idade: %d\n", atendePaciente->Nome, atendePaciente->RG, atendePaciente->Idade);
        printf("Entrada: %d/%d/%d\n\n", atendePaciente->Entrada->Dia, atendePaciente->Entrada->Mes,atendePaciente->Entrada->Ano);
    }
}

void exibirFila(Fila *queue) {
    Celula *proximo = queue->head;
    int i = 1;
    if(queue->qtde==0){
        printf("Fila vazia!\n");
    }else{
        printf("Exibindo fila em ordem: \n");
        while(proximo != NULL){
            printf("Paciente %dº: \n", i);
            printf("Nome: %s, RG: %s, Idade: %d\n", proximo->info->Nome, proximo->info->RG, proximo->info->Idade);
            printf("Entrada: %d/%d/%d\n\n", proximo->info->Entrada->Dia, proximo->info->Entrada->Mes,proximo->info->Entrada->Ano);
            proximo = proximo->proximo;
            i++;
        }
    }
}

void Atendimento(Fila *fila, LDE *lista, Pilha *pilha){
    int escolha;
    do{
        printf("\n1 - Enfileirar paciente\n");
        printf("2 - Atender proximo paciente\n");
        printf("3 - Mostrar fila\n");
        printf("0 - Sair\n");
        printf("Digite a opção: ");

        scanf("%d", &escolha);
        limpaBuffer();
        printf("\n");

        if(escolha==1){
            enfileirarFila(lista,fila,pilha);
        }else if(escolha==2){
            desenfileirarFila(fila,pilha);
        }else if(escolha==3){
            exibirFila(fila);
        }else if(escolha==0){
            printf("Voltando ao menu!\n");
            break;
        }else{
            printf("Opção Invalida!\n");
        }
    }while(1);
}

typedef struct Heap{
    struct Registro *valores[tamHeap];
    int qtde;
} Heap;

Heap *criar_heap(){
    Heap *heap = malloc(sizeof(Heap));
	heap->qtde = 0;
    return heap;
}

int filho_esq(int pai) { 
    int fe = 2*pai + 1;
    return fe;
}

int filho_dir(int pai) { 
    int fd = 2*pai + 2;
    return fd;
}

int pai(int filho) { 
    int p = (filho-1)/2;
    return p;
}

int ultimo_pai(Heap *h) { 
    int up = (h->qtde/2)-1;
    return up;
}

void peneirar(Heap *h, int pai) {
    int maior = pai;
    int fe = filho_esq(pai);
    if(fe<h->qtde){
        if(h->valores[fe]->Idade>h->valores[maior]->Idade){
            maior = fe;
        }
    }

    int fd = filho_dir(pai);
    if(fd<h->qtde){
        if(h->valores[fd]->Idade>h->valores[maior]->Idade){
            maior = fd;
        }
    }

    if(maior != pai){
        Registro *x = h->valores[pai];
        h->valores[pai] = h->valores[maior];
        h->valores[maior] = x;
        peneirar(h,maior);
    }
}

void construirHeap(Heap *h) {
    int x = ultimo_pai(h);
    for(int i = x;i>=0;i--){
        peneirar(h,i);
    }
}

void inserirHeap(Heap *h, Registro *info) {
    if (h->qtde >= tamHeap) {
        printf("Fila prioritária cheia!\n");
        return;
    }
    h->valores[h->qtde] = info;
    h->qtde++;
    construirHeap(h);
}

Registro *removerHeap(Heap *h) {
    if(h->qtde == 0){
        printf("\n");
        return NULL;
    }else{
        Registro *x = h->valores[0];
        h->valores[0] = h->valores[h->qtde-1];
        h->valores[h->qtde-1] = x;
        h->qtde--;
        construirHeap(h);
        return x;
    }
}

void exibirFilaPrioritaria(Heap *h) {
    printf("\n");

    if(h->qtde==0){
        printf("Fila vazia!\n");
    }else{
        printf("Exibindo fila: \n");
        for(int i = 0;i<h->qtde;i++){
            printf("Nome: %s, RG: %s, Idade: %d\n", h->valores[i]->Nome, h->valores[i]->RG, h->valores[i]->Idade);
            printf("Entrada: %d/%d/%d\n\n", h->valores[i]->Entrada->Dia, h->valores[i]->Entrada->Mes, h->valores[i]->Entrada->Ano);
        }
    }
}

void inserirFilaPrioritaria(LDE *lista, Heap *heap){
    char RG[tamRG];

    printf("Digite o RG a ser colocado na fila de atendimento: ");
    fgets(RG, tamRG, stdin);
    RG[strcspn(RG, "\n")] = 0;

    Registro *checaPaciente = acha_paciente_LDE(lista, RG);

    if(checaPaciente == NULL){
        printf("Paciente não encontrado!\n");
        return;
    }

    if(heap->qtde!=0){
        for(int i = 0;i<heap->qtde;i++){
            if(heap->valores[i]==checaPaciente){
                printf("Paciente já está na fila!\n");
                return;
            }            
        }
    }

    else{
        printf("Paciente enfileirado!\n");
        inserirHeap(heap, checaPaciente);
    }
}

void removerFilaPrioritaria(Heap *heap){
    Registro *atendePaciente = removerHeap(heap);

    if(atendePaciente == NULL){
        printf("Nenhum paciente na fila!\n");
        return;
    }else{
        printf("Proximo paciente a ser atendido: \n");
        printf("Nome: %s, RG: %s, Idade: %d\n", atendePaciente->Nome, atendePaciente->RG, atendePaciente->Idade);
        printf("Entrada: %d/%d/%d\n\n", atendePaciente->Entrada->Dia, atendePaciente->Entrada->Mes,atendePaciente->Entrada->Ano);
    }
}

void AtendimentoPrioritario(Heap *heap, LDE *lista){
    int escolha;
    do{
        printf("\n1 - Inserir paciente prioritario\n");
        printf("2 - Atender proximo paciente prioritario\n");
        printf("3 - Mostrar fila prioritaria\n");
        printf("0 - Sair\n");
        printf("Digite a opção: ");

        scanf("%d", &escolha);
        limpaBuffer();
        printf("\n");

        if(escolha==1){
            inserirFilaPrioritaria(lista,heap);
        }else if(escolha==2){
            removerFilaPrioritaria(heap);
        }else if(escolha==3){
            exibirFilaPrioritaria(heap);
        }else if(escolha==0){
            printf("Voltando ao menu!\n");
            break;
        }else{
            printf("Opção Invalida!\n");
        }
    }while(1);
}

void Pesquisa(ArvoreABB* arvoreabb){
    int escolha;
    if(arvoreabb->arvoreIdade->qtde==0){
        printf("Nenhum registro na procura %d\n", arvoreabb->arvoreIdade->qtde);
    }else{
        do{
            printf("\n1 - Mostrar registros ordenados por ano de registro\n");
            printf("2 - Mostrar registros ordenados por mes de registro\n");
            printf("3 - Mostrar registros ordenados por dia de registro\n");
            printf("4 - Mostrar registros ordenados por idade de registro\n");
            printf("0 - Sair\n");
            printf("Digite a opção: ");

            scanf("%d", &escolha);
            limpaBuffer();
            printf("\n");

            if(escolha==1){
                printf("Mostrando registros ordenados por ano de registro\n");
                in_ordem_abb(arvoreabb->arvoreAno->raiz);
            }else if(escolha==2){
                printf("Mostrando registros ordenados por mes de registro\n");
                in_ordem_abb(arvoreabb->arvoreMes->raiz);
            }else if(escolha==3){
                printf("Mostrando registros ordenados por dia de registro\n");
                in_ordem_abb(arvoreabb->arvoreDia->raiz);
            }else if(escolha==4){
                printf("Mostrando registros ordenados por idade de registro\n");
                in_ordem_abb(arvoreabb->arvoreIdade->raiz);
            }else if(escolha==0){
                printf("Voltando ao menu!\n");
                break;
            }else{
                printf("Opção Invalida!\n");
            }
        }while(1);
    }
}

void mostrarOperacoes(Pilha *pilha) {
    CellDesfazer *proximo = pilha->topo;
    if(pilha->qtde==0){
        printf("Nenhuma operação feita na fila de atendimento!\n");
    }else{
        printf("Exibindo operações feitas na fila de atendimento em ordem: \n\n");
        while(proximo != NULL){
            if(proximo->operacao==0){
                printf("Operação de desenfileirar o paciente: \n");
            }else{
                printf("Operação de enfileirar o paciente: \n");
            }
            printf("Nome: %s, RG: %s, Idade: %d\n", proximo->info->Nome, proximo->info->RG, proximo->info->Idade);
            printf("Entrada: %d/%d/%d\n\n", proximo->info->Entrada->Dia, proximo->info->Entrada->Mes,proximo->info->Entrada->Ano);
            proximo = proximo->proximo;
        }
    }
}

void Desfazer(Pilha *pilha, Fila *queue){
    int escolha;
    do{
        printf("\n1 - Mostrar operações\n");
        printf("2 - Desfazer ultima operação\n");
        printf("0 - Sair\n");
        printf("Digite a opção: ");

        scanf("%d", &escolha);
        limpaBuffer();
        printf("\n");

        if(escolha==1){
            mostrarOperacoes(pilha);
        }else if(escolha==2){
            desfazerOperacoes(pilha, queue);
        }else if(escolha==0){
            printf("Voltando ao menu!\n");
            break;
        }else{
            printf("Opção Invalida!\n");
        }
    }while(1);
}

void Salvar(LDE *lista) {
    FILE *f = fopen("lista.bin", "wb");
    if (f == NULL){
        printf("Erro ao abrir!\n");
        return;
    }

    
    int qtd = fwrite(&lista->qtde, sizeof(int), 1, f);
    if (qtd == 0){
        printf("Erro ao escrever!\n");
        return;
    }

    Celula *atual = lista->inicio;
    while (atual != NULL) {
        Registro *r = atual->info;

        fwrite(r->Nome, sizeof(char), tamNome, f);
        fwrite(r->RG, sizeof(char), tamRG, f);
        fwrite(&r->Idade, sizeof(int), 1, f);
        fwrite(&r->Entrada->Dia, sizeof(int), 1, f);
        fwrite(&r->Entrada->Mes, sizeof(int), 1, f);
        fwrite(&r->Entrada->Ano, sizeof(int), 1, f);

        atual = atual->proximo;
    }
    if (fclose(f)){
        printf("Erro ao fechar!\n");
        return;
    }
        
    printf("Lista salva!\n");
}

void Carregar(LDE *lista, ArvoreABB *arvoreabb) {
    FILE *f = fopen("lista.bin", "rb");
    if (f == NULL) {
        printf("Erro ao abrir!\n");
        return;
    }
    int tamanho;
    int qtd = fread(&tamanho, sizeof(int), 1, f);
    if(qtd==0){
        printf("Erro ao ler quantidade!\n");
        fclose(f);
        return;
    }
    
    lista->qtde = 0;
    lista->inicio = NULL;

    for (int i = 0; i < tamanho; i++) {
        Registro *info = malloc(sizeof(Registro));
        info->Entrada = malloc(sizeof(Data));

        fread(info->Nome, sizeof(char), tamNome, f);
        fread(info->RG, sizeof(char), tamRG, f);
        fread(&info->Idade, sizeof(int), 1, f);
        fread(&info->Entrada->Dia, sizeof(int), 1, f);
        fread(&info->Entrada->Mes, sizeof(int), 1, f);
        fread(&info->Entrada->Ano, sizeof(int), 1, f);

        inserir_paciente_LDE(lista, info);
        inserirProcurar(arvoreabb, info);
    }
    printf("Lista carregada!\n");
    fclose(f);
}

void SalvarCarregar(LDE *lista, ArvoreABB *arvoreabb){
    int escolha;
    do{
        printf("\n1 - Salvar\n");
        printf("2 - Carregar\n");
        printf("0 - Sair\n");
        printf("Digite a opção: ");

        scanf("%d", &escolha);
        limpaBuffer();
        printf("\n");

        if(escolha==1){
            Salvar(lista);
        }else if(escolha==2){
            Carregar(lista, arvoreabb);
        }else if(escolha==0){
            printf("Voltando ao menu!\n");
            break;
        }else{
            printf("Opção Invalida!\n");
        }
    }while(1);
}

void Sobre(){
    printf("\nCaio Panta Leão de Souza Fonseca\n");
        printf("R.A: 24.223.007-6\n");
        printf("4º Ciclo Vespertino \n");
        printf("Ciencias da Computação FEI\n");
        printf("CC4652 - ESTRUTURA DE DADOS\n");
        printf("21/05/2025\n\n");
}


int main(void) {
    LDE *lista = criar_lista();
    Fila *fila = criar_fila();
    Heap *heap = criar_heap();
    ArvoreABB *arvoreabb = cria_arvore_abb();
    Pilha *pilha = criar_pilha();

    

    int escolha;
    do{
        printf("\n1 - Cadastrar\n");
        printf("2 - Atendimento\n");
        printf("3 - Atendimento Prioritario\n");
        printf("4 - Pesquisa\n");
        printf("5 - Desfazer\n");
        printf("6 - Salvar|carregar lista\n");
        printf("7 - Sobre\n");
        printf("0 - Sair\n");
        printf("Digite a opção: ");
        scanf("%d", &escolha);
        limpaBuffer();

        if(escolha==1){
            Cadastrar(lista, arvoreabb);
        }else if(escolha==2){
            Atendimento(fila, lista, pilha);
        }else if(escolha==3){
            AtendimentoPrioritario(heap, lista);
        }else if(escolha==4){
            Pesquisa(arvoreabb);
        }else if(escolha==5){
            Desfazer(pilha, fila);
        }else if(escolha==6){
            SalvarCarregar(lista, arvoreabb);
        }else if(escolha==7){
            Sobre();
        }else if(escolha==0){
            printf("Saindo!\n");
            break;
        }else{
            printf("Opção Invalida!\n");
        }


    }while(1);
    return 0;
}