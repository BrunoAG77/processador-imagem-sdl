# Projeto 1: Processamento de Imagens

Documentação da implementação completa do projeto desenvolvido para processar imagens utilizando a linguagem *C* ou *C++* e a biblioteca *Simple DirectMedia Layer* *(SDL)*.

---

# Parte 1: Apresentação

## Objetivo

Desenvolver um software de processamento de imagens em linguagem *C* ou *C++*, usando a biblioteca *SDL (Simple DirectMedia Layer)* em sua versão 3.

## Escopo e Funcionalidades

O programa deverá ser executado via linha de comando, recebendo como argumento o caminho para um arquivo de imagem, como demonstrado no exemplo abaixo:

```makefile
programa caminho_da_imagem.ext
```

## Funcionalidades

O programa contém as funcionalidades descritas a seguir.

### Carregamento de Imagem

🟢 O programa deve ser capaz de carregar imagens nos formatos mais comuns, como *PNG, JPG* e *BMP*, usando a biblioteca *SDL_image*.

🟢 Deve tratar possíveis problemas, como arquivo não encontrado ou arquivo que não seja um formato de imagem válido.

### Análise e Conversão Para Escala de Cinza

🟢 O programa deve implementar uma função que verifica se a imagem carregada já está em escala de cinza ou se é colorida.

🟢 Caso a imagem seja colorida, o programa deve converter a imagem para escala de cinza usando a seguinte fórmula: $Y = 0.2125 \times R + 0.7154 \times G + 0.0721 \times B$.

🟢 A imagem em escala de cinza deve ser usada como base e/ou entrada para as operações seguintes.

### Interface Gráfica de Usuário (GUI) Com Duas Janelas

🟢 Janela Principal: deve exibir a imagem que está sendo processada. O tamanho da janela deve se adaptar ao tamanho da imagem carregada e deve iniciar centralizada no monitor principal.

🟢 Janela Secundária: deve exibir uma janela de tamanho fixo, posicionada ao lado da janela principal, exibindo o histograma da imagem e um botão de operação.

### Análise e Exibição do Histograma

🟢 Na janela secundária, o programa deve exibir o histograma da imagem.

🟢 O programa deve analisar o histograma e exibir as seguintes informações (usar a biblioteca *SDL_ttf*):

- Média de Intensidade: classificar a imagem como "clara", "média” ou “escura”;
- Desvio Padrão: classificar o contraste da imagem como “alto", "médio” ou “baixo".

### Equalização do Histograma

🟢 Na janela secundária, deve haver um botão desenhado com as primitivas da *SDL.*

🟢 Ao clicar no botão, o programa deve equalizar o histograma da imagem, atualizando a imagem exibida na janela principal e o histograma na janela secundária.

🟢 Ao clicar novamente no botão, o programa deve revertes a iamgem para a sua versão original em escala de cinza, sendo que o conteúdo das janelas deve ser atualizado de acordo.

🟢 O texto do botão deve mudar para refletir a ação — como “Equalizar” e "Ver Original”.

🟢 O estado do botão deve refletir as ações do usuário — cor azul para estado “neutro”, cor azul claro para estado “mouse em cima do botão” e cor azul escuro para estado “botão clicado”.

### Salvar Imagem

🟢 Ao pressionar a tecla *S* do teclado, o programa deve salvar a imagem atualmente exibida na janela principal em um arquivo chamado `output_image.png`.

🟢 Caso o arquivo `output_image.png` já exista, o programa deve sobrescrever o arquivo.

## Requisitos Técnicos

- **Linguagem:** *C* ou *C++*;
- **Bibliotecas Obrigatórias:** *SDL3* e *SDL_image*;
- **Qualidade de Código:** O código deve ser organizado em estruturas e funções com responsabilidades claras. O gerenciamento de memória deve ser feito corretamente;
- **Repositório:** O projeto (código-fonte e documentação), deve ser disponibilizado em um repositório público do *GitHub* ou serviço similar.

## Critérios de avaliação

Carregamento de Imagem

