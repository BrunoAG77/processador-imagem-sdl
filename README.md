# Projeto N1 Computação Visual - Processamento de Imagem

## Objetivo
Desenvolver um software de processamento de imagens em linguagem C ou C++, uasndo a biblioteca SDL (Simple DirectMedia Layer) em sua versão 3.

## Escopo do projeto e funcionalidades obrigatórias
O programa deverá ser executado via linha de comando, recebendo como argumento o caminho para
um arquivo de imagem. Por exemplo:

>programa caminho_da_imagem.ext

Sendo:
- programa: O nome do arquivo executável do programa, definido pelo grupo
- caminho_da_imagem.ext: O caminho do arquivo de imagem, incluindo sua extensão.

O programa deve implementar as seguintes funcionalidades:

### 1. Carregamento da imagem
- O programa deve ser capaz de carregar imagens nos formatos mais comuns, como PNG, JPG
e BMP, usando a biblioteca SDL_image.
- Deve tratar possíveis problemas, como arquivo não encontrado ou arquivo que não seja um
formato de imagem válido.

### 2. Análise e conversão para escala de cinza
- O programa deve implementar uma função que verifica se a imagem carregada já está em
escala de cinza ou se é colorida.
- Caso a imagem seja colorida, o programa deve converter a imagem para escala de cinza
usando a seguinte fórmula: 𝑌 = 0.2125 ∗ 𝑅 + 0.7154 ∗ 𝐺 + 0.0721 ∗ 𝐵.
- A imagem em escala de cinza deve ser usada como base/entrada para as operações seguintes.

### 3. Interface Gráfica de usuário (GUI) com duas janelas
- Janela principal: Deve exibir a imagem que está sendo processada. O tamanho da janela deve
se adaptar ao tamanho da imagem carregada e deve iniciar centralizada no monitor principal.
- Janela secundária (filha da janela principal): Uma janela de tamanho fixo (definido pelo
grupo), posicionada ao lado da janela principal. Deve exibir o histograma da imagem e um
botão de operação (ver item 5 adiante).

### 4. Análise e exibição de usuário do histograma
- Na janela secundária, o programa deve exibir o histograma da imagem
- O programa deve analisar o histograma e exibir as seguintes informações:
    - o Média de intensidade: Classificar a imagem como "clara", "média" ou "escura"
    - Desvio padrão: Classificar o contraste da imagem como "alto", "médio" ou "baixo"
- Sugestão: usar a biblioteca SDL_ttf para exibir informações em texto.

### 5. Equalização do histograma
- Na janela secundária, deve haver um botão (desenhado com primitivas da SDL).
- Ao clicar no botão, o programa deve equalizar o histograma da imagem, atualizando a imagem
exibida na janela principal e o histograma na janela secundária.
- Ao clicar novamente no botão, o programa deve reverter a imagem para a sua versão original
em escala de cinza, sendo que o conteúdo das janelas deve ser atualizado de acordo.
- O texto do botão deve mudar para refletir a ação (ex.: "Equalizar" / "Ver original").
- O estado do botão deve refletir as ações do usuário (ex.: cor azul para estado "neutro", cor
azul claro para estado "mouse em cima do botão", cor azul escuro para estado "botão clicado").

### 6. Salvar imagem
- Ao pressionar a tecla S do teclado, o programa deve salvar a imagem atualmente exibida na
janela principal em um arquivo chamado output_image.png
- Caso o arquivo output_image.png já exista, o programa deve sobrescrever o arquivo.