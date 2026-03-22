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
#include <SDL3_image/SDL_image.h>   // Mac/Linux; no Windows: <SDL3/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>       // para texto no botão

// Compilar no Mac:
// clang main.c -o app $(pkg-config --cflags --libs sdl3 sdl3-image sdl3-ttf)
//------------------------------------------------------------------------------
#define WINDOW_TITLE      "Processador de Imagem"
#define SEC_WINDOW_WIDTH  320
#define SEC_WINDOW_HEIGHT 420

// Caminho para uma fonte TTF — ajuste conforme seu sistema
#define FONT_PATH "/System/Library/Fonts/Helvetica.ttc"  // Mac
// Linux: "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"
// Windows: "C:/Windows/Fonts/arial.ttf"

//------------------------------------------------------------------------------
// Estado do botão
typedef enum {
    BTN_NORMAL  = 0,
    BTN_HOVER   = 1,
    BTN_PRESSED = 2
} ButtonState;

//------------------------------------------------------------------------------
void shutdown(void) {
    SDL_Log("shutdown()");
    TTF_Quit();
    SDL_Quit();
}

//------------------------------------------------------------------------------
void ValidarExtensao(const char* nomearquivo) {
    char copia[256];
    strncpy(copia, nomearquivo, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';
    char *ext = strrchr(copia, '.');
    if (ext == NULL) { printf("Nao possui extensao no caminho.\n"); exit(1); }
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
        if (pixels[i*3] != pixels[i*3+1] || pixels[i*3+1] != pixels[i*3+2]) {
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
    if (!img) { SDL_Log("Erro ao carregar imagem: %s", SDL_GetError()); return NULL; }
    SDL_Surface *conv = SDL_ConvertSurface(img, SDL_PIXELFORMAT_RGB24);
    SDL_DestroySurface(img);
    if (!conv) return NULL;
    Uint8 *pixels = (Uint8 *)conv->pixels;
    for (int i = 0; i < conv->w * conv->h; i++) {
        Uint8 gray = (Uint8)(0.2125f * pixels[i*3] + 0.7154f * pixels[i*3+1] + 0.0721f * pixels[i*3+2]);
        pixels[i*3] = pixels[i*3+1] = pixels[i*3+2] = gray;
    }
    return conv;
}

//------------------------------------------------------------------------------
// Etapa 5: Equalização de histograma
SDL_Surface* EqualizeHistogram(SDL_Surface *grayOriginal) {
    int total = grayOriginal->w * grayOriginal->h;
    Uint8 *srcPixels = (Uint8 *)grayOriginal->pixels;

    int hist[256] = {0};
    for (int i = 0; i < total; i++)
        hist[srcPixels[i * 3]]++;

    int cdf[256] = {0};
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++)
        cdf[i] = cdf[i-1] + hist[i];

    int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) { cdf_min = cdf[i]; break; }
    }

    Uint8 lut[256];
    for (int v = 0; v < 256; v++) {
        if (total - cdf_min == 0)
            lut[v] = v;
        else
            lut[v] = (Uint8)roundf(((float)(cdf[v] - cdf_min) / (total - cdf_min)) * 255.0f);
    }

    SDL_Surface *eq = SDL_DuplicateSurface(grayOriginal);
    if (!eq) return NULL;
    Uint8 *dstPixels = (Uint8 *)eq->pixels;
    for (int i = 0; i < total; i++) {
        Uint8 novo = lut[srcPixels[i * 3]];
        dstPixels[i*3] = dstPixels[i*3+1] = dstPixels[i*3+2] = novo;
    }
    return eq;
}

//------------------------------------------------------------------------------
void SalvarImagem(SDL_Renderer *renderer) {
    SDL_Surface *surf = SDL_RenderReadPixels(renderer, NULL);
    if (!surf) { SDL_Log("Erro ao capturar pixels: %s", SDL_GetError()); return; }
    if (IMG_SavePNG(surf, "output_image.png"))
        printf("Imagem salva como 'output_image.png'.\n");
    else
        SDL_Log("Erro ao salvar PNG: %s", SDL_GetError());
    SDL_DestroySurface(surf);
}

//------------------------------------------------------------------------------
float calcularMediaHist(int *hist, int total) {
    float mean = 0.0f;
    for (int i = 0; i < 256; i++) mean += i * hist[i];
    mean /= total;
    printf("Media: %.4f\n", mean);
    if (mean < 85)       printf("Imagem escura\n");
    else if (mean < 170) printf("Imagem media\n");
    else                 printf("Imagem clara\n");
    return mean;
}

