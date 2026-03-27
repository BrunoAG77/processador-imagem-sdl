/** 
 *  @name    Projeto 1: Processador de Imagem
 *  @brief   Implementação completa do projeto desenvolvido para processar imagens utilizando a linguagem C ou C++ e a biblioteca SDL3.
 *  @date    26-04-2025
 *  @authors Bruno Antico Galin (10417318); Ismael de Sousa Silva (10410870), Lucas Kenzo Kawamoto (10396359), Vitor Alves Pereira (10410862).
 */

#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <string.h>

#include <math.h>

#include <SDL3/SDL.h>

#include <SDL3/SDL_main.h>

#include <SDL3_image/SDL_image.h>   // utilize <SDL3/SDL_image.h> para executar no Windows

#include <SDL3_ttf/SDL_ttf.h>


#define WINDOW_TITLE "Processador de Imagem"

#define SEC_WINDOW_WIDTH  320

#define SEC_WINDOW_HEIGHT 420

#define FONT_PATH "/System/Library/Fonts/Helvetica.ttc" // utilize "C:/Windows/Fonts/arial.ttf" para executar no Windows


typedef enum {
    NORMAL = 0,
    EM_CIMA = 1,
    PRESSIONADO = 2
} estadosDoBotao;

/** 
 *  @brief Função utilitária para limpar recursos da SDL e encerrar o programa.
 *  @details Utilitária, não faz parte de nenhuma etapa específica.
 *  @param void Sem parâmetros.
 *  @return void
 */
void shutdown();

/**
 *  @brief Função utilizada para validar se a extensão do arquivo é suportada pelo programa.
 *  @details As extensões aceitas são: .jpg, .jpeg, .png e .bmp. 
 *  @param nomeDoArquivo Contém o nome do arquivo e extensão.
 *  @return retorn 0 se a extensão for válida, -1 se não houver extensão, -2 se a extensão não for aceita.
 */
int validarExtensao(const char* nomeDoArquivo);

/** 
 *  @brief Função utilizada para criar uma cópia do arquivo.
 *  @details Utilizada para evitar modificações no arquivo original durante o processamento.
 *  @param nomeDoArquivo Contém o nome do arquivo e extensão.
 *  @return Retorna uma cópia do nome do arquivo, ou seja, o caminho.
 */
char copiarArquivo(const char* nomeDoArquivo);

/**
 *  @brief Função utilizada para verificar se a imagem está em escala de cinza.
 *  @details Utilizada na etapa 2 para determinar se a imagem já está em escala de cinza ou se precisa ser convertida.
 *  @param nomeDoArquivo Contém o nome do arquivo e extensão.
 *  @return Retorna 1 se a imagem já estiver em escala de cinza, ou 0 caso contrário.
 */
int verificacaoDaImagem(const char* nomeDoArquivo);

/** 
 *  @brief Função utilizada para converter uma imagem colorida para escala de cinza.
 *  @details Utiliza a função de conversão de cor para cinza utilizando a fórmula: Gray = 0.2125 * R + 0.7154 * G + 0.0721 * B.
 *  @param image Contém o nome do arquivo e extensão da imagem a ser convertida.
 *  @return
 */
SDL_Surface* converterImagemParaEscalaDeCinza(const char* imagemColorida);

/**
 *  @brief Função utilizada para calcular a média do histograma de uma imagem.
 *  @details Função utilizada na etapa 4 para calcular a média do histograma e determinar se a imagem é clara, média ou escura.
 *  @param histograma Contém o histograma da imagem.
 *  @param total Contém o total de pixels da imagem.
 *  @return Retorna o valor da média do histograma.
 */
float calcularMediaDoHistograma(int *histograma, int total);

/**
 * @brief Função utilizada para calcular o desvio padrão do histograma de uma imagem.
 * @details Função utilizada na etapa 4 para calcular o desvio padrão do histograma
 * e determinar se a imagem tem contraste baixo, médio ou alto.
 * @param histograma Contém o histograma da imagem.
 * @param media Contém a média do histograma da imagem.
 * @param total Contém o total de pixels da imagem.
 * @return Retorna o valor do desvio padrão do histograma. 
 */