| Status | **Item de Avaliação** | Pontuação Máxima | Parcial | Ausente |
| --- | --- | --- | --- | --- |
| 🟢 | Carrega a imagem e realiza tratamento de erros corretamente. | 0,50 | 0,25 | 0,00 |

Análise de Conversão Para Escala de Cinza

| Status | **Item de Avaliação** | Pontuação Máxima | Parcial | Ausente |
| --- | --- | --- | --- | --- |
| 🟢 | Detecta corretamente se a imagem é colorida e converte para escala de cinza usando a fórmula indicada no enunciado. | 1,00 | 0,50 | 0,00 |

Interface Gráfica de Usuário (GUI)

| Status | **Item de Avaliação** | Pontuação Máxima | Parcial | Ausente |
| --- | --- | --- | --- | --- |
| 🟢 | A janela principal exibe a imagem que está sendo processada, se adapta ao tamanho da imagem e começa centralizada no monitor principal. | 0,50 | 0,25 | 0,00 |
| 🟢 | A janela secundária é criada como filha da janela principal, com tamanho, posição e conteúdo de acordo com o enunciado. | 0,50 | 0,25 | 0,00 |

Análise e Exibição do Histograma

| Status | **Item de Avaliação** | Pontuação Máxima | Parcial | Ausente |
| --- | --- | --- | --- | --- |
| 🟢 | O histograma da imagem atual é calculado corretamente e exibido de forma clara e proporcional na janela secundária. | 1,25 | 0,65 | 0,00 |
| 🟢 | As informações de análise do histograma são exibidas corretamente.  | 1,25 | 0,60 | 0,00 |

Equalização do Histograma

| Status | **Item de Avaliação** | Pontuação Máxima | Parcial | Ausente |
| --- | --- | --- | --- | --- |
| 🟢 | O histograma da imagem é equalizado corretamente.  | 1,50 | 0,75 | 0,00 |
| 🟢 | O programa consegue reverter para a imagem original (em escala de cinza), sem recarregar a imagem. | 1,00 | 0,50 | 0,00 |
| 🟢 | O botão de equalizar/voltar ao original na janela secundária é construído e exibido corretamente.  | 0,50 | 0,25 | 0,00 |

Salvar Imagem

| Status | **Item de Avaliação** | Pontuação Máxima | Parcial | Ausente |
| --- | --- | --- | --- | --- |
| 🟢 | O programa salva a imagem corretamente, de acordo com o enunciado. | 0,50 | 0,25 | 0,00 |

Repositório e Documentação

| Status | **Item de Avaliação** | Pontuação Máxima | Parcial | Ausente |
| --- | --- | --- | --- | --- |
| 🟢 | Arquivo `README.md` estruturado e com explicações detalhadas. | 1,00 | 0,50 | 0,00 |

Participação

| Status | **Item de Avaliação** | Pontuação Máxima | Parcial | Ausente |
| --- | --- | --- | --- | --- |
| 🟢 | Repositório com atualizações significativas e constantes (isto é, não há somente um ou dois commits enormes contendo praticamente todo o projeto pronto), com a participação de todos os integrantes do grupo.  | 0,50 | 0,25 | 0,00 |
		

---

# Parte 2: Implementação

Confira a seguir os detalhes da implementação das etapas descritas acima.

## Bibliotecas

Assim como definido nos critérios do projeto, foi utilizado bibliotecas da *SDL versão 3*, com o apoio das bibliotecas *stdio*, *stdlib*, *stdbool*, *string* e *math* para reproduzir os comportamentos esperados em cada etapa.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
```

---

## Definições e Estruturas

### Definições

```c
#define WINDOW_TITLE      "Processador de Imagem"

#define SEC_WINDOW_WIDTH  320

#define SEC_WINDOW_HEIGHT 420

