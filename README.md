# 💊 RemedIoT – Dispenser Inteligente de Medicamentos

## 📌 Descrição do Projeto

O **RemedIoT** é um projeto de Internet das Coisas (IoT) desenvolvido com foco na **assistência a idosos** no controle da medicação. O sistema foi pensado para garantir que o paciente tome seus remédios nos horários corretos, utilizando sensores e atuadores integrados a uma plataforma de monitoramento remoto.

Este projeto foi desenvolvido de acordo com o tema norteador da aplicação de IoT no âmbito da **Agenda 2030 da Organização das Nações Unidas (ONU) no Brasil**. A Agenda 2030 é um plano global para o desenvolvimento sustentável, que busca erradicar a pobreza, proteger o meio ambiente e promover a prosperidade, paz e liberdade para todos.

Segundo a ONU:

> "Os Objetivos de Desenvolvimento Sustentável são um apelo global à ação para acabar com a pobreza, proteger o meio ambiente e o clima e garantir que as pessoas, em todos os lugares, possam desfrutar de paz e de prosperidade."

Este projeto contribui especialmente para os ODS relacionados à saúde e bem-estar, com foco em apoiar a qualidade de vida da população idosa por meio da tecnologia.

Para conhecer a descrição completa das ODS, acesse o sítio oficial da ONU:  
[https://brasil.un.org/pt-br/sdgs](https://brasil.un.org/pt-br/sdgs)

O dispositivo RemedIoT é capaz de:
- Emitir **alertas sonoros** nos horários programados de medicação;
- Detectar se a gaveta foi **aberta ou não** com sensor magnético (tomada do remédio);
- Enviar dados via **MQTT** para um servidor intermediário;
- **Armazenar os dados sensoriais e de eventos no banco de dados InfluxDB** para análises temporais e monitoramento;
- Visualizar essas informações em dashboards configurados no Grafana;
- Acionar envio de **mensagens via WhatsApp** em caso de não adesão ao horário da medicação.

---

## 🧠 Tecnologias Utilizadas

### 🔧 Dispositivo
- **ESP32** com firmware desenvolvido em C++ (Arduino IDE)
- **Sensor magnético** – detecção de abertura da gaveta
- **Buzzer** – sinal sonoro para alertar horário de medicação

### 🔗 Plataforma Low Code
- **Node-RED**:
  - Fluxos de tratamento de dados recebidos via MQTT
  - Integração e gravação dos dados no **InfluxDB**
  - Envio automatizado de mensagens via WhatsApp
  - Lógica para controle de alarmes e contagem de eventos
- **InfluxDB**: banco de dados de séries temporais para armazenamento dos dados do dispenser
- **Grafana**: exibição de dashboard do estado da gaveta (Aberta/Fechada)

