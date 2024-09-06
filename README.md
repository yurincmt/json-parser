## JSON Parser
Construir um analisador JSON é uma maneira eficaz de aprender sobre técnicas de análise, que são úteis para uma variedade de aplicações, desde a interpretação de formatos de dados simples até o desenvolvimento de compiladores completos para linguagens de programação.

O processo de análise do parser geralmente é dividido em duas etapas: análise léxica e análise sintática. A **análise léxica** é o processo de dividir uma sequência de caracteres em partes significativas, chamadas tokens e atribuí-los a suas devidas classes, de seguindo a gramática. A **análise sintática** (também conhecida simplesmente como parsing) é o processo de examinar a lista de tokens para verificar se ela corresponde a uma gramática formal.

Dito isso, o JSON Parser seria, basicamente, é um verificardor de arquivo *.json, com fim de determinar se o conteúdo do arquivo está em conformidade com a gramática JSON de arquivos.

A ideia de construção desse analisador de arquivo JSON veio do desafio #02 do [Coding Challenges](https://codingchallenges.fyi/challenges/challenge-json-parser).

### Todo

- [ ] Melhorar as mensagens de erro, do lexer e do parser
- [ ] Pegar e mostrar a linha de erro, quando ocorrer um erro, sinalizando onde o erro começa
- [ ] Pegar apenas o token inválido quando ocorrer um erro

## Experimente em Sua Máquina

#### Clone o repositório

    git clone https://github.com/yurincmt/json-parser.git

#### Instale as Dependências

    sudo apt install build-essential libpcre3 libpcre3-dev

* build-essential instalará as principais ferramentas para fazer compilações em sua máquina
* libpcre3 instalará a biblioteca de regex perl para C

> Os comando make devem ser executado no diretório raiz do projeto (onde o arquivo Makefile se encontra).

#### Compilar binário

    make build

#### Execute alguns testes

No diretório data/ podem ser encontrados alguns arquivos JSON para testes, arquivos válidos e inválidos.

    ./jparser data/input.json

#### Instalar binário na máquina

    make install

#### Desinstalar da máquina

    make uninstall

## Estrutura do Projeto

## Json parser em ação!
