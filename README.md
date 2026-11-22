# ESP_RC

Projeto desenvolvido para realizar a comunicação entre um ESP32-S3 e uma controladora de rádio para drones da **Raccoon Flying**, utilizando o protocolo SBUS.

## Funcionamento

O portal de controle apresenta uma interface em modo paisagem, com dois joysticks virtuais:

## Conexão Wi-Fi

Ao iniciar, o ESP32-S3 cria um ponto de acesso Wi-Fi com as seguintes configurações:

| Parâmetro | Valor |
|---|---|
| SSID | `ESP_RC` |
| Senha | `??????` |
| Endereço do portal | `http://192.168.4.1/` |

Para acessar a interface:

1. Conecte o dispositivo à rede Wi-Fi `ESP_RC`.
2. Aguarde o redirecionamento automático para o portal cativo.
3. Caso a página não seja aberta automaticamente, acesse `http://192.168.4.1/` pelo navegador.

O redirecionamento DNS/HTTP está habilitado. Entretanto, a abertura automática do portal depende do suporte do sistema operacional utilizado.

## Comunicação SBUS

A comunicação entre o ESP32-S3 e a controladora de voo SpeedyBee F405 V4 é realizada por meio do protocolo SBUS.

O ESP32-S3 gera continuamente frames SBUS válidos de 25 bytes, utilizando 16 canais com resolução de 11 bits. A transmissão é realizada pela UART, com inversão de sinal por hardware do ESP32, dispensando a utilização de um inversor externo.

### Configuração do protocolo

| Parâmetro | Configuração |
|---|---|
| Protocolo | SBUS |
| Baud rate | 100000 bps |
| Bits de dados | 8 |
| Paridade | Even (par) |
| Stop bits | 2 |
| Formato | 8E2 |
| Sinal | Invertido |
| Estado em repouso (Idle) | Nível baixo |
| Tamanho do frame | 25 bytes |

## Observações

- A comunicação SBUS utiliza inversão de sinal por hardware do ESP32-S3.
- Não é necessário utilizar um circuito inversor externo.
- O canal RX está reservado para possíveis implementações futuras de telemetria.
- Os comandos de voo dependem da configuração correta dos modos AUX na controladora.
- Antes de qualquer operação, valide os canais e comandos com as hélices removidas.