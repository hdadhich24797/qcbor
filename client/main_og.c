#include <telemetry_codec.h>
#include <qcbor/qcbor.h>
#include <stdio.h>
#include <curl/curl.h>

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
    printf("Start of QCBOR Example\n");

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
    QCBORError err = telemetry_codec_encode(&data, buf, sizeof(buf), &size);
    if (err)
        printf("QCBOR Error: %d\n", err);

    /* Send data using Libcurl */
    /* Adopted from here: https://curl.se/libcurl/c/http-post.html */
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl)
    {
        /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3030/telemetry/1234");

        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, size);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    printf("\r\n****Let's decode****\r\n");

    telemetry_data_t decode_data;
    err = telemetry_codec_decode(&decode_data, buf, size);
    
    print_decode_data(&decode_data);
    return 0;
}
