#include <qcbor/qcbor.h>

/* Position enum */
enum
{
    telemetry_version_pos = 0,
    telemetry_rssi_pos,
    my_enum,
} telemetry_data_positions;

/* Used to store strings */
typedef struct
{
    char bytes[64];
    size_t size;
} telemetry_data_string_t;

/* Struct for telemetry data */
typedef struct
{
    telemetry_data_string_t version;
    int32_t rssi;
    uint32_t my_fav_number;
} telemetry_data_t;

/* Encode function */
QCBORError telemetry_codec_encode(const telemetry_data_t *p_data, uint8_t *p_buf, size_t len, size_t *p_size);

/* Decode function */
int telemetry_codec_decode(telemetry_data_t *p_data, const uint8_t *p_buf, size_t len);
