// Bruno Antico Galin RA: 10417318
// Ismael de Sousa Silva RA: 10410870
// Lucas Kenzo Kawamoto: 10396359
// Vitor Alves Pereira: 10410862
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

//------------------------------------------------------------------------------
#define WINDOW_TITLE     "Processador de Imagem"
#define SEC_WINDOW_WIDTH  320
#define SEC_WINDOW_HEIGHT 420

#define MAX_WINDOW_WIDTH  1280
#define MAX_WINDOW_HEIGHT 720

//------------------------------------------------------------------------------
void shutdown(void)
{
    SDL_Log("shutdown()");
    SDL_Quit();
}

//------------------------------------------------------------------------------
void ValidarExtensao(const char* nomearquivo) {
    char copia[256];
    strncpy(copia, nomearquivo, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';

    char *ext = strrchr(copia, '.');
    if (ext == NULL) {
        printf("Nao possui extensao no caminho.\n");
        exit(1);
    }

    if (strcmp(ext, ".jpg")  == 0 ||
        strcmp(ext, ".jpeg") == 0 ||
        strcmp(ext, ".png")  == 0 ||
        strcmp(ext, ".bmp")  == 0) {
        printf("Extensao aceita.\n");
    } else {
        printf("Este tipo de extensao nao e aceita!\n");
        exit(1);
    }
}

//------------------------------------------------------------------------------
bool VerificaImagem(const char* image) {
    SDL_Surface *img = IMG_Load(image);
    if (!img) return false;

    SDL_Surface *conv = SDL_ConvertSurface(img, SDL_PIXELFORMAT_RGB24);
    SDL_DestroySurface(img);
    if (!conv) return false;

    bool is_grayscale = true;
    Uint8 *pixels = (Uint8 *)conv->pixels;
    for (int i = 0; i < conv->w * conv->h; i++) {
        Uint8 r = pixels[i * 3 + 0];
        Uint8 g = pixels[i * 3 + 1];
        Uint8 b = pixels[i * 3 + 2];
        if (r != g || g != b) {
            is_grayscale = false;
            break;
        }
    }
    SDL_DestroySurface(conv);
    return is_grayscale;
}

//------------------------------------------------------------------------------
SDL_Surface* ConvertImageScaleGray(const char* image) {
    SDL_Surface *img = IMG_Load(image);
    if (!img) {
        SDL_Log("Erro ao carregar imagem: %s", SDL_GetError());
        return NULL;
    }

    SDL_Surface *conv = SDL_ConvertSurface(img, SDL_PIXELFORMAT_RGB24);
    SDL_DestroySurface(img);
    if (!conv) return NULL;

    Uint8 *pixels = (Uint8 *)conv->pixels;
    for (int i = 0; i < conv->w * conv->h; i++) {
        Uint8 r = pixels[i * 3 + 0];
        Uint8 g = pixels[i * 3 + 1];
        Uint8 b = pixels[i * 3 + 2];
        Uint8 gray = (Uint8)(0.2125f * r + 0.7154f * g + 0.0721f * b);
        pixels[i * 3 + 0] = gray;
        pixels[i * 3 + 1] = gray;
        pixels[i * 3 + 2] = gray;
    }
    return conv;
}

//------------------------------------------------------------------------------
// Parte 6: Salva a imagem exibida como output_image.png
void SalvarImagem(SDL_Renderer *renderer) {
    // SDL3: SDL_RenderReadPixels retorna SDL_Surface* diretamente (sem argumentos extras)
    SDL_Surface *surf = SDL_RenderReadPixels(renderer, NULL);
    if (!surf) {
        SDL_Log("Erro ao capturar pixels: %s", SDL_GetError());
        return;
    }

    if (IMG_SavePNG(surf, "output_image.png")) {
        printf("Imagem salva como 'output_image.png'.\n");
    } else {
        SDL_Log("Erro ao salvar PNG: %s", SDL_GetError());
    }

    SDL_DestroySurface(surf);
}

//------------------------------------------------------------------------------
// Parte 4: Análise e exibição do histograma (Média)
float calcularMediaHist(int *hist, int total){
    float mean = 0.0; // Média
    for (int i = 0; i < 256; i++) mean += i * hist[i];
    mean = mean/total;

    printf("Media: %.4f\n", mean);

    if (mean < 85) printf("Imagem escura\n");
    else if (mean < 170) printf("Imagem media\n");
    else printf("Imagem clara\n");

    return mean;
}

// Parte 4: Análise e exibição do histograma (Desvio Padrão)
float calcularStdHist(int *hist, float mean, int total){
    float var = 0.0; // Variância
    for (int i = 0; i < 256; i++) var += hist[i] * (i - mean) * (i - mean);
    float std = sqrt(var/total); // Desvio padrão é a raiz quadrada da variância

    printf("\nDesvio padrao: %.4f\n",std);

    if (std < 30) printf("Constraste baixo\n");
    else if (std < 70) printf("Contraste medio\n");
    else printf("Contraste alto\n");

    return std;
}

// Parte 4: Análise e exibição do histograma (Exibição)
void exibeHistograma(SDL_Renderer *secRenderer, int hist[]){
    int max = 0;
    for (int i = 0; i < 256; i++){
      if (hist[i] > max) max = hist[i];
    }

    SDL_FRect areaHistograma = { 10, 10, SEC_WINDOW_WIDTH - 20, SEC_WINDOW_HEIGHT - 80 };
    SDL_RenderRect(secRenderer, &areaHistograma);
    float largura = areaHistograma.w/256;
    SDL_SetRenderDrawColor(secRenderer,255,0,255,255);

    for (int i = 0; i < 256; i++){
      float altura = 0;
      if (max > 0) altura = ((float)hist[i]/max) * areaHistograma.h;
      
      SDL_FRect column = { // Design das colunas do histograma
        areaHistograma.x + i * largura,
        areaHistograma.y + areaHistograma.h - altura,
        largura,
        altura,
      };

      SDL_RenderFillRect(secRenderer,&column);
    }
}

//------------------------------------------------------------------------------
// Parte 3: Renderiza a janela secundária com área de histograma e botão
void RenderizarJanelaSecundaria(SDL_Renderer *secRenderer, int hist[]) {
    SDL_SetRenderDrawColor(secRenderer, 40, 40, 40, 255);
    SDL_RenderClear(secRenderer);

    // Área reservada para o histograma
    exibeHistograma(secRenderer, hist);

    // Botão "Equalizar" — azul (estado neutro)
    SDL_FRect botao = { 10, SEC_WINDOW_HEIGHT - 60, SEC_WINDOW_WIDTH - 20, 50 };
    SDL_SetRenderDrawColor(secRenderer, 30, 100, 200, 255);
    SDL_RenderFillRect(secRenderer, &botao);

    // Borda branca do botão
    SDL_SetRenderDrawColor(secRenderer, 255, 255, 255, 255);
    SDL_RenderRect(secRenderer, &botao);

    SDL_RenderPresent(secRenderer);
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    if (argc < 2){
      printf("O programa deve ser compilado como: \nprograma caminho_da_imagem.ext");
      return 1;
    }
    const char *ext_arquivo = argv[1];
    ValidarExtensao(ext_arquivo);

    bool imagemCinza = VerificaImagem(ext_arquivo);
    printf("Imagem %s em escala de cinza.\n", imagemCinza ? "ja esta" : "nao esta");

    atexit(shutdown);

    // SDL3: IMG_Init/IMG_Quit foram REMOVIDOS — SDL_Init ja cuida de tudo
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Erro ao iniciar a SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Surface *graySurface = ConvertImageScaleGray(ext_arquivo);

    if (!graySurface) {
        SDL_Log("Falha ao converter imagem para cinza.");
        SDL_Quit();
        return 1;
    }

    int imgW = graySurface->w;
    int imgH = graySurface->h;
    int total = imgW * imgH;

    int histograma[256] = {0};
    Uint8* pixels = (Uint8*) graySurface->pixels;

    for (int i = 0; i < total; i++){
      Uint8 gray = pixels[i*3]; // Valor de intensidade R=G=B
      histograma[gray]++; // incrementa a intensidade
    }

    float mean = calcularMediaHist(histograma, total);
    calcularStdHist(histograma, mean, total);

    // Centralizar janela principal no monitor
    SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *dm = SDL_GetCurrentDisplayMode(displayID);
    int screenW = dm ? dm->w : 1920;
    int screenH = dm ? dm->h : 1080;

    // Limita o tamanho da janela principal ao máximo definido
    int winW = (imgW > MAX_WINDOW_WIDTH)  ? MAX_WINDOW_WIDTH  : imgW;
    int winH = (imgH > MAX_WINDOW_HEIGHT) ? MAX_WINDOW_HEIGHT : imgH;

    int mainX = (screenW - winW) / 2;
    int mainY = (screenH - winH) / 2;

    // --- Janela Principal (tamanho = imagem, centralizada) ---
    SDL_Window *mainWindow = SDL_CreateWindow(WINDOW_TITLE, winW, winH, 0);
    if (!mainWindow) {
        SDL_Log("Erro ao criar janela principal: %s", SDL_GetError());
        return 1;
    }
    SDL_SetWindowPosition(mainWindow, mainX, mainY);

    // SDL3: SDL_CreateRenderer nao recebe flags numericas
    SDL_Renderer *mainRenderer = SDL_CreateRenderer(mainWindow, NULL);
    if (!mainRenderer) {
        SDL_Log("Erro ao criar renderer principal: %s", SDL_GetError());
        return 1;
    }

    // --- Janela Secundária (ao lado direito da principal, filha dela) ---
    int secX = mainX + imgW + 10;
    int secY = mainY;

    SDL_Window *secWindow = SDL_CreateWindow("Histograma", SEC_WINDOW_WIDTH, SEC_WINDOW_HEIGHT, 0);
    if (!secWindow) {
        SDL_Log("Erro ao criar janela secundaria: %s", SDL_GetError());
        return 1;
    }
    SDL_SetWindowPosition(secWindow, secX, secY);
    SDL_SetWindowParent(secWindow, mainWindow);

    SDL_Renderer *secRenderer = SDL_CreateRenderer(secWindow, NULL);
    if (!secRenderer) {
        SDL_Log("Erro ao criar renderer secundario: %s", SDL_GetError());
        return 1;
    }

    SDL_Texture *imgTexture = SDL_CreateTextureFromSurface(mainRenderer, graySurface);
    if (!imgTexture) {
        SDL_Log("Erro ao criar textura: %s", SDL_GetError());
        return 1;
    }

    // Fit proporcional: escala a imagem para caber na janela sem distorcer
    float scale = SDL_min((float)winW / imgW, (float)winH / imgH);
    float dstW  = imgW * scale;
    float dstH  = imgH * scale;
    float dstX  = ((float)winW - dstW) / 2.0f;
    float dstY  = ((float)winH - dstH) / 2.0f;

    SDL_FRect imgRect = { dstX, dstY, dstW, dstH };

    // --- Loop principal ---
    SDL_Event event;
    bool isRunning = true;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    isRunning = false;
                    break;

                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    isRunning = false;
                    break;

                case SDL_EVENT_KEY_DOWN: {
                    // SDL3: event.key.key (nao mais event.key.keysym.sym)
                    // SDLK_S captura tanto 's' quanto 'S'
                    if (event.key.key == SDLK_S) {
                        printf("Salvando imagem...\n");
                        SalvarImagem(mainRenderer);
                    }
                    break;
                }

                case SDL_EVENT_MOUSE_MOTION: {
                    // Bloco {} necessario para declarar variavel dentro do case
                    char windowTitle[64] = { 0 };
                    snprintf(windowTitle, sizeof(windowTitle),
                             "%s (%.0f, %.0f)", WINDOW_TITLE,
                             event.motion.x, event.motion.y);
                    SDL_SetWindowTitle(mainWindow, windowTitle);
                    break;
                }

                default:
                    break;
            }
        }

        // Renderizar janela principal
        SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mainRenderer);
        SDL_RenderTexture(mainRenderer, imgTexture, NULL, &imgRect);
        SDL_RenderPresent(mainRenderer);

        // Renderizar janela secundária
        RenderizarJanelaSecundaria(secRenderer, histograma);
    }

    // --- Limpeza ---
    SDL_DestroyTexture(imgTexture);
    SDL_DestroySurface(graySurface);
    SDL_DestroyRenderer(secRenderer);
    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(secWindow);
    SDL_DestroyWindow(mainWindow);

    return 0;
}
