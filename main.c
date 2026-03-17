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
    }
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  //Receber como entrada caminho do arquivo que será alterado
  char ext_arquivo[100]; // Entrada do arquivo
  
  printf("Adicione o caminho do arquivo da sua imagem: \n");
  fgets(ext_arquivo, sizeof(ext_arquivo), stdin);
  ext_arquivo[strcspn(ext_arquivo, "\n")] = '\0';

  const char* IMAGE_TEST = ext_arquivo; // Variavel constante que será armazenado local do arquivo

  ValidarExtensao(IMAGE_TEST);

  atexit(shutdown);

  const char* WINDOW_TITLE = "Hello, SDL";

  const int WINDOW_WIDTH = 640;
  const int WINDOW_HEIGHT = 480;
  const int WINDOW_TITLE_MAX_LENGTH = 64;

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Erro ao iniciar a SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
    &window, &renderer))
  {
    SDL_Log("Erro ao criar a janela e/ou renderizador: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  char windowTitle[WINDOW_TITLE_MAX_LENGTH] = { 0 };

  SDL_Texture *texBmp = IMG_LoadTexture(renderer, IMAGE_TEST);
  if (!texBmp)
  {
    SDL_Log("Erro ao carregar a imagem '%s': %s", IMAGE_TEST, SDL_GetError());
  }
  SDL_FRect bmpRect =
  {
    .x = 0.0f,
    .y = 0.0f,
    .w = texBmp ? texBmp->w : 0.0f,
    .h = texBmp ? texBmp->h : 0.0f,
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
    SDL_RenderTexture(renderer, texBmp, NULL, &bmpRect);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texBmp);
  texBmp = NULL;

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  return 0;
}