#define FONT_PATH "/System/Library/Fonts/Helvetica.ttc" 
```

### Estruturas

```c
typedef enum {
	
	NORMAL = 0,
	EM_CIMA = 1,	
	PRESSIONADO = 2

} estadosDoBotao;
```

---

## Carregamento da Imagem

Iniciando no carregamento da imagem, garantimos a princípio que o programa aceite somente um único arquivo nas extensões *JPG, JPEG, PNG* ou *BMP* como entrada e, tendo essas propriedades devidamente validadas, prossegue tentando abrir o arquivo; se existir, abre e prossegue para as etapas seguintes e senão, retorna uma mensagem de erro e encerra o programa.

No trecho abaixo, validamos se o usuário inseriu o arquivo ao executar o arquivo compilado:

```c
if (argc < 2) {

    printf("O programa deve ser compilado como:\nprograma caminho_da_imagem.ext");
    
		return 1;
    
 }
```

Se o usuário inseriu o arquivo, então validamos se a extensão está de acordo com o que foi definido anteriormente. Confira no trecho a seguir:

```c
onst char *ext_arquivo = argv[1];

    const int validacaoDoArquivo = validarExtensao(ext_arquivo);

    if (validacaoDoArquivo == -1) {

        printf("Nao possui extensao no caminho.\n");
        
        return 1;
    
    } else if (validacaoDoArquivo == -2) {
    
        printf("Este tipo de extensao nao e aceita!\n");
        
        return 1;

    } else {

        printf("Extensao aceita.\n");

    }
```

```c
int validarExtensao(const char* nomeDoArquivo) {
    
    char *extensaoDoArquivo = strrchr(nomeDoArquivo, '.');

    int resultado;
    
    if (extensaoDoArquivo == NULL) {

        resultado = -1;

    } else if (strcmp(extensaoDoArquivo, ".jpg")  == 0 ||
               strcmp(extensaoDoArquivo, ".jpeg") == 0 ||
               strcmp(extensaoDoArquivo, ".png")  == 0 ||
               strcmp(extensaoDoArquivo, ".bmp")  == 0) {
    
                resultado = 0;
    
    } else if (strcmp(extensaoDoArquivo, ".jpg")  != 0 ||
               strcmp(extensaoDoArquivo, ".jpeg") != 0 ||
               strcmp(extensaoDoArquivo, ".png")  != 0 ||
               strcmp(extensaoDoArquivo, ".bmp")  != 0 ) {
    
                resultado = -2;

    }

    return resultado;
}
```

## Análise e Conversão Para Escala de Cinza

Após validar a extensão do arquivo, o programa tenta abrir e analisá-lo para definir se a imagem inserida pelo usuário é colorida ou não; se for, converte para a escala de cinza utilizando a fórmula:

$$
Y = 0.2125 \times R + 0.7154 \times G + 0.0721 \times B
$$

Para fazer isso, utilizamos a função *verificacaoDaImagem*:

```c

