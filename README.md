## JSON Parser
Construir um analisador JSON é uma maneira eficaz de aprender sobre técnicas de análise, que são úteis para uma variedade de aplicações, desde a interpretação de formatos de dados simples até o desenvolvimento de compiladores completos para linguagens de programação.

O processo de análise do parser geralmente é dividido em duas etapas: análise léxica e análise sintática.

A **análise léxica (scanner)** é o processo de dividir uma sequência de caracteres em partes significativas, chamadas de tokens e atribuí-los a suas devidas classes, de acordo com uma gramática. São responsabilidades do scanner:

- Leitura de código-fonte
- Divisão em tokens
- Filtragem de espaços em branco e comentários
- Identificação de erros léxicos (quando o scanner encontrar uma sequência de caracteres que não corresponde a nenhum token válido da linguagem)
- Alimentação do analisador sintático

A **análise sintática (parser)** é o processo de examinar a lista de tokens recebida do scanner para verificar se ela corresponde a uma gramática formal.

Dito isso, o JSON Parser, basicamente, é um verificardor de arquivo *.json, com fim de determinar se o conteúdo do arquivo está em conformidade com a gramática de arquivos JSON.

A ideia de construção desse analisador de arquivo JSON veio do desafio #02 do [Coding Challenges](https://codingchallenges.fyi/challenges/challenge-json-parser).

### Todo

- [ ] Melhorar as mensagens de erro, do lexer e do parser
- [ ] Pegar e mostrar a linha de erro, quando ocorrer um erro, sinalizando onde o erro começa
- [ ] Pegar o token inválido quando ocorrer um erro no analisador léxico

## Json parser em ação!


## Especificação da gramática JSON do projeto

    json            --> <object> | <array>

    <object>        --> "{" <members> "}"
                      | "{" "}"

    <members>       --> <key> ":" <value> "," <members>
                      | <key> ":" <value>

    <array>         --> "[" "]"
                      | "[" <value> "]"
                      | "[" <value> "," <values> "]"

    <values>        --> <value>
                      | <value> "," <values

    <value>         --> <object> | <array> | <string> | <number> | <literal> |

    <key>           --> <string>

    <string>        --> "string"

    <literal>       --> true | false | null

    <number>        --> number

## Experimente em Sua Máquina

#### Clone o repositório

    git clone https://github.com/yurincmt/json-parser.git

#### Instale as Dependências

    # Debian
    sudo apt install build-essential libpcre3 libpcre3-dev

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

    .
    ├── data
    │   ├── input.json
    │   ├── test
    │   └── tests
    ├── json.grammar
    ├── Makefile
    ├── README.md
    └── src
        ├── lex.c
        ├── lex.h
        ├── lexmain.c
        ├── parser.c
        ├── parser.h
        └── parsermain.c