float calcularStdHist(int *hist, float mean, int total) {
    float var = 0.0f;
    for (int i = 0; i < 256; i++) var += hist[i] * (i - mean) * (i - mean);
    float std = sqrtf(var / total);
    printf("\nDesvio padrao: %.4f\n", std);
    if (std < 30)       printf("Constraste baixo\n");
    else if (std < 70)  printf("Contraste medio\n");
    else                printf("Contraste alto\n");
    return std;
}

void exibeHistograma(SDL_Renderer *secRenderer, int hist[]) {
    int max = 0;
    for (int i = 0; i < 256; i++) if (hist[i] > max) max = hist[i];

    SDL_FRect areaHistograma = { 10, 10, SEC_WINDOW_WIDTH - 20, SEC_WINDOW_HEIGHT - 80 };
    SDL_SetRenderDrawColor(secRenderer, 255, 255, 255, 255);
    SDL_RenderRect(secRenderer, &areaHistograma);

    float largura = areaHistograma.w / 256.0f;
    SDL_SetRenderDrawColor(secRenderer, 255, 0, 255, 255);
    for (int i = 0; i < 256; i++) {
        float altura = (max > 0) ? ((float)hist[i] / max) * areaHistograma.h : 0;
        SDL_FRect column = {
            areaHistograma.x + i * largura,
            areaHistograma.y + areaHistograma.h - altura,
            largura,
            altura
        };
        SDL_RenderFillRect(secRenderer, &column);
    }
}

