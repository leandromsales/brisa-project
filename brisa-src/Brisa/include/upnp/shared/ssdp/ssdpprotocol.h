#ifndef SSDP_PROTOCOL_H
#define SSDP_PROTOCOL_H

#define SSDP_ADDR "239.255.255.250"
#define SSDP_PORT 1900
#define SSDP_PORT_STR "1900"

static const QString SSDP_DISCOVERY_REQUEST =
	"M-SEARCH * HTTP/1.1\r\n"
	"HOST: 239.255.255.250:1900\r\n"
	"MAN: \"ssdp:discover\"\r\n"
	"ST: %1\r\n"
	"MX: %2\r\n"
	"USER-AGENT: %3 SSDP/1.0\r\n"
	"\r\n";

#define SSDP_DISCOVERY_RESPONSE                     \
        "HTTP/1.1 200 OK\r\n"                       \
        "Location: %s\r\n"                          \
        "%s"                                        \
        "Ext:\r\n"                                  \
        "USN: %s\r\n"                               \
        "Server: %s\r\n"                            \
        "Cache-Control: max-age=%d\r\n"             \
        "ST: %s\r\n"                                \
        "Date: %s\r\n"                              \
        "Content-Length: 0\r\n\r\n"

#define SSDP_ALIVE_MESSAGE                          \
        "NOTIFY * HTTP/1.1\r\n"                     \
        "Host: " SSDP_ADDR ":" SSDP_PORT_STR "\r\n" \
        "Cache-Control: max-age=%d\r\n"             \
        "Location: %s\r\n"                          \
        "%s"                                        \
        "Server: %s\r\n"                            \
        "NTS: ssdp:alive\r\n"                       \
        "NT: %s\r\n"                                \
        "USN: %s\r\n\r\n"

#define SSDP_BYEBYE_MESSAGE                         \
        "NOTIFY * HTTP/1.1\r\n"                     \
        "Host: " SSDP_ADDR ":" SSDP_PORT_STR "\r\n" \
        "NTS: ssdp:byebye\r\n"                     \
        "NT: %s\r\n"                                \
        "USN: %s\r\n\r\n"

#define SSDP_SEARCH_METHOD "M-SEARCH"
#define GENA_NOTIFY_METHOD "NOTIFY"

#define SSDP_ALIVE_NTS  "ssdp:alive"
#define SSDP_BYEBYE_NTS "ssdp:byebye"

#define SSDP_DEFAULT_MAX_AGE 1800
#define SSDP_DEFAULT_MX      3

#endif /* __SSDP_PROTOCOL_H__ */
