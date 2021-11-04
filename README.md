# UnB-SISMIC-Projeto-Final
Projeto final de Laboratório de Sistemas Microprocessados - UnB 2021/1
Aluno: Samuel James de Lima Barroso

# Sistema de monitoramento e controle de temperatura e humidade em sala climatizada com acesso remoto (IoT/WiFI)

## Motivação
Atualmente, tenho um projeto de hospedagem de servidores (bare-metal) que ficam em uma sala com ar-condicionado, 
basicamente uma “sala CPD”, mas prefiro me referir a ela como um projeto inicial de datacenter. Nesse contexto, é 
muito importante manter a temperatura da sala em valores controlados, idealmente entre 20ºC e 24ºC. Para isso, 
temos um ar condicionado ligado 24/7. Entretanto, o que aconteceria se o ar condicionado apresentasse problemas? 
Como poderíamos detectar rapidamente uma variação inesperada na temperatura/humidade? Seria possível controlar 
esse ar-condicionado enquanto estivesse longe de casa? Seria possível ter acesso a um histórico de temperatura da 
sala? Para que essas situações sejam solucionadas, elaborei esta proposta de projeto utilizando o MSP.

## Ideia do projeto

A ideia que tive foi utilizar o MSP conectado a um sensor de temperatura e umidade para coletar dados e transmitir
para um computador. Esses dados poderão ser transmitidos localmente (USB) ou pela internet (Extensão de projeto A).
Além disso, desejo controlar a temperatura do ar-condicionado utilizando um módulo infravermelho (Extensão de projeto B).
Os objetivos principais necessariamente serão implementados. As extensões de projeto são ideias extras opcionais, 
são independentes entre si, ou seja, a extensão B pode ser implementada sem que a extensão A seja feita, e 
vice-versa. Idealmente, as duas extensões serão implementadas, de acordo com o nível de dificuldade, tempo hábil e 
outros fatores. Nesse contexto, peço a opinião do professor a respeito de quais funcionalidades implementar, sendo 
possível, ainda, criar extensões alternativas.

## Objetivos principais
- Medir a temperatura e umidade com sensor DHT11 ou similar.
- Exibir informações em display LCD com comunicação I2C.
- Transmitir dados para servidor/computador local por meio de comunicação USB.
- Construir software que recebe dados do MSP e armazena em banco de dados, para disponibilizar histórico de temperaturas e humidades, além de poder enviar alertas via e-mail.


## Extensão de projeto A
- Transmitir os dados a um servidor remoto, por meio da internet, utilizando módulo WiFI ou GSM (rede M2M - celular). Dessa forma, não é necessário um servidor/computador local para se comunicar com o MSP utilizando a interface USB.

## Extensão de projeto B
- Controlar a temperatura do ar-condicionado por meio de módulo IR (infravermelho).
- Receber dados de controle via servidor remoto/local (dependendo se a extensão de projeto A foi implementada não)