int verificacaoDaImagem(const char* nomeDoArquivo) {

    SDL_Surface *imagem = IMG_Load(nomeDoArquivo);
    int resultado = 0;
    
    if (!imagem) {

        resultado = -1;

    }
    
    SDL_Surface *imagemConvertida = SDL_ConvertSurface(imagem, SDL_PIXELFORMAT_RGB24);
    
    SDL_DestroySurface(imagem);
    
    if (!imagemConvertida) {
        
        resultado = -2;
    
    }
    
    Uint8 *pixels = (Uint8 *)imagemConvertida->pixels;
    
    for (int i = 0; i < imagemConvertida->w * imagemConvertida->h; i++) {
    
        if (pixels[i*3] != pixels[i*3+1] || pixels[i*3+1] != pixels[i*3+2]) {
    
            resultado = -3;
    
            break;
    
        }
    
    }
    
    SDL_DestroySurface(imagemConvertida);
    
    return resultado;

}
```

Passo a passo, o algoritmo da função segue:

1. Tenta carregar a imagem como uma imagem de arquivo de disco com a função *IMG_Load* e assim converter a imagem com a estrutura *SDL_Surface* para melhor manipulação de *pixels*;
2. Valida se foi possível realizar a operação na imagem, retornando -1 se não;
3. A seguir, utiliza a função *SDL_ConvertSurface* para um formato de pixels padrão para facilitar e blindar o código. Retorna uma nova imagem com *SDL_Surface*;
4. Como a imagem convertida para o novo padrão de *pixels* é a que será utilizada adiante, a imagem convertida com a estrutura *SDL_Surface* anteriormente é destruída e/ou liberada com os seus recursos associados ao utilizar a função *SDL_DestroySurface;*
5. Valida se foi possível realizar a operação na imagem convertida, retornando -2 se não;
6. Acessa o ponteiro para a memória de pixels de uma *SDL_Surface* e guarda na variável do tipo “ponteiro para byte”, para acessar os bytes manualmente;
7. Acessa por meio de uma estrutura de repetição *for* cada *pixel* da imagem para validar se a imagem está em escala de cinza ou não.
    
    A condição `pixels[i3] != pixels[i3+1] || pixels[i3+1] != pixels[i3+2]` valida se os pixels estão em escala de cinza, isto é, R = G = B, ou não, R ≠ G ≠ B. Cada pixel ocupa 3 bytes e por isso é utilizado com frequência na condição.
    
    Se a condição for verdadeira, assume-se que a imagem é colorida e portanto, precisa ser convertida a seguir.
    
8. Destrói e/ou libera os recursos associados para a imagem convertida com *SDL_DestroySurface;*
9. Retorna o resultado.

Se a imagem não está em escala de cinza, então utilizamos a função *converterImagemParaEscalaDeCinza* e a equação $Y$ para converter a imagem:

```c
DL_Surface* converterImagemParaEscalaDeCinza(const char* imagemColorida) {

    SDL_Surface *imagem = IMG_Load(imagemColorida);
    
    if (!imagem) { 
        
        SDL_Log("Erro ao carregar imagem: %s", SDL_GetError()); 
        
        return NULL;

    }
    
    SDL_Surface *imagemConvertida = SDL_ConvertSurface(imagem, SDL_PIXELFORMAT_RGB24);
    
    SDL_DestroySurface(imagem);
    
    if (!imagemConvertida) {

        return NULL;

    }
    
    Uint8 *pixels = (Uint8 *)imagemConvertida->pixels;

    Uint8 cinza = 0;
    
    for (int i = 0; i < imagemConvertida->w * imagemConvertida->h; i++) {
    
        cinza = (Uint8)(0.2125f * pixels[i*3] + 0.7154f * pixels[i*3+1] + 0.0721f * pixels[i*3+2]);
        
        pixels[i*3] = pixels[i*3+1] = pixels[i*3+2] = cinza;
    
    }
    
    return imagemConvertida;

}
```

---

## Análise do Histograma

Iniciando pelo cálculo do histograma, utilizamos as funções *calcularMediaDoHistograma* e *calcularDesvioPadraoDoHistograma* para calcular a média e o desvio padrão, respectivamente.

```c
float calcularMediaDoHistograma(int *histograma, int total) {

    float media = 0.0f;
    
    for (int i = 0; i < 256; i++) {
        
        media += i * histograma[i];
    
    }
    
    media /= total;

    return media;

}
```

```c

float calcularDesvioPadraoDoHistograma(int *histograma, float media, int total) {
    
    float var = 0.0f;
    
    for (int i = 0; i < 256; i++) {
        
        var += histograma[i] * (i - media) * (i - media);
    
    }
    
    float std = sqrtf(var / total);
    
    return std;
}
```

Uma vez que a média foi calculada, avaliamos o resultado para definir se a imagem é escura, média ou clara:

```c
float media = calcularMediaDoHistograma(histograma, total);

    printf("Media: %.4f\n", media);
    
    if (media < 85) {

        printf("Imagem escura\n");

    } else if (media < 170) {

        printf("Imagem media\n");

    } else {

        printf("Imagem clara\n");

    }
```

De maneira similar, fazemos o mesmo para definir se o contraste da imagem é baixo, médio ou alto após calcular o desvio padrão:

```c
float desvioPadrao = calcularDesvioPadraoDoHistograma(histograma, media, total);

    printf("\nDesvio padrao: %.4f\n", desvioPadrao);
    
    if (desvioPadrao < 30) {

        printf("Constraste baixo\n");
    
    } else if (desvioPadrao < 70) {

        printf("Contraste medio\n");
    
    }  else {

        printf("Contraste alto\n");
        
    }