float calcularDesvioPadraoDoHistograma(int *histograma, float media, int total);

/**
 * @brief Função utilizada para equalizar o histograma de uma imagem em escala de cinza. 
 * @param imagemCinzaOriginal Contém a superfície da imagem em escala de cinza a ser equalizada.
 * @return Retorna uma nova superfície com o histograma equalizado, ou NULL em caso de falha.
 */
SDL_Surface* equalizarHistograma(SDL_Surface *imagemCinzaOriginal);

/** 
 *  @brief Função para exibir o histograma em uma janela.
 *  @param secRenderer Contém o renderer da janela secundária onde o histograma será exibido.
 * @param histograma Contém o histograma da imagem a ser exibido.
 * @return void
 */
void exibirHistograma(SDL_Renderer *secRenderer, int histograma[]);

/**
 *  @brief Função para renderizar a janela secundária.
 *  @param secRenderer Contém o renderer da janela secundária.
 *  @param hist Contém o histograma da imagem.
 *  @param estadoDoBotao Contém o estado do botão.
 *  @param equalizado Contém o status de equalização da imagem.
 *  @param font Contém a fonte utilizada para exibir texto.
 *  @return void
 */
void renderizarJanelaSecundaria(SDL_Renderer *secRenderer, int histograma[], estadosDoBotao estadoDoBotao, int equalizado, TTF_Font *font);

/**
 *  @brief Função para verificar se o mouse está sobre o botão na janela secundária.
 *  @param x Contém a coordenada x do mouse.
 *  @param y Contém a coordenada y do mouse.
 *  @return Retorna 1 se o mouse estiver sobre o botão, ou 0 caso contrário.
 */
int trackingDoBotao(float x, float y);

/**
 *  @brief Função para salvar a imagem.
 *  @param renderer Contém o renderer da janela principal.
 *  @return Retorna 0 em caso de sucesso, ou -1 em caso de falha.
 */
int salvarImagem(SDL_Renderer *renderer);

// --------------------------------------------------------------------------------------------------------------------------------------------- //

