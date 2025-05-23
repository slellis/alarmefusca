# Documentação do Projeto Alarme Fusca

Este repositório contém a documentação do projeto de alarme automotivo para Fusca, construída com [MkDocs](https://www.mkdocs.org/).

---

## 📄 Visite a Documentação Online

Se o GitHub Pages estiver habilitado, você poderá acessar em:

```
https://SEUUSUARIO.github.io/alarmefusca/
```

(Substitua `SEUUSUARIO` pelo seu nome de usuário do GitHub.)

---

## 📦 Estrutura do Projeto

```
alarme-fusca-docs/
├── docs/
│   ├── index.md
│   ├── Tutorial_MkDocs_Instalacao_e_Uso.md
│   ├── Documentacao_Completa_Alarme_Fusca.md
│   ├── Placa_Central_Logica_Alarme_Fusca.md
│   └── Placa_Potencia_Alarme_Fusca.md
└── mkdocs.yml
```

---

## 🚀 Como publicar no GitHub Pages

1. Instale o MkDocs:
```bash
pip install mkdocs
```

2. (Opcional) Instale o tema Material:
```bash
pip install mkdocs-material
```

3. No terminal, na raiz do projeto, execute:
```bash
mkdocs gh-deploy
```

Isso criará o branch `gh-pages` e publicará o site.

---

## 🔁 Atualizando a documentação

Sempre que editar arquivos `.md` ou `mkdocs.yml`, rode:

```bash
mkdocs build      # para gerar o site estático
mkdocs serve      # para rodar localmente
mkdocs gh-deploy  # para publicar no GitHub Pages
```

---

## ✍️ Licença
Este projeto é mantido por Sidney Lellis e licenciado de forma aberta.