```

---

## Equalização do Histograma

Para equalizar o histograma, redistribuímos os níveis de cinza (0 a 255) para usar melhor o intervalo inteiro e aumentar o contraste principalmente quando a imagem está concentrada em poucos tons. Com isso, seguimos o algoritmo:

1. Calcular o total de pixels da imagem cinza original;
2. Cria um ponteiro para os pixels da imagem cinza original;
3. Distribui o histograma de 0 a 255;
4. Calcular a função de distribuição acumulada (CDF);
5. Calcular o primeiro valor acumulado que não é zero para evitar que o menor tom presente mapeie para um valor maior que 0 e para que o preto seja mantido, conforme o possível;
6. Criar uma tabela de conversão para determinar qual vai ser o novo tom de cinza para cada tom antigo, de modo a considerar:
    1. Se um pixel não tem uma grande variação de tom, mantém a mesma intensidade;
    2. Caso contrário, transforme o tom de cinza anterior em um novo tom;
7. Reescrever os resultados obtidos na tabela com uma cópia da superfície.

Cada etapa descrita foi implementada da seguinte maneira:

**Tópico 1:**

```c
int total = imagemCinzaOriginal->w * imagemCinzaOriginal->h;
```

**Tópico 2:**

```c
Uint8 *pixelsDeOrigem = (Uint8 *)imagemCinzaOriginal->pixels;
```

**Tópico 3:**

```c
 int histograma[256] = {0};

    for (int i = 0; i < total; i++) {

        histograma[pixelsDeOrigem[i * 3]]++;
    
    }
```

**Tópico 4:**

```c
int cdf[256] = {0};
    
    cdf[0] = histograma[0];
    
    for (int i = 1; i < 256; i++) {

        cdf[i] = cdf[i-1] + histograma[i];

    }
```

**Tópico 5:**

```c
int primeiroValorAcumulado = 0;
    
    for (int i = 0; i < 256; i++) {

        if (cdf[i] > 0) { 
            
            primeiroValorAcumulado = cdf[i];
            
            break;
        
        }
    
    }
```

**Tópico 6:**

```c
  Uint8 tabelaDeConversao[256];
    
    for (int v = 0; v < 256; v++) {
    
        if (total - primeiroValorAcumulado == 0) {
            tabelaDeConversao[v] = v;
        
        } else {

            tabelaDeConversao[v] = (Uint8)roundf(((float)(cdf[v] - primeiroValorAcumulado) / (total - primeiroValorAcumulado)) * 255.0f);

        }
    
    }

```

**Tópico 7:**

```c
    SDL_Surface *imagemEqualizada = SDL_DuplicateSurface(imagemCinzaOriginal);

    if (!imagemEqualizada) {

        return NULL
        
    }

    Uint8 *pixelsDistribuidos = (Uint8 *)imagemEqualizada->pixels;
    Uint8 pixelNovo = 0;
    
    for (int i = 0; i < total; i++) {
    
        pixelNovo = tabelaDeConversao[pixelsDeOrigem[i * 3]];
    
        pixelsDistribuidos[i*3] = pixelsDistribuidos[i*3+1] = pixelsDistribuidos[i*3+2] = pixelNovo;
    
    }