int main(int argc, char *argv[]) {

    if (argc < 2) {

        printf("O programa deve ser compilado como:\nprograma caminho_da_imagem.ext");
    
        return 1;
    
    }

    const char *extensaoDoArquivo = argv[1];

    const int validacaoDoArquivo = validarExtensao(extensaoDoArquivo);

    if (validacaoDoArquivo == -1) {

        printf("Nao possui extensao no caminho.\n");
        
        return 1;
    
    } else if (validacaoDoArquivo == -2) {
    
        printf("Este tipo de extensao nao e aceita!\n");
        
        return 1;

    } else {

        printf("Extensao aceita.\n");

    }

    int imagemEstaEmEscalaDeCinza = verificacaoDaImagem(extensaoDoArquivo);

    SDL_Surface *superficieCinza = NULL;

    if (imagemEstaEmEscalaDeCinza == -1) {

        printf("Falha ao carregar a imagem.\n");
        
        return 1;
    
    } else if (imagemEstaEmEscalaDeCinza == -2) {
    
        printf("Falha ao converter a imagem para RGB24.\n");
        
        return 1;

    } else if (imagemEstaEmEscalaDeCinza == -3) {
    
        printf("A imagem nao esta em escala de cinza.\n");

        printf("Convertendo para escala de cinza...\n");

        superficieCinza = converterImagemParaEscalaDeCinza(extensaoDoArquivo);

        if (!superficieCinza) { 
            
            SDL_Log("Falha ao converter imagem.");
            
            return 1;
        
        }
        
    }
    
    printf("A imagem esta em escala de cinza.\n");

    atexit(shutdown);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
    
        SDL_Log("Erro ao iniciar a SDL: %s", SDL_GetError());
        
        return 1;
    
    }

    if (!TTF_Init()) {
    
        SDL_Log("Erro ao iniciar SDL_ttf: %s", SDL_GetError());
    
        return 1;
    
    }

    TTF_Font *font = TTF_OpenFont(FONT_PATH, 18);
    
    if (!font) {
    
        SDL_Log("Erro ao carregar fonte: %s — ajuste FONT_PATH no codigo.", 
        
        SDL_GetError());
        
        return 1;
    
    }

    int imgW = superficieCinza->w;

    int imgH = superficieCinza->h;
    
    int total = imgW * imgH;

    int histograma[256] = {0};

    Uint8 *pixelsCinzas = (Uint8 *)superficieCinza->pixels;

    for (int i = 0; i < total; i++) {
        
        histograma[pixelsCinzas[i*3]]++;
    
    }

    float media = calcularMediaDoHistograma(histograma, total);

    printf("Media: %.4f\n", media);
    
    if (media < 85) {

        printf("Imagem escura\n");

    } else if (media < 170) {

        printf("Imagem media\n");

    } else {

        printf("Imagem clara\n");

    }

    float desvioPadrao = calcularDesvioPadraoDoHistograma(histograma, media, total);

    printf("\nDesvio padrao: %.4f\n", desvioPadrao);
    
    if (desvioPadrao < 30) {

        printf("Constraste baixo\n");
    
    } else if (desvioPadrao < 70) {

        printf("Contraste medio\n");
    
    }  else {

        printf("Contraste alto\n");
        
    }

    SDL_Surface *superficieEqualizada = equalizarHistograma(superficieCinza);

    if (!superficieEqualizada) { 
        
        SDL_Log("Falha ao equalizar."); 
        
        return 1;
    
    }

    int histogramaEqualizado[256] = {0};
    
    Uint8 *eqPixels = (Uint8 *)superficieEqualizada->pixels;
    
    for (int i = 0; i < total; i++) {
        
        histogramaEqualizado[eqPixels[i*3]]++;

    }

    
    SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    
    const SDL_DisplayMode *dm = SDL_GetCurrentDisplayMode(displayID);
    
    int screenW = dm ? dm->w : 1920;
    
    int screenH = dm ? dm->h : 1080;

    SDL_Window *mainWindow = SDL_CreateWindow(WINDOW_TITLE, imgW, imgH, 0);
    
    if (!mainWindow) { 
        
        SDL_Log("Erro janela principal: %s", SDL_GetError()); 
        
        return 1; 
    
    }
    
    SDL_SetWindowPosition(mainWindow, (screenW - imgW) / 2, (screenH - imgH) / 2);

    SDL_Renderer *mainRenderer = SDL_CreateRenderer(mainWindow, NULL);
    
    if (!mainRenderer) { 
        
        SDL_Log("Erro renderer principal: %s", SDL_GetError()); 
        
        return 1;
    
    }

    int secX = (screenW - imgW) / 2 + imgW + 10;
    
    int secY = (screenH - imgH) / 2;
    
    SDL_Window *secWindow = SDL_CreateWindow("Histograma", SEC_WINDOW_WIDTH, 
    SEC_WINDOW_HEIGHT, 0);
    
    if (!secWindow) {
    
        SDL_Log("Erro janela sec: %s", SDL_GetError()); 
        
        return 1;
    
    }
    
    SDL_SetWindowPosition(secWindow, secX, secY);
    
    SDL_SetWindowParent(secWindow, mainWindow);

    SDL_Renderer *secRenderer = SDL_CreateRenderer(secWindow, NULL);
    
    if (!secRenderer) { 
        
        SDL_Log("Erro renderer sec: %s", SDL_GetError()); 
        
        return 1;
    
    }

    SDL_Texture *imgTexture = SDL_CreateTextureFromSurface(mainRenderer, superficieCinza);

    if (!imgTexture) { 
        
        SDL_Log("Erro textura: %s", SDL_GetError()); 
        
        return 1;
    
    }

    SDL_FRect imgRect = { 0.0f, 0.0f, (float)imgW, (float)imgH };

    bool equalizado = false;
    
    estadosDoBotao estadoDoBotao = NORMAL;
    
    SDL_WindowID secWindowID = SDL_GetWindowID(secWindow);

    SDL_Event event;

    bool isRunning = true;

    while (isRunning) {

        while (SDL_PollEvent(&event)) {

            switch (event.type) {

                case SDL_EVENT_QUIT:

                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:

                    isRunning = false;
    
                    break;

                case SDL_EVENT_KEY_DOWN:
    
                    if (event.key.key == SDLK_S) {
        
                        printf("Salvando imagem...\n");

                        int resultado;
        
                        resultado = salvarImagem(mainRenderer);
        
                        if (resultado == 0) {
        
                            printf("Imagem salva com sucesso.\n");
        
                        } else {
        
                            printf("Falha ao salvar a imagem.\n");
        
                        }

                    }
        
                    break;

                case SDL_EVENT_MOUSE_MOTION: {
        
                    if (event.motion.windowID == secWindowID) {
        
                        if (estadoDoBotao != PRESSIONADO) {
        
                            estadoDoBotao = trackingDoBotao(event.motion.x, event.motion.y) ? EM_CIMA : NORMAL;
        
                        }
                    
                    } else {
                        
                        char titulo[64];
                        
                        snprintf(titulo, sizeof(titulo), "%s (%.0f, %.0f)",
                                 WINDOW_TITLE, event.motion.x, event.motion.y);
                        
                        SDL_SetWindowTitle(mainWindow, titulo);
                    
                    }
                    
                    break;
                
                }

                case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                
                    if (event.button.windowID == secWindowID &&
                        event.button.button == SDL_BUTTON_LEFT &&
                        trackingDoBotao(event.button.x, event.button.y)) {

                        estadoDoBotao = PRESSIONADO;

                    }

                    break;

                }

                case SDL_EVENT_MOUSE_BUTTON_UP: {
                    
                    if (event.button.windowID == secWindowID &&
                        event.button.button == SDL_BUTTON_LEFT) {

                        int clicouNoBotao = (estadoDoBotao == PRESSIONADO) &&
                                             trackingDoBotao(event.button.x, event.button.y);

                        if (clicouNoBotao) {

                            equalizado = !equalizado;
                            
                            SDL_DestroyTexture(imgTexture);
                            
                            SDL_Surface *surfAtual = equalizado ? superficieEqualizada : superficieCinza;
                            imgTexture = SDL_CreateTextureFromSurface(mainRenderer, surfAtual);

                            printf("Imagem %s.\n", equalizado ? "equalizada" : "revertida para cinza");
                        }

                        estadoDoBotao = trackingDoBotao(event.button.x, event.button.y)
                                   ? EM_CIMA : NORMAL;
                    }
                    break;
                }

                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);

        SDL_RenderClear(mainRenderer);

        SDL_RenderTexture(mainRenderer, imgTexture, NULL, &imgRect);

        SDL_RenderPresent(mainRenderer);

        int *histogramaAtual = equalizado ? histogramaEqualizado : histograma;
        renderizarJanelaSecundaria(secRenderer, histogramaAtual, estadoDoBotao, equalizado, font);
    }

    
    SDL_DestroyTexture(imgTexture);
    
    SDL_DestroySurface(superficieEqualizada);
    
    SDL_DestroySurface(superficieCinza);
    
    SDL_DestroyRenderer(secRenderer);
    
    SDL_DestroyRenderer(mainRenderer);
    
    SDL_DestroyWindow(secWindow);
    
    SDL_DestroyWindow(mainWindow);
    
    TTF_CloseFont(font);
    
    return 0;

}

