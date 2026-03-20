// Bruno Antico Galin RA: 104
// 
// Ismael de Sousa Silva RA: 10410870

//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

//------------------------------------------------------------------------------
void shutdown(void)
{
  SDL_Log("shutdown()");
  SDL_Quit();
}

//------------------------------------------------------------------------------
void ValidarExtensao(const char* nomearquivo) {
    char copia[100];
    strcpy(copia, nomearquivo);

    // Procurar o último ponto
    char *ext = strrchr(copia, '.');

    if (ext == NULL) {
        printf("Nao possui extensao no caminho.\n"); // não tem extensão
    }

    // Compara extensões válidas
    if (strcmp(ext, ".jpg") == 0 ||
        strcmp(ext, ".png") == 0 ||
        strcmp(ext, ".bmp") == 0) {
        printf("Extensao aceita.\n");
    } else {
      printf("Este tipo de extensao nao e aceita!\n");
      exit(1);
    }
}

// Verifica se a imagem selecionada está em escala de cinza
bool VerificaImagem(char* image) {
  SDL_Surface *img = IMG_Load(image);
  SDL_Surface *conv = SDL_ConvertSurface(img, SDL_PIXELFORMAT_RGB24);
  
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
  
  return is_grayscale;
}

// Converter imagem para escala de cinza
SDL_Surface* ConvertImageScaleGray(const char* image) {
  SDL_Surface *img = IMG_Load(image);
  SDL_Surface *conv = SDL_ConvertSurface(img, SDL_PIXELFORMAT_RGB24);

  SDL_DestroySurface(img);
  
  Uint8 *pixels = (Uint8 *)conv->pixels;

  for (int i = 0; i < conv->w * conv->h; i++) {
    Uint8 r = pixels[i * 3 + 0];
    Uint8 g = pixels[i * 3 + 1];
    Uint8 b = pixels[i * 3 + 2];

    Uint8 gray = (0.2125 * r) + (0.7154* g) + (0.0721 * b);

    pixels[i * 3 + 0] = gray;
    pixels[i * 3 + 1] = gray;
    pixels[i * 3 + 2] = gray;
  }

  return conv;
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  //Receber como entrada caminho do arquivo que será alterado
  char ext_arquivo[100]; // Entrada do arquivo
  
  printf("Adicione o caminho do arquivo da sua imagem: \n");
  fgets(ext_arquivo, sizeof(ext_arquivo), stdin);
  ext_arquivo[strcspn(ext_arquivo, "\n")] = '\0';

  ValidarExtensao(ext_arquivo);
  
  //Verifica se a imagem está com escala de cinza
  bool VerificaImagemCinza = VerificaImagem(ext_arquivo);
  
  
  atexit(shutdown);
  
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Erro ao iniciar a SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  
  const char* WINDOW_TITLE = "Hello, SDL";
  const char* IMAGE_TEST = ext_arquivo; // Variavel constante que será armazenado local do arquivo
  enum constants {
    WINDOW_WIDTH = 640,
    WINDOW_HEIGHT = 480,
    WINDOW_TITLE_MAX_LENGTH = 64
  };

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Surface* textImg = ConvertImageScaleGray(IMAGE_TEST);
  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
    &window, &renderer))
    {
      SDL_Log("Erro ao criar a janela e/ou renderizador: %s", SDL_GetError());
      return SDL_APP_FAILURE;
    }
    
    char windowTitle[WINDOW_TITLE_MAX_LENGTH] = { 0 };
    
  // SDL_Texture *texBmp = IMG_LoadTexture(renderer, imgRend);
  SDL_Texture* imgRend = SDL_CreateTextureFromSurface(renderer, textImg);
  if (!imgRend)
  {
    SDL_Log("Erro ao carregar a imagem '%s': %s", imgRend, SDL_GetError());
  }
  SDL_FRect bmpRect =
  {
    .x = 0.0f,
    .y = 0.0f,
    .w = imgRend ? imgRend->w : 0.0f,
    .h = imgRend ? imgRend->h : 0.0f,
  };

  SDL_Event event;
  bool isRunning = true;
  while (isRunning)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_EVENT_QUIT:
          isRunning = false;
          break;

        case SDL_EVENT_MOUSE_MOTION:
          snprintf(windowTitle, WINDOW_TITLE_MAX_LENGTH,
            "%s (%.0f, %.0f)", WINDOW_TITLE, event.motion.x, event.motion.y);
          SDL_SetWindowTitle(window, windowTitle);
          break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, imgRend, NULL, &bmpRect);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(imgRend);
  imgRend = NULL;

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  return 0;
}