//------------------------------------------------------------------------------
// Etapa 5: Renderiza botão com estado visual e texto dinâmico
void RenderizarJanelaSecundaria(SDL_Renderer *secRenderer, int hist[],
                                 ButtonState btnState, bool equalizado,
                                 TTF_Font *font) {
    SDL_SetRenderDrawColor(secRenderer, 40, 40, 40, 255);
    SDL_RenderClear(secRenderer);

    exibeHistograma(secRenderer, hist);

    // Cores do botão conforme estado
    SDL_FRect botao = { 10, SEC_WINDOW_HEIGHT - 60, SEC_WINDOW_WIDTH - 20, 50 };

    switch (btnState) {
        case BTN_HOVER:
            SDL_SetRenderDrawColor(secRenderer, 80, 160, 255, 255);  // azul claro
            break;
        case BTN_PRESSED:
            SDL_SetRenderDrawColor(secRenderer, 10,  50, 120, 255);  // azul escuro
            break;
        default: // BTN_NORMAL
            SDL_SetRenderDrawColor(secRenderer, 30, 100, 200, 255);  // azul neutro
            break;
    }
    SDL_RenderFillRect(secRenderer, &botao);

    // Borda branca
    SDL_SetRenderDrawColor(secRenderer, 255, 255, 255, 255);
    SDL_RenderRect(secRenderer, &botao);

    // Texto do botão
    const char *texto = equalizado ? "Ver original" : "Equalizar";
    SDL_Color branco = {255, 255, 255, 255};
    SDL_Surface *textSurf = TTF_RenderText_Blended(font, texto, 0, branco);
    if (textSurf) {
        SDL_Texture *textTex = SDL_CreateTextureFromSurface(secRenderer, textSurf);
        if (textTex) {
            float tw, th;
            SDL_GetTextureSize(textTex, &tw, &th);
            // Centraliza o texto no botão
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

//------------------------------------------------------------------------------
// Verifica se um ponto (x,y) da janela sec está dentro do botão
bool PontoDentroBotao(float x, float y) {
    return (x >= 10 && x <= SEC_WINDOW_WIDTH - 10 &&
            y >= SEC_WINDOW_HEIGHT - 60 && y <= SEC_WINDOW_HEIGHT - 10);
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("O programa deve ser compilado como:\nprograma caminho_da_imagem.ext");
        return 1;
    }

    const char *ext_arquivo = argv[1];
    ValidarExtensao(ext_arquivo);
    bool imagemCinza = VerificaImagem(ext_arquivo);
    printf("Imagem %s em escala de cinza.\n", imagemCinza ? "ja esta" : "nao esta");

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
        SDL_Log("Erro ao carregar fonte: %s — ajuste FONT_PATH no codigo.", SDL_GetError());
        return 1;
    }

    // Superfície cinza original (nunca modificada)
    SDL_Surface *graySurface = ConvertImageScaleGray(ext_arquivo);
    if (!graySurface) { SDL_Log("Falha ao converter imagem."); return 1; }

    int imgW = graySurface->w;
    int imgH = graySurface->h;
    int total = imgW * imgH;

    // Histograma inicial (escala de cinza)
    int histograma[256] = {0};
    Uint8 *grayPixels = (Uint8 *)graySurface->pixels;
    for (int i = 0; i < total; i++) histograma[grayPixels[i*3]]++;

    float mean = calcularMediaHist(histograma, total);
    calcularStdHist(histograma, mean, total);

    // Superfície equalizada (gerada uma vez, reutilizada)
    SDL_Surface *eqSurface = EqualizeHistogram(graySurface);
    if (!eqSurface) { SDL_Log("Falha ao equalizar."); return 1; }

    // Histograma equalizado
    int histogramaEq[256] = {0};
    Uint8 *eqPixels = (Uint8 *)eqSurface->pixels;
    for (int i = 0; i < total; i++) histogramaEq[eqPixels[i*3]]++;

    // --- Janelas ---
    SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *dm = SDL_GetCurrentDisplayMode(displayID);
    int screenW = dm ? dm->w : 1920;
    int screenH = dm ? dm->h : 1080;

    SDL_Window *mainWindow = SDL_CreateWindow(WINDOW_TITLE, imgW, imgH, 0);
    if (!mainWindow) { SDL_Log("Erro janela principal: %s", SDL_GetError()); return 1; }
    SDL_SetWindowPosition(mainWindow, (screenW - imgW) / 2, (screenH - imgH) / 2);

    SDL_Renderer *mainRenderer = SDL_CreateRenderer(mainWindow, NULL);
    if (!mainRenderer) { SDL_Log("Erro renderer principal: %s", SDL_GetError()); return 1; }

    int secX = (screenW - imgW) / 2 + imgW + 10;
    int secY = (screenH - imgH) / 2;
    SDL_Window *secWindow = SDL_CreateWindow("Histograma", SEC_WINDOW_WIDTH, SEC_WINDOW_HEIGHT, 0);
    if (!secWindow) { SDL_Log("Erro janela sec: %s", SDL_GetError()); return 1; }
    SDL_SetWindowPosition(secWindow, secX, secY);
    SDL_SetWindowParent(secWindow, mainWindow);

    SDL_Renderer *secRenderer = SDL_CreateRenderer(secWindow, NULL);
    if (!secRenderer) { SDL_Log("Erro renderer sec: %s", SDL_GetError()); return 1; }

    // Textura inicial = cinza original
    SDL_Texture *imgTexture = SDL_CreateTextureFromSurface(mainRenderer, graySurface);
    if (!imgTexture) { SDL_Log("Erro textura: %s", SDL_GetError()); return 1; }

    SDL_FRect imgRect = { 0.0f, 0.0f, (float)imgW, (float)imgH };

    // --- Estado Etapa 5 ---
    bool      equalizado = false;
    ButtonState btnState = BTN_NORMAL;
    SDL_WindowID secWindowID = SDL_GetWindowID(secWindow);

    // --- Loop principal ---
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
                        SalvarImagem(mainRenderer);
                    }
                    break;

                case SDL_EVENT_MOUSE_MOTION: {
                    if (event.motion.windowID == secWindowID) {
                        // Atualiza estado hover do botão (apenas se não estiver pressionado)
                        if (btnState != BTN_PRESSED) {
                            btnState = PontoDentroBotao(event.motion.x, event.motion.y)
                                       ? BTN_HOVER : BTN_NORMAL;
                        }
                    } else {
                        // Janela principal: exibe coordenadas no título
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
                        PontoDentroBotao(event.button.x, event.button.y)) {
                        btnState = BTN_PRESSED;
                    }
                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_UP: {
                    if (event.button.windowID == secWindowID &&
                        event.button.button == SDL_BUTTON_LEFT) {

                        bool clicouNoBotao = (btnState == BTN_PRESSED) &&
                                             PontoDentroBotao(event.button.x, event.button.y);

                        if (clicouNoBotao) {
                            equalizado = !equalizado;

                            // Recria textura com a superfície correta
                            SDL_DestroyTexture(imgTexture);
                            SDL_Surface *surfAtual = equalizado ? eqSurface : graySurface;
                            imgTexture = SDL_CreateTextureFromSurface(mainRenderer, surfAtual);

                            printf("Imagem %s.\n", equalizado ? "equalizada" : "revertida para cinza");
                        }

                        // Volta ao estado hover ou normal
                        btnState = PontoDentroBotao(event.button.x, event.button.y)
                                   ? BTN_HOVER : BTN_NORMAL;
                    }
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

        // Renderizar janela secundária com histograma correto
        int *histAtual = equalizado ? histogramaEq : histograma;
        RenderizarJanelaSecundaria(secRenderer, histAtual, btnState, equalizado, font);
    }

    // --- Limpeza ---
    SDL_DestroyTexture(imgTexture);
    SDL_DestroySurface(eqSurface);
    SDL_DestroySurface(graySurface);
    SDL_DestroyRenderer(secRenderer);
    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(secWindow);
    SDL_DestroyWindow(mainWindow);
    TTF_CloseFont(font);
    return 0;
}