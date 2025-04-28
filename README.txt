Como rodar o projeto
	Pré-requisitos
		g++ (compilador C++)
		make (ferramenta de automação de compilação)

Instruções de uso
	Compilar e executar o programa: (Como o programa lê a entrada a partir do stdin, é necessário redirecionar um arquivo de entrada.)
		make < test/teste.txt

Compilar apenas:
	make build
	
Executar apenas (após ter compilado):
	make run < test/teste.txt

Execução manual (Caso deseje compilar e executar manualmente, utilize os seguintes comandos:)
	mkdir build
	mkdir bin
	g++ -c src/task/Task.cpp -o build/task.o
	g++ -c src/background_server/BackgroundServer.cpp -o build/bs.o
	g++ src/main.cpp build/task.o build/bs.o -o bin/run.exe
	./bin/run.exe < test/teste.txt
	
Observações
	O projeto foi testado em ambientes Linux e Windows, e funcionou corretamente em ambos.

Importante: O último build foi realizado em Windows, se o projeto for transferido para Linux, é necessário apagar os arquivos previamente gerados nas pastas build/ e bin/.
Para isso, execute:
	make clean
	
Importante: o projeto utiliza imports com caminhos relativos. Portanto, não altere a estrutura de pastas e arquivos. Mover ou renomear qualquer arquivo ou diretório quebrará a compilação e a execução do projeto.