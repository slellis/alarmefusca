# Tutorial: Criando documentação com MkDocs no Windows

## 1. Pré-requisitos
Você precisa ter o **Python** instalado no seu Windows. Para verificar:
```bash
python --version
```
Se não tiver, baixe em: https://www.python.org/downloads/

## 2. Instalar o MkDocs
No terminal (PowerShell ou CMD):
```bash
pip install mkdocs
```

## 3. Criar um novo projeto MkDocs
Escolha uma pasta onde seu site será criado:
```bash
mkdocs new alarme-fusca-docs
cd alarme-fusca-docs
```

Você verá uma estrutura assim:
```
alarme-fusca-docs/
├── docs/
│   └── index.md
└── mkdocs.yml
```

## 4. Copiar seu arquivo `.md`
Substitua o `docs/index.md` pelo seu `Placa_Potencia_Alarme_Fusca.md`, ou adicione-o como uma nova página:
```bash
copy caminho\para\Placa_Potencia_Alarme_Fusca.md docs/
```

## 5. Editar o menu no `mkdocs.yml`
Abra o arquivo `mkdocs.yml` e adicione sua página ao menu:
```yaml
site_name: Alarme Fusca
nav:
  - Início: index.md
  - Placa de Potência: Placa_Potencia_Alarme_Fusca.md
```

## 6. Rodar o site localmente
No terminal:
```bash
mkdocs serve
```

Abra o navegador:  
[http://127.0.0.1:8000](http://127.0.0.1:8000)

## 7. Publicar (opcional)
Depois que estiver satisfeito, você pode publicar com:
```bash
mkdocs build
```
Ou diretamente no GitHub Pages:
```bash
mkdocs gh-deploy
```
