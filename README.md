# Jardins de Acácio — irrigação automática

Sistema de irrigação automatizada dos jardins do LESC/UFC, projeto do Clube do
Hardware (2024). Três partes: aplicativo de controle em Python/Kivy, ESP8266
como ponte Wi-Fi e STM32 acionando a irrigação.

## Funcionamento

O aplicativo (`aplicativo_python/`) envia comandos pela rede ao ESP8266
(`Esp01.ino`), que os repassa por serial ao STM32 (`main.c`), responsável por
acionar as válvulas. Interface com telas de controle e de status.

## Estrutura

    aplicativo_python/   aplicativo Kivy (telas .kv, fontes e imagens)
    Esp01.ino            firmware do ESP8266 (ponte Wi-Fi ↔ serial)
    main.c               firmware do STM32 (acionamento)
    ESP8266-STM32/       integração e testes da comunicação

## Stack

Python, Kivy, C (STM32), Arduino (ESP8266).
