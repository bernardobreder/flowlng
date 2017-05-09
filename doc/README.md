# Introdução

O objetivo da linguagem é criar um ambiente de desenvolvimento para desenvolvimento de páginas Web com Servidor Rest.

A vantagem dessa abordagem é criar um ecosistema veloz para criação de páginas web

# Test

Os testes do site deve ser executado na forma de watch e executado de acordo com o contexto da mudança

Através das dependencias dos modulos, os modulos dependentes devem ser executados para validar a integração entre modulos

# Relatório de Dependencia

A linguagem deve mostrar uma relatório de dependencia entre modulos para que possa informar aos desenvolvedores o grau de aclopamento

# TypeScript

A linguagem irá ser tipado apra melhorar a escrita do código

# Modularidade

Os componentes deve ser modularizado para facilitar o desenvolvimento

# Importação Automatica

A importação deve ser feito através do id do modulo

# Playground

A linguagem deve disponibilizar um meio pelo qual seja possível testar códigos na forma de playground e disponibilizar através disso uma documentação no formato MarkDown que executa os código nele atribuido utilizando os modulos do projeto

# Gerencia de Modulo

Essa linguagem irá gerenciar os modulos de tal forma que cada modulo tenha dependencia de outros.

# Modulos Privados

A linguagem deve disponibilizar uma forma pela qual a empresa internamente possa armazenar os seus modulos privados e dessa forma possa ser gerenciado pelo projeto

# Auto Refresh do Browser

A linguagem deve disponiblizar uma meio pela qual o navegador possa atualizar conforme a mudança do código

# Entrada de Saída

Todas as operações de entrada e saida serão tratado async mas no código irá ser apresentado como sync.

# Organização de diretório de modulos

Cada modulo irá ter um diretório proprio com o nome do seu id. Cada diretório terá um arquivo de descrição que especifica detalhes do projeto. Quando a linguagem for invocada, todos os modulos de modo lazy irão ser carregado.

```bash
$ ls
httpserver/
server/
stringbuilder/

flow server
```

# Construção do Site

Para efetuar a construção do site, é preciso criar uma infraestrutura básica para poder gerar o código html do site. Idealmente, o site pode ser construido com o conteudo de vários arquivos MarkDown anexado a alguns arquivos Css.