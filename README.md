# Solar System OpenGL

![Solar System](https://avatars.githubusercontent.com/u/116035045?v=4)

## 📝 O que o código faz

Esse projeto propõe a implementação de um sistema solar simplificado utilizando OpenGL.

## 🚀 Instruções de como compilar e executar

Pré-requisitos:  
Antes de começar, é necessário ter instalado em sua máquina as seguintes ferramentas:

C++     - Linguagem de programação usada.  
SDL2    - Biblioteca usada para criar janelas e gerenciar eventos de entrada (teclado e mouse).  
GLM     - Biblioteca de matemática voltada para gráficos 3D.  
OpenGL  - API de renderização gráfica.  

Você pode instalar as bibliotecas e a API digitando no seu terminal:
```
sudo apt install freeglut3-dev # OpenGL
```
```
sudo apt install libsdl2-dev # SDL2
```
```
sudo apt install libglm-dev # GLM
```

### Compilando o programa  
Clone o repositório com:

```
git clone https://github.com/Josea4848/solar-system-opengl.git
```
Faça o make com:
```
mkdir build && cd build
```
```
cmake ..
```
```
make
```
### Executando o programa
Após compilar, dentro da pasta build, execute no terminal:
```
./solar
```

## 🎮 Controles da câmera e simulação

| Tecla               | Ação                          |
| ------------------- | ----------------------------- |
| **W**               | Mover câmera para frente      |
| **S**               | Mover câmera para trás        |
| **A**               | Mover câmera para a esquerda  |
| **D**               | Mover câmera para a direita   |
| **ESPAÇO**          | Mover câmera para cima        |
| **CTRL (esquerdo)** | Mover câmera para baixo       |
| **P**               | Pausar/retomar simulação      |
| **ESC**             | Encerrar o programa           |
| **Mouse**           | Controlar a direção da câmera |

## 🚨 Principais problemas encontrados

Dificuldade 1: O tamanho do Sol e as distancias dos planetas até o Sol, se mantidos em escala, dificultariam a vizualização do projeto.

Dificuldade 2: Fazer com que o Sol fosse a fonte de luz do sistema.

Dificuldade 3: Encontrar uma forma boa de modelar os anéis de Saturno.

Dificuldade 4: Calcular os vetores da câmera.

## ✨ O que pode ser melhorado

Melhoria 1: Utilizar Ray Tracing ou Path Tracing para iluminação.

Melhoria 2: Adição das principais luas do sistema solar.

Melhoria 3: Mudança da órbita dos planetas em relação ao Sol para uma elipse.

Melhoria 4: Definir uma escala de velocidade para a rotação e translação dos planetas para maior fidelidade.

Melhoria 5: Mudança na forma de fazer os anéis de Saturno.

## 👥 O que cada integrante fez

[José Alves de Oliveira Neto]: Criação das structs e das classes, interação com teclado e mouse, definição do Sol e de sua função como fonte de luz.  

[Carlos Rafael Torres Miranda Novack]: Definição dos planetas e suas texturas, definição da skybox, definição das rotações dos planetas.  

[Emanuel Thadeu dos Santos Conrado]: Representação dos anéis de Saturno, definição das translações dos planetas.