// --------------------------------------------------------------------------------------------------------------------------------------------- //

void shutdown() {

    SDL_Log("shutdown()");
    
    TTF_Quit();
    
    SDL_Quit();

}


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


int verificacaoDaImagem(const char* nomeDoArquivo) {

    SDL_Surface *imagem = IMG_Load(nomeDoArquivo);
    
    if (!imagem) {

        return -1;

    }
    
    SDL_Surface *imagemConvertida = SDL_ConvertSurface(imagem, SDL_PIXELFORMAT_RGB24);
    
    SDL_DestroySurface(imagem);
    
    if (!imagemConvertida) {
        
        return -2;
    
    }
    
    Uint8 *pixels = (Uint8 *)imagemConvertida->pixels;
    
    for (int i = 0; i < imagemConvertida->w * imagemConvertida->h; i++) {
    
        if (pixels[i*3] != pixels[i*3+1] || pixels[i*3+1] != pixels[i*3+2]) {
    
            return -3;
    
        }
    
    }
    
    SDL_DestroySurface(imagemConvertida);
    
    return 0;

}


SDL_Surface* converterImagemParaEscalaDeCinza(const char* imagemColorida) {

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


float calcularMediaDoHistograma(int *histograma, int total) {

    float media = 0.0f;
    
    for (int i = 0; i < 256; i++) {
        
        media += i * histograma[i];
    
    }
    
    media /= total;

    return media;

}


float calcularDesvioPadraoDoHistograma(int *histograma, float media, int total) {
    
    float var = 0.0f;
    
    for (int i = 0; i < 256; i++) {
        
        var += histograma[i] * (i - media) * (i - media);
    
    }
    
    float std = sqrtf(var / total);
    
    return std;
}


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


void renderizarJanelaSecundaria(SDL_Renderer *secRenderer, int histograma[], estadosDoBotao estadoDoBotao, int equalizado, TTF_Font *font) {

    SDL_SetRenderDrawColor(secRenderer, 40, 40, 40, 255);
    
    SDL_RenderClear(secRenderer);

    exibirHistograma(secRenderer, histograma);

    // Cores do botão conforme estado
    SDL_FRect botao = { 10, SEC_WINDOW_HEIGHT - 60, SEC_WINDOW_WIDTH - 20, 50 };

    switch (estadoDoBotao) {
        case EM_CIMA:
            
            SDL_SetRenderDrawColor(secRenderer, 80, 160, 255, 255);  // azul claro
            
            break;
        
        case PRESSIONADO:
        
            SDL_SetRenderDrawColor(secRenderer, 10,  50, 120, 255);  // azul escuro
            
            break;
        
        default:
        
            SDL_SetRenderDrawColor(secRenderer, 30, 100, 200, 255);  // azul neutro
            
            break;
    
    }
    
    SDL_RenderFillRect(secRenderer, &botao);

    // Borda branca
    SDL_SetRenderDrawColor(secRenderer, 255, 255, 255, 255);
    
    SDL_RenderRect(secRenderer, &botao);

    const char *texto = ((equalizado == 1) ? "Ver original" : "Equalizar");

    SDL_Color branco = {255, 255, 255, 255};

    SDL_Surface *textSurf = TTF_RenderText_Blended(font, texto, 0, branco);

    if (textSurf) {

        SDL_Texture *textTex = SDL_CreateTextureFromSurface(secRenderer, textSurf);

        if (textTex) {

            float tw, th;

            SDL_GetTextureSize(textTex, &tw, &th);

            // Posicionamento do texto do botão no centro
            SDL_FRect textRect = {
                botao.x + (botao.w - tw) / 2.0f,
                botao.y + (botao.h - th) / 2.0f,
                tw, th
            };

            SDL_RenderTexture(secRenderer, textTex, NULL, &textRect);
            
            SDL_DestroyTexture(textTex);
        }

        SDL_DestroySurface(textSurf);
        
    }

    SDL_RenderPresent(secRenderer);

}


int trackingDoBotao(float x, float y) {
    return (x >= 10 && x <= SEC_WINDOW_WIDTH - 10 &&
            y >= SEC_WINDOW_HEIGHT - 60 && y <= SEC_WINDOW_HEIGHT - 10);
}


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
