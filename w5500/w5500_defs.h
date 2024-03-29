#define W5500_BSB_COMMON (0x00u)
#define W5500_BSB_SOCKET(n) (((n & 0x07u) << 2u) + 1u)
#define W5500_BSB_SOCKET_TX(n) (((n & 0x07u) << 2u) + 2u)
#define W5500_BSB_SOCKET_RX(n) (((n & 0x07u) << 2u) + 3u)

#define W5500_ADR_GAR (0x0001)
#define W5500_ADR_SUBR (0x0005)
#define W5500_ADR_SHAR (0x0009)
#define W5500_ADR_SIPR (0x000F)
#define W5500_ADR_VERSIONR (0x0039)
#define W5500_ADR_PHYCFGR (0x002E)

#define W5500_ADR_Sn_MR (0x0000)
#define W5500_PROTO_TCP (1)
#define W5500_ADR_Sn_CR (0x0001)
#define W5500_Sn_CR_OPEN 0x01
#define W5500_Sn_CR_LISTEN 0x02
#define W5500_Sn_CR_CONNECT 0x04
#define W5500_Sn_CR_DISCON 0x08
#define W5500_Sn_CR_CLOSE 0x10
#define W5500_Sn_CR_SEND 0x20
#define W5500_Sn_CR_SEND_MAC 0x21
#define W5500_Sn_CR_SEND_KEEP 0x22
#define W5500_Sn_CR_RECV 0x40

#define W5500_ADR_Sn_SR (0x0003)
#define W5500_Sn_SR_SOCK_CLOSED 0x00
#define W5500_Sn_SR_SOCK_INIT 0x13
#define W5500_Sn_SR_SOCK_LISTEN 0x14
#define W5500_Sn_SR_SOCK_ESTABLISHED 0x17
#define W5500_Sn_SR_SOCK_CLOSE_WAIT 0x1C
#define W5500_Sn_SR_SOCK_UDP 0x22
#define W5500_Sn_SR_SOCK_MACRAW 0x42
#define W5500_Sn_SR_SOCK_SYNSENT 0x15
#define W5500_Sn_SR_SOCK_SYNRECV 0x16
#define W5500_Sn_SR_SOCK_FIN_WAIT 0x18
#define W5500_Sn_SR_SOCK_CLOSING 0x1A
#define W5500_Sn_SR_SOCK_TIME_WAIT 0x1B
#define W5500_Sn_SR_SOCK_LAST_ACK 0x1D

#define W5500_ADR_Sn_PORT (0x0004u)
#define W5500_ADR_Sn_RXBUF_SIZE (0x001Eu)
#define W5500_ADR_Sn_TXBUF_SIZE (0x001Fu)
#define W5500_ADR_Sn_TX_FSR (0x0020u)
#define W5500_ADR_Sn_TX_RD (0x0022u)
#define W5500_ADR_Sn_TX_WR (0x0024u)
#define W5500_ADR_Sn_RX_RSR (0x0026u)
#define W5500_ADR_Sn_RX_RD (0x0028u)
#define W5500_ADR_Sn_RX_WR (0x002Au)

#define W5500_SOCK_SIZE_2K 2

#define OPMODE_VDM (0)
#define OPMODE_FIX8 (1)
#define OPMODE_FIX16 (2)
#define OPMODE_FIX32 (3)
#define OPMODE_R (0 << 2)
#define OPMODE_W (1 << 2)