```

- **Função Completa**
    
    ```c
    SDL_Surface* equalizarHistograma(SDL_Surface *imagemCinzaOriginal) {
    
        int total = imagemCinzaOriginal->w * imagemCinzaOriginal->h;
    
        Uint8 *pixelsDeOrigem = (Uint8 *)imagemCinzaOriginal->pixels;
    
        int histograma[256] = {0};
    
        for (int i = 0; i < total; i++) {
    
            histograma[pixelsDeOrigem[i * 3]]++;
        
        }
    
        int cdf[256] = {0};
        
        cdf[0] = histograma[0];
        
        for (int i = 1; i < 256; i++) {
    
            cdf[i] = cdf[i-1] + histograma[i];
    
        }
    
        int primeiroValorAcumulado = 0;
        
        for (int i = 0; i < 256; i++) {
    
            if (cdf[i] > 0) { 
                
                primeiroValorAcumulado = cdf[i];
                
                break;
            
            }
        
        }
    
        Uint8 tabelaDeConversao[256];
        
        for (int v = 0; v < 256; v++) {
        
            if (total - primeiroValorAcumulado == 0) {
                tabelaDeConversao[v] = v;
            
            } else {
    
                tabelaDeConversao[v] = (Uint8)roundf(((float)(cdf[v] - primeiroValorAcumulado) / (total - primeiroValorAcumulado)) * 255.0f);
    
            }
        
        }
    
        SDL_Surface *imagemEqualizada = SDL_DuplicateSurface(imagemCinzaOriginal);
    
        if (!imagemEqualizada) {
    
            return NULL;
            
        } 
    
        Uint8 *pixelsDistribuidos = (Uint8 *)imagemEqualizada->pixels;
        Uint8 pixelNovo = 0;
        
        for (int i = 0; i < total; i++) {
        
            pixelNovo = tabelaDeConversao[pixelsDeOrigem[i * 3]];
        
            pixelsDistribuidos[i*3] = pixelsDistribuidos[i*3+1] = pixelsDistribuidos[i*3+2] = pixelNovo;
        
        }
        
        return imagemEqualizada;
    
    }
    ```
    

---

## Exibição do Histograma

Para exibir o histograma, utilizamos a função *exibirHistograma*, que desenha a visualização do histograma para um vetor.

```c
void exibirHistograma(SDL_Renderer *secRenderer, int histograma[]) {
    
    int max = 0;

    for (int i = 0; i < 256; i++) {
        
        if (histograma[i] > max) {
            
            max = histograma[i];

        }
    }

    SDL_FRect areaDoHistograma = { 10, 10, SEC_WINDOW_WIDTH - 20, SEC_WINDOW_HEIGHT - 80 };

    SDL_SetRenderDrawColor(secRenderer, 255, 255, 255, 255);
    
    SDL_RenderRect(secRenderer, &areaDoHistograma);

    float largura = areaDoHistograma.w / 256.0f;
    
    SDL_SetRenderDrawColor(secRenderer, 255, 0, 255, 255);
    
    for (int i = 0; i < 256; i++) {
    
        float altura = (max > 0) ? ((float)histograma[i] / max) * areaDoHistograma.h : 0;
    
        SDL_FRect coluna = {
            areaDoHistograma.x + i * largura,
            areaDoHistograma.y + areaDoHistograma.h - altura,
            largura,
            altura
        };
    
        SDL_RenderFillRect(secRenderer, &coluna);
    
    }

}
```

Primeiro, percorre os 256 valores para descobrir qual é a maior frequência (max), e usa esse valor para normalizar as alturas das barras, garantindo que a maior coluna ocupe toda a altura disponível e as demais fiquem proporcionais. 

Em seguida, ela define uma área retangular na tela onde o gráfico será exibido, desenha a moldura dessa área e calcula a largura de cada coluna (dividindo a largura total por 256). 

Por fim, para cada índice de 0 a 255, calcula a altura correspondente, posiciona a barra a partir da base do retângulo e desenha um retângulo preenchido, resultando num gráfico de barras que representa visualmente a distribuição dos valores do histograma.

---

## Salvar Imagem

Para salvar a imagem, utilizamos a função *salvarImagem* ao pressionar CTRL+S no teclado. Como foi estabelecido o critério de sempre salvá-la em extensão *PNG*, todas as imagens salvas seguem essa extensão.

```c

int salvarImagem(SDL_Renderer *renderer) {

    SDL_Surface *surface = SDL_RenderReadPixels(renderer, NULL);
    int resultado = 0;
    
    if (!surface) {
        
        resultado = -1;
    }
    
    if (IMG_SavePNG(surface, "./out/output_image.png")) {

        printf("Imagem salva como 'out/output_image.png'.\n");

        resultado = 0;
    
    } else {

        SDL_Log("Erro ao salvar PNG: %s", SDL_GetError());
        
        resultado = -1;
    }
    
    SDL_DestroySurface(surface);

    return resultado;

}
```

**Observação:** como as imagens possuem um nome já definido (`output_image.png`), toda imagem será sobreescrita ao tentar salvar.
