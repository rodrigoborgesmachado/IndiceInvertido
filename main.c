#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct documentos{
    char caminho[100];
    int indice;
} documentos;

struct stopwords{
    char palavra[1000];
    int indice;
};

struct palavras{
    char palavra[1000];
    int ehstopword;
};

///função que valida se a palavra está na lista de palavras para evitar colocar palavras repetidas
int verificaseexiste (struct palavras* a,int i, char *b){
	int j=0;
	for(;j<i;j++){
		if(!(strcmp(a[j].palavra, b))) return 1;
	}
	return 0;
}

int verificasepertence(struct stopwords *palavrasdesconsideradas,int linhasarquivosstopwords,char *temp){
    int j=0;
	for(;j<linhasarquivosstopwords;j++){

		if(!(strcmp(palavrasdesconsideradas[j].palavra, temp))) return 1;
	}
	return 0;
}


///TODO receber dois parâmetros na inicialização: caminho, caminhostopwords
int main(int argc, char *argv[ ])
{
    char c, caminho[100] = {"documentos.txt"};
    char caminhostopwords[100] = {"stopwords.txt"};
    strcpy(caminho, argv[1]);
    strcpy(caminhostopwords, argv[2]);
    printf("%s \n%s\n", caminho, caminhostopwords);
    system("pause");
    //printf("Digite o caminho do arquivo que contém os caminhos dos arquivos da base(sem espacos): ");
    //scanf("%s", caminho);
    FILE *file;
    int i = 0, j=0, numlinhadocumentos = 1;
    file = fopen(caminho, "r");
    ///conta quantas linhas há no arquivo lido.
    while( (c=fgetc(file))!= EOF )
        if(c == '\n')
		  numlinhadocumentos++;
    fclose(file);

    struct documentos docs[numlinhadocumentos];

    file = fopen(caminho, "r");
    ///parte que coloca no vetor de documentos todos os caminhos dos arquivos da base de dados e seu indice.
    char caminhododocumento;
    if(file == NULL){
        printf("Erro, nao foi possivel abrir o arquivo\n");
        return 0;
    }
    while( (caminhododocumento=fgetc(file))!= EOF ){
        if(caminhododocumento == '\n'){
            docs[i].caminho[j] = '\0';
            docs[i].indice = i;
            i++;
            j=0;
        }
        else{
            docs[i].caminho[j++] =caminhododocumento;
        }
    }
    docs[i].caminho[j] = '\0';
    docs[i].indice = i;
    j=0;
    fclose(file);


    int linhasarquivosstopwords=1;
    //printf("Digite o caminho do arquivo que contém as stop words(sem espacos): ");
    //scanf("%s", caminhostopwords);
    file = fopen(caminhostopwords, "r");
    ///conta quantas linhas há no arquivo lido.
    while( (c=fgetc(file))!= EOF )
        if(c == '\n' || c == ' ' || c == '.' || c == '!' || c == '?' || c == ';')
		  linhasarquivosstopwords++;
    fclose(file);
    struct stopwords palavrasdesconsideradas[linhasarquivosstopwords];
    file = fopen(caminhostopwords, "r");

    ///parte que coloca no vetor de palavras todos as stopwords e seus indices.
    if(file == NULL){
        printf("Erro, nao foi possivel abrir o arquivo\n");
        return 0;
    }
    i=j=0;
    while( (caminhododocumento=fgetc(file))!= EOF ){
        if(caminhododocumento == '\n' || caminhododocumento == ' ' || caminhododocumento == '.' || caminhododocumento == '!' || caminhododocumento == '?' || caminhododocumento == ';'){
            palavrasdesconsideradas[i].palavra[j] = '\0';
            palavrasdesconsideradas[i].indice = i;
            i++;
            j=0;
        }
        else{
            palavrasdesconsideradas[i].palavra[j++] =caminhododocumento;
        }
    }
    if(i<linhasarquivosstopwords){
        palavrasdesconsideradas[i].palavra[j] = '\0';
        palavrasdesconsideradas[i].indice = i;
    }

    fclose(file);



    ///ler todas as palavras de todos os documentos.
    int totaldearquivos = numlinhadocumentos, totaldepalavras=0;


    ///conta a quantidade de palavras existente em todos os documentos;
    for(i=0;i<totaldearquivos;i++)
    {
        file = fopen(docs[i].caminho, "r");
        while( (c=fgetc(file))!= EOF ){
            if(c == '\n' || c == ' ' || c == '.' || c == '!' || c == '?' || c == ';'){
                totaldepalavras++;
            }
        }
        fclose(file);
    }
    totaldepalavras+=totaldearquivos; ///Necessário porque ele não soma a quantidade quando chega ao final do arquivo

    struct palavras palavrasdosdocumentos[totaldepalavras];
    ///Armazena todas as palavras na lista de palavras
    int quantidadetotalpalavrasdiferentes=0;
    for(i=0;i<totaldearquivos;i++)
    {
        j=0;
        char temp[1000];
        file = fopen(docs[i].caminho, "r");

        while( (c=fgetc(file))!= EOF ){
            if(c == '\n' || c == ' ' || c == '.' || c == '!' || c == '?' || c == ';'){
                temp[j]='\0';
                if(!verificasepertence(palavrasdesconsideradas, linhasarquivosstopwords, temp)){
                    if(!verificaseexiste(palavrasdosdocumentos, quantidadetotalpalavrasdiferentes, temp)){
                        strcpy(palavrasdosdocumentos[quantidadetotalpalavrasdiferentes].palavra, temp);
                        palavrasdosdocumentos[quantidadetotalpalavrasdiferentes].ehstopword = 0;
                        quantidadetotalpalavrasdiferentes++;
                    }
                }
                j=0;
            }
            else{
                temp[j++]=c;
            }
        }
        temp[j]='\0';
        if(!verificasepertence(palavrasdesconsideradas, linhasarquivosstopwords, temp)){
            if(!verificaseexiste(palavrasdosdocumentos, i, palavrasdosdocumentos[i].palavra)){
                palavrasdosdocumentos[i].ehstopword = 0;
                strcpy(palavrasdosdocumentos[i].palavra, temp);
            }
        }
        fclose(file);
    }


    ///Montar a tabela com as repeticoes das palavras por arquivo.
    int matriz[numlinhadocumentos][quantidadetotalpalavrasdiferentes];
    for(i=0;i<numlinhadocumentos;i++){
        for(j=0;j<quantidadetotalpalavrasdiferentes;j++){
            matriz[i][j]=0;
            char temp[1000];
            file = fopen(docs[i].caminho, "r");
            int z=0;
            while( (c=fgetc(file))!= EOF ){
                if(c == '\n' || c == ' ' || c == '.' || c == '!' || c == '?' || c == ';'){
                    temp[z]='\0';
                    if(!strcmp(palavrasdosdocumentos[j].palavra, temp)){
                        matriz[i][j]++;
                    }
                    z=0;
                }
                else{
                    temp[z++]=c;
                }
            }
            fclose(file);
        }
    }

//    ///arquivos de base de dados:
//    printf("Base de dados: \n");
//    for(i=0;i<numlinhadocumentos;i++) printf("%s\n", docs[i].caminho);
//    printf("\n\n");
//    ///palavras diferentes lidas:
//    printf("Palavras: \n");
//    for(i=0;i < quantidadetotalpalavrasdiferentes;i++)printf("%s\n", palavrasdosdocumentos[i].palavra);
//    printf("\n\n");
//
//    for(i=0;i<numlinhadocumentos;i++){
//        for(j=0;j<quantidadetotalpalavrasdiferentes;j++){
//            printf("%d ", matriz[i][j]);
//        }
//        printf("\n");
//    }
    remove("index.txt");
    file = fopen("index.txt", "w");

    for(j=0;j<quantidadetotalpalavrasdiferentes;j++){
        fprintf(file, "%s: ",palavrasdosdocumentos[j].palavra);
        for(i=0;i<numlinhadocumentos;i++){
            if(matriz[i][j])
                fprintf(file, "%d,%d ", i+1, matriz[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);

    return 0;
}
