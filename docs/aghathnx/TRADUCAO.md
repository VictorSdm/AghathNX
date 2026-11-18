# Traduções do AghathNX

O AghathNX vem em 27 idiomas. Ele abre no idioma do Windows e permite trocar em
**Configurações > Interface > Idioma**. Os textos que ainda não foram traduzidos aparecem em
inglês.

| Idioma | Arquivo | Situação |
|---|---|---|
| Português (Brasil) | `pt_BR.ts` | completo |
| Árabe, japonês, sueco, chinês simplificado, espanhol, alemão, coreano, russo, ucraniano, italiano | `ar.ts`, `ja_JP.ts`, `sv.ts`, `zh_CN.ts`, `es.ts`, `de.ts`, `ko_KR.ts`, `ru_RU.ts`, `uk.ts`, `it.ts` | cerca de 65–70% |
| Polonês, norueguês, francês, turco, chinês tradicional, português (Portugal), holandês, vietnamita, húngaro, catalão | `pl.ts`, `nb.ts`, `fr.ts`, `tr_TR.ts`, `zh_TW.ts`, `pt_PT.ts`, `nl.ts`, `vi.ts`/`vi_VN.ts`, `hu.ts`, `ca.ts` | cerca de 40–60% |
| Indonésio, tcheco, dinamarquês, grego, finlandês | `id.ts`, `cs.ts`, `da.ts`, `el.ts`, `fi.ts` | menos de 35% |

Os textos que faltam são principalmente os recursos novos do Fritchii e do AghathNX. Eles já
estão nos arquivos, marcados como "unfinished", esperando tradução.

## Como ajudar a traduzir

1. Instale o **Qt Linguist**. Ele vem com o Qt; no Windows também há versões avulsas.
2. Abra o arquivo do seu idioma em `dist/languages/`.
3. Traduza as linhas sem tradução. Mantenha `%1`, `%2`, `%n`, as tags HTML e o `&`, que marca a
   tecla de atalho, em algum lugar do texto.
4. Salve e envie um pull request.

Para editar sem o Qt Linguist, procure no arquivo por `type="unfinished"`, escreva a tradução
dentro de `<translation>` e apague o `type="unfinished"`.

O build compila esses arquivos para dentro do `aghathnx.exe`
(`-DENABLE_QT_TRANSLATION=ON` no workflow).
