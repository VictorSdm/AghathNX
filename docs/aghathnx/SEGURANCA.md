# AghathNX: auditoria de segurança e dependências de terceiros

Revisão feita em novembro de 2026 sobre o AghathNX v1.0 (base: Fritchii v5.8.4).

## Como foi feita

O código tem cerca de 890 mil linhas em C/C++ (`src/` e `externals/`). Nenhuma pessoa lê tudo
isso linha por linha de forma útil. Por isso, a revisão foi feita assim:

1. **Busca em todos os arquivos** por tudo que dá poder ao programa: acesso à rede (HTTP,
   sockets), execução de outros programas, carregamento de DLLs, escrita no registro, exclusão
   de arquivos, leitura de área de transferência, teclado, credenciais, variáveis de ambiente,
   IPs fixos e dados codificados escondidos (base64 longo).
2. **Leitura completa** de cada arquivo encontrado nessa busca, para entender o que ele faz e
   quando faz.
3. **Revisão do build**: tudo que o build baixa da internet e de quem vem.

O núcleo do emulador (CPU, GPU, áudio, serviços do console) vem do yuzu e do Eden, projetos
públicos revisados há anos por muita gente. O foco foi no que os forks (Lichii, Fritchii)
adicionaram e em tudo que toca a internet ou o computador do usuário.

## Resultado

**Nada malicioso foi encontrado.** Não há telemetria, envio de dados pessoais, keylogger,
mineração, coleta de arquivos ou senhas, nem envio automático de logs. O "Exportar log" ainda
apaga o nome de usuário do Windows do arquivo antes de salvar.

### Onde o programa acessa a internet

| Recurso | Para onde | Quando | O que envia |
|---|---|---|---|
| Notícias do console emulado | `api.github.com`, releases do **VictorSdm/AghathNX** | Quando um jogo ou o menu abre as notícias | Nada além do pedido público |
| Busca de ícone/capa | switchiconshowdown.com, SteamGridDB, Wikipedia, Bing, repositório público de capas do Eden | Só quando o usuário clica e permite busca online | O nome do jogo |
| DLSS | GitHub da NVIDIA | Só quando o usuário clica em baixar e aceita a licença | Nada; os arquivos são conferidos pela assinatura digital da NVIDIA |
| FSR 4 | GitHub da AMD | Só quando o usuário clica em baixar e aceita a licença | Nada; os arquivos são conferidos por hash e pela assinatura digital da AMD |
| Amiibo | amiiboapi.com | Só na tela de Amiibo | Nada além do pedido público |
| Multiplayer | Servidor de salas `api.ynet-fun.xyz` (comunidade) | Só quando o usuário abre ou cria salas públicas | Apelido e dados da sala |
| Jogos com internet | O que o jogo emulado acessar | Só se o usuário desligar o modo avião do console emulado (vem ligado) | O que o jogo enviar |
| Discord | Discord local | Desligado nos builds oficiais | Nome do jogo em execução |
| Atualização automática | — | **Desligada** | — |

### Execução de programas e DLLs

- O programa reinicia a si mesmo (teste do Vulkan e cópia de dados) e abre o Explorer para
  mostrar pastas. Nada externo é executado.
- As DLLs da NVIDIA e da AMD só são carregadas depois de o Windows confirmar a assinatura
  digital do fabricante.
- As exclusões de arquivos acontecem só em pastas do próprio emulador (cache, backups, saves)
  e quase sempre depois de confirmação e com backup.

### Pontos de atenção (baixo risco)

- `src/common/fs/symlink.cpp` usa `mklink` via `system()` quando o Windows recusa um link
  simbólico. Os caminhos são das próprias pastas do emulador.
- A importação de saves por zip extrai o arquivo escolhido pelo usuário numa pasta temporária.
  Importe só zips de confiança.

## O que foi desvinculado de outros projetos

| Antes | Agora |
|---|---|
| Notícias liam as releases do Lichii (zortos293/Lichii) | Leem as releases do AghathNX |
| Imagens das notícias vinham do servidor do Eden (eden-emu.dev) | Não são baixadas |
| Discord usava o aplicativo do Eden (aparecia como Eden) e consultava o repositório de capas do Eden a cada jogo | Exige o ID do aplicativo próprio do AghathNX (`AGHATHNX_DISCORD_CLIENT_ID`) e usa o ícone do AghathNX |
| Links "Código-fonte", "Reportar problema" e "Créditos" do Sobre apontavam para o Eglantine | Apontam para o repositório do AghathNX |
| Links de ajuda e de erro apontavam para a documentação do Eden | Apontam para `docs/user` do AghathNX |
| Site do instalador: Eglantine | Repositório do AghathNX |
| Programa se identificava como "Lichii" ou "Eden" nos acessos à internet | Identifica-se como "AghathNX" |
| Apelido padrão no multiplayer: "Eden" | "AghathNX" |
| Atualizador apontava para o Fritchii | Desligado e apontando para o AghathNX |
| Botões de Discord e doação do Frizti | Escondidos até existirem links próprios |

### Ainda ligados a terceiros (decisão sua)

- **Servidor de salas multiplayer** (`api.ynet-fun.xyz`): é da comunidade e usado por vários
  emuladores. Usuários do AghathNX aparecem nas mesmas salas públicas. A alternativa é
  hospedar um servidor próprio ou deixar só as salas diretas por IP.
- **Busca de capas** no repositório público do Eden: é só leitura de imagens públicas, sob
  demanda.
- **Botão "Mods"** do menu clássico abre o arquivo público de mods do Eden no GitHub.
- **Arquivos de Linux** (`dist/io.github.zortos293.lichii.*`, `dist/dev.eden_emu.*`): não entram
  no build de Windows. Precisam de nome próprio se um dia houver build de Linux.

## Segurança do build

Os builds são feitos pelo GitHub Actions a partir do código público do repositório.

- Bibliotecas baixadas do código-fonte (Boost, fmt, zlib e outras) já tinham o hash SHA-512
  fixado em `cpmfile.json`.
- Quatro binários **pré-compilados** que vão dentro do AghathNX vinham conferidos só pelo hash
  publicado junto com eles, então quem controla aquela conta poderia trocá-los:
  OpenSSL e SDL3 (conta `crueter-ci`), FFmpeg com AV1 (conta pessoal `Takoroka`) e sirit
  (`eden-emulator`). **Agora o hash de cada um está fixado** (`pinned_windows_amd64`), e o build
  para com erro se o arquivo publicado mudar.
- As ações de terceiros do GitHub Actions estão fixadas por hash de commit, não por tag.
- O download do sccache é conferido por SHA-256, e o instalador do Vulkan SDK só roda com
  assinatura digital válida.
- O LLVM (compilador) vem do Chocolatey na versão mais recente. O passo "Show compiler" do log
  mostra qual versão foi usada.

Ao atualizar uma dessas dependências de propósito, troque também o hash correspondente.
