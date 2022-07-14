#include <telemetry_codec.h>
#include <stdio.h>
#include <stdlib.h>

#include "qcbor/qcbor.h"
#include "cn-cbor/cn-cbor.h"
#include "cbor.h"

struct MyTest {
    uint8_t varA;
    int varB;
    char name[30];
};

static CborError decode(CborValue *it)
{
    while (!cbor_value_at_end(it)) {
        CborError err;
        CborType type = cbor_value_get_type(it);
        switch (type) {
        case CborArrayType:
            break;
        case CborIntegerType: {
            int64_t val;
            cbor_value_get_int64(it, &val);     // can't fail
            printf("%lld\n", (long long)val);
            break;
            }

        case CborTextStringType: {
            char *buf;
            size_t n;
            err = cbor_value_dup_text_string(it, &buf, &n, it);
            if (err)
                return err;     // parse error
            printf("\"%s\"\n", buf);
            free(buf);
            continue;
            }   

        case CborBooleanType: {
            bool val;
            cbor_value_get_boolean(it, &val);       // can't fail
            puts(val ? "true" : "false");
            break;
            }

        case CborFloatType: {
            float f;
            cbor_value_get_float(it, &f);
            printf("%f\n", f);
            break;
        }
        case CborInvalidType:
            printf("invalid\r\n");
            break;
        }
        err = cbor_value_advance_fixed(it);
        if (err)
            return err;
    return CborNoError;
    }
}

/*  TinyCBOR    */
int fun()
{
    struct MyTest obj;
    obj.varA = 100;
    obj.varB = 10;
    strcpy(obj.name , "hrishi");

    uint8_t buff[100];

    //Encode
    CborEncoder encoder;
    CborEncoder array;
    cbor_encoder_init(&encoder, buff, sizeof(buff), 0);

    cbor_encoder_create_array(&encoder, &array, CborIndefiniteLength);
    cbor_encode_int(&array, obj.varA);
    cbor_encode_int(&array, obj.varB);
    cbor_encode_text_string(&array, obj.name, strlen(obj.name));
    cbor_encoder_close_container(&encoder, &array);
    printf("***Encode Tiny-CBOR Done\r\n");

    //Decode 
    CborParser parser;
    CborValue value;
    CborValue l_array;
    cbor_parser_init(buff, sizeof(buff), 0, &parser, &value);
    cbor_value_enter_container(&value, &l_array); 
 
    printf("%d\r\n", decode(&l_array));
    cbor_value_advance_fixed(&l_array);
    printf("%d\r\n", decode(&l_array));
    printf("***Decode Tiny-CBOR Done\r\n");
    return 0;
}

void print_decode_data(telemetry_data_t *p_data)
{

    if (!p_data)
        return;
    printf("version is %s\r\n", p_data->version.bytes);
    printf("Size of version is %lu\r\n", p_data->version.size);
    printf("RSSI is %d\r\n", p_data->rssi);
    printf("Fav number is %d\r\n", p_data->my_fav_number);
}

int main()
{
    /* Create the object */
    telemetry_data_t data = {
        .my_fav_number = 24,
        .rssi = -49,
        .version = {
            .bytes = "0.2.0",
            .size = strlen("0.2.0"),
        }};

    /* Encode the data */
    uint8_t buf[256];
    size_t size = 0;
    printf("***Encode using QCBOR\r\n");
    QCBORError err = telemetry_codec_encode(&data, buf, sizeof(buf), &size);
    if (err)
        printf("***QCBOR Error: %d\n", err);

    telemetry_data_t decode_data;
    err = telemetry_codec_decode(&decode_data, buf, size);
    
    print_decode_data(&decode_data);
    printf("***Decode using QCBOR Done\r\n");

    /*  TinyCBOR    */
    fun();
    return 0;
}
